#pragma once

#include "BitmapSteganography.h"

namespace CVStegoCrypt
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace CVNetExtensions;

	public ref class DisplayInfoForm : public SecondaryForm
	{
	public:
		property bool DisplayFilePath {
			public: bool get() { return _displayPath; }
			public: void set(bool value) { _displayPath = value; }
		}

		property bool LoadedBitmapMode {
			public: bool get() { return (box != nullptr && _displayPath); }
		}

		property bool CreatedBitmapMode {
			public: bool get() { return (box != nullptr && !_displayPath); }
		}

		property bool LoadedInfoMode {
			public: bool get() { return (box == nullptr); }
		}

		DisplayInfoForm(LabelButton^ controlButton, String^ title, bool displayBitmap) : SecondaryForm(controlButton)
		{
			InitializeComponent();
			_initializeUI(displayBitmap);
			Text = title;
			_displayPath = true;
		}

		void setup(BitmapSteganography^ steganography)
		{
			if(LoadedInfoMode) {
				info->Location = Point(12, 12);
				info[0]->Visible = true;
				info[0]->Text = steganography->InfoPath;
				info[1]->Text = steganography->InfoLength.ToString("N0") + " B";
			}
			else if(LoadedBitmapMode) {
				info->Location = Point(12, 12);
				box->Location = Point(12, info->Bottom + 10);

				box->Size = steganography->LoadedBitmapSize;
				box->Image = steganography->getLoadedBitmap();

				info[0]->Visible = true;
				info[0]->Text = steganography->BitmapPath;
				info[1]->Text = steganography->LoadedBitmapLength.ToString("N0") + " B";
				info[2]->Text = steganography->LoadedBitmapSize.Width.ToString() + " x " + steganography->LoadedBitmapSize.Height.ToString();
				info[3]->Text = steganography->LoadedBitmapPixelCount.ToString("N0");
			}
			else {
				info->Location = Point(12, 12 - info[0]->TitleSize.Height);
				box->Location = Point(12, info->Bottom + 10);

				box->Size = steganography->CreatedBitmapSize;
				box->Image = steganography->getCreatedBitmap();

				info[0]->Visible = false;
				info[1]->Text = steganography->CreatedBitmapLength.ToString("N0") + " B";
				info[2]->Text = steganography->CreatedBitmapSize.Width.ToString() + " x " + steganography->CreatedBitmapSize.Height.ToString();
				info[3]->Text = steganography->CreatedBitmapPixelCount.ToString("N0");
			}

			_setClientSize();
		}

	protected:
		~DisplayInfoForm()
		{
			if (components)
				delete components;
		}

	private:
		PictureBox^ box;
		TitledLabelList^ info;
		bool _displayPath;

		System::ComponentModel::Container ^components;

		void _setClientSize()
		{
			info->fixSize();
			if(LoadedInfoMode)
				ClientSize = System::Drawing::Size(info->Right + 12, info->Bottom + 12);
			else
				ClientSize = System::Drawing::Size(((info->Right > box->Right) ? info->Right : box->Right) + 12, box->Bottom + 12);
		}

#pragma region Custom Initialization
		void _initializeUI(bool displayBitmap)
		{
			info = gcnew TitledLabelList(this, 0, Point(20, 0));

			info->addControl("infoPath", "File path:");
			info->addControl("infoSize", "File size:");

			if(displayBitmap) {
				info->addControl("infoDimensions", "Image size:");
				info->addControl("infoPixels", "Image pixel count:");

				box = gcnew PictureBox();
				box->BackColor = Color::White;
				Controls->Add(box);
			}
			else box = nullptr;

			for(Int32 i = 0; i < info->Count; ++i) {
				info[i]->TitleFont = gcnew System::Drawing::Font("Microsoft Sans Serif", 8, FontStyle::Bold);
				info[i]->LabelFont = info[i]->TitleFont;
				info[i]->ControlForeColor = Color::LightGreen;
				info[i]->TitleForeColor = Color::LightSkyBlue;
			}

			info->align();
			info->Location = Point(12, 12);

			if(displayBitmap) box->Location = Point(12, info->Bottom + 10);
		}
#pragma endregion

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->components = gcnew System::ComponentModel::Container();
			this->BackColor = Color::FromArgb(50, 50, 60);
			this->Name = "DisplayInfoForm";
			this->Padding = System::Windows::Forms::Padding(0);
		}
#pragma endregion
	};
}
