#pragma once

#include "StegoBitsNominator.h"
#include "BitIterator.h"

namespace CVStegoCrypt
{
	using namespace System;

	public ref class BitmapIterator : public BitIterator
	{
		public:
			property bool isInitialized {
				public: bool get()
						{
							return (isLinked && _colorBits[0] != nullptr && _colorBits[1] != nullptr && _colorBits[2] != nullptr &&
									(_colorBits[0]->Length > 0 || _colorBits[1]->Length > 0 || _colorBits[2]->Length > 0));
						}
			}

			BitmapIterator();
			BitmapIterator(array<Byte>^ linkedData);
			BitmapIterator(array<Byte>^ linkedData, Int32 startByte);
			BitmapIterator(array<Byte>^ linkedData, Int32 startByte, Int32 endByte);

			void initializeColorBits(StegoBitsNominator^ red, StegoBitsNominator^ green, StegoBitsNominator^ blue);

			void iterate();

		protected:
			array<array<Byte>^>^ _colorBits;
			Byte _iColor;
			Byte _colorBit;

			void _checkColorIndex();
	};
}