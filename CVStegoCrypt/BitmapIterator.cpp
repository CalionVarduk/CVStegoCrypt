#include "BitmapIterator.h"

using namespace CVStegoCrypt;

BitmapIterator::BitmapIterator()
	: BitIterator(), _colorBits(gcnew array<array<Byte>^>(3)), _iColor(0), _colorBit(0)
{
	_colorBits[0] = _colorBits[1] = _colorBits[2] = nullptr;
}

BitmapIterator::BitmapIterator(array<Byte>^ linkedData)
	: BitIterator(linkedData), _colorBits(gcnew array<array<Byte>^>(3)), _iColor(0), _colorBit(0)
{
	_colorBits[0] = _colorBits[1] = _colorBits[2] = nullptr;
}

BitmapIterator::BitmapIterator(array<Byte>^ linkedData, Int32 startByte)
	: BitIterator(linkedData, startByte), _colorBits(gcnew array<array<Byte>^>(3)), _iColor(0), _colorBit(0)
{
	_colorBits[0] = _colorBits[1] = _colorBits[2] = nullptr;
}

BitmapIterator::BitmapIterator(array<Byte>^ linkedData, Int32 startByte, Int32 endByte)
	: BitIterator(linkedData, startByte, endByte), _colorBits(gcnew array<array<Byte>^>(3)), _iColor(0), _colorBit(0)
{
	_colorBits[0] = _colorBits[1] = _colorBits[2] = nullptr;
}

void BitmapIterator::initializeColorBits(StegoBitsNominator^ red, StegoBitsNominator^ green, StegoBitsNominator^ blue)
{
	_colorBits[0] = gcnew array<Byte>(blue->SetBitsCount);
	_colorBits[1] = gcnew array<Byte>(green->SetBitsCount);
	_colorBits[2] = gcnew array<Byte>(red->SetBitsCount);

	for(Byte j = 0, r = 0, g = 0, b = 0; j < 8; ++j) {
		if(blue[j]) _colorBits[0][b++] = j;
		if(green[j]) _colorBits[1][g++] = j;
		if(red[j]) _colorBits[2][r++] = j;
	}

	_iColor = _iByte % 3;
	_colorBit = 0;
	_checkColorIndex();
	_iBit = _colorBits[_iColor][_colorBit];
}

inline void BitmapIterator::iterate()
{
	if(++_colorBit >= _colorBits[_iColor]->Length) {
		_colorBit = 0;
		++_iByte;
		if(++_iColor == 3) _iColor = 0;
		_checkColorIndex();
	}
	_iBit = _colorBits[_iColor][_colorBit];
}

inline void BitmapIterator::_checkColorIndex()
{
	if(_colorBits[_iColor]->Length == 0) {
		++_iByte;
		if(++_iColor == 3) _iColor = 0;

		if(_colorBits[_iColor]->Length == 0) {
			++_iByte;
			if(++_iColor == 3) _iColor = 0;
		}
	}
}