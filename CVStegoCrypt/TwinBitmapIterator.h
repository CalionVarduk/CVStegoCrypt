#pragma once

#include "BitmapIterator.h"

namespace CVStegoCrypt
{
	using namespace System;

	public ref class TwinBitmapIterator
	{
		public:
			property array<Byte>^ BitmapData1 {
				public: array<Byte>^ get() { return _iterator->LinkedData; }
				public: void set(array<Byte>^ value) { _iterator->LinkedData = value; }
			};

			property array<Byte>^ BitmapData2 {
				public: array<Byte>^ get() { return _data2; }
				public: void set(array<Byte>^ value) { _data2 = value; }
			};

			property Int32 iByte {
				public: Int32 get() { return _iterator->iByte; }
				public: void set(Int32 value) { _iterator->iByte = value; }
			};

			property Byte iBit {
				public: Byte get() { return _iterator->iBit; }
				public: void set(Byte value) { _iterator->iBit = value; }
			};

			property Int32 EndByte {
				public: Int32 get() { return _iterator->EndByte; }
				public: void set(Int32 value) { _iterator->EndByte = value; }
			};

			property bool CurrentBit1 {
				public: bool get() { return _iterator->CurrentBit; }
				public: void set(bool value) { _iterator->CurrentBit = value; }
			}

			property bool CurrentBit2 {
				public: bool get() { return ((_data2[iByte] & BitIterator::TrueBitMasks[iBit]) > 0); }
				public: void set(bool value)
						{
							if(value) _data2[iByte] |= BitIterator::TrueBitMasks[iBit];
							else _data2[iByte] &= BitIterator::FalseBitMasks[iBit];
						}
			}

			property Byte CurrentBitMask {
				public: Byte get() { return _iterator->CurrentBitMask; }
			}

			property bool isLinked {
				public: bool get() { return _iterator->isLinked && _data2 != nullptr; }
			}

			property bool isCorrectlyLinked {
				public: bool get() { return isLinked && (_iterator->LinkedData->Length == _data2->Length); }
			}

			property bool isDone {
				public: bool get() { return _iterator->isDone; }
			}

			property bool isInitialized {
				public: bool get() { return _data2 != nullptr && _iterator->isInitialized; }
			}

			property BitmapIterator^ Iterator {
				public: BitmapIterator^ get() { return _iterator; }
			}

			TwinBitmapIterator();
			TwinBitmapIterator(array<Byte>^ bmpData1, array<Byte>^ bmpData2);
			TwinBitmapIterator(array<Byte>^ bmpData1, array<Byte>^ bmpData2, Int32 startByte);
			TwinBitmapIterator(array<Byte>^ bmpData1, array<Byte>^ bmpData2, Int32 startByte, Int32 endByte);

			void initializeColorMasks(StegoBitsNominator^ red, StegoBitsNominator^ green, StegoBitsNominator^ blue);

			void swapData();

			void iterate();

		protected:
			BitmapIterator^ _iterator;
			array<Byte>^ _data2;
	};
}