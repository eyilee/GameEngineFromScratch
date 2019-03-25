#include "BmpParser.h"

using namespace Engine;

Image Engine::BmpParser::Parse(const Buffer& buf)
{
	Image img;
	BITMAP_FILE_HEADER* pFileHeader = reinterpret_cast<BITMAP_FILE_HEADER*>(buf.m_pData);
	BITMAP_HEADER* pBmpHeader = reinterpret_cast<BITMAP_HEADER*>(buf.m_pData + BITMAP_FILEHEADER_SIZE);
	if (pFileHeader->Signature == 0x4D42 /* 'B''M' */) {
		std::cout << "Asset is Windows BMP file" << std::endl;
		std::cout << "BMP Header" << std::endl;
		std::cout << "----------------------------" << std::endl;
		std::cout << "File Size: " << pFileHeader->Size << std::endl;
		std::cout << "Data Offset: " << pFileHeader->BitsOffset << std::endl;
		std::cout << "Image Width: " << pBmpHeader->Width << std::endl;
		std::cout << "Image Height: " << pBmpHeader->Height << std::endl;
		std::cout << "Image Planes: " << pBmpHeader->Planes << std::endl;
		std::cout << "Image BitCount: " << pBmpHeader->BitCount << std::endl;
		std::cout << "Image Compression: " << pBmpHeader->Compression << std::endl;
		std::cout << "Image Size: " << pBmpHeader->SizeImage << std::endl;

		img.Width = pBmpHeader->Width;
		img.Height = pBmpHeader->Height;
		img.bitcount = 32;
		img.pitch = ((img.Width * img.bitcount >> 3) + 3) & ~3;
		img.data_size = img.pitch * img.Height;
		img.data = reinterpret_cast<R8G8B8A8Unorm*>(g_pMemoryManager->Allocate(img.data_size));

		if (img.bitcount < 24) {
			std::cout << "Sorry, only true color BMP is supported at now." << std::endl;
		}
		else {
			uint8_t* pSourceData = buf.m_pData + pFileHeader->BitsOffset;
			for (int32_t y = img.Height - 1; y >= 0; y--) {
				for (uint32_t x = 0; x < img.Width; x++) {
					(img.data + img.Width * (img.Height - y - 1) + x)->bgra = *reinterpret_cast<R8G8B8A8Unorm*>(pSourceData + img.pitch * y + x * (img.bitcount >> 3));
				}
			}
		}
	}

	return img;
}
