#include <iostream>
#include <fstream>
#include "SF2toM4A.h"

int main()
{
	std::string soundfont, rom;
	std::cout << "SoundFont: ";
	std::cin >> soundfont;
	std::cout << "ROM: ";
	std::cin >> rom;

	SF2toM4A inserter(rom, soundfont);

	UINT32 vg_fs, ds_fs;
	std::cout << "FreeSpace for voicegroup table + alpha: ";
	std::cin >> std::hex >> vg_fs;
	std::cout << "FreeSpace for samples: ";
	std::cin >> std::hex >> ds_fs;

	inserter.setFreeSpace(vg_fs, ds_fs);

	inserter.addAll();
	std::cout << "Finished!";
	std::cin >> soundfont; // wait lol

	return 0;
}
