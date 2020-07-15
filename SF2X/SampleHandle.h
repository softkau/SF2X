#pragma once

#include "SF2HandleBase.h"
#include "sfsample.h"

namespace sf
{
	class SampleHandle final : public SF2HandleBase
	{
	public:
		SampleHandle(std::istream& is, const Range& smpl_range, const Range& search);
		~SampleHandle();

		sf::Sample parse(UINT32 ndx);
		UINT32 size() noexcept;

	private:
		sf::SampleHeader gethdr(UINT32 ndx) noexcept;

		Chunk _smpllist, _shdrlist;

		const UINT32 _sz_shdr = 46;
		UINT32 _size;
	};
}
