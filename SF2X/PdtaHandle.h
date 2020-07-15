#pragma once

#include "SF2HandleBase.h"
#include "sflist.h"
#include <vector>
#include <array>

namespace sf
{
	class PdtaHandle : public SF2HandleBase
	{
	public:
		PdtaHandle(std::istream& is, const Range& pdta_range, UINT32 sz_hdr, const std::array<std::string, 4>& fourcc);
		virtual ~PdtaHandle() = 0;

		Range range(char ck_no) noexcept;
		UINT32 size() noexcept;

	protected:
		UINT32 hdr_pos(UINT32 ndx) noexcept;
		sf::ZoneList getzone(UINT32 bag_ndx) noexcept;

	private:
		sf::BagList getbag(UINT32 ndx) noexcept;
		sf::ModList getmod(UINT32 ndx) noexcept;
		sf::GenList getgen(UINT32 ndx) noexcept;

		Chunk _hdrlist, _baglist, _modlist, _genlist;

		const UINT32 _sz_hdr;
		const UINT32 _sz_bag = 4;
		const UINT32 _sz_mod = 10;
		const UINT32 _sz_gen = 4;

		UINT32 _size; //number of headers, excluding the terminal header;
	};
}
