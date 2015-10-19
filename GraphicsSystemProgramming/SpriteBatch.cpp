#include "SpriteBatch.h"

#define BATCHSIZE 10000

SpriteBatch::SpriteBatch()
{
	m_pDevCon = nullptr;
	m_pDevice = nullptr;

	m_pInputLayout = nullptr;

	m_pPixelShader = nullptr;
	m_pVertexShader = nullptr;

	m_pIndexBuffer = nullptr;
	m_pVertexBuffer = nullptr;

	m_pCurrentTexture = nullptr;
}

void SpriteBatch::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pContext)
{
	m_pDevCon = p_pContext;
	m_pDevice = p_pDevice;

	m_pVertices = new SpriteBatch_Vertex[BATCHSIZE * 4];
	m_DrawCallCount = 0;

	m_ScreenHeight = 600;
	m_ScreenWidth = 800;

	// VertexBuffer erstellen
	D3D11_BUFFER_DESC _VBufferDesc;
	ZeroMemory(&_VBufferDesc, sizeof(_VBufferDesc));

	_VBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	_VBufferDesc.ByteWidth = BATCHSIZE * 4 *sizeof(SpriteBatch_Vertex);
	_VBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_VBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_VBufferDesc, nullptr, &m_pVertexBuffer);

	// IndexBuffer erstellen
	D3D11_BUFFER_DESC _IBufferDesc;
	ZeroMemory(&_IBufferDesc, sizeof(_IBufferDesc));

	_IBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	_IBufferDesc.ByteWidth = BATCHSIZE * 6 * sizeof(unsigned int);
	_IBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_IBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_IBufferDesc, nullptr, &m_pIndexBuffer);

	unsigned int _Indices[BATCHSIZE * 6];
	for (int x = 0; x < BATCHSIZE; x++)
	{
		_Indices[x * 6 + 0] = x * 4 + 0;
		_Indices[x * 6 + 1] = x * 4 + 1;
		_Indices[x * 6 + 2] = x * 4 + 2;
		_Indices[x * 6 + 3] = x * 4 + 0;
		_Indices[x * 6 + 4] = x * 4 + 2;
		_Indices[x * 6 + 5] = x * 4 + 3;
	}
	D3D11_MAPPED_SUBRESOURCE _MSR;

	m_pDevCon->Map(m_pIndexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_MSR);
	memcpy(_MSR.pData, _Indices, BATCHSIZE * 6 * sizeof(unsigned int));
	m_pDevCon->Unmap(m_pIndexBuffer, 0);


	// VertexShader laden
	ID3D10Blob* _pVertexShader;
	ID3D10Blob* _pShaderError;

	if (S_OK != D3DX11CompileFromFile(
		TEXT("SpriteBatch.hlsl"), nullptr, nullptr, TEXT("VShader"),
		TEXT("vs_5_0"), 0, 0, nullptr, &_pVertexShader, &_pShaderError, nullptr))
	{
		MessageBox(0, (char*)_pShaderError->GetBufferPointer(),
			TEXT("Fehler im VShader"), 0);
		PostQuitMessage(1);
		return;
	}
	m_pDevice->CreateVertexShader(_pVertexShader->GetBufferPointer(),
		_pVertexShader->GetBufferSize(), nullptr, &m_pVertexShader);
	// PixelShader laden
	ID3D10Blob* _pPixelShader;

	if (S_OK != D3DX11CompileFromFile(
		TEXT("SpriteBatch.hlsl"), nullptr, nullptr, TEXT("PShader"),
		TEXT("ps_5_0"), 0, 0, nullptr, &_pPixelShader, &_pShaderError, nullptr))
	{
		MessageBox(0, (char*)_pShaderError->GetBufferPointer(),
			TEXT("Fehler im PShader"), 0);
		PostQuitMessage(1);
		return;
	}
	m_pDevice->CreatePixelShader(_pPixelShader->GetBufferPointer(),
		_pPixelShader->GetBufferSize(), nullptr, &m_pPixelShader);
	// InputLayout anlegen
	
	D3D11_INPUT_ELEMENT_DESC _IED[3];
	_IED[0].SemanticName = "POSITION";
	_IED[0].SemanticIndex = 0;
	_IED[0].AlignedByteOffset = 0;
	_IED[0].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	_IED[0].InputSlot = 0;
	_IED[0].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_IED[0].InstanceDataStepRate = 0;

	_IED[1].SemanticName = "COLOR";
	_IED[1].SemanticIndex = 0;
	_IED[1].AlignedByteOffset = 12;
	_IED[1].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
	_IED[1].InputSlot = 0;
	_IED[1].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_IED[1].InstanceDataStepRate = 0;

	_IED[2].SemanticName = "TEXCOORD";
	_IED[2].SemanticIndex = 0;
	_IED[2].AlignedByteOffset = 28;
	_IED[2].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
	_IED[2].InputSlot = 0;
	_IED[2].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_IED[2].InstanceDataStepRate = 0;

	m_pDevice->CreateInputLayout(_IED, 3, _pVertexShader->GetBufferPointer(),
		_pVertexShader->GetBufferSize(), &m_pInputLayout);

	D3D11_BLEND_DESC _BDesc;
	ZeroMemory(&_BDesc, sizeof(_BDesc));

	_BDesc.AlphaToCoverageEnable = false;
	_BDesc.IndependentBlendEnable = false;

	_BDesc.RenderTarget[0].BlendEnable = true;
	_BDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	_BDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	_BDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	_BDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;


	_BDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	_BDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
	_BDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;

	m_pDevice->CreateBlendState(&_BDesc, &m_pBlendState);

}

