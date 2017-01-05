#pragma once

#include <Windows.h>
#include "ButtonExtensions.h"
#include "DisplayInfoForm.h"

namespace CVStegoCrypt
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace CVNetExtensions;

	[System::Runtime::InteropServices::DllImport("user32.dll")]
	extern int SendMessage(IntPtr hWnd, Int32 wMsg, bool wParam, Int32 lParam);

	public ref class SteganographyForm : public SecondaryForm
	{
	public:
		SteganographyForm(LabelButton^ controlButton) : SecondaryForm(controlButton)
		{
			DoubleBuffered = true;

			InitializeComponent();
			_initializeUI();

			steganography = gcnew BitmapSteganography(bitsRed, bitsGreen, bitsBlue);
			bitmapDisplay = gcnew DisplayInfoForm(buttonDisplayBmp, "CV StegoCrypt : Steganography : Loaded bitmap", true);
			infoDisplay = gcnew DisplayInfoForm(buttonDisplayInfo, "CV StegoCrypt : Steganography : Loaded information", false);
			outputDisplay = gcnew DisplayInfoForm(buttonDisplayOutput, "CV StegoCrypt : Steganography : Created bitmap", true);
			outputDisplay->DisplayFilePath = false;

			_setEvents();
		}

	protected:
		~SteganographyForm()
		{
			if (components)
				delete components;
		}

		virtual void _customValidation() override
		{
			bitsRed->validateAll();
			bitsGreen->validateAll();
			bitsBlue->validateAll();
			UI_validateLines();
		}

	private:
	#pragma region Fields
		BitmapSteganography^ steganography;

		DisplayInfoForm^ bitmapDisplay;
		DisplayInfoForm^ infoDisplay;
		DisplayInfoForm^ outputDisplay;

		LabelButton^ buttonConceal;
		LabelButton^ buttonExtract;
		LabelButton^ buttonErase;

		LabelButton^ buttonLoadBitmap;
		LabelButton^ buttonLsb;
		LabelButton^ buttonLoadInfo;

		LabelButton^ buttonEraseZero;
		LabelButton^ buttonEraseOne;
		LabelButton^ buttonEraseRandom;

		LabelButton^ buttonAction;
		LabelButton^ buttonSaveOutput;

		LabelButton^ buttonDisplayBmp;
		LabelButton^ buttonDisplayInfo;
		LabelButton^ buttonDisplayOutput;

		Color titleColor;
		Label^ titleChooseMode;
		Label^ titleChooseBits;
		Label^ titleChooseErase;
		StegoBitsNominator^ bitsRed;
		StegoBitsNominator^ bitsGreen;
		StegoBitsNominator^ bitsBlue;

		TitledTextBoxList^ boxes;

		bool initialChoiceMade;

		array<Point>^ backupLocationsConceal;
		array<Point>^ backupLocationsExtract;
		array<Point>^ backupLocationsErase;

		Graphics^ graphics;
		Pen^ pen1;
		Pen^ pen2;
		Color activeRedLine, activeGreenLine, activeBlueLine;
		Color inactiveLine;

		array<Point>^ concealLoadBmpLine;
		array<Point>^ extractLoadBmpLine;
		array<Point>^ eraseLoadBmpLine;

		array<Point>^ loadDisplayBmpLine;

		array<Point>^ loadBmpNominatorLine;
		array<Point>^ lsbNominatorLine;
		array<Point>^ nominatorBGLine;
		array<Point>^ nominatorGRLine;

		array<array<Point>^>^ rBits1Lines;
		array<array<Point>^>^ rBits2Lines;
		array<array<Point>^>^ rBits3Lines;

		array<array<Point>^>^ gBits1Lines;
		array<array<Point>^>^ gBits2Lines;
		array<array<Point>^>^ gBits3Lines;

		array<array<Point>^>^ bBits1Lines;
		array<array<Point>^>^ bBits2Lines;
		array<array<Point>^>^ bBits3Lines;

		array<Point>^ rNominatorLine;
		array<Point>^ gNominatorLine;
		array<Point>^ bNominatorLine;

		array<Point>^ rNominatorConcealLine;
		array<Point>^ gNominatorConcealLine;
		array<Point>^ bNominatorConcealLine;

		array<Point>^ maxInfoPosConcealLine;
		array<Point>^ loadInfoPosConcealLine;
		array<Point>^ loadDisplayInfoConcealLine;

		array<Point>^ posActionConcealLine;
		array<Point>^ actionSaveOutputConcealLine;
		array<Point>^ saveDisplayOutputConcealLine;

		array<Point>^ posActionExtractLine;
		array<Point>^ actionSaveOutputExtractLine;

		array<Point>^ rNominatorEraseLine;
		array<Point>^ gNominatorEraseLine;
		array<Point>^ bNominatorEraseLine;

		array<array<Point>^>^ eraseModeLines;

		array<Point>^ actionSaveOutputEraseLine;
		array<Point>^ saveDisplayOutputEraseLine;

		System::ComponentModel::Container ^components;
	#pragma endregion

		void Event_ButtonConcealClicked(Object^ sender, MouseEventArgs^ e)
		{
			if(buttonExtract->isActivated || buttonErase->isActivated || !initialChoiceMade) {
				buttonExtract->forceDeactivate();
				buttonErase->forceDeactivate();
				UI_modeChanged();
			}
		}

		void Event_ButtonExtractClicked(Object^ sender, MouseEventArgs^ e)
		{
			if(buttonConceal->isActivated || buttonErase->isActivated || !initialChoiceMade) {
				buttonConceal->forceDeactivate();
				buttonErase->forceDeactivate();
				UI_modeChanged();
			}		
		}

		void Event_ButtonEraseClicked(Object^ sender, MouseEventArgs^ e)
		{
			if(buttonConceal->isActivated || buttonExtract->isActivated || !initialChoiceMade) {
				buttonConceal->forceDeactivate();
				buttonExtract->forceDeactivate();
				UI_modeChanged();
			}
		}

		void Event_ButtonLoadBitmapClicked(Object^ sender, MouseEventArgs^ e)
		{
			steganography->clear();

			bool isWindowShown = buttonDisplayBmp->isActivated;
			bitmapDisplay->closeIfShown();
			UI_disableNominators();
			buttonDisplayBmp->disable();
			
			OpenFileDialog^ dialog = gcnew OpenFileDialog();
            dialog->Filter = "Bitmap |*.bmp";
            dialog->ShowDialog();

			if(dialog->FileName != "") {
				if(steganography->loadBitmap(dialog->FileName)) {
					UI_enableNominators();
					buttonDisplayBmp->enable();
					bitmapDisplay->setup(steganography);
					if(isWindowShown) bitmapDisplay->display();
					SimpleBox::display("Bitmap loaded successfully!", "CVStegoCrypt : Load confirmation");
				}
				else
					SimpleBox::display("Bitmap loading unsuccessful.", "CVStegoCrypt : Load error");
			}
		}

		void Event_ButtonLsbClicked(Object^ sender, MouseEventArgs^ e)
		{
			bitsRed->setLsb();
			bitsGreen->setLsb();
			bitsBlue->setLsb();
			UI_validNominators();
		}

		void Event_ButtonLoadInfoClicked(Object^ sender, MouseEventArgs^ e)
		{
			bool isWindowShown = buttonDisplayInfo->isActivated;
			infoDisplay->closeIfShown();
			UI_disableStartPosition();
			buttonDisplayInfo->disable();

			OpenFileDialog^ dialog = gcnew OpenFileDialog();
            dialog->ShowDialog();

			if(dialog->FileName != "") {
				Int32 outcome = steganography->loadInformation(dialog->FileName);
				if(outcome == 0) {
					UI_enableStartPosition();
					buttonDisplayInfo->enable();
					infoDisplay->setup(steganography);
					if(isWindowShown) infoDisplay->display();
					SimpleBox::display("Information loaded successfully!", "CVStegoCrypt : Load confirmation");
				}
				else if(outcome > 0)
					SimpleBox::display("Loaded information is too large by " + outcome.ToString("N0") + " bytes.", "CVStegoCrypt : Load error");
				else
					SimpleBox::display("Information loading unsuccessful.", "CVStegoCrypt : Load error");
			}
		}

		void Event_ButtonActionClicked(Object^ sender, MouseEventArgs^ e)
		{
			UI_disableSave();

			if(buttonErase->isActivated)
				actionErase();
			else {
				Int32 startPosition;
				if(Int32::TryParse(boxes[2]->Text, startPosition)) {
					if(--startPosition < 0)
						SimpleBox::display("Start position must be greater than zero.", "CVStegoCrypt : Action error");
					else if(buttonConceal->isActivated)
						actionConceal(startPosition);
					else if(buttonExtract->isActivated)
						actionExtract(startPosition);
				}
				else
					SimpleBox::display("Start position must be a number.", "CVStegoCrypt : Action error");
			}
		}

		void Event_ButtonSaveClicked(Object^ sender, MouseEventArgs^ e)
		{
			SaveFileDialog^ dialog = gcnew SaveFileDialog();
			if(!buttonExtract->isActivated)
				dialog->Filter = "Bitmap 24-bit |*.bmp";
			dialog->ShowDialog();

			if(dialog->FileName != "") {
				if(!buttonExtract->isActivated) {
					steganography->saveBitmap(dialog->FileName);
					SimpleBox::display("Bitmap saved successfully!", "CVStegoCrypt : Save confirmation");
				}
				else {
					steganography->saveInformation(dialog->FileName);
					SimpleBox::display("Information saved successfully!", "CVStegoCrypt : Save confirmation");
				}
			}
		}

		void Event_MaxPosTextChanged(Object^ sender, EventArgs^ e)
		{
			if(boxes[2]->Text->Length > 0)
				UI_enableAction();
			else
				UI_disableAction();
		}

		void Event_NominatorClicked(Object^ sender, MouseEventArgs^ e)
		{
			bool atLeastOneOn = checkNominator(bitsRed);
			if(!atLeastOneOn) {
				atLeastOneOn = checkNominator(bitsGreen);
				if(!atLeastOneOn)
					atLeastOneOn = checkNominator(bitsBlue);
			}

			if(atLeastOneOn)
				UI_validNominators();
			else
				UI_disableLoadInfo();
		}

		void Event_ButtonEraseZeroClicked(Object^ sender, MouseEventArgs^ e)
		{
			buttonEraseOne->forceDeactivate();
			buttonEraseRandom->forceDeactivate();
			UI_enableAction();
		}

		void Event_ButtonEraseOneClicked(Object^ sender, MouseEventArgs^ e)
		{
			buttonEraseZero->forceDeactivate();
			buttonEraseRandom->forceDeactivate();
			UI_enableAction();
		}

		void Event_ButtonEraseRandomClicked(Object^ sender, MouseEventArgs^ e)
		{
			buttonEraseZero->forceDeactivate();
			buttonEraseOne->forceDeactivate();
			UI_enableAction();
		}

		void Event_FormClosing(Object^ sender, FormClosingEventArgs^ e)
		{
			bitmapDisplay->closeIfShown();
			infoDisplay->closeIfShown();
			outputDisplay->closeIfShown();
		}

		bool checkNominator(StegoBitsNominator^ nominator)
		{
			for(Byte i = 0; i < 8; ++i)
				if(nominator[i]) return true;
			return false;
		}

		void actionConceal(Int32 startPosition)
		{
			if(startPosition >= steganography->MaxStartPosition)
				SimpleBox::display("Start position is too big by " + (startPosition - steganography->MaxStartPosition + 1).ToString("N0") + ".", "CVStegoCrypt : Action error");
			else {
				steganography->conceal(startPosition);
				UI_enableSave();
				SimpleBox::display("Concealing successful!", "CVStegoCrypt : Action confirmation");
			}
		}

		void actionExtract(Int32 startPosition)
		{
			if(startPosition >= steganography->LoadedBitmapLength)
				SimpleBox::display("Start position is too big by " + (startPosition - steganography->LoadedBitmapLength + 1).ToString("N0") + ".", "CVStegoCrypt : Action error");
			else if(steganography->extract(startPosition)) {
				UI_enableSave();
				SimpleBox::display("Extraction successful!", "CVStegoCrypt : Action confirmation");
			}
			else
				SimpleBox::display("Extraction unsuccessful.", "CVStegoCrypt : Action error");
		}

		void actionErase()
		{
			if(buttonEraseZero->isActivated)
				steganography->erase(BitmapSteganography::EraseMode::Zeros);
			else if(buttonEraseOne->isActivated)
				steganography->erase(BitmapSteganography::EraseMode::Ones);
			else
				steganography->erase(BitmapSteganography::EraseMode::Random);

			UI_enableSave();
			SimpleBox::display("Erasement successful!", "CVStegoCrypt : Action confirmation");
		}

#pragma region UI Handling
		void UI_modeChanged()
		{
			SendMessage(Handle, WM_SETREDRAW, false, 0);

			initialChoiceMade = true;
			if(buttonConceal->isActivated)
				UI_concealModeOn();
			else if(buttonExtract->isActivated)
				UI_extractModeOn();
			else
				UI_eraseModeOn();

			if(steganography->BitmapLoaded) {
				UI_enableNominators();
				Event_NominatorClicked(nullptr, nullptr);
			}
			else UI_disableNominators();

			ClientSize = System::Drawing::Size(ClientSize.Width, buttonSaveOutput->Bottom + 12);
			graphics = Graphics::FromHwnd(Handle);

			SendMessage(Handle, WM_SETREDRAW, true, 0);
			Refresh();
			_customValidation();
		}

		void UI_concealModeOn()
		{
			UI_setNominatorTitle("Choose pixel bits to conceal information into:");
			UI_setActionText("Conceal!");

			UI_eraseModeVisibility(false);
			UI_loadInfoVisibility(true);
			boxes[2]->Visible = true;

			boxes[2]->Location = backupLocationsConceal[0];
			buttonAction->Location = backupLocationsConceal[1];
			buttonSaveOutput->Location = backupLocationsConceal[2];
			buttonDisplayOutput->Location = Point(buttonSaveOutput->Right + 10, buttonSaveOutput->Top);
			buttonDisplayOutput->Visible = true;
		}

		void UI_extractModeOn()
		{
			UI_setNominatorTitle("Choose pixel bits to extract information from:");
			UI_setActionText("Try to extract!");

			UI_eraseModeVisibility(false);
			UI_loadInfoVisibility(false);
			boxes[2]->Visible = true;

			boxes[2]->Location = backupLocationsExtract[0];
			buttonAction->Location = backupLocationsExtract[1];
			buttonSaveOutput->Location = backupLocationsExtract[2];
			buttonDisplayOutput->Visible = false;

			infoDisplay->closeIfShown();
		}

		void UI_eraseModeOn()
		{
			UI_setNominatorTitle("Choose pixel bits to erase information from:");
			UI_setActionText("Erase!");

			UI_eraseModeVisibility(true);
			UI_loadInfoVisibility(false);
			boxes[2]->Visible = false;

			buttonAction->Location = backupLocationsErase[0];
			buttonSaveOutput->Location = backupLocationsErase[1];
			buttonDisplayOutput->Location = Point(buttonSaveOutput->Right + 10, buttonSaveOutput->Top);
			buttonDisplayOutput->Visible = true;

			infoDisplay->closeIfShown();
		}

		void UI_loadInfoVisibility(bool visible)
		{
			boxes[0]->Visible = visible;
			boxes[1]->Visible = visible;
			buttonLoadInfo->Visible = visible;
			buttonDisplayInfo->Visible = visible;
		}

		void UI_eraseModeVisibility(bool visible)
		{
			titleChooseErase->Visible = visible;
			buttonEraseZero->Visible = visible;
			buttonEraseOne->Visible = visible;
			buttonEraseRandom->Visible = visible;
		}

		void UI_setNominatorTitle(String^ title)
		{
			titleChooseBits->Text = title;
			titleChooseBits->Location = Point((ClientSize.Width - titleChooseBits->PreferredWidth) >> 1, buttonLoadBitmap->Bottom + 10);
		}

		void UI_setActionText(String^ text)
		{
			buttonAction->Text = text;
		}

		void UI_disableNominators()
		{
			titleChooseBits->ForeColor = Color::DarkGray;
			buttonLsb->disable();
			bitsRed->reset();
			bitsGreen->reset();
			bitsBlue->reset();
			bitsRed->Enabled = false;
			bitsGreen->Enabled = false;
			bitsBlue->Enabled = false;
			UI_disableLoadInfo();
		}

		void UI_enableNominators()
		{
			titleChooseBits->ForeColor = titleColor;
			buttonLsb->enable();
			bitsRed->Enabled = true;
			bitsGreen->Enabled = true;
			bitsBlue->Enabled = true;
			UI_validateLines();
		}

		void UI_validNominators()
		{
			UI_disableStartPosition();

			if(buttonConceal->isActivated)
				UI_enableLoadInfo();
			else if(buttonExtract->isActivated)
				UI_enableStartPosition();
			else
				UI_enableEraseMode();
		}

		void UI_disableEraseMode()
		{
			titleChooseErase->ForeColor = Color::DarkGray;
			buttonEraseZero->forceDeactivate();
			buttonEraseOne->forceDeactivate();
			buttonEraseRandom->forceDeactivate();
			buttonEraseZero->disable();
			buttonEraseOne->disable();
			buttonEraseRandom->disable();
		}

		void UI_enableEraseMode()
		{
			titleChooseErase->ForeColor = titleColor;
			buttonEraseZero->enable();
			buttonEraseOne->enable();
			buttonEraseRandom->enable();

			if(buttonEraseZero->isActivated || buttonEraseOne->isActivated || buttonEraseRandom->isActivated)
				UI_enableAction();
			else UI_validateLines();
		}

		void UI_disableLoadInfo()
		{
			boxes[0]->Text = "";
			boxes[0]->Enabled = false;
			infoDisplay->closeIfShown();
			buttonLoadInfo->disable();
			buttonDisplayInfo->disable();
			UI_disableEraseMode();
			UI_disableStartPosition();
		}

		void UI_enableLoadInfo()
		{
			boxes[0]->Text = steganography->MaxInfoLength.ToString("N0") + " B";
			boxes[0]->Enabled = true;
			buttonLoadInfo->enable();

			if(steganography->InfoLoaded) {
				buttonDisplayInfo->enable();
				UI_enableStartPosition();
				Int32 maxStartPosition = steganography->MaxStartPosition;
				boxes[1]->Text = steganography->MaxStartPosition.ToString("N0");
				boxes[1]->ControlForeColor = (maxStartPosition > 0) ? boxes[0]->ControlForeColor : buttonAction->normalForeColor;
			}
			UI_validateLines();
		}

		void UI_disableStartPosition()
		{
			boxes[1]->Text = "";
			boxes[2]->Text = "";
			boxes[1]->Enabled = false;
			boxes[2]->Enabled = false;
			UI_disableAction();
		}

		void UI_enableStartPosition()
		{
			boxes[1]->ControlForeColor = boxes[0]->ControlForeColor;
			boxes[1]->Text = steganography->MaxStartPosition.ToString("N0");
			boxes[1]->Enabled = true;
			boxes[2]->Enabled = true;
			UI_validateLines();
		}

		void UI_disableAction()
		{
			buttonAction->disable();
			UI_disableSave();
		}

		void UI_enableAction()
		{
			buttonAction->enable();
			UI_validateLines();
		}

		void UI_disableSave()
		{
			outputDisplay->closeIfShown();
			buttonSaveOutput->disable();
			buttonDisplayOutput->disable();
			UI_validateLines();
		}

		void UI_enableSave()
		{
			if(!buttonExtract->isActivated) outputDisplay->setup(steganography);
			buttonSaveOutput->enable();
			buttonDisplayOutput->enable();
			UI_validateLines();
		}

		void UI_validateLines()
		{
			if(initialChoiceMade) {
				UI_drawStaticLines();

				if(!buttonErase->isActivated) {
					UI_drawConcealOrExtractNominatorLines();

					if(buttonConceal->isActivated) UI_drawConcealModeLines();
					else UI_drawExtractOutputLines();
				}
				else UI_drawEraseModeLines();
			}
		}

		void UI_drawStaticLines()
		{
			UI_drawOperationModeLines();
			if(steganography->BitmapLoaded) UI_drawLoadedBitmapLines();
			else UI_drawUnloadedBitmapLines();
			UI_drawStaticNominatorLines();
		}

		void UI_drawOperationModeLines()
		{
			UI_drawLine(pen2, (buttonConceal->isActivated) ? activeGreenLine : inactiveLine, concealLoadBmpLine);
			UI_drawLine(pen2, (buttonExtract->isActivated) ? activeGreenLine : inactiveLine, extractLoadBmpLine);
			UI_drawLine(pen2, (buttonErase->isActivated) ? activeGreenLine : inactiveLine, eraseLoadBmpLine);
		}

		void UI_drawLoadedBitmapLines()
		{
			UI_drawLine(pen2, activeBlueLine, loadDisplayBmpLine);
			UI_drawLine(pen2, activeGreenLine, loadBmpNominatorLine);
			UI_drawLine(pen2, activeGreenLine, lsbNominatorLine);
			UI_drawLine(pen2, activeGreenLine, nominatorBGLine);
			UI_drawLine(pen2, activeGreenLine, nominatorGRLine);
		}

		void UI_drawUnloadedBitmapLines()
		{
			UI_drawLine(pen2, inactiveLine, loadDisplayBmpLine);
			UI_drawLine(pen2, inactiveLine, loadBmpNominatorLine);
			UI_drawLine(pen2, inactiveLine, lsbNominatorLine);
			UI_drawLine(pen2, inactiveLine, nominatorBGLine);
			UI_drawLine(pen2, inactiveLine, nominatorGRLine);
		}

		void UI_drawStaticNominatorLines()
		{
			UI_drawRedNominatorLines();
			UI_drawGreenNominatorLines();
			UI_drawBlueNominatorLines();
			UI_drawNominatorLines();
		}

		void UI_drawRedNominatorLines()
		{
			for(Int32 i = 0; i < 8; ++i)
				UI_drawLine(pen1, (bitsRed[i]) ? activeRedLine : inactiveLine, rBits1Lines[i]);

			for(Int32 i = 0; i < 4; ++i)
				UI_drawLine(pen1, (bitsRed[i << 1] || bitsRed[(i << 1) + 1]) ? activeRedLine : inactiveLine, rBits2Lines[i]);

			UI_drawLine(pen1, (bitsRed[0] || bitsRed[1] || bitsRed[2] || bitsRed[3]) ? activeRedLine : inactiveLine, rBits3Lines[0]);
			UI_drawLine(pen1, (bitsRed[4] || bitsRed[5] || bitsRed[6] || bitsRed[7]) ? activeRedLine : inactiveLine, rBits3Lines[1]);
		}

		void UI_drawGreenNominatorLines()
		{
			for(Int32 i = 0; i < 8; ++i)
				UI_drawLine(pen1, (bitsGreen[i]) ? activeGreenLine : inactiveLine, gBits1Lines[i]);

			for(Int32 i = 0; i < 4; ++i)
				UI_drawLine(pen1, (bitsGreen[i << 1] || bitsGreen[(i << 1) + 1]) ? activeGreenLine : inactiveLine, gBits2Lines[i]);

			UI_drawLine(pen1, (bitsGreen[0] || bitsGreen[1] || bitsGreen[2] || bitsGreen[3]) ? activeGreenLine : inactiveLine, gBits3Lines[0]);
			UI_drawLine(pen1, (bitsGreen[4] || bitsGreen[5] || bitsGreen[6] || bitsGreen[7]) ? activeGreenLine : inactiveLine, gBits3Lines[1]);
		}

		void UI_drawBlueNominatorLines()
		{
			for(Int32 i = 0; i < 8; ++i)
				UI_drawLine(pen1, (bitsBlue[i]) ? activeBlueLine : inactiveLine, bBits1Lines[i]);

			for(Int32 i = 0; i < 4; ++i)
				UI_drawLine(pen1, (bitsBlue[i << 1] || bitsBlue[(i << 1) + 1]) ? activeBlueLine : inactiveLine, bBits2Lines[i]);

			UI_drawLine(pen1, (bitsBlue[0] || bitsBlue[1] || bitsBlue[2] || bitsBlue[3]) ? activeBlueLine : inactiveLine, bBits3Lines[0]);
			UI_drawLine(pen1, (bitsBlue[4] || bitsBlue[5] || bitsBlue[6] || bitsBlue[7]) ? activeBlueLine : inactiveLine, bBits3Lines[1]);
		}

		void UI_drawNominatorLines()
		{
			UI_drawLine(pen1, (steganography->BitsPerRed > 0) ? activeRedLine : inactiveLine, rNominatorLine);
			UI_drawLine(pen1, (steganography->BitsPerGreen > 0) ? activeGreenLine : inactiveLine, gNominatorLine);
			UI_drawLine(pen1, (steganography->BitsPerBlue > 0) ? activeBlueLine : inactiveLine, bNominatorLine);
		}

		void UI_drawConcealOrExtractNominatorLines()
		{
			UI_drawLine(pen1, (steganography->BitsPerRed > 0) ? activeRedLine : inactiveLine, rNominatorConcealLine);
			UI_drawLine(pen1, (steganography->BitsPerGreen > 0) ? activeGreenLine : inactiveLine, gNominatorConcealLine);
			UI_drawLine(pen1, (steganography->BitsPerBlue > 0) ? activeBlueLine : inactiveLine, bNominatorConcealLine);
		}

		void UI_drawConcealModeLines()
		{
			UI_drawConcealLoadedInfoLines();
			UI_drawConcealOutputLines();
		}

		void UI_drawConcealLoadedInfoLines()
		{
			UI_drawLine(pen2, (boxes[0]->Enabled) ? activeBlueLine : inactiveLine, maxInfoPosConcealLine);
			if(steganography->InfoLoaded) {
				UI_drawLine(pen2, activeGreenLine, loadInfoPosConcealLine);
				UI_drawLine(pen2, activeBlueLine, loadDisplayInfoConcealLine);
			}
			else {
				UI_drawLine(pen2, inactiveLine, loadInfoPosConcealLine);
				UI_drawLine(pen2, inactiveLine, loadDisplayInfoConcealLine);
			}
		}

		void UI_drawConcealOutputLines()
		{
			UI_drawLine(pen2, (boxes[2]->Text->Length > 0) ? activeGreenLine : inactiveLine, posActionConcealLine);
			if(buttonSaveOutput->isEnabled) {
				UI_drawLine(pen2, activeRedLine, actionSaveOutputConcealLine);
				UI_drawLine(pen2, activeBlueLine, saveDisplayOutputConcealLine);
			}
			else {
				UI_drawLine(pen2, inactiveLine, actionSaveOutputConcealLine);
				UI_drawLine(pen2, inactiveLine, saveDisplayOutputConcealLine);
			}
		}

		void UI_drawExtractOutputLines()
		{
			UI_drawLine(pen2, (boxes[2]->Text->Length > 0) ? activeGreenLine : inactiveLine, posActionExtractLine);
			UI_drawLine(pen2, (buttonSaveOutput->isEnabled) ? activeRedLine : inactiveLine, actionSaveOutputExtractLine);
		}

		void UI_drawEraseModeLines()
		{
			UI_drawEraseNominatorLines();
			UI_drawEraseTypeOfEraseLines();
			UI_drawEraseOutputLines();
		}

		void UI_drawEraseNominatorLines()
		{
			UI_drawLine(pen1, (steganography->BitsPerRed > 0) ? activeRedLine : inactiveLine, rNominatorEraseLine);
			UI_drawLine(pen1, (steganography->BitsPerGreen > 0) ? activeGreenLine : inactiveLine, gNominatorEraseLine);
			UI_drawLine(pen1, (steganography->BitsPerBlue > 0) ? activeBlueLine : inactiveLine, bNominatorEraseLine);
		}

		void UI_drawEraseTypeOfEraseLines()
		{
			UI_drawLine(pen2, (buttonEraseZero->isActivated) ? activeGreenLine : inactiveLine, eraseModeLines[0]);
			UI_drawLine(pen2, (buttonEraseOne->isActivated) ? activeGreenLine : inactiveLine, eraseModeLines[1]);
			UI_drawLine(pen2, (buttonEraseZero->isActivated || buttonEraseOne->isActivated) ? activeGreenLine : inactiveLine, eraseModeLines[2]);
			UI_drawLine(pen2, (buttonEraseRandom->isActivated) ? activeGreenLine : inactiveLine, eraseModeLines[3]);
			UI_drawLine(pen2, (buttonEraseZero->isActivated || buttonEraseOne->isActivated || buttonEraseRandom->isActivated) ? activeGreenLine : inactiveLine, eraseModeLines[4]);
		}

		void UI_drawEraseOutputLines()
		{
			if(buttonSaveOutput->isEnabled) {
				UI_drawLine(pen2, activeRedLine, actionSaveOutputEraseLine);
				UI_drawLine(pen2, activeBlueLine, saveDisplayOutputEraseLine);
			}
			else {
				UI_drawLine(pen2, inactiveLine, actionSaveOutputEraseLine);
				UI_drawLine(pen2, inactiveLine, saveDisplayOutputEraseLine);
			}
		}

		void UI_drawLine(Pen^ pen, Color color, array<Point>^ line)
		{
			pen->Color = color;
			graphics->DrawLines(pen, line);
		}
#pragma endregion

#pragma region Custom Initialization
		void _initializeUI()
		{
			titleColor = ColorExtensions::GetScaled(Color::LightGreen, 0.8f);
			_initializeChoiceButtons();
			_initializeFileButtons();
			_initializeEraseModeButtons();
			_initializeOtherButtons();
			_initializeBitNominators();
			_initializeBoxes();

			ClientSize = System::Drawing::Size(_initializeLocationsAndWidth(), buttonConceal->Bottom + 12);
			_initializeLocationBackups();
			_initializeLinePoints();
		}

		void _initializeChoiceButtons()
		{
			titleChooseMode = gcnew Label();
			titleChooseMode->AutoSize = true;
			titleChooseMode->Font = gcnew System::Drawing::Font("Microsoft Sans Serif", 10, FontStyle::Bold);
			titleChooseMode->BackColor = Color::Transparent;
			titleChooseMode->ForeColor = titleColor;
			titleChooseMode->Name = "titleChooseMode";
			titleChooseMode->Text = "OPERATING MODE";
			Controls->Add(titleChooseMode);

			System::Drawing::Size buttonSize = System::Drawing::Size(130, 22);
			Color baseColor = Color::LightGreen;

			buttonConceal = gcnew LabelButton(this, "buttonConcealInfo", "Concealment");
			ButtonExtensions::setProperties(buttonConceal, true, buttonSize, baseColor);
			
			buttonExtract = gcnew LabelButton(this, "buttonExtractInfo", "Extraction");
			ButtonExtensions::setProperties(buttonExtract, true, buttonSize, baseColor);

			buttonErase = gcnew LabelButton(this, "buttonEraseInfo", "Erasement");
			ButtonExtensions::setProperties(buttonErase, true, buttonSize, baseColor);
		}

		void _initializeFileButtons()
		{
			System::Drawing::Size buttonSize = System::Drawing::Size(200, 22);
			Color baseColor = Color::LightSkyBlue;

			buttonLoadBitmap = gcnew LabelButton(this, "buttonLoadBitmap", "Load bitmap");
			ButtonExtensions::setProperties(buttonLoadBitmap, false, buttonSize, baseColor);

			buttonLoadInfo = gcnew LabelButton(this, "buttonLoadInfo", "Load information");
			ButtonExtensions::setProperties(buttonLoadInfo, false, buttonSize, baseColor);

			buttonSaveOutput = gcnew LabelButton(this, "buttonSaveOutput", "Save outcome");
			ButtonExtensions::setProperties(buttonSaveOutput, false, buttonSize, baseColor);

			buttonDisplayBmp = gcnew LabelButton(this, "buttonDisplayBmp", "i");
			ButtonExtensions::setProperties(buttonDisplayBmp, true, System::Drawing::Size(20, 22), baseColor);
			buttonDisplayBmp->disable();

			buttonDisplayInfo = gcnew LabelButton(this, "buttonDisplayInfo", "i");
			ButtonExtensions::setProperties(buttonDisplayInfo, true, System::Drawing::Size(20, 22), baseColor);

			buttonDisplayOutput = gcnew LabelButton(this, "buttonDisplayOutput", "i");
			ButtonExtensions::setProperties(buttonDisplayOutput, true, System::Drawing::Size(20, 22), baseColor);
		}

		void _initializeEraseModeButtons()
		{
			titleChooseErase = gcnew Label();
			titleChooseErase->AutoSize = true;
			titleChooseErase->Font = gcnew System::Drawing::Font("Microsoft Sans Serif", 10, FontStyle::Italic);
			titleChooseErase->BackColor = Color::Transparent;
			titleChooseErase->Name = "titleChooseErase";
			titleChooseErase->Text = "Choose erase mode:";
			Controls->Add(titleChooseErase);

			System::Drawing::Size buttonSize = System::Drawing::Size(150, 22);
			Color baseColor = Color::LightGreen;

			buttonEraseZero = gcnew LabelButton(this, "buttonEraseZero", "Zeros");
			ButtonExtensions::setProperties(buttonEraseZero, true, buttonSize, baseColor);

			buttonEraseOne = gcnew LabelButton(this, "buttonEraseOne", "Ones");
			ButtonExtensions::setProperties(buttonEraseOne, true, buttonSize, baseColor);

			buttonEraseRandom = gcnew LabelButton(this, "buttonEraseRandom", "Random");
			ButtonExtensions::setProperties(buttonEraseRandom, true, buttonSize, baseColor);
		}

		void _initializeOtherButtons()
		{
			buttonLsb = gcnew LabelButton(this, "buttonLsb", "Least significant bit method");
			ButtonExtensions::setProperties(buttonLsb, false, System::Drawing::Size(300, 22), Color::LightGreen);

			buttonAction = gcnew LabelButton(this, "buttonAction", "Conceal!");
			ButtonExtensions::setProperties(buttonAction, false, System::Drawing::Size(200, 22), Color::LightCoral);
		}

		void _initializeBitNominators()
		{
			titleChooseBits = gcnew Label();
			titleChooseBits->AutoSize = true;
			titleChooseBits->Font = gcnew System::Drawing::Font("Microsoft Sans Serif", 10, FontStyle::Italic);
			titleChooseBits->BackColor = Color::Transparent;
			titleChooseBits->Name = "titleChooseBits";
			titleChooseBits->Text = "Choose pixel bits to conceal information into:";
			Controls->Add(titleChooseBits);

			bitsRed = gcnew StegoBitsNominator(this, "red", "Red color bits", Color::LightCoral);
			bitsGreen = gcnew StegoBitsNominator(this, "green", "Green color bits", Color::LightGreen);
			bitsBlue = gcnew StegoBitsNominator(this, "blue", "Blue color bits", Color::LightSkyBlue);
		}

		void _initializeBoxes()
		{
			boxes = gcnew TitledTextBoxList(this, -1, Point(20, -1));

			boxes->addControl("boxMaxInfoSize", "Max information size:");
			boxes->addControl("boxMaxStartPos", "Max start position:");
			boxes->addControl("boxStartPos", "Start position:");

			boxes[0]->ReadOnly = true;
			boxes[1]->ReadOnly = true;
			boxes[2]->MaxLength = 15;

			boxes[0]->ControlSize = System::Drawing::Size(150, 21);
			boxes[1]->ControlSize = System::Drawing::Size(150, 21);
			boxes[2]->ControlSize = System::Drawing::Size(150, 21);

			boxes[0]->TitleForeColor = ColorExtensions::GetScaled(Color::LightSkyBlue, 0.8f);
			boxes[1]->TitleForeColor = boxes[0]->TitleForeColor;
			boxes[2]->TitleForeColor = ColorExtensions::GetScaled(Color::LightGreen, 0.8f);

			boxes[0]->ControlForeColor = ColorExtensions::GetScaled(Color::LightSkyBlue, 0.9f);
			boxes[1]->ControlForeColor = boxes[0]->ControlForeColor;
			boxes[2]->ControlForeColor = ColorExtensions::GetScaled(Color::LightGreen, 0.9f);

			boxes[0]->ControlBackColor = Color::FromArgb(58, 64, 78);
			boxes[1]->ControlBackColor = boxes[0]->ControlBackColor;
			boxes[2]->ControlBackColor = Color::FromArgb(59, 67, 68);

			boxes[0]->DisabledBackColor = Color::FromArgb(72, 72, 78);
			boxes[1]->DisabledBackColor = Color::FromArgb(72, 72, 78);
			boxes[2]->DisabledBackColor = Color::FromArgb(72, 72, 78);

			boxes->align();
		}

		Int32 _initializeLocationsAndWidth()
		{
			Int32 clientWidth = __setChoiceButtonsLocations(Point(12, 8));
			buttonLoadBitmap->Location = Point((clientWidth - buttonLoadBitmap->Width) >> 1, buttonExtract->Bottom + 20);
			buttonDisplayBmp->Location = Point(buttonLoadBitmap->Left - buttonDisplayBmp->Width - 10, buttonLoadBitmap->Top);
			
			__setBitNominatorsLocations(clientWidth);
			buttonLoadInfo->Location = Point((clientWidth - buttonLoadInfo->Width) >> 1, bitsBlue->Bottom + boxes[0]->ControlSize.Height + 69);
			buttonDisplayInfo->Location = Point(buttonLoadInfo->Left - buttonDisplayInfo->Width - 10, buttonLoadInfo->Top);

			__setBoxesLocations(clientWidth);
			__setEraseModeButtonsLocations(clientWidth);

			buttonAction->Location = Point((clientWidth - buttonAction->Width) >> 1, boxes->Bottom + 10);
			buttonSaveOutput->Location = Point((clientWidth - buttonSaveOutput->Width) >> 1, buttonAction->Bottom + 10);
			buttonDisplayOutput->Location = Point(buttonSaveOutput->Right + 10, buttonSaveOutput->Top);

			return clientWidth;
		}

		Int32 __setChoiceButtonsLocations(Point origin)
		{
			Int32 width = (buttonConceal->Width * 3) + (origin.X << 1) - 2;

			titleChooseMode->Location = Point((width - titleChooseMode->PreferredWidth) >> 1, origin.Y);
			buttonConceal->Location = Point(origin.X, titleChooseMode->Bottom + 5);
			buttonExtract->Location = Point(buttonConceal->Right - 1, buttonConceal->Top);
			buttonErase->Location = Point(buttonExtract->Right - 1, buttonConceal->Top);

			return width;
		}

		void __setBitNominatorsLocations(Int32 clientWidth)
		{
			Int32 bitsWidth = bitsRed->Width + bitsGreen->Width + bitsBlue->Width + 10;

			titleChooseBits->Location = Point((clientWidth - titleChooseBits->PreferredWidth) >> 1, buttonLoadBitmap->Bottom + 10);
			buttonLsb->Location = Point((clientWidth - buttonLsb->Width) >> 1, titleChooseBits->Bottom + 5);
			bitsRed->Location = Point((clientWidth - bitsWidth) >> 1, buttonLsb->Bottom + 5);
			bitsGreen->Location = Point(bitsRed->Right + 5, bitsRed->Top);
			bitsBlue->Location = Point(bitsGreen->Right + 5, bitsGreen->Top);
		}

		void __setBoxesLocations(Int32 clientWidth)
		{
			Int32 controlHeight = boxes[0]->ControlSize.Height;

			boxes->Location = Point((clientWidth - boxes->Width) >> 1, buttonLoadInfo->Bottom - controlHeight + 12);
			boxes[0]->Location = Point(boxes[0]->Left, buttonLoadInfo->Top - controlHeight - 8);
		}

		void __setEraseModeButtonsLocations(Int32 clientWidth)
		{
			titleChooseErase->Location = Point((clientWidth - titleChooseErase->PreferredWidth) >> 1, boxes[0]->Top);
			buttonEraseZero->Location = Point((clientWidth - buttonEraseZero->Width) >> 1, titleChooseErase->Bottom + 5);
			buttonEraseOne->Location = Point(buttonEraseZero->Left, buttonEraseZero->Bottom - 1);
			buttonEraseRandom->Location = Point(buttonEraseZero->Left, buttonEraseOne->Bottom - 1);
		}

		void _initializeLocationBackups()
		{
			initialChoiceMade = false;

			backupLocationsConceal = gcnew array<Point>(3);
			backupLocationsConceal[0] = boxes[2]->Location;
			backupLocationsConceal[1] = buttonAction->Location;
			backupLocationsConceal[2] = buttonSaveOutput->Location;

			backupLocationsExtract = gcnew array<Point>(3);
			backupLocationsExtract[0] = boxes[0]->Location;
			backupLocationsExtract[1] = Point(buttonAction->Left, boxes[0]->Bottom + 10);
			backupLocationsExtract[2] = Point(buttonSaveOutput->Left, backupLocationsExtract[1].Y + buttonAction->Height + 10);

			backupLocationsErase = gcnew array<Point>(2);
			backupLocationsErase[0] = Point(buttonAction->Left, buttonEraseRandom->Bottom + 10);
			backupLocationsErase[1] = Point(buttonSaveOutput->Left, backupLocationsErase[0].Y + buttonAction->Height + 10);
		}

		void _initializeLinePoints()
		{
			graphics = Graphics::FromHwnd(Handle);
			pen1 = gcnew Pen(Color::White, 1);
			pen2 = gcnew Pen(Color::White, 2);
			inactiveLine = Color::FromArgb(72, 72, 78);
			activeRedLine = ColorExtensions::GetScaled(Color::LightCoral, 0.65f);
			activeGreenLine = ColorExtensions::GetScaled(Color::LightGreen, 0.65f);
			activeBlueLine = ColorExtensions::GetScaled(Color::LightSkyBlue, 0.65f);

			_initializeStaticLinePoints();
			_initializeConcealLinePoints();
			_initializeExtractLinePoints();
			_initializeEraseLinePoints();
		}

		void _initializeStaticLinePoints()
		{
			__initializeModeButtonLines();
			__initializeLoadBmpButtonLines();
			__initializeRedNominatorLines();
			__initializeGreenNominatorLines();
			__initializeBlueNominatorLines();
			__initializeBaseNominatorLines();
		}

		void __initializeModeButtonLines()
		{
			concealLoadBmpLine = gcnew array<Point>(4);
			extractLoadBmpLine = gcnew array<Point>(2);
			eraseLoadBmpLine = gcnew array<Point>(4);

			concealLoadBmpLine[0] = Point(buttonConceal->Right - (buttonConceal->Width / 3), buttonConceal->Bottom);
			concealLoadBmpLine[1] = Point(concealLoadBmpLine[0].X, concealLoadBmpLine[0].Y + 10);
			concealLoadBmpLine[2] = Point(buttonLoadBitmap->Left + (buttonLoadBitmap->Width >> 1) - 25, concealLoadBmpLine[1].Y);
			concealLoadBmpLine[3] = Point(concealLoadBmpLine[2].X, buttonLoadBitmap->Top);

			extractLoadBmpLine[0] = Point(buttonExtract->Left + (buttonExtract->Width >> 1), buttonExtract->Bottom);
			extractLoadBmpLine[1] = Point(extractLoadBmpLine[0].X, buttonLoadBitmap->Top);

			eraseLoadBmpLine[0] = Point(buttonErase->Left + (buttonErase->Width / 3), buttonErase->Bottom);
			eraseLoadBmpLine[1] = Point(eraseLoadBmpLine[0].X, eraseLoadBmpLine[0].Y + 10);
			eraseLoadBmpLine[2] = Point(buttonLoadBitmap->Left + (buttonLoadBitmap->Width >> 1) + 25, eraseLoadBmpLine[1].Y);
			eraseLoadBmpLine[3] = Point(eraseLoadBmpLine[2].X, buttonLoadBitmap->Top);
		}

		void __initializeLoadBmpButtonLines()
		{
			loadDisplayBmpLine = gcnew array<Point>(2);
			loadBmpNominatorLine = gcnew array<Point>(4);
			lsbNominatorLine = gcnew array<Point>(2);
			nominatorBGLine = gcnew array<Point>(2);
			nominatorGRLine = gcnew array<Point>(2);

			loadDisplayBmpLine[0] = Point(buttonLoadBitmap->Left, buttonLoadBitmap->Top + (buttonLoadBitmap->Height >> 1));
			loadDisplayBmpLine[1] = Point(buttonDisplayBmp->Right, loadDisplayBmpLine[0].Y);

			loadBmpNominatorLine[0] = Point(buttonLoadBitmap->Right, loadDisplayBmpLine[0].Y);
			loadBmpNominatorLine[1] = Point(buttonLsb->Right + 25, loadBmpNominatorLine[0].Y);
			loadBmpNominatorLine[2] = Point(loadBmpNominatorLine[1].X, bitsBlue->Bottom - (bitsBlue->BoxSize.Height >> 1));
			loadBmpNominatorLine[3] = Point(bitsBlue->Right, loadBmpNominatorLine[2].Y);

			lsbNominatorLine[0] = Point(buttonLsb->Right, buttonLsb->Top + (buttonLsb->Height >> 1));
			lsbNominatorLine[1] = Point(loadBmpNominatorLine[1].X - 1, lsbNominatorLine[0].Y);

			nominatorBGLine[0] = Point(bitsBlue->Left, loadBmpNominatorLine[3].Y);
			nominatorBGLine[1] = Point(bitsGreen->Right, nominatorBGLine[0].Y);

			nominatorGRLine[0] = Point(bitsGreen->Left, nominatorBGLine[0].Y);
			nominatorGRLine[1] = Point(bitsRed->Right, nominatorGRLine[0].Y);
		}

		void __initializeRedNominatorLines()
		{
			array<Int32>^ bitsHalfPos = gcnew array<Int32>(8);
			array<Int32>^ bitsFullPos = gcnew array<Int32>(7);
			bitsHalfPos[0] = bitsRed->Left + (bitsRed->BoxSize.Width >> 1);
			bitsFullPos[0] = bitsRed->Left + bitsRed->BoxSize.Width - 1;
			for(Int32 i = 1; i < 8; ++i)
				bitsHalfPos[i] = bitsHalfPos[i - 1] + bitsRed->BoxSize.Width - 1;
			for(Int32 i = 1; i < 7; ++i)
				bitsFullPos[i] = bitsFullPos[i - 1] + bitsRed->BoxSize.Width - 1;

			rBits1Lines = gcnew array<array<Point>^>(8);
			for(Int32 i = 0; i < 8; ++i) {
				rBits1Lines[i] = gcnew array<Point>(3);
				rBits1Lines[i][0] = Point(bitsHalfPos[i], bitsRed->Bottom);
				rBits1Lines[i][1] = Point(bitsHalfPos[i], bitsRed->Bottom + 6);
				if((i & 1) == 0)
					rBits1Lines[i][2] = Point(bitsFullPos[i], rBits1Lines[i][1].Y);
				else
					rBits1Lines[i][2] = Point(rBits1Lines[i - 1][2].X, rBits1Lines[i][1].Y);
			}

			rBits2Lines = gcnew array<array<Point>^>(4);
			for(Int32 i = 0; i < 4; ++i) {
				rBits2Lines[i] = gcnew array<Point>(3);
				rBits2Lines[i][0] = Point(bitsFullPos[i << 1], rBits1Lines[i][1].Y);
				rBits2Lines[i][1] = Point(rBits2Lines[i][0].X, rBits2Lines[i][0].Y + 7);
				if((i & 1) == 0)
					rBits2Lines[i][2] = Point(bitsFullPos[(i << 1) + 1], rBits2Lines[i][1].Y);
				else
					rBits2Lines[i][2] = Point(rBits2Lines[i - 1][2].X, rBits2Lines[i][1].Y);
			}

			rBits3Lines = gcnew array<array<Point>^>(2);
			rBits3Lines[0] = gcnew array<Point>(3);
			rBits3Lines[1] = gcnew array<Point>(3);

			rBits3Lines[0][0] = Point(bitsFullPos[1], rBits2Lines[0][1].Y);
			rBits3Lines[0][1] = Point(bitsFullPos[1], rBits2Lines[0][1].Y + 7);
			rBits3Lines[0][2] = Point(bitsFullPos[3], rBits3Lines[0][1].Y);

			rBits3Lines[1][0] = Point(bitsFullPos[5], rBits2Lines[2][1].Y);
			rBits3Lines[1][1] = Point(bitsFullPos[5], rBits3Lines[0][1].Y);
			rBits3Lines[1][2] = Point(bitsFullPos[3], rBits3Lines[1][1].Y);
		}

		void __initializeGreenNominatorLines()
		{
			gBits1Lines = gcnew array<array<Point>^>(8);
			for(Int32 i = 0; i < 8; ++i) {
				gBits1Lines[i] = gcnew array<Point>(3);
				for(Int32 j = 0; j < 3; ++j) {
					gBits1Lines[i][j].X = rBits1Lines[i][j].X + (bitsGreen->Left - bitsRed->Left);
					gBits1Lines[i][j].Y = rBits1Lines[i][j].Y;
				}
			}

			gBits2Lines = gcnew array<array<Point>^>(4);
			for(Int32 i = 0; i < 4; ++i) {
				gBits2Lines[i] = gcnew array<Point>(3);
				for(Int32 j = 0; j < 3; ++j) {
					gBits2Lines[i][j].X = rBits2Lines[i][j].X + (bitsGreen->Left - bitsRed->Left);
					gBits2Lines[i][j].Y = rBits2Lines[i][j].Y;
				}
			}

			gBits3Lines = gcnew array<array<Point>^>(2);
			for(Int32 i = 0; i < 2; ++i) {
				gBits3Lines[i] = gcnew array<Point>(3);
				for(Int32 j = 0; j < 3; ++j) {
					gBits3Lines[i][j].X = rBits3Lines[i][j].X + (bitsGreen->Left - bitsRed->Left);
					gBits3Lines[i][j].Y = rBits3Lines[i][j].Y;
				}
			}
		}

		void __initializeBlueNominatorLines()
		{
			bBits1Lines = gcnew array<array<Point>^>(8);
			for(Int32 i = 0; i < 8; ++i) {
				bBits1Lines[i] = gcnew array<Point>(3);
				for(Int32 j = 0; j < 3; ++j) {
					bBits1Lines[i][j].X = gBits1Lines[i][j].X + (bitsBlue->Left - bitsGreen->Left);
					bBits1Lines[i][j].Y = gBits1Lines[i][j].Y;
				}
			}

			bBits2Lines = gcnew array<array<Point>^>(4);
			for(Int32 i = 0; i < 4; ++i) {
				bBits2Lines[i] = gcnew array<Point>(3);
				for(Int32 j = 0; j < 3; ++j) {
					bBits2Lines[i][j].X = gBits2Lines[i][j].X + (bitsBlue->Left - bitsGreen->Left);
					bBits2Lines[i][j].Y = gBits2Lines[i][j].Y;
				}
			}

			bBits3Lines = gcnew array<array<Point>^>(2);
			for(Int32 i = 0; i < 2; ++i) {
				bBits3Lines[i] = gcnew array<Point>(3);
				for(Int32 j = 0; j < 3; ++j) {
					bBits3Lines[i][j].X = gBits3Lines[i][j].X + (bitsBlue->Left - bitsGreen->Left);
					bBits3Lines[i][j].Y = gBits3Lines[i][j].Y;
				}
			}
		}

		void __initializeBaseNominatorLines()
		{
			rNominatorLine = gcnew array<Point>(4);
			gNominatorLine = gcnew array<Point>(2);
			bNominatorLine = gcnew array<Point>(4);

			rNominatorLine[0] = Point(rBits3Lines[0][2].X, rBits3Lines[0][1].Y);
			rNominatorLine[1] = Point(rNominatorLine[0].X, rNominatorLine[0].Y + 12);
			rNominatorLine[2] = Point(gBits3Lines[0][2].X - 1, rNominatorLine[1].Y);
			rNominatorLine[3] = Point(rNominatorLine[2].X, rNominatorLine[1].Y + 12);

			gNominatorLine[0] = Point(gBits3Lines[0][2].X, gBits3Lines[0][1].Y);
			gNominatorLine[1] = Point(gNominatorLine[0].X, rNominatorLine[3].Y + 1);

			bNominatorLine[0] = Point(bBits3Lines[0][2].X, bBits3Lines[0][1].Y);
			bNominatorLine[1] = Point(bNominatorLine[0].X, rNominatorLine[1].Y);
			bNominatorLine[2] = Point(gNominatorLine[0].X + 1, bNominatorLine[1].Y);
			bNominatorLine[3] = Point(bNominatorLine[2].X, rNominatorLine[3].Y);
		}

		void _initializeConcealLinePoints()
		{
			__initializeConcealNominatorLines();
			__initializeLoadInfoButtonConcealLines();
			__initializeSaveOutputButtonConcealLines();
		}

		void __initializeConcealNominatorLines()
		{
			rNominatorConcealLine = gcnew array<Point>(4);
			gNominatorConcealLine = gcnew array<Point>(3);
			bNominatorConcealLine = gcnew array<Point>(3);

			rNominatorConcealLine[0] = Point(rNominatorLine[3].X, rNominatorLine[3].Y);
			rNominatorConcealLine[1] = Point(rNominatorConcealLine[0].X, rNominatorConcealLine[0].Y + 2);
			rNominatorConcealLine[2] = Point(boxes[0]->Right - (boxes[0]->ControlSize.Width / 6) - 1, rNominatorConcealLine[1].Y);
			rNominatorConcealLine[3] = Point(rNominatorConcealLine[2].X, boxes[0]->Top);

			gNominatorConcealLine[0] = Point(gNominatorLine[1].X, gNominatorLine[1].Y);
			gNominatorConcealLine[1] = Point(rNominatorConcealLine[2].X + 1, gNominatorConcealLine[0].Y);
			gNominatorConcealLine[2] = Point(gNominatorConcealLine[1].X, boxes[0]->Top);

			bNominatorConcealLine[0] = Point(bNominatorLine[3].X, bNominatorLine[3].Y);
			bNominatorConcealLine[1] = Point(gNominatorConcealLine[1].X + 1, bNominatorConcealLine[0].Y);
			bNominatorConcealLine[2] = Point(bNominatorConcealLine[1].X, boxes[0]->Top);
		}

		void __initializeLoadInfoButtonConcealLines()
		{
			maxInfoPosConcealLine = gcnew array<Point>(2);
			loadInfoPosConcealLine = gcnew array<Point>(3);
			loadDisplayInfoConcealLine = gcnew array<Point>(2);

			maxInfoPosConcealLine[0] = Point(gNominatorConcealLine[2].X, boxes[0]->Bottom);
			maxInfoPosConcealLine[1] = Point(maxInfoPosConcealLine[0].X, boxes[1]->Top);

			loadInfoPosConcealLine[0] = Point(buttonLoadInfo->Right, buttonLoadInfo->Top + (buttonLoadInfo->Height >> 1));
			loadInfoPosConcealLine[1] = Point(maxInfoPosConcealLine[0].X - 5, loadInfoPosConcealLine[0].Y);
			loadInfoPosConcealLine[2] = Point(loadInfoPosConcealLine[1].X, boxes[1]->Top);

			loadDisplayInfoConcealLine[0] = Point(buttonLoadInfo->Left, loadInfoPosConcealLine[0].Y);
			loadDisplayInfoConcealLine[1] = Point(buttonDisplayInfo->Right, loadDisplayInfoConcealLine[0].Y);
		}

		void __initializeSaveOutputButtonConcealLines()
		{
			posActionConcealLine = gcnew array<Point>(3);
			actionSaveOutputConcealLine = gcnew array<Point>(4);
			saveDisplayOutputConcealLine = gcnew array<Point>(2);

			posActionConcealLine[0] = Point(maxInfoPosConcealLine[0].X, backupLocationsConceal[0].Y + boxes[2]->Height - 1);
			posActionConcealLine[1] = Point(posActionConcealLine[0].X, backupLocationsConceal[1].Y + (buttonAction->Height >> 1));
			posActionConcealLine[2] = Point(backupLocationsConceal[1].X + buttonAction->Width, posActionConcealLine[1].Y);

			actionSaveOutputConcealLine[0] = Point(backupLocationsConceal[1].X, posActionConcealLine[1].Y);
			actionSaveOutputConcealLine[1] = Point(actionSaveOutputConcealLine[0].X - 25, actionSaveOutputConcealLine[0].Y);
			actionSaveOutputConcealLine[2] = Point(actionSaveOutputConcealLine[1].X, backupLocationsConceal[2].Y + (buttonSaveOutput->Height >> 1));
			actionSaveOutputConcealLine[3] = Point(backupLocationsConceal[2].X, actionSaveOutputConcealLine[2].Y);

			saveDisplayOutputConcealLine[0] = Point(backupLocationsConceal[2].X + buttonSaveOutput->Width, actionSaveOutputConcealLine[3].Y);
			saveDisplayOutputConcealLine[1] = Point(saveDisplayOutputConcealLine[0].X + (buttonDisplayOutput->Left - buttonSaveOutput->Right), saveDisplayOutputConcealLine[0].Y);
		}

		void _initializeExtractLinePoints()
		{
			posActionExtractLine = gcnew array<Point>(3);
			actionSaveOutputExtractLine = gcnew array<Point>(4);

			posActionExtractLine[0] = Point(gNominatorConcealLine[2].X, backupLocationsExtract[0].Y + boxes[2]->Height - 1);
			posActionExtractLine[1] = Point(posActionExtractLine[0].X, backupLocationsExtract[1].Y + (buttonAction->Height >> 1));
			posActionExtractLine[2] = Point(backupLocationsExtract[1].X + buttonAction->Width, posActionExtractLine[1].Y);

			actionSaveOutputExtractLine[0] = Point(backupLocationsExtract[1].X, posActionExtractLine[1].Y);
			actionSaveOutputExtractLine[1] = Point(actionSaveOutputExtractLine[0].X - 25, actionSaveOutputExtractLine[0].Y);
			actionSaveOutputExtractLine[2] = Point(actionSaveOutputExtractLine[1].X, backupLocationsExtract[2].Y + (buttonSaveOutput->Height >> 1));
			actionSaveOutputExtractLine[3] = Point(backupLocationsExtract[2].X, actionSaveOutputExtractLine[2].Y);
		}

		void _initializeEraseLinePoints()
		{
			__initializeEraseNominatorLines();
			__initializeEraseModeLines();
			__initializeSaveOutputButtonEraseLines();
		}

		void __initializeEraseNominatorLines()
		{
			rNominatorEraseLine = gcnew array<Point>(4);
			gNominatorEraseLine = gcnew array<Point>(4);
			bNominatorEraseLine = gcnew array<Point>(5);

			rNominatorEraseLine[0] = Point(rNominatorLine[3].X, rNominatorLine[3].Y);
			rNominatorEraseLine[1] = Point(buttonEraseOne->Left - 51, rNominatorEraseLine[0].Y);
			rNominatorEraseLine[2] = Point(rNominatorEraseLine[1].X, buttonEraseZero->Top + (buttonEraseZero->Height >> 1) + 1);
			rNominatorEraseLine[3] = Point(buttonEraseZero->Left, rNominatorEraseLine[2].Y);

			gNominatorEraseLine[0] = Point(gNominatorLine[1].X, gNominatorLine[1].Y);
			gNominatorEraseLine[1] = Point(rNominatorEraseLine[1].X + 1, gNominatorEraseLine[0].Y);
			gNominatorEraseLine[2] = Point(gNominatorEraseLine[1].X, rNominatorEraseLine[2].Y - 1);
			gNominatorEraseLine[3] = Point(buttonEraseZero->Left, gNominatorEraseLine[2].Y);

			bNominatorEraseLine[0] = Point(bNominatorLine[3].X, bNominatorLine[3].Y);
			bNominatorEraseLine[1] = Point(bNominatorEraseLine[0].X, bNominatorEraseLine[0].Y + 2);
			bNominatorEraseLine[2] = Point(gNominatorEraseLine[1].X + 1, bNominatorEraseLine[1].Y);
			bNominatorEraseLine[3] = Point(bNominatorEraseLine[2].X, gNominatorEraseLine[2].Y - 1);
			bNominatorEraseLine[4] = Point(buttonEraseZero->Left, bNominatorEraseLine[3].Y);
		}

		void __initializeEraseModeLines()
		{
			eraseModeLines = gcnew array<array<Point>^>(5);
			eraseModeLines[0] = gcnew array<Point>(3);
			eraseModeLines[1] = gcnew array<Point>(2);
			eraseModeLines[2] = gcnew array<Point>(2);
			eraseModeLines[3] = gcnew array<Point>(2);
			eraseModeLines[4] = gcnew array<Point>(3);

			eraseModeLines[0][0] = Point(buttonEraseZero->Right, buttonEraseZero->Top + (buttonEraseZero->Height >> 1));
			eraseModeLines[0][1] = Point(buttonEraseZero->Right + 50, eraseModeLines[0][0].Y);
			eraseModeLines[0][2] = Point(eraseModeLines[0][1].X, buttonEraseOne->Top + (buttonEraseOne->Height >> 1) - 1);

			eraseModeLines[1][0] = Point(buttonEraseOne->Right, eraseModeLines[0][2].Y + 1);
			eraseModeLines[1][1] = Point(eraseModeLines[0][1].X - 1, eraseModeLines[1][0].Y);

			eraseModeLines[2][0] = Point(eraseModeLines[0][1].X, eraseModeLines[0][2].Y);
			eraseModeLines[2][1] = Point(eraseModeLines[2][0].X, buttonEraseRandom->Top + (buttonEraseRandom->Height >> 1) - 1);

			eraseModeLines[3][0] = Point(buttonEraseRandom->Right, eraseModeLines[2][1].Y + 1);
			eraseModeLines[3][1] = Point(eraseModeLines[1][1].X, eraseModeLines[3][0].Y);

			eraseModeLines[4][0] = Point(eraseModeLines[0][1].X, eraseModeLines[2][1].Y);
			eraseModeLines[4][1] = Point(eraseModeLines[4][0].X, backupLocationsErase[0].Y + (buttonAction->Height >> 1));
			eraseModeLines[4][2] = Point(backupLocationsErase[0].X + buttonAction->Width, eraseModeLines[4][1].Y);
		}

		void __initializeSaveOutputButtonEraseLines()
		{
			actionSaveOutputEraseLine = gcnew array<Point>(4);
			saveDisplayOutputEraseLine = gcnew array<Point>(2);

			actionSaveOutputEraseLine[0] = Point(backupLocationsErase[0].X, backupLocationsErase[0].Y + (buttonAction->Height >> 1));
			actionSaveOutputEraseLine[1] = Point(actionSaveOutputEraseLine[0].X - 25, actionSaveOutputEraseLine[0].Y);
			actionSaveOutputEraseLine[2] = Point(actionSaveOutputEraseLine[1].X, backupLocationsErase[1].Y + (buttonSaveOutput->Height >> 1));
			actionSaveOutputEraseLine[3] = Point(backupLocationsErase[1].X, actionSaveOutputEraseLine[2].Y);

			saveDisplayOutputEraseLine[0] = Point(backupLocationsErase[1].X + buttonSaveOutput->Width, actionSaveOutputEraseLine[3].Y);
			saveDisplayOutputEraseLine[1] = Point(saveDisplayOutputEraseLine[0].X + (buttonDisplayOutput->Left - buttonSaveOutput->Right), saveDisplayOutputEraseLine[0].Y);
		}

		void _setEvents()
		{
			FormClosing += gcnew FormClosingEventHandler(this, &SteganographyForm::Event_FormClosing);

			buttonConceal->MouseClickExtension += gcnew LabelButtonMouseClickHandler(this, &SteganographyForm::Event_ButtonConcealClicked);
			buttonExtract->MouseClickExtension += gcnew LabelButtonMouseClickHandler(this, &SteganographyForm::Event_ButtonExtractClicked);
			buttonErase->MouseClickExtension += gcnew LabelButtonMouseClickHandler(this, &SteganographyForm::Event_ButtonEraseClicked);
			buttonLoadBitmap->MouseClickExtension += gcnew LabelButtonMouseClickHandler(this, &SteganographyForm::Event_ButtonLoadBitmapClicked);
			buttonLsb->MouseClickExtension += gcnew LabelButtonMouseClickHandler(this, &SteganographyForm::Event_ButtonLsbClicked);
			buttonEraseZero->MouseClickExtension += gcnew LabelButtonMouseClickHandler(this, &SteganographyForm::Event_ButtonEraseZeroClicked);
			buttonEraseOne->MouseClickExtension += gcnew LabelButtonMouseClickHandler(this, &SteganographyForm::Event_ButtonEraseOneClicked);
			buttonEraseRandom->MouseClickExtension += gcnew LabelButtonMouseClickHandler(this, &SteganographyForm::Event_ButtonEraseRandomClicked);
			buttonLoadInfo->MouseClickExtension += gcnew LabelButtonMouseClickHandler(this, &SteganographyForm::Event_ButtonLoadInfoClicked);
			buttonSaveOutput->MouseClickExtension += gcnew LabelButtonMouseClickHandler(this, &SteganographyForm::Event_ButtonSaveClicked);
			buttonAction->MouseClickExtension += gcnew LabelButtonMouseClickHandler(this, &SteganographyForm::Event_ButtonActionClicked);

			bitsRed->addMouseClickEvent(gcnew CheckBoxMouseClickHandler(this, &SteganographyForm::Event_NominatorClicked));
			bitsGreen->addMouseClickEvent(gcnew CheckBoxMouseClickHandler(this, &SteganographyForm::Event_NominatorClicked));
			bitsBlue->addMouseClickEvent(gcnew CheckBoxMouseClickHandler(this, &SteganographyForm::Event_NominatorClicked));

			boxes[2]->addTextChangedEvent(gcnew EventHandler(this, &SteganographyForm::Event_MaxPosTextChanged));
		}
#pragma endregion

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->components = gcnew System::ComponentModel::Container();
			this->BackColor = Color::FromArgb(50, 50, 60);
			this->Text = L"CV StegoCrypt : Steganography";
			this->Name = L"SteganographyForm";
			this->Padding = System::Windows::Forms::Padding(0);
		}
#pragma endregion
	};
}