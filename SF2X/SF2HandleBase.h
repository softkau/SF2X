#pragma once

#include "chunk.h"
#include "FileHandle.h"
#include <iostream>

namespace sf
{
	class SF2HandleBase
	{
	public:
		SF2HandleBase(std::istream& ifs);
		virtual ~SF2HandleBase() = 0;

		std::istream& is;
	protected:
		Chunk searchChunk(const Range& range, const std::string& fourcc);
	};
}
