#pragma once

#include "Particle.h"
#include "Camera.h"

#define MAXPARTICLES 100000


class ParticleSystem
{
public:
	ParticleSystem();

	void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon);

	void Update();

	void Render(Camera* p_pCamera);

	void Update(float DeltaTime);

	void SpawnParticle(D3DXVECTOR3 Position, D3DXVECTOR3 Velocity, D3DXVECTOR4 Color);

private:
	Particle m_Particles[MAXPARTICLES];
	Particle_Vertex m_Vertices[MAXPARTICLES * 4];

	int m_ParticleCount;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDevCon;

private:
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;

	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;

	ID3D11InputLayout* m_pInputLayout;

	ID3D11ShaderResourceView* m_pTexture;

	ID3D11Buffer* m_pConstantBuffer;
};