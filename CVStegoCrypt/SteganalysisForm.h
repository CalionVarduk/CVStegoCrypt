#pragma once

#include "BitmapSteganalysis.h"

namespace CVStegoCrypt
{
	using namespace System;
	using namespace System::Text;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace CVNetExtensions;

	public ref class SteganalysisForm : public SecondaryForm
	{
	public:
		SteganalysisForm(LabelButton^ controlButton) : SecondaryForm(controlButton)
		{
			InitializeComponent();
			_initializeUI();

			steganalysis = gcnew BitmapSteganalysis(bitsRed, bitsGreen, bitsBlue);
		}

	protected:
		~SteganalysisForm()
		{
			if (components)
				delete components;
		}

		virtual void _customValidation() override
		{
			bitsRed->validateAll();
			bitsGreen->validateAll();
			bitsBlue->validateAll();
		}

	private:
		BitmapSteganalysis^ steganalysis;

		LabelButton^ buttonLoadOriginal;
		LabelButton^ buttonLoadCarry;
		LabelButton^ buttonAction;
		LabelButton^ buttonGuess;
		LabelButton^ buttonTryHard;

		StegoBitsNominator^ bitsRed;
		StegoBitsNominator^ bitsGreen;
		StegoBitsNominator^ bitsBlue;

		TitledTextBoxList^ inputBoxes;

		TextBox^ outcomeBox;

		System::ComponentModel::Container ^components;

		void Event_ButtonLoadOriginalClicked(Object^ sender, MouseEventArgs^ e)
		{
			OpenFileDialog^ dialog = gcnew OpenFileDialog();
            dialog->Filter = "Bitmap |*.bmp";
            dialog->ShowDialog();

			if(dialog->FileName != "") {
				steganalysis->loadOriginalBitmap(dialog->FileName);
				outcomeBox->Text = "";
				inputBoxes[0]->Text = "";
				inputBoxes[1]->Text = "";
				inputBoxes[2]->Text = "";
				inputBoxes[3]->Text = "";
				inputBoxes[4]->Text = "";
			}
		}

		void Event_ButtonLoadCarryClicked(Object^ sender, MouseEventArgs^ e)
		{
			OpenFileDialog^ dialog = gcnew OpenFileDialog();
            dialog->Filter = "Bitmap |*.bmp";
            dialog->ShowDialog();

			if(dialog->FileName != "") {
				steganalysis->loadCarryBitmap(dialog->FileName);
				outcomeBox->Text = "";
				inputBoxes[0]->Text = "1";
				inputBoxes[1]->Text = steganalysis->CarryBitmapLength.ToString();
				inputBoxes[2]->Text = "1";
				inputBoxes[3]->Text = steganalysis->CarryBitmapLength.ToString();
				inputBoxes[4]->Text = steganalysis->CarryBitmapLength.ToString();
			}
		}

		void Event_ButtonActionClicked(Object^ sender, MouseEventArgs^ e)
		{
			UInt32 startPos, endPos, minSize, maxSize, maxUnchangedBits, first, last;
			parseInputBoxes(startPos, endPos, minSize, maxSize, maxUnchangedBits);
			List<AnalysisPotentialOutcome^>^ outcomes = steganalysis->compareBitmaps(startPos, endPos, maxUnchangedBits, first, last);

			SimpleBox::display("First: " + first + ", Last: " + last);

			if(outcomes->Count > 0) {
				StringBuilder^ outcStr = gcnew StringBuilder("Count: " + outcomes->Count + "\r\n");
				for(Int32 i = 0; i < outcomes->Count; ++i)
					outcStr->Append((i + 1).ToString() + ". " + outcomes[i]->ToString() + "\r\n");

				outcomeBox->Text = outcStr->ToString();
			}
			else SimpleBox::display("Nope");
		}

		void Event_ButtonTryHardClicked(Object^ sender, MouseEventArgs^ e)
		{
			UInt32 startPos, endPos, minSize, maxSize, maxUnchangedBits;
			parseInputBoxes(startPos, endPos, minSize, maxSize, maxUnchangedBits);
			List<AnalysisOutcome^>^ outcomes = steganalysis->analizeCarry(startPos, endPos, minSize, maxSize);

			if(outcomes->Count > 0) {
				StringBuilder^ outcStr = gcnew StringBuilder("Count: " + outcomes->Count + "\r\n");
				for(Int32 i = 0; i < outcomes->Count; ++i)
					outcStr->Append((i + 1).ToString() + ". " + outcomes[i]->ToString() + "\r\n");

				outcomeBox->Text = outcStr->ToString();
			}
			else SimpleBox::display("Nope");
		}

		void Event_ButtonGuessClicked(Object^ sender, MouseEventArgs^ e)
		{
			steganalysis->guessNomimatedBits();
		}

		void parseInputBoxes(UInt32% startPos, UInt32% endPos, UInt32% minSize, UInt32% maxSize, UInt32% maxUnchangedBits)
		{
			startPos = TextBoxParser::tryUInt32(inputBoxes[0], 1, steganalysis->CarryBitmapLength);
			endPos = TextBoxParser::tryUInt32(inputBoxes[1], startPos, steganalysis->CarryBitmapLength);
			minSize = TextBoxParser::tryUInt32(inputBoxes[2], 1, steganalysis->CarryBitmapLength);
			maxSize = TextBoxParser::tryUInt32(inputBoxes[3], minSize, steganalysis->CarryBitmapLength);
			maxUnchangedBits = TextBoxParser::tryUInt32(inputBoxes[4], 1, steganalysis->CarryBitmapLength);

			--startPos;
			--endPos;
		}

#pragma region Custom Initialization
		void _initializeUI()
		{
			bitsRed = gcnew StegoBitsNominator(this, "red", "Red color bits", Color::LightCoral);
			bitsGreen = gcnew StegoBitsNominator(this, "green", "Green color bits", Color::LightGreen);
			bitsBlue = gcnew StegoBitsNominator(this, "blue", "Blue color bits", Color::LightSkyBlue);

			Int32 bitsWidth = bitsRed->Width + bitsGreen->Width + bitsBlue->Width + 10;
			
			bitsRed->Location = Point(12, 12);
			bitsGreen->Location = Point(bitsRed->Right + 5, bitsRed->Top);
			bitsBlue->Location = Point(bitsGreen->Right + 5, bitsGreen->Top);

			buttonLoadOriginal = gcnew LabelButton(this, "buttonLoadOriginal", "Load original");
			ButtonExtensions::setProperties(buttonLoadOriginal, false, System::Drawing::Size(130, 22), Color::LightSkyBlue);

			buttonLoadCarry = gcnew LabelButton(this, "buttonLoadCarry", "Load carry");
			ButtonExtensions::setProperties(buttonLoadCarry, false, System::Drawing::Size(130, 22), Color::LightSkyBlue);

			buttonAction = gcnew LabelButton(this, "buttonAction", "Run!");
			ButtonExtensions::setProperties(buttonAction, false, System::Drawing::Size(130, 22), Color::LightCoral);

			buttonGuess = gcnew LabelButton(this, "buttonGuess", "Guess Bits");
			ButtonExtensions::setProperties(buttonGuess, false, System::Drawing::Size(130, 22), Color::LightCoral);

			buttonTryHard = gcnew LabelButton(this, "buttonTryHard", "Try Hard!");
			ButtonExtensions::setProperties(buttonTryHard, false, System::Drawing::Size(130, 22), Color::LightCoral);

			inputBoxes = gcnew TitledTextBoxList(this, -1, Point(15, -1));

			inputBoxes->addControl("boxStartPos", "Start Position:");
			inputBoxes->addControl("boxEndPos", "End Position:");
			inputBoxes->addControl("boxMinSize", "Minimum Size:");
			inputBoxes->addControl("boxMaxSize", "Maximum Size:");
			inputBoxes->addControl("boxMaxUnchangedBits", "Max Unchanged Bits:");

			for(int i = 0; i < inputBoxes->Count; ++i) {
				inputBoxes[i]->MaxLength = 15;
				inputBoxes[i]->ControlSize = System::Drawing::Size(150, 21);
				inputBoxes[i]->TitleForeColor = ColorExtensions::GetScaled(Color::LightGreen, 0.8f);
				inputBoxes[i]->ControlForeColor = ColorExtensions::GetScaled(Color::LightGreen, 0.9f);
				inputBoxes[i]->ControlBackColor = Color::FromArgb(59, 67, 68);
			}

			inputBoxes->align();

			buttonLoadOriginal->Location = Point(12, bitsRed->Bottom + 12);
			buttonLoadCarry->Location = Point(buttonLoadOriginal->Right + 10, buttonLoadOriginal->Top);
			buttonAction->Location = Point(12, buttonLoadCarry->Bottom + 12);
			buttonGuess->Location = Point(buttonAction->Right + 10, buttonAction->Top);
			buttonTryHard->Location = Point(12, buttonGuess->Bottom + 12);

			inputBoxes->Location = Point(12, buttonTryHard->Bottom + 50);

			outcomeBox->Size = System::Drawing::Size(420, inputBoxes->Bottom - bitsRed->Top);
			outcomeBox->Location = Point(inputBoxes->Right + 20, 12);

			ClientSize = System::Drawing::Size(outcomeBox->Right + 12, outcomeBox->Bottom + 12);

			buttonLoadOriginal->MouseClickExtension += gcnew LabelButtonMouseClickHandler(this, &SteganalysisForm::Event_ButtonLoadOriginalClicked);
			buttonLoadCarry->MouseClickExtension += gcnew LabelButtonMouseClickHandler(this, &SteganalysisForm::Event_ButtonLoadCarryClicked);
			buttonAction->MouseClickExtension += gcnew LabelButtonMouseClickHandler(this, &SteganalysisForm::Event_ButtonActionClicked);
			buttonGuess->MouseClickExtension += gcnew LabelButtonMouseClickHandler(this, &SteganalysisForm::Event_ButtonGuessClicked);
			buttonTryHard->MouseClickExtension += gcnew LabelButtonMouseClickHandler(this, &SteganalysisForm::Event_ButtonTryHardClicked);
		}
#pragma endregion

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			outcomeBox = gcnew TextBox();
			outcomeBox->Multiline = true;
			outcomeBox->ReadOnly = true;
			outcomeBox->ForeColor = Color::Black;
			outcomeBox->BackColor = Color::WhiteSmoke;
			outcomeBox->MaxLength = 1 << 20;
			outcomeBox->ScrollBars = ScrollBars::Vertical;
			outcomeBox->Font = gcnew System::Drawing::Font("Microsoft Sans Serif", 10.5f);
			Controls->Add(outcomeBox);

			this->components = gcnew System::ComponentModel::Container();
			this->BackColor = Color::FromArgb(50, 50, 60);
			this->Text = L"CV StegoCrypt : Steganalysis";
			this->Name = L"SteganalysisForm";
			this->Padding = System::Windows::Forms::Padding(0);
		}
#pragma endregion
	};
}
