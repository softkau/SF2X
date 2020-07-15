#pragma once

#include <vector>
#include "types.h"

namespace sf
{
	struct InstrumentHeader
	{
		CHAR inst_name[20] = { 0 };
		WORD inst_bag_ndx = 0;
	};

	struct InstrumentZone
	{
		int atkVolEnv = 1;
		int decVolEnv = 1;
		int susVolEnv = 0;
		int relVolEnv = 1;

		unsigned char keyRange[2] = { 0, 127 };
		unsigned char velocityRange[2] = { 0, 127 };
		int pan = 0;

		char rootKey = -1;
		int sampleID = -1;
		unsigned char sampleMode = 0;
	};

	struct Instrument
	{
		InstrumentHeader header;
		std::vector<InstrumentZone> zone;
	};
}