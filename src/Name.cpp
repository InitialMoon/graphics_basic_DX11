#include "Name.h"

using namespace DirectX;
NameVertices::NameVertices(D3D11_PRIMITIVE_TOPOLOGY type)
{
	topology = type;

	nameVertices = new GameApp::VertexPosColor[]
	{
		{ XMFLOAT3(0.800000,-1.000000,2.165741), XMFLOAT4(0, 1, 0, 1.0f) },
		{ XMFLOAT3(0.800000,1.000000,2.165741), XMFLOAT4(0, 1, 1, 1.0f) },
		{ XMFLOAT3(0.800000,-1.000000,0.165741), XMFLOAT4(1, 0, 1, 1.0f) },
		{ XMFLOAT3(0.800000,1.000000,0.165741), XMFLOAT4(0, 0, 0, 1.0f) },
		{ XMFLOAT3(1.200000,-1.000000,2.165741), XMFLOAT4(1, 0, 0, 1.0f) },
		{ XMFLOAT3(1.200000,1.000000,2.165741), XMFLOAT4(1, 1, 0, 1.0f) },
		{ XMFLOAT3(1.200000,-1.000000,0.165741), XMFLOAT4(1, 1, 1, 1.0f) },
		{ XMFLOAT3(1.200000,1.000000,0.165741), XMFLOAT4(0, 1, 1, 1.0f) },
		{ XMFLOAT3(-1.200000,-1.000000,3.000000), XMFLOAT4(0, 1, 1, 1.0f) },
		{ XMFLOAT3(-1.200000,1.000000,3.000000), XMFLOAT4(0, 1, 1, 1.0f) },
		{ XMFLOAT3(-1.200000,-1.000000,0.000000), XMFLOAT4(1, 1, 0, 1.0f) },
		{ XMFLOAT3(-1.200000,1.000000,0.000000), XMFLOAT4(0, 0, 0, 1.0f) },
		{ XMFLOAT3(-0.800000,-1.000000,3.000000), XMFLOAT4(0, 1, 0, 1.0f) },
		{ XMFLOAT3(-0.800000,1.000000,3.000000), XMFLOAT4(1, 0, 0, 1.0f) },
		{ XMFLOAT3(-0.800000,-1.000000,0.000000), XMFLOAT4(1, 1, 0, 1.0f) },
		{ XMFLOAT3(-0.800000,1.000000,0.000000), XMFLOAT4(0, 1, 0, 1.0f) },
		{ XMFLOAT3(-3.194206,-1.000000,0.348226), XMFLOAT4(1, 1, 0, 1.0f) },
		{ XMFLOAT3(-3.194206,1.000000,0.348226), XMFLOAT4(1, 1, 1, 1.0f) },
		{ XMFLOAT3(0.555463,-1.000000,-2.959338), XMFLOAT4(0, 1, 0, 1.0f) },
		{ XMFLOAT3(0.555463,1.000000,-2.959338), XMFLOAT4(0, 1, 1, 1.0f) },
		{ XMFLOAT3(-2.929601,-1.000000,0.648199), XMFLOAT4(0, 1, 0, 1.0f) },
		{ XMFLOAT3(-2.929601,1.000000,0.648199), XMFLOAT4(0, 0, 0, 1.0f) },
		{ XMFLOAT3(0.820068,-1.000000,-2.659365), XMFLOAT4(0, 1, 1, 1.0f) },
		{ XMFLOAT3(0.820068,1.000000,-2.659365), XMFLOAT4(0, 0, 1, 1.0f) },
		{ XMFLOAT3(0.056728,-1.000000,-1.959024), XMFLOAT4(1, 1, 1, 1.0f) },
		{ XMFLOAT3(0.056728,1.000000,-1.959024), XMFLOAT4(0, 0, 1, 1.0f) },
		{ XMFLOAT3(0.342146,-1.000000,-2.239268), XMFLOAT4(0, 1, 0, 1.0f) },
		{ XMFLOAT3(0.342146,1.000000,-2.239268), XMFLOAT4(1, 0, 0, 1.0f) },
		{ XMFLOAT3(2.859163,-1.000000,0.895158), XMFLOAT4(0, 0, 0, 1.0f) },
		{ XMFLOAT3(2.859163,1.000000,0.895158), XMFLOAT4(0, 0, 1, 1.0f) },
		{ XMFLOAT3(3.144582,-1.000000,0.614915), XMFLOAT4(0, 1, 1, 1.0f) },
		{ XMFLOAT3(3.144582,1.000000,0.614915), XMFLOAT4(1, 1, 0, 1.0f) }
	};
	verticesCount = 32;

	nameIndices = new WORD[]
	{
		0,2,1,
		2,6,3,
		6,4,7,
		4,0,5,
		2,0,6,
		7,5,3,
		8,10,9,
		10,14,11,
		14,12,15,
		12,8,13,
		10,8,14,
		15,13,11,
		16,18,17,
		18,22,19,
		22,20,23,
		20,16,21,
		16,20,18,
		23,21,19,
		24,26,25,
		26,30,27,
		30,28,31,
		28,24,29,
		24,28,26,
		31,29,27,
		2,3,1,
		6,7,3,
		4,5,7,
		0,1,5,
		0,4,6,
		5,1,3,
		10,11,9,
		14,15,11,
		12,13,15,
		8,9,13,
		8,12,14,
		13,9,11,
		18,19,17,
		22,23,19,
		20,21,23,
		16,17,21,
		20,22,18,
		21,17,19,
		26,27,25,
		30,31,27,
		28,29,31,
		24,25,29,
		28,30,26,
		29,25,27
	};
	indexCount = 144;

	// 处理顶点颜色
	for (UINT i = 0; i < verticesCount; i++)
	{
		nameVertices[i].color.x = nameVertices[i].pos.x + 0.5f;
		nameVertices[i].color.y = nameVertices[i].pos.y + 0.5f;
		nameVertices[i].color.z = nameVertices[i].pos.z * 10.0f + 0.5f;
		nameVertices[i].pos.x *= 3.0f;
		nameVertices[i].pos.y *= 3.0f;
		nameVertices[i].pos.z *= 3.0f;
	}
}

NameVertices::~NameVertices()
{
	delete nameVertices;
}

GameApp::VertexPosColor* NameVertices::GetNameVertices()
{
	return nameVertices;
}

WORD* NameVertices::GetNameIndices()
{
	return nameIndices;
}

D3D11_PRIMITIVE_TOPOLOGY NameVertices::GetTopology()
{
	return topology;
}

UINT NameVertices::GetVerticesCount()
{
	return verticesCount;
}

UINT NameVertices::GetIndexCount()
{
	return indexCount;
}
