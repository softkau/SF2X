#include "SF2toM4A.h"

namespace
{
	UINT32 inbound(UINT32 x, UINT32 min, UINT32 max)
	{
		if (x > max)
		{
			return max;
		}
		else if (x < min)
		{
			return min;
		}
		else
		{
			return x;
		}
	}

	bool isDrum(const sf::Instrument& inst)
	{
		for (const auto& i : inst.zone)
			if (i.keyRange[1] - i.keyRange[0] != 0)
				return false;
		return true;
	}

	SampleInfo getsinfo(const sf::InstrumentZone& izone, const sf::SampleHeader& shdr)
	{
		SampleInfo sinfo;
		sinfo.bit_depth = 16;
		sinfo.loopflag = izone.sampleMode;
		sinfo.loop_point = (sinfo.loopflag) ? shdr.start_loop - shdr.start : 0;
		sinfo.overriding_rootkey = (izone.rootKey != -1) ? izone.rootKey : shdr.original_pitch;
		sinfo.pitch_correction = shdr.pitch_correction;
		sinfo.smpl_rate = shdr.sample_rate;

		return sinfo;
	}

	m4a::DirectSound getds(const sf::InstrumentZone& izone, const SF2toM4A::ADSRconstant& c, UINT32 smpl_offset, UINT32 key)
	{
		m4a::DirectSound entry;
		entry.smpl_ptr = smpl_offset + 0x08000000;
		entry.midi_key = key;
		entry.panning = 0x80 + inbound((izone.pan + 500.0) * 128 / 1000.0, 0x00, 0x7F);
		entry.atk = inbound(static_cast<UINT32>(c.c_atk / (izone.atkVolEnv * 0.001f)), 0x01, 0xFF);
		entry.dec = inbound(static_cast<UINT32>(256 - c.c_dec / (izone.decVolEnv * 0.001f)), 0x00, 0xFF);
		entry.sus = inbound(static_cast<UINT32>(255 - izone.susVolEnv * 255 / c.c_sus), 0x00, 0xFF);
		entry.rel = inbound(static_cast<UINT32>(256 - c.c_rel / (izone.relVolEnv * 0.001f)), 0x00, 0xFF);

		return entry;
	}
}

SF2toM4A::SF2toM4A(const std::string& rom, const std::string& soundfont)
{
	this->rom.open(rom, std::ios::binary | std::ios::in);
	this->soundfont.open(soundfont, std::ios::binary);

	if (!(this->rom.is_open())) throw std::runtime_error("Unable to open rom");
	if (!(this->soundfont.is_open())) throw std::runtime_error("Unable to open soundfont");

	sf_handle.reset(new sf::SF2Handle(this->soundfont));
	misc_offset = 0;
}

SF2toM4A::~SF2toM4A()
{
	rom.close();
	soundfont.close();
}

void SF2toM4A::setFreeSpace(UINT32 vg_pos, UINT32 ds_pos)
{
	voice_table.reset(new VoiceTable(rom, vg_pos));
	ds_handle.reset(new DirectSoundHandle(this->rom, ds_pos));
	misc_offset = voice_table->start_pos + 128 * m4a::sizeof_entry;
}

void SF2toM4A::addAll()
{
	UINT32 sz = sf_handle->preset_handle->size();
	for (UINT32 p = 0; p < sz; p++)
	{
		sf::Preset preset = sf_handle->preset_handle->parse(p);
		addInstrument(preset.zone[0].instNdx, preset.header.preset);
	}
}

void SF2toM4A::addBank(UINT32 bank)
{
	UINT32 sz = sf_handle->preset_handle->size();
	for (UINT32 p = 0; p < sz; p++)
	{
		sf::Preset preset = sf_handle->preset_handle->parse(p);
		if (preset.header.bank == bank)
			addInstrument(preset.zone[0].instNdx, preset.header.preset);
	}
}

void SF2toM4A::addInstrument(UINT32 inst_ndx, UINT32 vg_ndx)
{
	if (voice_table.get() == nullptr) throw std::runtime_error("Freespace is not initialized!");

	sf::Instrument inst = sf_handle->inst_handle->parse(inst_ndx);

	if (inst.zone.size() == 1)
	{
		addizone(inst.zone[0], *voice_table, vg_ndx, false);
	}
	else
	{
		bool is_drum = isDrum(inst);

		VoiceTable split_table(rom, misc_offset);
		if (is_drum)
		{
			for (const auto& i : inst.zone)
			{
				addizone(i, split_table, i.keyRange[0], true);
			}
			m4a::Precussion prec;
			prec.sub_ptr = misc_offset + 0x08000000;
			misc_offset += 128 * m4a::sizeof_entry;
			voice_table->addEntry(vg_ndx, prec);
		}
		else
		{
			UINT32 key_offset = misc_offset + inst.zone.size() * m4a::sizeof_entry;
			unsigned char ndx = 0;
			for (const auto& i : inst.zone)
			{
				addizone(i, split_table, ndx, false);
				for (UINT32 key = i.keyRange[0]; key <= i.keyRange[1]; key++)
				{
					rom.seekp((unsigned long long)key_offset + key);
					rom.write(reinterpret_cast<const char*>(&ndx), 1);
				}
				ndx++;
			}
			m4a::MultiSplit multi;
			multi.sub_ptr = misc_offset + 0x08000000;
			multi.keytab_ptr = key_offset + 0x08000000;
			misc_offset = key_offset + 128;
			voice_table->addEntry(vg_ndx, multi);
		}
	}
}

void SF2toM4A::addizone(const sf::InstrumentZone& izone, VoiceTable& vt, UINT32 vt_ndx, bool drum)
{
	sf::Sample smpl = sf_handle->smpl_handle->parse(izone.sampleID);
	SampleInfo sinfo = getsinfo(izone, smpl.header);
	Range r = ds_handle->add(smpl.wav, sinfo);

	m4a::DirectSound entry = getds(izone, ADSR, r.begin, (drum ? smpl.header.original_pitch : 0x00));
	vt.addEntry(vt_ndx, entry);
}
