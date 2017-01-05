#pragma once

#include "TitledControl.h"

namespace CVNetExtensions
{
	using namespace System;
	using namespace System::Windows::Forms;
	using namespace System::Drawing;

	ref class TitledLabel : public TitledControl<Label>
	{
		public:
			property String^ Text {
				public: String^ get() { return _control->Text; }
				public: void set(String^ value) { _control->Text = value; }
			}

			property Font^ LabelFont {
				public: Font^ get() { return _control->Font; }
				public: void set(Font^ value) { _control->Font = value; }
			}

			property ContentAlignment TextAlign {
				public: ContentAlignment get() { return _control->TextAlign; }
				public: void set(ContentAlignment value) { _control->TextAlign = value; }
			}

			property bool AutoSize {
				public: bool get() { return _control->AutoSize; }
				public: void set(bool value) { _control->AutoSize = value; }
			}

			TitledLabel(Control^ parent, String^ name);
			TitledLabel(Control^ parent, String^ name, String^ title);

		private:
			void _initializeLabel(String^ name);
	};
}