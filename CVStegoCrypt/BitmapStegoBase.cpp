#include "BitmapStegoBase.h"

using namespace CVStegoCrypt;

BitmapStegoBase::BitmapStegoBase(StegoBitsNominator^ red, StegoBitsNominator^ green, StegoBitsNominator^ blue)
{
	_rBits = red;
	_gBits = green;
	_bBits = blue;
}

UInt32 BitmapStegoBase::_getPixelsNeeded(UInt32 size)
{
	return ((size << 3) + BitsPerPixel - 1) / BitsPerPixel;
}

bool BitmapStegoBase::_loadBmp(String^ filePath, String^% out_pathStorage, Bitmap^% out_bitmap)
{
	if(BitmapIO::load(filePath, out_bitmap)) {
		out_pathStorage = filePath;
		return true;
	}
	return false;
}

void BitmapStegoBase::_copyBitmapImage(Bitmap^ source, Bitmap^% destination)
{
	destination = gcnew Bitmap(source->Width, source->Height, source->PixelFormat);
	Graphics::FromImage(destination)->DrawImage(source, Rectangle(Point(0, 0), source->Size));
}

BitmapData^ BitmapStegoBase::_initWritableBitmapData(Bitmap^% bmp, array<Byte>^% out_byteArray)
{
	BitmapData^ data = bmp->LockBits(Rectangle(Point(0, 0), bmp->Size), ImageLockMode::ReadWrite, bmp->PixelFormat);
	out_byteArray = gcnew array<Byte>(data->Stride * bmp->Height);
	Marshal::Copy(data->Scan0, out_byteArray, 0, out_byteArray->Length);
	return data;
}

void BitmapStegoBase::_initReadOnlyBitmapData(Bitmap^% bmp, array<Byte>^% out_byteArray)
{
	BitmapData^ data = bmp->LockBits(Rectangle(Point(0, 0), bmp->Size), ImageLockMode::ReadOnly, bmp->PixelFormat);
	out_byteArray = gcnew array<Byte>(data->Stride * bmp->Height);
	Marshal::Copy(data->Scan0, out_byteArray, 0, out_byteArray->Length);
	bmp->UnlockBits(data);
}

void BitmapStegoBase::_unlockWritableBitmap(Bitmap^% bmp, BitmapData^% data, array<Byte>^% byteArray)
{
	Marshal::Copy(byteArray, 0, data->Scan0, byteArray->Length);
	bmp->UnlockBits(data);
	byteArray = gcnew array<Byte>(0);
}

BitmapIterator^ BitmapStegoBase::_initBitmapIterator(array<Byte>^ bmpData, Int32 startPosition, Int32 endPosition)
{
	BitmapIterator^ i = gcnew BitmapIterator(bmpData, startPosition, endPosition);
	i->initializeColorBits(_rBits, _gBits, _bBits);
	return i;
}