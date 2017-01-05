#pragma once

#include "SteganographyForm.h"
#include "SteganalysisForm.h"
#include "AboutForm.h"

namespace CVStegoCrypt
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace CVNetExtensions;

	public ref class MainForm : public AutoValidatedForm
	{
	public:
		MainForm()
		{
			InitializeComponent();
			_initializeUI();

			steganographyModule = gcnew SteganographyForm(buttonSteganography);
			steganalysisModule = gcnew SteganalysisForm(buttonSteganalysis);
			aboutForm = gcnew AboutForm(titleStegoCrypt);

			FormClosing += gcnew FormClosingEventHandler(this, &MainForm::Event_FormClosing);
		}

	protected:
		~MainForm()
		{
			if (components)
				delete components;
		}

	private:
		LabelButton^ titleStegoCrypt;
		LabelButton^ buttonSteganography;
		LabelButton^ buttonSteganalysis;

		SteganographyForm^ steganographyModule;
		SteganalysisForm^ steganalysisModule;
		AboutForm^ aboutForm;

		System::ComponentModel::Container ^components;

		void Event_FormClosing(Object^ sender, FormClosingEventArgs^ e)
		{
			System::Windows::Forms::DialogResult result = SimpleBox::displayWithResult("Do you really want to close CV StegoCrypt? :(");
			if(result == System::Windows::Forms::DialogResult::Cancel || result == System::Windows::Forms::DialogResult::Abort)
				e->Cancel = true;
		}

#pragma region Custom Initialization
		void _initializeUI()
		{
			_initializeTitle();
			_initializeButtons();
			ClientSize = _initializeLocationsAndSize();
		}

		void _initializeTitle()
		{
			titleStegoCrypt = gcnew LabelButton(this, "titleStegoCrypt", "CV StegoCrypt");
			titleStegoCrypt->AutoSize = true;
			titleStegoCrypt->BorderStyle = System::Windows::Forms::BorderStyle::None;
			titleStegoCrypt->Font = gcnew System::Drawing::Font("Microsoft Sans Serif", 20, FontStyle::Italic | FontStyle::Bold);
			titleStegoCrypt->changeColorsOnActivation = true;
			titleStegoCrypt->changeColorsOnMouseOver = true;

			titleStegoCrypt->normalBackColor = Color::Transparent;
			titleStegoCrypt->normalForeColor = ColorExtensions::GetScaled(Color::LightSkyBlue, 0.9f);
			titleStegoCrypt->mouseOverBackColor = Color::Transparent;
			titleStegoCrypt->mouseOverForeColor = Color::LightSkyBlue;
			titleStegoCrypt->activeBackColor = Color::Transparent;
			titleStegoCrypt->activeForeColor = Color::LightSkyBlue;
			titleStegoCrypt->activeMouseOverBackColor = Color::Transparent;
			titleStegoCrypt->activeMouseOverForeColor = Color::LightSkyBlue;
			titleStegoCrypt->resetDisplayedColors();
		}

		void _initializeButtons()
		{
			buttonSteganography = gcnew LabelButton(this, "buttonSteganography", "Steganography Module");
			buttonSteganography->Font = gcnew System::Drawing::Font("Microsoft Sans Serif", 12, FontStyle::Bold);
			buttonSteganography->setFixedSize(System::Drawing::Size(250, 25));
			buttonSteganography->Location = Point(12, titleStegoCrypt->PreferredHeight + 28);
			ButtonExtensions::setColorScheme(buttonSteganography, Color::LightSkyBlue);

			buttonSteganalysis = gcnew LabelButton(this, "buttonSteganalysis", "Steganalysis Module");
			buttonSteganalysis->Font = gcnew System::Drawing::Font("Microsoft Sans Serif", 12, FontStyle::Bold);
			buttonSteganalysis->setFixedSize(System::Drawing::Size(250, 25));
			buttonSteganalysis->Location = Point(buttonSteganography->Right + 9, buttonSteganography->Top);
			ButtonExtensions::setColorScheme(buttonSteganalysis, Color::LightSkyBlue);
		}

		System::Drawing::Size _initializeLocationsAndSize()
		{
			System::Drawing::Size clientSize;

			buttonSteganography->Location = Point(12, titleStegoCrypt->PreferredHeight + 28);
			buttonSteganalysis->Location = Point(buttonSteganography->Right + 9, buttonSteganography->Top);

			clientSize.Width = buttonSteganalysis->Right + 12;
			clientSize.Height = buttonSteganalysis->Bottom + 12;

			titleStegoCrypt->Location = Point((clientSize.Width - titleStegoCrypt->PreferredWidth) >> 1, 12);

			return clientSize;
		}
#pragma endregion

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->components = gcnew System::ComponentModel::Container();
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->BackColor = Color::FromArgb(50, 50, 60);
			this->Text = L"CV StegoCrypt";
			this->Name = L"MainForm";
			this->Padding = System::Windows::Forms::Padding(0);
		}
#pragma endregion
	};
}
