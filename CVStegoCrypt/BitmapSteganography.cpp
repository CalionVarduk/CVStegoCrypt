#include "BitmapSteganography.h"

using namespace CVStegoCrypt;

BitmapSteganography::BitmapSteganography(StegoBitsNominator^ red, StegoBitsNominator^ green, StegoBitsNominator^ blue)
	: BitmapStegoBase(red, green, blue)
{
	clear();
}

void BitmapSteganography::conceal(Int32 startPosition)
{
	if(startPosition >= MaxStartPosition || _loadedBitmap == nullptr || _info->Length == 0)
		return;

	_copyBitmapImage(_loadedBitmap, _createdBitmap);
	BitmapData^ data = _initWritableBitmapData(_createdBitmap, _bmpData);
	
	_concealInfo(startPosition);

	_unlockWritableBitmap(_createdBitmap, data, _bmpData);
}

bool BitmapSteganography::extract(Int32 startPosition)
{
	if((UInt32)startPosition >= _getMaxStartPosition(4) || _loadedBitmap == nullptr)
		return false;

	_initReadOnlyBitmapData(_loadedBitmap, _bmpData);
	return _tryExtractInfo(startPosition);
}

void BitmapSteganography::erase(EraseMode mode)
{
	if(_loadedBitmap == nullptr)
		return;

	_copyBitmapImage(_loadedBitmap, _createdBitmap);
	BitmapData^ data = _initWritableBitmapData(_createdBitmap, _bmpData);

	_eraseInfo(mode);

	_unlockWritableBitmap(_createdBitmap, data, _bmpData);
}

Int32 BitmapSteganography::loadInformation(String^ filePath)
{
	try {
		_infoPath = filePath;
		FileStream^ _stream = gcnew FileStream(filePath, FileMode::Open);
		BinaryReader^ bReader = gcnew BinaryReader(_stream);
		Int32 lengthDifference = (Int32)bReader->BaseStream->Length - MaxInfoLength;

		if(lengthDifference <= 0) {
			_info = bReader->ReadBytes((Int32)bReader->BaseStream->Length);
			bReader->Close();
			return 0;
		}
		bReader->Close();
		return lengthDifference;
	}
	catch(Exception^ e)
	{ e; return -1; }
}

void BitmapSteganography::saveInformation(String^ filePath)
{
	FileStream^ _stream = gcnew FileStream(filePath, FileMode::Create);
	BinaryWriter^ bWriter = gcnew BinaryWriter(_stream);
	bWriter->Write(_info);
	bWriter->Close();
}

bool BitmapSteganography::loadBitmap(String^ filePath)
{
	clear();
	return _loadBmp(filePath, _bmpPath, _loadedBitmap);
}

void BitmapSteganography::saveBitmap(String^ filePath)
{ BitmapIO::save(filePath, _createdBitmap); }

inline Bitmap^ BitmapSteganography::getLoadedBitmap()
{ return _loadedBitmap; }

inline Bitmap^ BitmapSteganography::getCreatedBitmap()
{ return _createdBitmap; }

void BitmapSteganography::clear()
{
	_bmpPath = "";
	_infoPath = "";
	_info = gcnew array<Byte>(0);
	_bmpData = gcnew array<Byte>(0);
	_loadedBitmap = nullptr;
	_createdBitmap = nullptr;
}

UInt32 BitmapSteganography::_getMaxStartPosition(UInt32 byteLength)
{
	Byte bitsPerPixel = BitsPerPixel;

	if(bitsPerPixel > 0) {
		UInt32 infoBitsCeil = (byteLength << 3) + bitsPerPixel - 1;
		UInt32 pixelsNeeded = infoBitsCeil / bitsPerPixel;
		UInt32 maxStartPosition = ((LoadedBitmapPixelCount - pixelsNeeded) * 3) + 1;

		UInt32 pixelsNeededR = (infoBitsCeil += BitsPerRed) / bitsPerPixel;
		if(pixelsNeededR == pixelsNeeded) {
			++maxStartPosition;
			UInt32 pixelsNeededG = (infoBitsCeil += BitsPerGreen) / bitsPerPixel;
			if(pixelsNeededG == pixelsNeeded)
				++maxStartPosition;
		}

		return maxStartPosition;
	}
	return 0;
}

UInt32 BitmapSteganography::_parseInfoLength()
{
	UInt32 infoLength = _info[0];
	infoLength |= ((UInt32)_info[1] << 8);
	infoLength |= ((UInt32)_info[2] << 16);
	infoLength |= ((UInt32)_info[3] << 24);
	return infoLength;
}

void BitmapSteganography::_prepareInfoToConceal()
{
	UInt32 infoSize = _info->Length;

	Array::Resize(_info, _info->Length + 4);
	for(Int32 i = _info->Length - 1; i >= 4; --i)
		_info[i] = _info[i - 4];

	_info[0] = (Byte)(infoSize & 255);
	_info[1] = (Byte)((infoSize >> 8) & 255);
	_info[2] = (Byte)((infoSize >> 16) & 255);
	_info[3] = (Byte)(infoSize >> 24);
}

inline void BitmapSteganography::_concealInfo(Int32 startPosition)
{
	_prepareInfoToConceal();

	BitmapIterator^ iBmp = _initBitmapIterator(_bmpData, startPosition, _bmpData->Length - 1);
	for(BitIterator^ iInfo = gcnew BitIterator(_info); !iInfo->isDone; iInfo->iterate(), iBmp->iterate())
		iBmp->CurrentBit = iInfo->CurrentBit;
}

inline bool BitmapSteganography::_tryExtractInfo(Int32 startPosition)
{
	_info = gcnew array<Byte>(4);

	BitmapIterator^ iBmp = _initBitmapIterator(_bmpData, startPosition, _bmpData->Length - 1);
	for(BitIterator^ iInfo = gcnew BitIterator(_info); !iInfo->isDone; iInfo->iterate(), iBmp->iterate())
		iInfo->CurrentBit = iBmp->CurrentBit;

	UInt32 infoLength = _parseInfoLength();

	if((UInt32)startPosition < _getMaxStartPosition(infoLength + 4)) {
		_info = gcnew array<Byte>(infoLength);
		for(BitIterator^ iInfo = gcnew BitIterator(_info); !iInfo->isDone; iInfo->iterate(), iBmp->iterate())
			iInfo->CurrentBit = iBmp->CurrentBit;

		_bmpData = gcnew array<Byte>(0);
		return true;
	}

	_info = gcnew array<Byte>(0);
	_bmpData = gcnew array<Byte>(0);
	return false;
}

inline void BitmapSteganography::_eraseInfo(EraseMode mode)
{
	if(mode == EraseMode::Random) {
		Random^ rng = gcnew Random();
		Byte b = 0;
		Int32 rand = rng->Next();
		for(BitmapIterator^ i = _initBitmapIterator(_bmpData, 0, _bmpData->Length - 1); !i->isDone; i->iterate(), ++b) {
			if(b > 31) { b = 0; rand = rng->Next(); }
			i->CurrentBit = (((rand >> b) & 1) == 1);
		}
	}
	else {
		bool value = (mode == EraseMode::Ones) ? true : false;
		for(BitmapIterator^ i = _initBitmapIterator(_bmpData, 0, _bmpData->Length - 1); !i->isDone; i->iterate())
			i->CurrentBit = value;
	}
}