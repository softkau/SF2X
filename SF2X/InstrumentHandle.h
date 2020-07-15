#pragma once

#include "PdtaHandle.h"
#include "sfinstrument.h"

namespace sf
{
	class InstrumentHandle final : public PdtaHandle
	{
	public:
		InstrumentHandle(std::istream& is, const Range& pdta_range);
		~InstrumentHandle();
		sf::Instrument parse(UINT32 ndx);

	private:
		sf::InstrumentHeader gethdr(UINT32 ndx) noexcept;
		void addgen(sf::InstrumentZone& zone, const sf::GenList& gen) noexcept;
	};
}
