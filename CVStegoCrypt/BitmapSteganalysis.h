#pragma once

#include "BitmapStegoBase.h"
#include "AnalysisPotentialOutcome.h"
#include "TwinBitmapIterator.h"

namespace CVStegoCrypt
{
	using namespace System;
	using namespace System::Drawing;
	using namespace System::IO;

	public ref class BitmapSteganalysis : public BitmapStegoBase
	{
		public:
		#pragma region Properties
			property String^ OriginalBitmapPath {
				public: String^ get() { return _originalBmpPath; }
			}

			property String^ CarryBitmapPath {
				public: String^ get() { return _carryBmpPath; }
			}

			property bool OriginalBitmapLoaded {
				public: bool get() { return (_originalBitmap != nullptr); }
			}

			property bool CarryBitmapLoaded {
				public: bool get() { return (_carryBitmap != nullptr); }
			}

			property Int32 OriginalBitmapPixelCount {
				public: Int32 get() { return OriginalBitmapSize.Width * OriginalBitmapSize.Height; }
			}

			property Int32 OriginalBitmapLength {
				public: Int32 get() { return OriginalBitmapPixelCount * 3; }
			}

			property Size OriginalBitmapSize {
				public: Size get() { return (_originalBitmap != nullptr) ? _originalBitmap->Size : Size(0, 0); }
			}

			property Int32 CarryBitmapPixelCount {
				public: Int32 get() { return CarryBitmapSize.Width * CarryBitmapSize.Height; }
			}

			property Int32 CarryBitmapLength {
				public: Int32 get() { return CarryBitmapPixelCount * 3; }
			}

			property Size CarryBitmapSize {
				public: Size get() { return (_carryBitmap != nullptr) ? _carryBitmap->Size : Size(0, 0); }
			}
		#pragma endregion

			BitmapSteganalysis(StegoBitsNominator^ red, StegoBitsNominator^ green, StegoBitsNominator^ blue);

			List<AnalysisPotentialOutcome^>^ compareBitmaps(UInt32 startPosition, UInt32 endPosition, UInt32 maxUnchangedBits, UInt32% out_firstDiffPos, UInt32% out_lastDiffPos);
			List<AnalysisOutcome^>^ analizeCarry(UInt32 startPosition, UInt32 endPosition, UInt32 minSize, UInt32 maxSize);
			bool guessNomimatedBits();

			bool loadOriginalBitmap(String^ filePath);
			bool loadCarryBitmap(String^ filePath);

			Bitmap^ getOriginalBitmap();
			Bitmap^ getCarryBitmap();

			void clear();

		private:
			ref class AnalysisBuffer
			{
				public:
					Byte current;
					Int32 cycles;
					UInt64 bits;
					array<UInt32>^ positions;

					AnalysisBuffer();
					
					void setNext(BitmapIterator^ i, bool value);
					void swapBits();
					UInt32 getIntStartingFrom(Byte bit);

				private:
					array<UInt32>^ _posBuffer;
			};

			String^ _originalBmpPath;
			String^ _carryBmpPath;
			array<Byte>^ _carryBmpData;
			array<Byte>^ _originalBmpData;
			Bitmap^ _originalBitmap;
			Bitmap^ _carryBitmap;

			Byte _analizeNext32Bits(BitmapIterator^ i, AnalysisBuffer^ buffer);

			void _trySetBitNominators();
	};
}

