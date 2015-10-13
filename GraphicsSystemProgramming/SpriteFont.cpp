#include "SpriteFont.h"

#include <fstream>


struct CharDesc
{
	UINT32 ID;
	UINT16 x;
	UINT16 y;
	UINT16 width;
	UINT16 height;
	INT16 xoffset;
	INT16 yoffset;
	INT16 xadvance;
	UINT8 page;
	UINT8 channel;
};

SpriteFont::SpriteFont()
{
	m_pSpriteSheet = nullptr;
	ZeroMemory(&m_Chars, sizeof(m_Chars));
};

SpriteFont::~SpriteFont()
{

}

void SpriteFont::Load(char* p_pFileName, ID3D11Device* p_pDevice)
{
	std::ifstream _FileStream(p_pFileName, std::ios::binary);

	int _VersionID = 0;

	_FileStream.read((char*)&_VersionID, 4);

	if ((_VersionID & 0x00FFFFFF) != 0x00464D42)
		return;

	if ((_VersionID & 0xFF000000) != 0x03000000)
		return;



	char _CurrentBlockType;
	int _CurrentBlockSize;

	char _Trash[1024];
	char _Pages[256];

	int _CharCount = 0;

	CharDesc _Desc;

	while (!_FileStream.eof())
	{
		_CurrentBlockType = _FileStream.get();
		_FileStream.read((char*)&_CurrentBlockSize, 4);


		switch (_CurrentBlockType)
		{
			case 1: // Font Info
				_FileStream.read(_Trash, _CurrentBlockSize);
				break;

			case 2: // Common info
				_FileStream.read(_Trash, _CurrentBlockSize);
				break;

			case 3: // Welche Textur wird verwendet
				_FileStream.read(_Pages, _CurrentBlockSize);
				D3DX11CreateShaderResourceViewFromFile(p_pDevice, _Pages, nullptr, nullptr, &m_pSpriteSheet, nullptr);
				break;


			case 4: // Einzelne Chars
				_CharCount = _CurrentBlockSize / 20;

				for (int x = 0; x < _CharCount; x++)
				{
					_FileStream.read((char*)&_Desc, 20);

					m_Chars[_Desc.ID].m_X = _Desc.x;
					m_Chars[_Desc.ID].m_Y = _Desc.y;
					m_Chars[_Desc.ID].m_Height = _Desc.height;
					m_Chars[_Desc.ID].m_Width = _Desc.width;

					m_Chars[_Desc.ID].RelativeHeight = _Desc.height / 32.0f;
					m_Chars[_Desc.ID].RelativeWidth = _Desc.width / 32.0f;

					m_Chars[_Desc.ID].RelativeOffsetY = _Desc.yoffset / 32.0f;

				}

				break;

			case 5:
				_FileStream.read(_Trash, _CurrentBlockSize);
				
				break;
		}

		int asd = 0;
	}

}