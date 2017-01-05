#pragma once

namespace CVNetExtensions
{
	using namespace System;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;

	template <class T>
	ref class TitledControl abstract
	{
		public:
		#pragma region Properties
			property bool Enabled {
				public: bool get() { return _control->Enabled; }
				public: void set(bool value)
						{
							if(_control->Enabled != value) {
								_control->Enabled = value;
								if(_control->Enabled) {
									_title->ForeColor = _titleForeColor;
									_title->BackColor = _titleBackColor;
									_control->ForeColor = _controlForeColor;
									_control->BackColor = _controlBackColor;
								}
								else {
									_title->ForeColor = _disabledForeColor;
									_title->BackColor = Color::Transparent;
									_control->ForeColor = _disabledForeColor;
									_control->BackColor = _disabledBackColor;
								}
							}
						}
			}

			property String^ Title {
				public: String^ get() { return _title->Text; }
				public: void set(String^ value) { _title->Text = value; }
			}

			property System::Drawing::Size TitleSize {
				public: System::Drawing::Size get() { return _title->PreferredSize; }
			}

			property System::Drawing::Size ControlSize {
				public: System::Drawing::Size get() { return _control->Size; }
				public: void set(System::Drawing::Size value) { return _control->Size = value; }
			}

			property System::Drawing::Size Size {
				public: System::Drawing::Size get() { return System::Drawing::Size(Width, Height); }
			}

			property Int32 Width {
				public: Int32 get() { return Right - Left; }
			}

			property Int32 Height {
				public: Int32 get() { return Bottom - Top; }
			}

			property Int32 TitleRight {
				public: Int32 get() { return _title->Right; }
			}

			property Int32 ControlRight {
				public: Int32 get() { return _control->Right; }
			}

			property Int32 Right {
				public: Int32 get() { return Math::Max(TitleRight, ControlRight); }
			}

			property Int32 TitleLeft {
				public: Int32 get() { return _title->Left; }
			}

			property Int32 ControlLeft {
				public: Int32 get() { return _control->Left; }
			}

			property Int32 Left {
				public: Int32 get() { return Math::Min(TitleLeft, ControlLeft); }
			}

			property Int32 TitleBottom {
				public: Int32 get() { return _title->Bottom; }
			}

			property Int32 ControlBottom {
				public: Int32 get() { return _control->Bottom; }
			}

			property Int32 Bottom {
				public: Int32 get() { return Math::Max(TitleBottom, ControlBottom); }
			}

			property Int32 TitleTop {
				public: Int32 get() { return _title->Top; }
			}

			property Int32 ControlTop {
				public: Int32 get() { return _control->Top; }
			}

			property Int32 Top {
				public: Int32 get() { return Math::Min(TitleTop, ControlTop); }
			}

			property bool Visible {
				public: bool get() { return _title->Visible; }
				public: void set(bool value) { _title->Visible = value; _control->Visible = value; }
			}

			property Int32 VerticalAllignment {
				public: Int32 get() { return (_title->Height - _control->Height) >> 1; }
			}

			property Int32 HorizontalAllignment {
				public: Int32 get() { return ((_title->PreferredWidth - _control->Width) >> 1) - _title->PreferredWidth; }
			}

			property Color TitleForeColor {
				public: Color get() { return _titleForeColor; }
				public: void set(Color value)
						{
							_titleForeColor = value;
							if(_control->Enabled)
								_title->ForeColor = value;
						}
			}

			property Color TitleBackColor {
				public: Color get() { return _titleBackColor; }
				public: void set(Color value)
						{
							_titleBackColor = value;
							if(_control->Enabled)
								_title->BackColor = value;
						}
			}

			property Color ControlForeColor {
				public: Color get() { return _controlForeColor; }
				public: void set(Color value)
						{
							_controlForeColor = value;
							if(_control->Enabled)
								_control->ForeColor = value;
						}
			}

			property Color ControlBackColor {
				public: Color get() { return _controlBackColor; }
				public: void set(Color value)
						{
							_controlBackColor = value;
							if(_control->Enabled)
								_control->BackColor = value;
						}
			}

			property Color DisabledForeColor {
				public: Color get() { return _disabledForeColor; }
				public: void set(Color value)
						{
							_disabledForeColor = value;
							if(!_control->Enabled) {
								_title->ForeColor = value;
								_control->ForeColor = value;
							}
						}
			}

			property Color DisabledBackColor {
				public: Color get() { return _disabledBackColor; }
				public: void set(Color value)
						{
							_disabledBackColor = value;
							if(!_control->Enabled)
								_control->BackColor = value;
						}
			}

			property Font^ TitleFont {
				public: Font^ get() { return _title->Font; }
				public: void set(Font^ value) { _title->Font = value; }
			}

			property Point Offset {
				public: Point get() { return Point(ControlLeft - TitleRight, ControlTop - TitleTop); }
				public: void set(Point value) { _control->Location = Point(TitleRight + value.X, TitleTop + value.Y); }
			}

			property Point Location {
				public: Point get() { return _title->Location; }
				public: void set(Point value)
						{
							Point offset = Offset;
							_title->Location = value;
							_control->Location = Point(TitleRight + offset.X, TitleTop + offset.Y);
						}
			}
		#pragma endregion

			TitledControl(Control^ parent, String^ name);
			TitledControl(Control^ parent, String^ name, String^ title);

			void removeFromParent();

		protected:
			Label^ _title;
			T^ _control;

			Color _titleBackColor;
			Color _titleForeColor;
			Color _controlBackColor;
			Color _controlForeColor;
			Color _disabledBackColor;
			Color _disabledForeColor;

		private:
			void _initialize(String^ name);
	};

	template <class T>
	TitledControl<T>::TitledControl(Control^ parent, String^ name)
	{
		_initialize(name);
		parent->Controls->Add(_title);
		parent->Controls->Add(_control);
	}

	template <class T>
	TitledControl<T>::TitledControl(Control^ parent, String^ name, String^ title)
	{
		_initialize(name);
		parent->Controls->Add(_title);
		parent->Controls->Add(_control);
		_title->Text = title;
	}

	template <class T>
	void TitledControl<T>::removeFromParent()
	{
		Control^ parent = _title->Parent;
		parent->Controls->Remove(_title);
		parent->Controls->Remove(_control);
	}

	template <class T>
	void TitledControl<T>::_initialize(String^ name)
	{
		_title = gcnew Label();
		_title->AutoSize = true;
		_title->BackColor = Color::Transparent;
		_title->Font = gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.5f, FontStyle::Bold, GraphicsUnit::Point, (Byte)(238));
		_title->Name = name + "_TITLE";

		_control = gcnew T();
		_control->Name = name + "_CONTROL";

		_titleForeColor = _title->ForeColor;
		_titleBackColor = _title->BackColor;
		_controlForeColor = _control->ForeColor;
		_controlBackColor = _control->BackColor;
		_disabledForeColor = Color::DarkGray;
		_disabledBackColor = Color::LightGray;
	}
}