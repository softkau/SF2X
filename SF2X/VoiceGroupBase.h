#pragma once

#include <iostream>
#include "types.h"
#include "m4a.h"

class VoiceGroupBase
{
public:
	VoiceGroupBase(std::ostream& os);
	virtual ~VoiceGroupBase() = 0;

	std::ostream& os;
protected:
	const UINT32 _stride = 4;

	void align();
	void addPaddings();
private:

};

