#include "PresetHandle.h"
#include <stdexcept>

sf::PresetHandle::PresetHandle(std::istream& is, const Range& pdta_range) : PdtaHandle(is, pdta_range, 38, { "phdr", "pbag", "pmod", "pgen" })
{
}

sf::PresetHandle::~PresetHandle()
{
}

sf::Preset sf::PresetHandle::parse(UINT32 ndx)
{
	if (ndx >= size()) throw std::out_of_range("index out of range (or accessed terminal preset header)");

	sf::PresetHeader phdr = gethdr(ndx);
	sf::PresetHeader phdr_next = gethdr(ndx + 1);

	std::vector<sf::ZoneList> pzone;
	pzone.reserve(phdr_next.preset_bag_ndx - phdr.preset_bag_ndx + 1);
	for (UINT32 i = phdr.preset_bag_ndx; i < phdr_next.preset_bag_ndx; i++)
	{
		pzone.push_back(getzone(i));
	}
	
	sf::Preset preset;
	preset.header = std::move(phdr);
	preset.zone.reserve(pzone.size());
	for (const auto& i : pzone)
	{
		sf::PresetZone z;
		for (const auto& j : i.gen)
		{
			addgen(z, j);
		}
		preset.zone.push_back(std::move(z));
	}
	return preset;
}

sf::PresetHeader sf::PresetHandle::gethdr(UINT32 ndx) noexcept
{
	sf::PresetHeader phdr;
	UINT32 pos = hdr_pos(ndx);
	
	is.seekg(pos);
	is.read((char*)(phdr.preset_name), 20);

	phdr.preset = readUshort(is, pos + 20);
	phdr.bank = readUshort(is, pos + 22);
	phdr.preset_bag_ndx = readUshort(is, pos + 24);
	phdr.library = readUint(is, pos + 26);
	phdr.genre = readUint(is, pos + 30);
	phdr.morphology = readUint(is, pos + 34);

	return phdr;
}

void sf::PresetHandle::addgen(sf::PresetZone& zone, const sf::GenList& gen) noexcept
{
	switch (gen.gen_oper)
	{
	case sf::Generator::REVERB_EFFECTS_SEND:												break;
	case sf::Generator::PAN:																break;
	case sf::Generator::DELAY_VOL_ENV:														break;
	case sf::Generator::ATTACK_VOL_ENV:														break;
	case sf::Generator::HOLD_VOL_ENV:														break;
	case sf::Generator::DECAY_VOL_ENV:														break;
	case sf::Generator::SUSTAIN_VOL_ENV:													break;
	case sf::Generator::RELEASE_VOL_ENV:													break;
	case sf::Generator::INSTRUMENT:				zone.instNdx = gen.getSigned();				break;
	case sf::Generator::KEY_RANGE:				zone.keyRange[0] = gen.getLow();
												zone.keyRange[1] = gen.getHigh();			break;
	case sf::Generator::VEL_RANGE:				zone.velocityRange[0] = gen.getLow();
												zone.velocityRange[1] = gen.getHigh();		break;
	case sf::Generator::FINE_TUNE:															break;
	}
}
