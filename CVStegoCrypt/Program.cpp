#include "MainForm.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace CVNetExtensions;

[STAThread]
void Main(array<String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

	SimpleBox::initialize();
	SimpleBox::BackColor = Color::FromArgb(50, 50, 60);
	SimpleBox::ForeColor = Color::LightSkyBlue;

	Color scaled = ColorExtensions::GetScaled(Color::LightSkyBlue, 0.9f);
	SimpleBox::ButtonNormalBackColor = Color::FromArgb(24, Color::LightSkyBlue);
	SimpleBox::ButtonNormalForeColor = scaled;
	SimpleBox::ButtonMousedOverBackColor = Color::FromArgb(48, Color::LightSkyBlue);
	SimpleBox::ButtonMousedOverForeColor = Color::LightSkyBlue;

	CVStegoCrypt::MainForm myForm;
    Application::Run(%myForm);
}