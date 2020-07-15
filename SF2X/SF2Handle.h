#pragma once

#include <memory>

#include "SF2HandleBase.h"
#include "PresetHandle.h"
#include "InstrumentHandle.h"
#include "SampleHandle.h"

namespace sf
{
	class SF2Handle final : private SF2HandleBase
	{
	public:
		SF2Handle(std::istream& is);
		~SF2Handle();

		std::unique_ptr<PresetHandle> preset_handle;
		std::unique_ptr<InstrumentHandle> inst_handle;
		std::unique_ptr<SampleHandle> smpl_handle;

	private:
		Chunk _sfbk, _info, _sdta, _pdta;
		UINT32 _len;
	};
}