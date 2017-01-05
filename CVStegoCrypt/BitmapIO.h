#pragma once

namespace CVStegoCrypt
{
	using namespace System;
	using namespace System::Drawing;
	using namespace System::Drawing::Imaging;
	using namespace System::IO;

	public ref class BitmapIO abstract
	{
		public:
			static bool load(String^ filePath, Bitmap^% out_bitmap)
			{
				try {
					Image^ image = Image::FromFile(filePath);
					out_bitmap = gcnew Bitmap(image->Width, image->Height, PixelFormat::Format24bppRgb);
					Graphics::FromImage(out_bitmap)->DrawImage(image, System::Drawing::Rectangle(0, 0, image->Width, image->Height));
					return true;
				}
				catch(FileNotFoundException^ e)
				{ e; return false; }
			}

			static bool save(String^ filePath, Bitmap^ bmp)
			{
				if(bmp != nullptr) {
					bmp->Save(filePath, ImageFormat::Bmp);
					return true;
				}
				return false;
			}
	};
}