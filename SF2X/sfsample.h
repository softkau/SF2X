#pragma once

#include "types.h"
#include <vector>

namespace sf
{
	enum class SampleLink : WORD
	{
		monoSample = 1,
		rightSample = 2,
		leftSample = 4,
		linkedSample = 8,
		RomMonoSample = 0x8001,
		RomRightSample = 0x8002,
		RomLeftSample = 0x8004,
		RomLinkedSample = 0x8008
	};

	struct SampleHeader
	{
		CHAR sample_name[20] = { 0 };
		DWORD start = 0;
		DWORD end = 0;
		DWORD start_loop = 0;
		DWORD end_loop = 0;
		DWORD sample_rate = 0;
		BYTE original_pitch = 0;
		CHAR pitch_correction = 0;
		WORD sample_link = 0;
		SampleLink sample_type = SampleLink::monoSample;
	};

	struct Sample
	{
		SampleHeader header;
		std::vector<unsigned char> wav;
	};
}