void SpriteBatch::Begin()
{
	m_DrawCallCount = 0;
}

void SpriteBatch::End()
{
	if (m_DrawCallCount == 0)
		return;

	// Daten von m_pVertices auf die Grafikkarte kopieren (m_pVertexBuffer)
	D3D11_MAPPED_SUBRESOURCE _MSR;

	m_pDevCon->Map(m_pVertexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_MSR);
	memcpy(_MSR.pData, m_pVertices, m_DrawCallCount * 4 * sizeof(SpriteBatch_Vertex));
	m_pDevCon->Unmap(m_pVertexBuffer, 0);
		

	// States setzen
	m_pDevCon->PSSetShaderResources(0, 1, &m_pCurrentTexture);

	m_pDevCon->OMSetBlendState(m_pBlendState, nullptr, 0xFFFFFFFF);
	
	m_pDevCon->PSSetShader(m_pPixelShader, nullptr, 0);
	m_pDevCon->VSSetShader(m_pVertexShader, nullptr, 0);

	UINT _Stride = sizeof(SpriteBatch_Vertex);
	UINT _Offset = 0;
	m_pDevCon->IASetInputLayout(m_pInputLayout);
	m_pDevCon->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	m_pDevCon->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &_Stride, &_Offset);

	m_pDevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// DrawCall
	m_pDevCon->DrawIndexed(m_DrawCallCount * 6, 0, 0);
}

