#pragma once

#include "VoiceGroupBase.h"
#include "DirectSoundHandle.h"
#include "m4a.h"

class VoiceTable final : public VoiceGroupBase
{
public:
	VoiceTable(std::ostream& os, UINT32 start_pos);
	~VoiceTable();

	const UINT32 start_pos;
	void addEntry(UINT8 ndx, const m4a::VgEntry& entry);

};

