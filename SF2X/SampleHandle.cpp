#include "SampleHandle.h"
#include <stdexcept>

sf::SampleHandle::SampleHandle(std::istream& is, const Range& smpl_range, const Range& search) : SF2HandleBase(is)
{
	_smpllist = searchChunk(smpl_range, "smpl");
	_shdrlist = searchChunk(search, "shdr");

	_size = _shdrlist.sz / _sz_shdr - 1;
}

sf::SampleHandle::~SampleHandle()
{
}

sf::Sample sf::SampleHandle::parse(UINT32 ndx)
{
	if (ndx >= _size) throw std::out_of_range("index out of range (or accessed terminal sample header)");

	sf::SampleHeader shdr = gethdr(ndx);
	
	sf::Sample smpl;
	UINT32 smpl_sz = 2 * (shdr.end - shdr.start);
	smpl.wav.resize(smpl_sz);
	
	UINT32 pos = 2 * shdr.start + _smpllist.pos;
	is.seekg(pos);
	is.read((char*)(&smpl.wav[0]), smpl_sz);

	smpl.header = std::move(shdr);

	return smpl;
}

UINT32 sf::SampleHandle::size() noexcept
{
	return _size;
}

sf::SampleHeader sf::SampleHandle::gethdr(UINT32 ndx) noexcept
{
	UINT32 pos = _sz_shdr * ndx + _shdrlist.pos;
	sf::SampleHeader shdr;
	is.seekg(pos);
	is.read((char*)(shdr.sample_name), 20);
	shdr.start = readUint(is, pos + 20);
	shdr.end = readUint(is, pos + 24);
	shdr.start_loop = readUint(is, pos + 28);
	shdr.end_loop = readUint(is, pos + 32);
	shdr.sample_rate = readUint(is, pos + 36);
	shdr.original_pitch = readUchar(is, pos + 40);
	shdr.pitch_correction = static_cast<CHAR>(readUchar(is, pos + 41));
	shdr.sample_link = readUshort(is, pos + 42);
	shdr.sample_type = static_cast<sf::SampleLink>(readUshort(is, pos + 44));
	return shdr;
}
