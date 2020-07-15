#include "PdtaHandle.h"
#include <stdexcept>

sf::PdtaHandle::PdtaHandle(std::istream& is, const Range& pdta_range, UINT32 sz_hdr, const std::array<std::string, 4>& fourcc) : SF2HandleBase(is), _sz_hdr(sz_hdr)
{
	_hdrlist = searchChunk(pdta_range, fourcc[0]);
	_baglist = searchChunk({ _hdrlist.pos + _hdrlist.sz, pdta_range.end }, fourcc[1]);
	_modlist = searchChunk({ _baglist.pos + _baglist.sz, pdta_range.end }, fourcc[2]);
	_genlist = searchChunk({ _modlist.pos + _modlist.sz, pdta_range.end }, fourcc[3]);

	_size = _hdrlist.sz / sz_hdr - 1;
}

sf::PdtaHandle::~PdtaHandle()
{
}

Range sf::PdtaHandle::range(char ck_no) noexcept
{
	switch (ck_no)
	{
	case 0: { return { _hdrlist.pos, _hdrlist.pos + _hdrlist.sz }; }
	case 1: { return { _baglist.pos, _baglist.pos + _baglist.sz }; }
	case 2: { return { _modlist.pos, _modlist.pos + _modlist.sz }; }
	case 3: { return { _genlist.pos, _genlist.pos + _genlist.sz }; }
	default: { return { 0, 0 }; }
	}
}

UINT32 sf::PdtaHandle::size() noexcept
{
	return _size;
}

UINT32 sf::PdtaHandle::hdr_pos(UINT32 ndx) noexcept
{
	return _hdrlist.pos + ndx * _sz_hdr;
}

sf::ZoneList sf::PdtaHandle::getzone(UINT32 bag_ndx) noexcept
{
	sf::BagList bag = getbag(bag_ndx);
	sf::BagList bag_next = getbag(bag_ndx + 1);

	sf::ZoneList zone;
	zone.mod.reserve(bag_next.mod_ndx - bag.mod_ndx);
	zone.gen.reserve(bag_next.gen_ndx - bag.gen_ndx);

	for (UINT32 i = bag.mod_ndx; i < bag_next.mod_ndx; i++)
	{
		zone.mod.push_back(getmod(i));
	}

	for (UINT32 i = bag.gen_ndx; i < bag_next.gen_ndx; i++)
	{
		zone.gen.push_back(getgen(i));
	}
	return zone;
}

sf::BagList sf::PdtaHandle::getbag(UINT32 ndx) noexcept
{
	UINT32 pos = _baglist.pos + ndx * _sz_bag;
	return sf::BagList{ readUshort(is, pos), readUshort(is, pos + 2) };
}

sf::ModList sf::PdtaHandle::getmod(UINT32 ndx) noexcept
{
	return sf::ModList();
}

sf::GenList sf::PdtaHandle::getgen(UINT32 ndx) noexcept
{
	UINT32 pos = _genlist.pos + ndx * _sz_gen;
	return sf::GenList{ static_cast<sf::Generator>(readUshort(is, pos)), readUshort(is, pos + 2) };
}