void SpriteBatch::DrawTexture(ID3D11ShaderResourceView* p_pSRV, Rect Destination, Rect Source, D3DXVECTOR4 Color)
{
	if (p_pSRV != m_pCurrentTexture)
	{
		End();
		Begin();
		m_pCurrentTexture = p_pSRV;
	}

	float _PosXStart = Destination.x / m_ScreenWidth * 2 - 1;
	float _PosXEnd = (Destination.x + Destination.width) / m_ScreenWidth * 2 - 1;
	float _PosYStart = -1 * (Destination.y / m_ScreenHeight * 2 - 1);
	float _PosYEnd = -1 * ((Destination.y + Destination.height) / m_ScreenHeight * 2 - 1);

	m_pVertices[m_DrawCallCount * 4 + 0].Position
		= D3DXVECTOR3(_PosXStart, _PosYEnd, 0);
	m_pVertices[m_DrawCallCount * 4 + 1].Position
		= D3DXVECTOR3(_PosXStart, _PosYStart, 0);
	m_pVertices[m_DrawCallCount * 4 + 2].Position
		= D3DXVECTOR3(_PosXEnd, _PosYStart, 0);
	m_pVertices[m_DrawCallCount * 4 + 3].Position
		= D3DXVECTOR3(_PosXEnd, _PosYEnd, 0);

	m_pVertices[m_DrawCallCount * 4 + 0].Color = Color;
	m_pVertices[m_DrawCallCount * 4 + 1].Color = Color;
	m_pVertices[m_DrawCallCount * 4 + 2].Color = Color;
	m_pVertices[m_DrawCallCount * 4 + 3].Color = Color;

	float _UVXStart = Source.x;
	float _UVXEnd = Source.x + Source.width;
	float _UVYStart = Source.y;
	float _UVYEnd = Source.y + Source.height;

	m_pVertices[m_DrawCallCount * 4 + 0].TexCoord = D3DXVECTOR2(_UVXStart, _UVYEnd);
	m_pVertices[m_DrawCallCount * 4 + 1].TexCoord = D3DXVECTOR2(_UVXStart, _UVYStart);
	m_pVertices[m_DrawCallCount * 4 + 2].TexCoord = D3DXVECTOR2(_UVXEnd, _UVYStart);
	m_pVertices[m_DrawCallCount * 4 + 3].TexCoord = D3DXVECTOR2(_UVXEnd, _UVYEnd);


	m_DrawCallCount++;

	if (m_DrawCallCount >= BATCHSIZE)
	{
		End();
		Begin();
	}
}


void SpriteBatch::DrawTexture(ID3D11ShaderResourceView* p_pTexture, Rect Destination, Rect Source)
{
	DrawTexture(p_pTexture, Destination, Source, D3DXVECTOR4(1, 1, 1, 1));
}

void SpriteBatch::DrawTexture(ID3D11ShaderResourceView* p_pTexture, Rect Destination, D3DXVECTOR4 p_Color)
{
	DrawTexture(p_pTexture, Destination, Rect(0, 0, 1, 1), p_Color);
}

void SpriteBatch::DrawTexture(ID3D11ShaderResourceView* p_pTexture, Rect Destination)
{
	DrawTexture(p_pTexture, Destination, Rect(0, 0, 1, 1), D3DXVECTOR4(1, 1, 1, 1));
}


void SpriteBatch::DrawString(SpriteFont* p_pFont, const char* p_pText, float p_X, float p_Y, float p_Height, D3DXVECTOR4 p_Color)
{
	int _LetterCount = strlen(p_pText);

	float _XAdvance = 0;

	for (int x = 0; x < _LetterCount; x++)
	{
		// Einen Buchstaben Zeichnen

		// Bei Leerzeichen einfach nur etwas weiter gehen und nichts zeichnen
		if (p_pText[x] == ' ')
		{
			_XAdvance += 10;
			continue;
		}


		int _CharID = p_pText[x];

		if (_CharID < 0)
			_CharID += 256;

		CharDescription _Desc = p_pFont->m_Chars[_CharID];

		// Schritt 1 Destination Rect

		Rect _Destination(p_X + _XAdvance,
			p_Y + _Desc.RelativeOffsetY * p_Height,
			_Desc.RelativeWidth * p_Height,
			_Desc.RelativeHeight * p_Height);

		// Schritt 2 Source Rectangle
		Rect _Source(_Desc.m_X / 256.0f, _Desc.m_Y / 256.0f, _Desc.m_Width / 256.0f, _Desc.m_Height / 256.0f);

		// Schritt 3 In X-Richtung weiter gehen
		_XAdvance += _Desc.RelativeWidth * p_Height;


		// Schritt 4 Draw Texture
		DrawTexture(p_pFont->m_pSpriteSheet, _Destination, _Source, p_Color);

	}

}

