#include "FileHandle.h"

UINT32 searchString(std::istream& is, Range range, const std::string& str, UINT32 stride)
{
	const UINT32 sz = str.size();
	std::string buff(sz, ' ');

	for (UINT32 i = range.begin; i + sz < range.end; i += stride)
	{
		is.seekg(i);
		is.read(&buff[0], sz);
		if (str.compare(buff) == 0) return i;
	}
	return range.end;
}

UINT32 readUint(std::istream& is, UINT32 offset)
{
	unsigned char buff[4];
	is.seekg(offset);
	is.read((char*)buff, 4);
	UINT32 res = 0;
	res += buff[0];
	res += buff[1] << 8 * 1;
	res += buff[2] << 8 * 2;
	res += buff[3] << 8 * 3;
	return res;
}

UINT16 readUshort(std::istream& is, UINT32 offset)
{
	unsigned char buff[2];
	is.seekg(offset);
	is.read((char*)buff, 2);
	UINT16 res = 0;
	res += buff[0];
	res += buff[1] << 8 * 1;
	return res;
}

UINT8 readUchar(std::istream& is, UINT32 offset)
{
	char buff;
	is.seekg(offset);
	is.read(&buff, 1);
	return static_cast<UINT8>(buff);
}
