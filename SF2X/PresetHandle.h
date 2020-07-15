#pragma once

#include "PdtaHandle.h"
#include "sfpreset.h"

namespace sf
{
	class PresetHandle final : public PdtaHandle
	{
	public:
		PresetHandle(std::istream& is, const Range& pdta_range);
		~PresetHandle();

		sf::Preset parse(UINT32 ndx);

	private:
		sf::PresetHeader gethdr(UINT32 ndx) noexcept;
		void addgen(sf::PresetZone& zone, const sf::GenList& gen) noexcept;
	};
}


