#include "VoiceTableBase.h"

VoiceTable::VoiceTable(std::ostream& os, UINT32 start_pos) : VoiceGroupBase(os), start_pos(start_pos)
{
}

VoiceTable::~VoiceTable()
{
}

void VoiceTable::addEntry(UINT8 ndx, const m4a::VgEntry& entry)
{
	if (ndx > 127) throw std::out_of_range("Invalid voice table index");
	
	UINT32 pos = m4a::sizeof_entry * ndx + start_pos;
	const auto bin = entry.bin();

	os.seekp(pos);
	os.write((const char*)&bin[0], bin.size());
}
