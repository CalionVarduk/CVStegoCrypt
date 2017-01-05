#pragma once

namespace CVStegoCrypt
{
	using namespace System;

	public ref class BitIterator
	{
		public:
			property array<Byte>^ LinkedData {
				public: array<Byte>^ get() { return _data; }
				public: void set(array<Byte>^ value)
						{
							_data = value;
							iByte = _iByte;
							EndByte =_endByte;
						}
			};

			property Int32 iByte {
				public: Int32 get() { return _iByte; }
				public: void set(Int32 value)
						{
							_iByte = (!isLinked || value < 0) ? 0 : (value < _data->Length) ? value : _data->Length;
						}
			};

			property Byte iBit {
				public: Byte get() { return _iBit; }
				public: void set(Byte value) { _iBit = (value < 8) ? value : 7; }
			};

			property Int32 EndByte {
				public: Int32 get() { return _endByte; }
				public: void set(Int32 value)
						{
							_endByte = (!isLinked || value < 0) ? -1 : (value < _data->Length) ? value : _data->Length - 1;
						}
			};

			property bool CurrentBit {
				public: bool get() { return ((_data[_iByte] & TrueBitMasks[_iBit]) > 0); }
				public: void set(bool value)
						{
							if(value) _data[_iByte] |= TrueBitMasks[_iBit];
							else _data[_iByte] &= FalseBitMasks[_iBit];
						}
			}

			property Byte CurrentBitMask {
				public: Byte get() { return TrueBitMasks[_iBit]; }
			}

			property bool isLinked {
				public: bool get() { return (_data != nullptr); }
			}

			property bool isDone {
				public: bool get() { return (_iByte > _endByte); }
			}

			static initonly array<Byte>^ TrueBitMasks = gcnew array<Byte>{ 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
			static initonly array<Byte>^ FalseBitMasks = gcnew array<Byte>{ 0xFE, 0xFD, 0xFB, 0xF7, 0xEF, 0xDF, 0xBF, 0x7F};

			BitIterator();
			BitIterator(array<Byte>^ linkedData);
			BitIterator(array<Byte>^ linkedData, Int32 startByte);
			BitIterator(array<Byte>^ linkedData, Int32 startByte, Int32 endByte);

			void iterate();

		protected:
			array<Byte>^ _data;
			Int32 _endByte;
			Int32 _iByte;
			Byte _iBit;
	};
}