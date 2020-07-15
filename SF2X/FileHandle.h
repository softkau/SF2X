#pragma once

#include <fstream>
#include <string>
#include "types.h"

struct Range;

struct Range
{
	UINT32 begin;
	UINT32 end;

	Range() : begin(0), end(0) {};
	Range(UINT32 beg, UINT32 end) : begin(beg), end(end) {}
};

UINT32 searchString(std::istream& is, Range range, const std::string& str, UINT32 stride);
UINT32 readUint(std::istream& is, UINT32 offset);
UINT16 readUshort(std::istream& is, UINT32 offset);
UINT8 readUchar(std::istream& is, UINT32 offset);

