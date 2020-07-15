#pragma once

#include <vector>
#include "types.h"

namespace sf
{
	struct PresetHeader
	{
		CHAR preset_name[20] = { 0 };
		WORD preset = 0;
		WORD bank = 0;
		WORD preset_bag_ndx = 0;
		DWORD library = 0;
		DWORD genre = 0;
		DWORD morphology = 0;
	};

	struct PresetZone
	{
		int atkVolEnv = 1;
		int decVolEnv = 1;
		int susVolEnv = 0;
		int relVolEnv = 1;

		unsigned char keyRange[2] = { 0, 127 };
		unsigned char velocityRange[2] = { 0, 127 };
		int pan = 0;

		int instNdx = -1;
	};

	struct Preset
	{
		PresetHeader header;
		std::vector<PresetZone> zone;
	};
}