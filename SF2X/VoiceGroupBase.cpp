#include "VoiceGroupBase.h"

VoiceGroupBase::VoiceGroupBase(std::ostream& os) : os(os)
{

}

VoiceGroupBase::~VoiceGroupBase()
{

}


void VoiceGroupBase::align()
{
	UINT32 pos = static_cast<UINT32>(os.tellp());

	if (pos % _stride != 0)
		pos += _stride - (pos % _stride);

	os.seekp(pos);
}

void VoiceGroupBase::addPaddings()
{
	while ((UINT32)os.tellp() % _stride != 0)
	{
		char padding = 0x00;
		os.write(&padding, 1);
	}
}
