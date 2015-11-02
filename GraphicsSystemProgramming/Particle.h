#pragma once

#include "d3dinclude.h"

struct Particle
{
	D3DXVECTOR3 Position;
	D3DXVECTOR4 Color;
	D3DXVECTOR3 Velocity;
};


struct Particle_Vertex
{
	D3DXVECTOR3 Position; // 12
	D3DXVECTOR2 PositionInQuad; // 8
	D3DXVECTOR4 Color; // 16
	D3DXVECTOR2 UV;
};

struct Particle_CBuffer
{
	D3DXMATRIX m_WVP;
	D3DXVECTOR4 m_Right;
	D3DXVECTOR4 m_Up;
};