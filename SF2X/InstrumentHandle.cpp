#include "InstrumentHandle.h"
#include <stdexcept>

sf::InstrumentHandle::InstrumentHandle(std::istream& is, const Range& pdta_range) : PdtaHandle(is, pdta_range, 22, { "inst", "ibag", "imod", "igen" })
{
}

sf::InstrumentHandle::~InstrumentHandle()
{
}

sf::Instrument sf::InstrumentHandle::parse(UINT32 ndx)
{
	if (ndx >= size()) throw std::out_of_range("index out of range (or accessed terminal instrument header)");

	sf::InstrumentHeader inst = gethdr(ndx);
	sf::InstrumentHeader inst_next = gethdr(ndx + 1);

	std::vector<sf::ZoneList> izone;
	izone.reserve(inst_next.inst_bag_ndx - inst.inst_bag_ndx + 1);
	for (UINT32 i = inst.inst_bag_ndx; i < inst_next.inst_bag_ndx; i++)
	{
		izone.push_back(getzone(i));
	}

	sf::Instrument instrument;
	instrument.header = std::move(inst);
	instrument.zone.reserve(izone.size());
	for (const auto& i : izone)
	{
		sf::InstrumentZone z;
		for (const auto& j : i.gen)
		{
			addgen(z, j);
		}
		instrument.zone.push_back(std::move(z));
	}
	return instrument;
}

sf::InstrumentHeader sf::InstrumentHandle::gethdr(UINT32 ndx) noexcept
{
	sf::InstrumentHeader inst;
	UINT32 pos = hdr_pos(ndx);

	is.seekg(pos);
	is.read((char*)(inst.inst_name), 20);
	inst.inst_bag_ndx = readUshort(is, pos + 20);
	return inst;
}

void sf::InstrumentHandle::addgen(sf::InstrumentZone& zone, const sf::GenList& gen) noexcept
{
	switch (gen.gen_oper)
	{
	case sf::Generator::REVERB_EFFECTS_SEND:																			break;
	case sf::Generator::PAN:					zone.pan = gen.getSigned();												break;
	case sf::Generator::DELAY_VOL_ENV:																					break;
	case sf::Generator::ATTACK_VOL_ENV:			zone.atkVolEnv = (int)(pow(2.0, gen.getSigned() / 1200.0) * 1000);		break;
	case sf::Generator::HOLD_VOL_ENV:																					break;
	case sf::Generator::DECAY_VOL_ENV:			zone.decVolEnv = (int)(pow(2.0, gen.getSigned() / 1200.0) * 1000);		break;
	case sf::Generator::SUSTAIN_VOL_ENV:		zone.susVolEnv = gen.getSigned();										break;
	case sf::Generator::RELEASE_VOL_ENV:		zone.relVolEnv = (int)(pow(2.0, gen.getSigned() / 1200.0) * 1000);		break;
	case sf::Generator::KEY_RANGE:				zone.keyRange[0] = gen.getLow();
												zone.keyRange[1] = gen.getHigh();										break;
	case sf::Generator::VEL_RANGE:				zone.velocityRange[0] = gen.getLow();
												zone.velocityRange[1] = gen.getHigh();									break;
	case sf::Generator::FINE_TUNE:																						break;
	case sf::Generator::SAMPLE_ID:				zone.sampleID = gen.amount;												break;
	case sf::Generator::SAMPLE_MODES:			zone.sampleMode = gen.getLow();											break;
	case sf::Generator::OVERRIDING_ROOTKEY:		zone.rootKey = static_cast<char>(gen.getLow());							break;
	}
}
