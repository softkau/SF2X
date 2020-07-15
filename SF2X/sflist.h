#pragma once

#include "types.h"
#include <vector>

namespace sf
{
	enum class Generator : WORD
	{
		START_ADDRS_OFFSET = 0,
		END_ADDRS_OFFSET = 1,
		START_LOOP_ADDRS_OFFSET = 2,
		END_LOOP_ADDRS_OFFSET = 3,
		START_ADDRS_COARSE_OFFSET = 4,


		REVERB_EFFECTS_SEND = 16,
		PAN = 17,
		DELAY_VOL_ENV = 33,
		ATTACK_VOL_ENV = 34,
		HOLD_VOL_ENV = 35,
		DECAY_VOL_ENV = 36,
		SUSTAIN_VOL_ENV = 37,
		RELEASE_VOL_ENV = 38,

		INSTRUMENT = 41,
		KEY_RANGE = 43,
		VEL_RANGE = 44,
		KEY_NUM = 46,
		VELOCITY = 47,

		FINE_TUNE = 52,
		SAMPLE_ID = 53,
		SAMPLE_MODES = 54,
		OVERRIDING_ROOTKEY = 58
	};

	struct BagList
	{
		WORD gen_ndx;
		WORD mod_ndx;
	};

	struct ModList
	{
		// TODO: i dunno
	};

	struct GenList
	{
		Generator gen_oper;
		WORD amount;
		SHORT getSigned() const { return static_cast<SHORT>(amount); }
		BYTE getLow() const { return (BYTE)(amount & 0x00FF); }
		BYTE getHigh() const { return (BYTE)((amount >> 8) & 0x00FF); }
	};

	struct ZoneList
	{
		std::vector<ModList> mod;
		std::vector<GenList> gen;
	};
}
