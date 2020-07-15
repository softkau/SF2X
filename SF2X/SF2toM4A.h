#pragma once

#include <memory>
#include "SF2Handle.h"
#include "DirectSoundHandle.h"
#include "VoiceTableBase.h"

// this class is kinda weirdly coded, so its code is going to be changed soon
class SF2toM4A
{
public:
	SF2toM4A(const std::string& rom, const std::string& soundfont);
	~SF2toM4A();

	void setFreeSpace(UINT32 vg_pos, UINT32 ds_pos);

	void addAll();

	void addInstrument(UINT32 inst_ndx, UINT32 vg_ndx);

	struct ADSRconstant
	{
		float c_atk = 4.3f; // larger -> faster attack
		float c_dec = 19.7f; // larger -> faster decay
		float c_sus = 1000.0f; // larger -> smaller sustain level
		float c_rel = 35.0f; // larger -> faster release
	} ADSR;

private:
	void addizone(const sf::InstrumentZone& izone, VoiceTable& vt, UINT32 vt_ndx, bool drum);

	UINT32 misc_offset;

	std::ofstream rom;
	std::ifstream soundfont;

	std::unique_ptr<sf::SF2Handle> sf_handle;

	std::unique_ptr<DirectSoundHandle> ds_handle;
	std::unique_ptr<VoiceTable> voice_table;
};

