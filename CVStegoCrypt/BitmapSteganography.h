#pragma once

#include "BitmapStegoBase.h"

namespace CVStegoCrypt
{
	using namespace System;
	using namespace System::Drawing;
	using namespace System::IO;

	public ref class BitmapSteganography : public BitmapStegoBase
	{
		public:
			enum class EraseMode { Zeros, Ones, Random };

		#pragma region Properties
			property String^ BitmapPath {
				public: String^ get() { return _bmpPath; }
			}

			property String^ InfoPath {
				public: String^ get() { return _infoPath; }
			}

			property bool BitmapLoaded {
				public: bool get() { return (_loadedBitmap != nullptr); }
			}

			property bool BitmapCreated {
				public: bool get() { return (_createdBitmap != nullptr); }
			}

			property bool InfoLoaded {
				public: bool get() { return (_info->Length > 0); }
			}

			property Int32 InfoLength {
				public: Int32 get() { return _info->Length; }
			}

			property Int32 LoadedBitmapPixelCount {
				public: Int32 get() { return LoadedBitmapSize.Width * LoadedBitmapSize.Height; }
			}

			property Int32 LoadedBitmapLength {
				public: Int32 get() { return LoadedBitmapPixelCount * 3; }
			}

			property Size LoadedBitmapSize {
				public: Size get() { return (_loadedBitmap != nullptr) ? _loadedBitmap->Size : Size(0, 0); }
			}

			property Int32 CreatedBitmapPixelCount {
				public: Int32 get() { return CreatedBitmapSize.Width * CreatedBitmapSize.Height; }
			}

			property Int32 CreatedBitmapLength {
				public: Int32 get() { return CreatedBitmapPixelCount * 3; }
			}

			property Size CreatedBitmapSize {
				public: Size get() { return (_createdBitmap != nullptr) ? _createdBitmap->Size : Size(0, 0); }
			}

			property Int32 MaxInfoLength {
				public: Int32 get()
						{
							Int32 maxBitLength = LoadedBitmapPixelCount * BitsPerPixel;
							return (maxBitLength >> 3) - 4;
						}
			}

			property Int32 MaxStartPosition {
				public: Int32 get() { return _getMaxStartPosition(InfoLength + 4); }
			}
		#pragma endregion

			BitmapSteganography(StegoBitsNominator^ red, StegoBitsNominator^ green, StegoBitsNominator^ blue);

			void conceal(Int32 startPosition);
			bool extract(Int32 startPosition);
			void erase(EraseMode mode);

			Int32 loadInformation(String^ filePath);
			void saveInformation(String^ filePath);

			bool loadBitmap(String^ filePath);
			void saveBitmap(String^ filePath);

			Bitmap^ getLoadedBitmap();
			Bitmap^ getCreatedBitmap();

			void clear();

		private:
			String^ _bmpPath;
			String^ _infoPath;
			array<Byte>^ _info;
			array<Byte>^ _bmpData;
			Bitmap^ _loadedBitmap;
			Bitmap^ _createdBitmap;

			UInt32 _getMaxStartPosition(UInt32 byteLength);

			UInt32 _parseInfoLength();
			void _prepareInfoToConceal();

			void _concealInfo(Int32 startPosition);
			bool _tryExtractInfo(Int32 startPosition);
			void _eraseInfo(EraseMode mode);
	};
}