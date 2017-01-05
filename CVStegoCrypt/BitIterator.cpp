#include "BitIterator.h"

using namespace CVStegoCrypt;

BitIterator::BitIterator()
	: _data(nullptr), _endByte(0), _iByte(0), _iBit(0)
{}

BitIterator::BitIterator(array<Byte>^ linkedData)
	: _data(linkedData), _iByte(0), _iBit(0)
{
	EndByte = _data->Length - 1;
}

BitIterator::BitIterator(array<Byte>^ linkedData, Int32 startByte)
	: _data(linkedData), _iBit(0)
{
	iByte = startByte;
	EndByte = _data->Length - 1;
}

BitIterator::BitIterator(array<Byte>^ linkedData, Int32 startByte, Int32 endByte)
	: _data(linkedData), _iBit(0)
{
	iByte = startByte;
	EndByte = endByte;
}

inline void BitIterator::iterate()
{
	if(++_iBit >= 8) {
		_iBit = 0;
		++_iByte;
	}
}