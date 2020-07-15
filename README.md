# SF2X
sf2 to m4a voicegroup tool (written in C++)

# WHAT DOES IT DO?
Currently, it adds all instruments in soundfont files.
To be more precise, it adds all instruments that are referenced in the first zones of the presets. The other zones of the presets are neglected.

This tool will support features such as choosing which instruments to add in the future.


# ABOUT THE CODE
Please keep it in mind that it's still at the very basic stage, and my coding skills are not really great, so the code may be hard to read.
The code consists of 3 parts: soundfont reader, soundfont inserter, misc.

Special thanks for ipatix for teaching me bunch of m4a related stuff :D


# HOW TO USE
Open the exe (build):


Soundfont: (soundfont dir)

ROM: (rom dir)

FreeSpace for voicegroup table + alpha: (freespace offset #1)

FreeSpace for samples: (freespace offset #2)

Finished!


freespace offset #1: this will be your start of voicegroup. "alpha" means that it might add extra stuff after the voicegroup table, such as key split table, precussion table, and sub-instrument table.

freespace offset #2: the extra-large space where samples will be located in. Make sure you have enough freespace to fit them in.

# (WARNING!!!)

The tool might be unstable at the moment, so please backup your ROM before using it. Also, only soundfont files that were ripped from pokemon DS games are recommended. I never tested on other games, so there's no guarantee that it'll work
