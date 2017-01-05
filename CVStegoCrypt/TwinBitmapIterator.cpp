#include "TwinBitmapIterator.h"

using namespace CVStegoCrypt;

TwinBitmapIterator::TwinBitmapIterator()
	: _iterator(gcnew BitmapIterator()), _data2(nullptr)
{}

TwinBitmapIterator::TwinBitmapIterator(array<Byte>^ bmpData1, array<Byte>^ bmpData2)
	: _iterator(gcnew BitmapIterator(bmpData1)), _data2(bmpData2)
{}

TwinBitmapIterator::TwinBitmapIterator(array<Byte>^ bmpData1, array<Byte>^ bmpData2, Int32 startByte)
	: _iterator(gcnew BitmapIterator(bmpData1, startByte)), _data2(bmpData2)
{}

TwinBitmapIterator::TwinBitmapIterator(array<Byte>^ bmpData1, array<Byte>^ bmpData2, Int32 startByte, Int32 endByte)
	: _iterator(gcnew BitmapIterator(bmpData1, startByte, endByte)), _data2(bmpData2)
{}

void TwinBitmapIterator::initializeColorMasks(StegoBitsNominator^ red, StegoBitsNominator^ green, StegoBitsNominator^ blue)
{ _iterator->initializeColorBits(red, green, blue); }

inline void TwinBitmapIterator::swapData()
{
	array<Byte>^ tmp = _data2;
	_data2 = _iterator->LinkedData;
	_iterator->LinkedData = tmp;
}

inline void TwinBitmapIterator::iterate()
{ _iterator->iterate(); }