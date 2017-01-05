#include "StegoBitsNominator.h"

using namespace CVStegoCrypt;

StegoBitsNominator::StegoBitsNominator(Control^ parent, String^ name, String^ title, Color titleColor)
{
	_title = gcnew Label();
	_title->Text = title;
	_title->Name = name + "_TITLE";
	_title->Font = gcnew Font(L"Microsoft Sans Serif", 8, FontStyle::Bold, GraphicsUnit::Point, (Byte)238);
	_title->ForeColor = titleColor;
	_title->BackColor = Color::Transparent;
	_title->Location = Point(0, 0);
	_title->AutoSize = true;

	_boxes = gcnew array<TitledCheckBox^>(8);

	Point location = Point(0, _title->PreferredHeight);
	for(Int32 i = 0; i < _boxes->Length; ++i) {
		_boxes[i] = gcnew TitledCheckBox(parent, name + "_BIT" + i.ToString(), (i + 1).ToString());
		_boxes[i]->ControlSize = System::Drawing::Size(13, 13);
		_boxes[i]->Title = (i + 1).ToString();
		_boxes[i]->TitleFont = gcnew Font(L"Microsoft Sans Serif", 6.5f, FontStyle::Bold, GraphicsUnit::Point, (Byte)238);
		_boxes[i]->ControlForeColor = ColorExtensions::GetScaled(titleColor, 0.8f);
		_boxes[i]->TitleForeColor = titleColor;
		_boxes[i]->DisabledForeColor = Color::DarkGray;
		_boxes[i]->Offset = Point(_boxes[i]->HorizontalAllignment, _boxes[i]->TitleSize.Height);
		_boxes[i]->Location = location;
		location.X += _boxes[i]->ControlSize.Width - 1;
	}

	_title->Location = Point((Width - _title->PreferredWidth) >> 1, 0);
	parent->Controls->Add(_title);
}

void StegoBitsNominator::addMouseClickEvent(CheckBoxMouseClickHandler^ handler)
{
	for(Int32 i = 0; i < _boxes->Length; ++i)
		_boxes[i]->MouseClick += handler;
}

void StegoBitsNominator::set(Byte i, bool switchOn)
{
	if(i < _boxes->Length && switchOn != _boxes[i]->isActivated) {
		if(switchOn) _boxes[i]->forceActivate();
		else _boxes[i]->forceDeactivate();
	}
}

void StegoBitsNominator::setLsb()
{
	_boxes[0]->forceActivate();
	for(Int32 i = 1; i < _boxes->Length; ++i)
		_boxes[i]->forceDeactivate();
}

void StegoBitsNominator::reset()
{
	for(Int32 i = 0; i < _boxes->Length; ++i)
		_boxes[i]->forceDeactivate();
}

inline bool StegoBitsNominator::operator[] (Byte i)
{ return _boxes[i]->isActivated; }

Byte StegoBitsNominator::getCode()
{
	Byte output = 0;
	for(Int32 i = 0; i < _boxes->Length; ++i)
		if(_boxes[i]->isActivated)
			output |= (1 << i);
	return output;
}

void StegoBitsNominator::validateAll()
{
	for(Int32 i = 0; i < _boxes->Length; ++i)
		_boxes[i]->validate();
}