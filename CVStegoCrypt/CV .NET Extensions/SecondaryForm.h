#pragma once

#include "AutoValidatedForm.h"
#include "LabelButton.h"

namespace CVNetExtensions
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class SecondaryForm : public AutoValidatedForm
	{
	public:
		property bool isShown {
			public: bool get() { return _isShown; }
			private: void set(bool value) { _isShown = value; }
		}

		property bool isModal {
			public: bool get() { return _isModal; }
			public: void set(bool value) { _isModal = value; }
		}

		SecondaryForm(LabelButton^ controlButton) : AutoValidatedForm()
		{
			InitializeComponent();
			_controlButton = controlButton;
			if(_controlButton != nullptr) {
				_controlButton->runOnMouseClickProcedure = true;
				_controlButton->isActivatedOnClick = true;
				_controlButton->isOnClickDeactivatable = false;
				_controlButton->MouseClickExtension += gcnew LabelButtonMouseClickHandler(this, &SecondaryForm::Event_ControlButtonClicked);
			}
			isShown = false;
			isModal = false;
		}

		void display()
		{
			if(isShown) {
				System::Media::SystemSounds::Beep->Play();
				Focus();
			}
			else {
				isShown = true;
				if(isModal) ShowDialog();
				else Show();
				Refresh();
				_customValidation();
			}
		}

		bool closeIfShown()
		{
			if(_controlButton->isActivated) {
				Close();
				return true;
			}
			return false;
		}

	protected:
		~SecondaryForm()
		{
			if (components)
				delete components;
		}

	private:
		LabelButton^ _controlButton;
		bool _isShown;
		bool _isModal;
		System::ComponentModel::Container^ components;

		void Event_ControlButtonClicked(Object^ sender, MouseEventArgs^ e)
		{ display(); }

		void Event_FormClosing(Object^ sender, FormClosingEventArgs^ e)
		{
			e->Cancel = true;
			if(_controlButton != nullptr)
				_controlButton->forceDeactivate();
			isShown = false;
			Hide();
		}

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->SuspendLayout();
			// 
			// SecondaryForm
			// 
			this->components = gcnew System::ComponentModel::Container();
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"SecondaryForm";
			this->Text = L"SecondaryForm";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &SecondaryForm::Event_FormClosing);
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
