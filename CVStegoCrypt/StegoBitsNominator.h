#pragma once

#include "CVNetExtensions.h"

namespace CVStegoCrypt
{
	using namespace System;
	using namespace System::Windows::Forms;

	using namespace CVNetExtensions;

	public ref class StegoBitsNominator
	{
		public:
			property bool Enabled {
				public: bool get() { return _boxes[0]->Enabled; }
				public: void set(bool value)
						{
							if(_boxes[0]->Enabled != value) {
								for(Int32 i = 0; i < _boxes->Length; ++i)
									_boxes[i]->Enabled = value;

								_title->ForeColor = (_boxes[0]->Enabled) ? _boxes[0]->TitleForeColor : _boxes[0]->DisabledForeColor;
							}
						}
			}

			property Point Location {
				public: Point get() { return Point(_boxes[0]->Left, _title->Top); }
				public: void set(Point value)
						{
							Point location = Location;
							Point delta = Point(value.X - location.X, value.Y - location.Y);

							_title->Left += delta.X;
							_title->Top += delta.Y;
							for(Int32 i = 0; i < _boxes->Length; ++i)
								_boxes[i]->Location = Point(_boxes[i]->Location.X + delta.X, _boxes[i]->Location.Y + delta.Y);
						}
			}

			property Int32 Left {
				public: Int32 get() { return _boxes[0]->Left; }
			}

			property Int32 Right {
				public: Int32 get() { return _boxes[_boxes->Length - 1]->Right; }
			}

			property Int32 Top {
				public: Int32 get() { return _title->Top; }
			}

			property Int32 Bottom {
				public: Int32 get() { return _boxes[0]->Bottom; }
			}

			property System::Drawing::Size Size {
				public: System::Drawing::Size get() { return System::Drawing::Size(Width, Height); }
			}

			property System::Drawing::Size BoxSize {
				public: System::Drawing::Size get() { return _boxes[0]->ControlSize; }
			}

			property Int32 Width {
				public: Int32 get() { return _boxes[_boxes->Length - 1]->Right - _boxes[0]->Left; }
			}

			property Int32 Height {
				public: Int32 get() { return _boxes[0]->Bottom - _title->Top; }
			}

			property Byte SetBitsCount {
				public: Byte get()
						{
							Byte bitsCount = 0;
							for(Byte i = 0; i < 8; ++i)
								if(_boxes[i]->isActivated) ++bitsCount;
							return bitsCount;
						}
			}

			StegoBitsNominator(Control^ parent, String^ name, String^ title, Color titleColor);

			void addMouseClickEvent(CheckBoxMouseClickHandler^ handler);

			void set(Byte i, bool switchOn);
			void setLsb();
			void reset();

			bool operator[] (Byte i);
			Byte getCode();

			void validateAll();

		private:
			Label^ _title;
			array<TitledCheckBox^>^ _boxes;
	};
}