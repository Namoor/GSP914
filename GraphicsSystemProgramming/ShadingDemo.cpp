#include "ShadingDemo.h"
#include "ShadingDemo_Structs.h"

#define PLANEVERTEXCOUNTPERDIMENSION 256

#define DERIATIVEDISTANCE 0.002f

void ShadingDemo::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon)
{
	m_pDevice = p_pDevice;
	m_pDevCon = p_pDevCon;

	TimeSinceStart = 0;

	// Vertex Buffer
	const int VertexCount = PLANEVERTEXCOUNTPERDIMENSION * PLANEVERTEXCOUNTPERDIMENSION;

	D3D11_BUFFER_DESC _VBDesc;
	ZeroMemory(&_VBDesc, sizeof(_VBDesc));

	_VBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	_VBDesc.ByteWidth = VertexCount * sizeof(ShadingDemo_Vertex);
	_VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_VBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	
	m_pDevice->CreateBuffer(&_VBDesc, nullptr, &m_pVertexBuffer);

	ShadingDemo_Vertex* _Vertices = new ShadingDemo_Vertex[VertexCount];

	int _CurrentIndex = 0;

	for (int Y = 0; Y < PLANEVERTEXCOUNTPERDIMENSION; Y++)
		for (int X = 0; X < PLANEVERTEXCOUNTPERDIMENSION; X++)
		{
			float _X = (float)X / (PLANEVERTEXCOUNTPERDIMENSION - 1);
			float _Y = (float)Y / (PLANEVERTEXCOUNTPERDIMENSION - 1);

			float _XP = _X * 2 - 1;
			float _YP = _Y * 2 - 1;

			D3DXVECTOR3 _Pos = D3DXVECTOR3(_XP * 5, GetHeightAt(_XP, _YP), _YP * 5);

			D3DXVECTOR3 _ddx = D3DXVECTOR3(_XP * 5 + DERIATIVEDISTANCE, GetHeightAt(_XP + DERIATIVEDISTANCE, _YP), _YP * 5);
			D3DXVECTOR3 _ddy = D3DXVECTOR3(_XP * 5, GetHeightAt(_XP, _YP + DERIATIVEDISTANCE), _YP * 5 + DERIATIVEDISTANCE);

			_ddx -= _Pos;
			_ddy -= _Pos;



			D3DXVec3Normalize(&_Vertices[_CurrentIndex].Tangent, &_ddx);
			//D3DXVec3Normalize(&_Vertices[_CurrentIndex].BiTangent, &_ddy);

			

			_Vertices[_CurrentIndex].Position = _Pos;
			D3DXVec3Cross(&_Vertices[_CurrentIndex].Normal, &_ddy, &_ddx);
			D3DXVec3Normalize(&_Vertices[_CurrentIndex].Normal, &_Vertices[_CurrentIndex].Normal);

			D3DXVec3Cross(&_Vertices[_CurrentIndex].BiTangent, &_Vertices[_CurrentIndex].Tangent, &_Vertices[_CurrentIndex].Normal);
			

			_Vertices[_CurrentIndex].UV = D3DXVECTOR2(_X, _Y);

			_CurrentIndex++;
		}

	D3D11_MAPPED_SUBRESOURCE _VBMSR;

	m_pDevCon->Map(m_pVertexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_VBMSR);
	memcpy(_VBMSR.pData, _Vertices, VertexCount * sizeof(ShadingDemo_Vertex));
	m_pDevCon->Unmap(m_pVertexBuffer, 0);

	delete _Vertices;

	// IndexBuffer
	const int _QuadCountPerDimension = PLANEVERTEXCOUNTPERDIMENSION - 1;

	m_IndexCount = _QuadCountPerDimension * _QuadCountPerDimension * 6;

	D3D11_BUFFER_DESC _IBDesc;
	ZeroMemory(&_IBDesc, sizeof(_IBDesc));

	_IBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	_IBDesc.ByteWidth = m_IndexCount * sizeof(unsigned int);
	_IBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_IBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_IBDesc, nullptr, &m_pIndexBuffer);


	unsigned int* _Indices = new unsigned int[_QuadCountPerDimension * _QuadCountPerDimension * 6];

	_CurrentIndex = 0;

	for (int y = 0; y < _QuadCountPerDimension; y++)
	{
		for (int x = 0; x < _QuadCountPerDimension; x++)
		{
			// CurrentIndex 0

			int _VertexBottomLeft = y * PLANEVERTEXCOUNTPERDIMENSION + x;
			int _VertexBottomRight = _VertexBottomLeft + 1;
			int _VertexTopLeft = _VertexBottomLeft + PLANEVERTEXCOUNTPERDIMENSION;
			int _VertexTopRight = _VertexTopLeft + 1;

			_Indices[_CurrentIndex * 6 + 0] = _VertexBottomLeft;
			_Indices[_CurrentIndex * 6 + 1] = _VertexTopLeft;
			_Indices[_CurrentIndex * 6 + 2] = _VertexTopRight;

			_Indices[_CurrentIndex * 6 + 3] = _VertexBottomLeft;
			_Indices[_CurrentIndex * 6 + 4] = _VertexTopRight;
			_Indices[_CurrentIndex * 6 + 5] = _VertexBottomRight;



			_CurrentIndex++;
		}
	}


	//// Dreieck0
	//_Indices[0] = 0;
	//_Indices[1] = 2;
	//_Indices[2] = 3;
	//
	//// Dreieck1
	//_Indices[3] = 0;
	//_Indices[4] = 3;
	//_Indices[5] = 1;

	D3D11_MAPPED_SUBRESOURCE _IBMSR;

	m_pDevCon->Map(m_pIndexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_IBMSR);
	memcpy(_IBMSR.pData, _Indices, m_IndexCount * sizeof(unsigned int));
	m_pDevCon->Unmap(m_pIndexBuffer, 0);

	delete _Indices;

	// VertexShader
	ID3D10Blob* _pVertexShader;
	ID3D10Blob* _pError;

	if (D3DX11CompileFromFile("ShadingDemo.hlsl", nullptr, nullptr, "VShader", "vs_5_0", 0, 0, nullptr,
		&_pVertexShader, &_pError, nullptr) != S_OK)
	{
		MessageBox(nullptr, (char*)_pError->GetBufferPointer(), "Fehler im VertexShader", MB_OK);

		PostQuitMessage(0);
		return;
	}

	m_pDevice->CreateVertexShader(_pVertexShader->GetBufferPointer(), _pVertexShader->GetBufferSize(), nullptr, &m_pVertexShader);


	// PixelShader
	ID3D10Blob* _pPixelShader;

	if (D3DX11CompileFromFile("ShadingDemo.hlsl", nullptr, nullptr, "PShader", "ps_5_0", 0, 0, nullptr,
		&_pPixelShader, &_pError, nullptr) != S_OK)
	{
		MessageBox(nullptr, (char*)_pError->GetBufferPointer(), "Fehler im PixelShader", MB_OK);

		PostQuitMessage(0);
		return;
	}

	m_pDevice->CreatePixelShader(_pPixelShader->GetBufferPointer(), _pPixelShader->GetBufferSize(), nullptr, &m_pPixelShader);



	// InputLayout

	D3D11_INPUT_ELEMENT_DESC _IED[5];

	_IED[0].SemanticName = "POSITION";
	_IED[0].SemanticIndex = 0;
	_IED[0].AlignedByteOffset = 0;
	_IED[0].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	_IED[0].InputSlot = 0;
	_IED[0].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_IED[0].InstanceDataStepRate = 0;

	_IED[1].SemanticName = "TEXCOORD";
	_IED[1].SemanticIndex = 0;
	_IED[1].AlignedByteOffset = 12;
	_IED[1].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
	_IED[1].InputSlot = 0;
	_IED[1].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_IED[1].InstanceDataStepRate = 0;

	_IED[2].SemanticName = "NORMAL";
	_IED[2].SemanticIndex = 0;
	_IED[2].AlignedByteOffset = 20;
	_IED[2].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	_IED[2].InputSlot = 0;
	_IED[2].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_IED[2].InstanceDataStepRate = 0;

	_IED[3].SemanticName = "NORMAL";  // tangent
	_IED[3].SemanticIndex = 1;
	_IED[3].AlignedByteOffset = 32;
	_IED[3].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	_IED[3].InputSlot = 0;
	_IED[3].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_IED[3].InstanceDataStepRate = 0;

	_IED[4].SemanticName = "NORMAL";   // bitangent
	_IED[4].SemanticIndex = 2;
	_IED[4].AlignedByteOffset = 44;
	_IED[4].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	_IED[4].InputSlot = 0;
	_IED[4].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_IED[4].InstanceDataStepRate = 0;

	m_pDevice->CreateInputLayout(_IED, 5, _pVertexShader->GetBufferPointer(), _pVertexShader->GetBufferSize(), &m_pInputLayout);

	// ConstantBufferMatrix
	D3D11_BUFFER_DESC _MatrixBufferDesc;
	ZeroMemory(&_MatrixBufferDesc, sizeof(_MatrixBufferDesc));

	_MatrixBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	_MatrixBufferDesc.ByteWidth = sizeof(ShadingDemo_MatrixConstantBuffer);
	_MatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_MatrixBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_MatrixBufferDesc, nullptr, &m_pMatrixConstantBuffer);

	// ConstantBufferLight
	D3D11_BUFFER_DESC _LightBufferDesc;
	ZeroMemory(&_LightBufferDesc, sizeof(_LightBufferDesc));

	_LightBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	_LightBufferDesc.ByteWidth = sizeof(ShadingDemo_LightConstantBuffer);
	_LightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_LightBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_LightBufferDesc, nullptr, &m_pLightConstantBuffer);

	// Texture

	D3DX11CreateShaderResourceViewFromFile(m_pDevice, "Rocks_dif.jpg", nullptr, nullptr, &m_pTexture, nullptr);
	D3DX11CreateShaderResourceViewFromFile(m_pDevice, "Rocks_Spec.jpg", nullptr, nullptr, &m_pTexture_Spec, nullptr);
	D3DX11CreateShaderResourceViewFromFile(m_pDevice, "Rocks_Nrm.dds", nullptr, nullptr, &m_pTexture_Nrm, nullptr);
};



