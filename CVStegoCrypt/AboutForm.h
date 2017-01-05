#pragma once

#include "CVNetExtensions.h"

namespace CVStegoCrypt
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace CVNetExtensions;

	public ref class AboutForm : public SecondaryForm
	{
	public:
		AboutForm(LabelButton^ controlButton) : SecondaryForm(controlButton)
		{
			InitializeComponent();

		}

	protected:
		~AboutForm()
		{
			if (components)
				delete components;
		}

	private:

		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->components = gcnew System::ComponentModel::Container();
			this->BackColor = Color::FromArgb(50, 50, 60);
			this->Text = L"About CV StegoCrypt";
			this->Name = L"AboutForm";
			this->Padding = System::Windows::Forms::Padding(0);
		}
#pragma endregion
	};
}
