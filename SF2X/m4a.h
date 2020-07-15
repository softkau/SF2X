#pragma once

#include "types.h"
#include <array>
#include "endian.h"

namespace m4a
{
	const UINT32 sizeof_entry = 12;

	struct VgEntry
	{
		virtual std::array<UINT8, sizeof_entry> bin() const = 0;
	};

	struct DirectSound : VgEntry
	{
		const UINT8 type = 0x00;
		UINT8 midi_key = 0;
		UINT8 panning = 0;
		UINT32 smpl_ptr = 0;
		UINT8 atk = 0;
		UINT8 dec = 0;
		UINT8 sus = 0;
		UINT8 rel = 0;

		std::array<UINT8, sizeof_entry> bin() const override
		{
			std::array<UINT8, sizeof_entry> res;

			auto tab = byteswap(smpl_ptr);

			res[0] = type;
			res[1] = midi_key;
			res[2] = 0x00;
			res[3] = panning;
			res[4] = (tab >> 24) & 0xFF;
			res[5] = (tab >> 16) & 0xFF;
			res[6] = (tab >> 8) & 0xFF;
			res[7] = tab & 0xFF;
			res[8] = atk;
			res[9] = dec;
			res[10] = sus;
			res[11] = rel;

			return res;
		}
	};

	struct MultiSplit : VgEntry
	{
		const UINT8 type = 0x40;
		UINT32 sub_ptr;
		UINT32 keytab_ptr;

		std::array<UINT8, sizeof_entry> bin() const override
		{
			std::array<UINT8, sizeof_entry> res = { 0 };

			auto tab1 = byteswap(sub_ptr);
			auto tab2 = byteswap(keytab_ptr);

			res[0] = type;

			res[4] = (tab1 >> 24) & 0xFF;
			res[5] = (tab1 >> 16) & 0xFF;
			res[6] = (tab1 >> 8) & 0xFF;
			res[7] = tab1 & 0xFF;

			res[8] = (tab2 >> 24) & 0xFF;
			res[9] = (tab2 >> 16) & 0xFF;
			res[10] = (tab2 >> 8) & 0xFF;
			res[11] = tab2 & 0xFF;

			return res;
		}
	};

	struct Precussion : VgEntry
	{
		const UINT8 type = 0x80;
		UINT32 sub_ptr;

		std::array<UINT8, sizeof_entry> bin() const override
		{
			std::array<UINT8, sizeof_entry> res = { 0 };

			auto tab = byteswap(sub_ptr);

			res[0] = type;

			res[4] = (tab >> 24) & 0xFF;
			res[5] = (tab >> 16) & 0xFF;
			res[6] = (tab >> 8) & 0xFF;
			res[7] = tab & 0xFF;

			return res;
		}
	};

}