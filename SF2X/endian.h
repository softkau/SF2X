#pragma once

#include <climits>

template <typename T>
T byteswap(const T val)
{
	unsigned int bytes = sizeof T;

	T res = 0;

	for (unsigned int i = 0; i < bytes; i++)
	{
		res += ((val >> (i * CHAR_BIT)) & UCHAR_MAX) << ((bytes - i - 1) * CHAR_BIT);
	}
	return res;
}