#pragma once

#include "CV .NET Extensions\LabelButton.h"

namespace CVNetExtensions
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class SimpleBox abstract
	{
		public:
			static property Color BackColor {
				public: Color get() { return _box->BackColor; }
				public: void set(Color value) { _box->BackColor = value; }
			}

			static property Color ForeColor {
				public: Color get() { return _info->ForeColor; }
				public: void set(Color value) { _info->ForeColor = value; }
			}

			static property Color ButtonNormalBackColor {
				public: Color get() { return _buttonNormalBackColor; }
				public: void set(Color value) { _buttonNormalBackColor = value; }
			}

			static property Color ButtonNormalForeColor {
				public: Color get() { return _buttonNormalForeColor; }
				public: void set(Color value) { _buttonNormalForeColor = value; }
			}

			static property Color ButtonMousedOverBackColor {
				public: Color get() { return _buttonMousedOverBackColor; }
				public: void set(Color value) { _buttonMousedOverBackColor = value; }
			}

			static property Color ButtonMousedOverForeColor {
				public: Color get() { return _buttonMousedOverForeColor; }
				public: void set(Color value) { _buttonMousedOverForeColor = value; }
			}

			static property String^ Title {
				public: String^ get() { return _box->Text; }
				public: void set(String^ value) { _box->Text = value; }
			}

			static property Font^ InfoFont {
				public: Font^ get() { return _info->Font; }
				public: void set(Font^ value) { _info->Font = value; }
			}

			static void initialize()
			{
				_box->Name = "SimpleBox";
				_box->FormBorderStyle = FormBorderStyle::FixedToolWindow;
				_box->MinimizeBox = false;
				_box->MaximizeBox = false;

				_info->Name = "SimpleBoxInfo";
				_info->Font = gcnew Font("Microsoft Sans Serif", 10, FontStyle::Bold);
				_info->AutoSize = true;
				_info->BackColor = Color::Transparent;
				_info->Location = Point(12, 12);
				_box->Controls->Add(_info);

				_info->MouseDown += gcnew MouseEventHandler(&Event_MouseDown);
				_info->MouseUp += gcnew MouseEventHandler(&Event_MouseUp);
				_info->MouseMove += gcnew MouseEventHandler(&Event_MouseMove);

				_box->MouseDown += gcnew MouseEventHandler(&Event_MouseDown);
				_box->MouseUp += gcnew MouseEventHandler(&Event_MouseUp);
				_box->MouseMove += gcnew MouseEventHandler(&Event_MouseMove);
				_box->FormClosing += gcnew FormClosingEventHandler(&Event_BoxClosing);
				_box->Load += gcnew EventHandler(&Event_BoxLoad);
			}

			static void display(String^ message)
			{
				_info->Text = message;
				_box->ClientSize = Size(_info->Left + _info->PreferredWidth + 12, _info->Top + _info->PreferredHeight + 12);
				_box->ShowDialog();
				_info->Text = "";
			}

			static void display(String^ message, String^ title)
			{
				String^ memTitle = _box->Text;
				_box->Text = title;
				display(message);
				_box->Text = memTitle;
			}

			static DialogResult displayWithResult(String^ message)
			{
				LabelButton^ _confirm = gcnew LabelButton(_box, "SimpleBoxConfirm", "Confirm");
				_confirm->changeColorsOnMouseOver = true;
				_confirm->changeColorsOnActivation = false;
				_confirm->runOnMouseClickProcedure = true;
				_confirm->isActivatedOnClick = true;
				_confirm->isOnClickDeactivatable = true;
				_confirm->normalBackColor = _buttonNormalBackColor;
				_confirm->normalForeColor = _buttonNormalForeColor;
				_confirm->mouseOverBackColor = _buttonMousedOverBackColor;
				_confirm->mouseOverForeColor = _buttonMousedOverForeColor;
				_confirm->resetDisplayedColors();
				_confirm->setFixedSize(Size(150, 20));
				_confirm->MouseClickExtension += gcnew LabelButtonMouseClickHandler(&Event_ButtonClicked);

				LabelButton^ _cancel = gcnew LabelButton(_box, "SimpleBoxCancel", "Cancel");
				_cancel->changeColorsOnMouseOver = true;
				_cancel->changeColorsOnActivation = false;
				_cancel->runOnMouseClickProcedure = true;
				_cancel->isActivatedOnClick = true;
				_cancel->isOnClickDeactivatable = true;
				_cancel->normalBackColor = _buttonNormalBackColor;
				_cancel->normalForeColor = _buttonNormalForeColor;
				_cancel->mouseOverBackColor = _buttonMousedOverBackColor;
				_cancel->mouseOverForeColor = _buttonMousedOverForeColor;
				_cancel->resetDisplayedColors();
				_cancel->setFixedSize(Size(150, 20));
				_cancel->MouseClickExtension += gcnew LabelButtonMouseClickHandler(&Event_ButtonClicked);

				_info->Text = message;

				Int32 width = _confirm->Width + _cancel->Width + 10;
				Int32 diff = width - _info->PreferredWidth;
				if(diff < 0) {
					width -= diff;
					_confirm->Location = Point(12 - (diff >> 1), _info->Top + _info->PreferredHeight + 12);
					_cancel->Location = Point(_confirm->Right + 10, _confirm->Top);
				}
				else {
					_info->Location = Point(12 + (diff >> 1), 12);
					_confirm->Location = Point(12, _info->Top + _info->PreferredHeight + 12);
					_cancel->Location = Point(_confirm->Right + 10, _confirm->Top);
				}

				_box->ClientSize = Size(width + 24, _confirm->Bottom + 12);
				_box->ShowDialog();
				_info->Text = "";
				_info->Location = Point(12, 12);

				DialogResult result;
				if(_confirm->isActivated)
					result = DialogResult::OK;
				else if(_cancel->isActivated)
					result = DialogResult::Cancel;
				else
					result = DialogResult::Abort;

				_box->Controls->Remove(_confirm);
				_box->Controls->Remove(_cancel);
				
				return result;
			}

		private:
			static bool _mouseDown = false;
			static Form^ _box = gcnew Form();
			static Label^ _info = gcnew Label();
			static Color _buttonNormalBackColor, _buttonNormalForeColor;
			static Color _buttonMousedOverBackColor, _buttonMousedOverForeColor;

			static void Event_BoxClosing(Object^ sender, FormClosingEventArgs^ e)
			{
				e->Cancel = true;
				_box->Hide();
			}

			static void Event_BoxLoad(Object^ sender, EventArgs^ e)
			{
				_box->SetDesktopLocation(Cursor::Position.X - (_box->Width >> 1), Cursor::Position.Y - (_box->Height >> 1));
			}

			static void Event_ButtonClicked(Object^ sender, MouseEventArgs^ e)
			{
				_box->Close();
			}

			static void Event_MouseDown(Object^ sender, MouseEventArgs^ e)
			{
				_mouseDown = true;
			}

			static void Event_MouseUp(Object^ sender, MouseEventArgs^ e)
			{
				_mouseDown = false;
			}

			static void Event_MouseMove(Object^ sender, MouseEventArgs^ e)
			{
				if(_mouseDown)
					_box->SetDesktopLocation(Cursor::Position.X - (_box->Width >> 1), Cursor::Position.Y - (_box->Height >> 1));
			}
	};
}