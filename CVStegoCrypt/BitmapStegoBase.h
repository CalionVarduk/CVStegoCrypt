#pragma once

#include "BitmapIterator.h"
#include "BitmapIO.h"

namespace CVStegoCrypt
{
	using namespace System;
	using namespace System::Drawing;
	using namespace System::IO;
	using namespace System::Runtime::InteropServices;

	public ref class BitmapStegoBase
	{
		public:
			property Byte BitsPerRed {
				public: Byte get() { return _rBits->SetBitsCount; }
			}

			property Byte BitsPerGreen {
				public: Byte get() { return _gBits->SetBitsCount; }
			}

			property Byte BitsPerBlue {
				public: Byte get() { return _bBits->SetBitsCount; }
			}

			property Byte BitsPerPixel {
				public: Byte get() { return BitsPerRed + BitsPerGreen + BitsPerBlue; }
			}

			BitmapStegoBase(StegoBitsNominator^ red, StegoBitsNominator^ green, StegoBitsNominator^ blue);

		protected:
			StegoBitsNominator^ _rBits;
			StegoBitsNominator^ _gBits;
			StegoBitsNominator^ _bBits;

			UInt32 _getPixelsNeeded(UInt32 size);

			bool _loadBmp(String^ filePath, String^% out_pathStorage, Bitmap^% out_bitmap);
			void _copyBitmapImage(Bitmap^ source, Bitmap^% destination);
			BitmapData^ _initWritableBitmapData(Bitmap^% bmp, array<Byte>^% out_byteArray);
			void _initReadOnlyBitmapData(Bitmap^% bmp, array<Byte>^% out_byteArray);
			void _unlockWritableBitmap(Bitmap^% bmp, BitmapData^% data, array<Byte>^% byteArray);

			BitmapIterator^ _initBitmapIterator(array<Byte>^ bmpData, Int32 startPosition, Int32 endPosition);
	};
}