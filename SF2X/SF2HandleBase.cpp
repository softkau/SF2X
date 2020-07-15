#include "SF2HandleBase.h"
#include <stdexcept>

sf::SF2HandleBase::SF2HandleBase(std::istream& is) : is(is)
{

}

sf::SF2HandleBase::~SF2HandleBase()
{
}

sf::Chunk sf::SF2HandleBase::searchChunk(const Range& range, const std::string& fourcc)
{
	UINT32 pos = searchString(is, range, fourcc, 2);
	UINT32 sz;
	if (pos + 8 < range.end)
	{
		sz = readUint(is, pos + 4);

		if (pos + sz >= range.end)
		{
			throw std::runtime_error("byte size of " + fourcc + " exceeds search range");
		}
	}
	else
	{
		throw std::runtime_error(fourcc + " chunk not found within range");
	}
	Chunk ck = { sz, pos + 8 };

	return ck;
}
