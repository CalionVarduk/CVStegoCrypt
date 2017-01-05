#pragma once

#include "CVNetExtensions.h"

namespace CVStegoCrypt
{
	using namespace System;
	using namespace System::Drawing;

	using namespace CVNetExtensions;

	[System::Runtime::CompilerServices::ExtensionAttribute]
	public ref class ButtonExtensions abstract sealed
	{
		public:
			[System::Runtime::CompilerServices::ExtensionAttribute]
			static void setColorScheme(LabelButton^ button, Color baseColor)
			{
				Color scaledColor = ColorExtensions::GetScaled(baseColor, 0.9f);

				button->changeColorsOnActivation = true;
				button->changeColorsOnMouseOver = true;

				button->normalBackColor = Color::FromArgb(24, baseColor);
				button->normalForeColor = scaledColor;
				button->mouseOverBackColor = Color::FromArgb(48, baseColor);
				button->mouseOverForeColor = baseColor;
				button->activeBackColor = scaledColor;
				button->activeForeColor = Color::Black;
				button->activeMouseOverBackColor = baseColor;
				button->activeMouseOverForeColor = Color::FromArgb(50, 50, 60);
				button->disabledBackColor = Color::FromArgb(72, 72, 78);
				button->disabledForeColor = Color::DarkGray;
				button->resetDisplayedColors();
			}

			[System::Runtime::CompilerServices::ExtensionAttribute]
			static void setProperties(LabelButton^ button, bool isClicked, System::Drawing::Size size, Color baseColor)
			{
				button->Font = gcnew System::Drawing::Font("Microsoft Sans Serif", 10, FontStyle::Bold);
				button->setFixedSize(size);
				button->isActivatedOnClick = isClicked;
				button->isOnClickDeactivatable = !isClicked;
				button->runOnMouseClickProcedure = true;
				ButtonExtensions::setColorScheme(button, baseColor);
			}
	};
}