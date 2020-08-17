#include "Utils.h"

string printBits(int const size, void const * const ptr)
{
	string bits;
	if(size > 32)
	{
		;
		//todo
	}
	else
	{
		unsigned int a = 0x01;
		unsigned int *b = (unsigned int *)(ptr);

		for(int i=size-1; i >= 0; i--)
		{
			if ((*b) & (a << i))
			{
				bits += "1";
			}
			else
			{
				bits += "0";
			}
		}
	}

	return bits;
}