float ShadingDemo::GetHeightAt(float x, float z)
{
	//return cos(x * 1.5f) + cos(z* 1.5f);
	//return 0;

	return (sin(x * 10.0f) + sin(z* 10.0f)) * 1.0f;

	float _DistSqr = x * x + z * z;

	if (_DistSqr > 1)
		return 0;


	return sqrt(1 - _DistSqr);

}

void ShadingDemo::Update(float DeltaTime)
{
	TimeSinceStart += DeltaTime;
}

void ShadingDemo::Render(Camera* p_pCamera, DirectionalLight* p_pDirectionalLight)
{
	D3DXMATRIX _Rotation;
	D3DXMatrixRotationY(&_Rotation, 1 * TimeSinceStart);


	// Constant Buffer Aktualisieren
	ShadingDemo_LightConstantBuffer _LCB;
	ShadingDemo_MatrixConstantBuffer _MCB;

	_LCB.CameraPosition = p_pCamera->GetPosition4();

	//_LCB.DirectionalLightDirection = D3DXVECTOR4(0.7f, -0.7f, 0, 0);
	D3DXVECTOR4 _Light = D3DXVECTOR4(0.7f, -0.7f, 0, 0);
	//D3DXVec4Transform(&_LCB.DirectionalLightDirection, &_Light, &_Rotation);
	_LCB.DirectionalLightDirection = p_pDirectionalLight->GetDirection4();
	D3DXVec4Normalize(&_LCB.DirectionalLightDirection, &_LCB.DirectionalLightDirection);
	
	_LCB.DirectionalLightColor = D3DXVECTOR4(1, 1, 1, 0);

	_LCB.PointLightPosition = D3DXVECTOR4(cos(TimeSinceStart * 2), 1, sin(TimeSinceStart * 2), 0);
	_LCB.PointLightColor = D3DXVECTOR4(1, 0, 0, 5);

	_LCB.R_SpecRoughness_G_SpecIntensity.x = 10;
	_LCB.R_SpecRoughness_G_SpecIntensity.y = 0.0;


	D3DXMATRIX _WorldMatrix;

	D3DXMatrixScaling(&_WorldMatrix, 1, 1, 1);
	//D3DXMatrixRotationX(&_WorldMatrix, 1);

	//_WorldMatrix *= _Rotation;

	//D3DXMatrixRotationX(&_WorldMatrix, D3DXToRadian(90));

	_MCB.MVP = _WorldMatrix *  p_pCamera->GetViewMatrix() * p_pCamera->GetProjMatrix();
	_MCB.M = _WorldMatrix;
	_MCB.LightViewProj = p_pDirectionalLight->GetViewProjectionMatrix();


	D3DXMatrixInverse(&_MCB.M_TransInv, nullptr, &_WorldMatrix);
	D3DXMatrixTranspose(&_MCB.M_TransInv, &_MCB.M_TransInv);

	_LCB.M = _MCB.M_TransInv;
	D3D11_MAPPED_SUBRESOURCE _MSR;

	m_pDevCon->Map(m_pLightConstantBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_MSR);
	memcpy(_MSR.pData, &_LCB, sizeof(_LCB));
	m_pDevCon->Unmap(m_pLightConstantBuffer, 0);

	m_pDevCon->Map(m_pMatrixConstantBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_MSR);
	memcpy(_MSR.pData, &_MCB, sizeof(_MCB));
	m_pDevCon->Unmap(m_pMatrixConstantBuffer, 0);

	// Alles Binden

	unsigned int Offset = 0;
	unsigned int Stride = sizeof(ShadingDemo_Vertex);

	m_pDevCon->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	m_pDevCon->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &Stride, &Offset);
	m_pDevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pDevCon->IASetInputLayout(m_pInputLayout);

	m_pDevCon->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pDevCon->VSSetConstantBuffers(0, 1, &m_pMatrixConstantBuffer);
	m_pDevCon->VSSetConstantBuffers(1, 1, &m_pLightConstantBuffer);
	m_pDevCon->VSSetShaderResources(2, 1, &m_pTexture_Nrm);

	m_pDevCon->PSSetShader(m_pPixelShader, nullptr, 0);
	m_pDevCon->PSSetConstantBuffers(0, 1, &m_pLightConstantBuffer);
	m_pDevCon->PSSetShaderResources(0, 1, &m_pTexture);
	m_pDevCon->PSSetShaderResources(1, 1, &m_pTexture_Spec);
	m_pDevCon->PSSetShaderResources(2, 1, &m_pTexture_Nrm);
	ID3D11ShaderResourceView* _ShadowTexture = p_pDirectionalLight->GetShadowMap()->GetTexture();
	m_pDevCon->PSSetShaderResources(3, 1, &_ShadowTexture);

	// DrawCall
	m_pDevCon->DrawIndexed(m_IndexCount, 0, 0);
}



