#pragma once

namespace CVNetExtensions
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class AutoValidatedForm : public Form
	{
	public:
		AutoValidatedForm()
		{
			InitializeComponent();
		}

	protected:
		~AutoValidatedForm()
		{
			if (components)
				delete components;
		}

		virtual void _customValidation()
		{}

	private:
		System::ComponentModel::Container^ components;

		void Event_FormMove(Object^ sender, EventArgs^ e)
		{ _customValidation(); }

		void Event_FormShown(Object^ sender, EventArgs^ e)
		{
			Refresh();
			_customValidation();
		}

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->SuspendLayout();
			// 
			// AutoValidatedForm
			//
			this->components = gcnew System::ComponentModel::Container();
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(200, 200);
			this->Name = L"AutoValidatedForm";
			this->Text = L"AutoValidatedForm";
			this->Shown += gcnew System::EventHandler(this, &AutoValidatedForm::Event_FormShown);
			this->Move += gcnew System::EventHandler(this, &AutoValidatedForm::Event_FormMove);
			this->ResumeLayout(false);

		}
#pragma endregion
	};
}
