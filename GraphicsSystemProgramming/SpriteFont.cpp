#include "SpriteFont.h"

#include <fstream>


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

	while (!_FileStream.eof())
	{
		_CurrentBlockType = _FileStream.get();
		_FileStream.read((char*)&_CurrentBlockSize, 4);


		switch (_CurrentBlockType)
		{
			case 1: // Font Info
				_FileStream.read(_Trash, _CurrentBlockSize);
				break;


			case 4: // Einzelne Chars

				break;
		}

		int asd = 0;
	}

}