void ShadingDemo::RenderDepth(D3DXMATRIX p_ViewProj, D3DXVECTOR4 RenderingPosition)
{
	D3DXMATRIX _Rotation;
	D3DXMatrixRotationY(&_Rotation, 1 * TimeSinceStart);


	// Constant Buffer Aktualisieren
	ShadingDemo_MatrixConstantBuffer _MCB;


	D3DXMATRIX _WorldMatrix;

	D3DXMatrixScaling(&_WorldMatrix, 1, 1, 1);
	
	_MCB.MVP = _WorldMatrix *  p_ViewProj;
	_MCB.M = _WorldMatrix;


	D3DXMatrixInverse(&_MCB.M_TransInv, nullptr, &_WorldMatrix);
	D3DXMatrixTranspose(&_MCB.M_TransInv, &_MCB.M_TransInv);
	
	
	D3D11_MAPPED_SUBRESOURCE _MSR;

	
	m_pDevCon->Map(m_pMatrixConstantBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_MSR);
	memcpy(_MSR.pData, &_MCB, sizeof(_MCB));
	m_pDevCon->Unmap(m_pMatrixConstantBuffer, 0);

	// Alles Binden

	unsigned int Offset = 0;
	unsigned int Stride = sizeof(ShadingDemo_Vertex);

	m_pDevCon->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	m_pDevCon->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &Stride, &Offset);
	m_pDevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pDevCon->IASetInputLayout(m_pInputLayout);

	m_pDevCon->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pDevCon->VSSetConstantBuffers(0, 1, &m_pMatrixConstantBuffer);
	m_pDevCon->VSSetShaderResources(2, 1, &m_pTexture_Nrm);

	m_pDevCon->PSSetShader(nullptr, nullptr, 0);
	
	// DrawCall
	m_pDevCon->DrawIndexed(m_IndexCount, 0, 0);
}



