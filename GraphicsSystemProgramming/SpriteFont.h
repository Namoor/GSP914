#pragma once

#include "d3dinclude.h"

struct CharDescription
{
	// Position und Größe des Buchstabens auf dem SpriteSheet
	float m_X;
	float m_Y;
	float m_Width;
	float m_Height;

	// Größe des Chars in Relation zur Schriftgröße
	float RelativeHeight;
	float RelativeWidth;

	float RelativeOffsetY;
};

class SpriteFont
{
public:
	SpriteFont();
	~SpriteFont();

	void Load(char* p_pFilename, ID3D11Device* p_pDevice);

	ID3D11ShaderResourceView* m_pSpriteSheet;

	CharDescription m_Chars[256];
};