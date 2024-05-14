//***************************************************************************************
// Geometry.h by X_Jun(MKXJun) (C) 2018-2019 All Rights Reserved.
// Licensed under the MIT License.
//
// 生成常见的几何体网格模型
// Generate common geometry meshes.
//***************************************************************************************

#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <vector>
#include <string>
#include <map>
#include "Vertex.h"

namespace Geometry
{
	// 网格数据
	template<class VertexType = VertexPosNormalTex, class IndexType = WORD>
	struct MeshData
	{
		std::vector<VertexType> vertexVec;	// 顶点数组
		std::vector<IndexType> indexVec;	// 索引数组

		MeshData()
		{
			// 需检验索引类型合法性
			static_assert(sizeof(IndexType) == 2 || sizeof(IndexType) == 4, "The size of IndexType must be 2 bytes or 4 bytes!");
			static_assert(std::is_unsigned<IndexType>::value, "IndexType must be unsigned integer!");
		}
	};

	// 新增的名字模板
	template<class VertexType = VertexPosNormalTex, class IndexType = WORD>
	MeshData<VertexType, IndexType> CreateName(const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

	// 创建球体网格数据，levels和slices越大，精度越高。
	template<class VertexType = VertexPosNormalTex, class IndexType = WORD>
	MeshData<VertexType, IndexType> CreateSphere(float radius = 1.0f, UINT levels = 20, UINT slices = 20,
		const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

	// 创建立方体网格数据
	template<class VertexType = VertexPosNormalTex, class IndexType = WORD>
	MeshData<VertexType, IndexType> CreateBox(float width = 2.0f, float height = 2.0f, float depth = 2.0f,
		const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

	// 创建圆柱体网格数据，slices越大，精度越高。
	template<class VertexType = VertexPosNormalTex, class IndexType = WORD>
	MeshData<VertexType, IndexType> CreateCylinder(float radius = 1.0f, float height = 2.0f, UINT slices = 20,
		const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

	// 创建只有圆柱体侧面的网格数据，slices越大，精度越高
	template<class VertexType = VertexPosNormalTex, class IndexType = WORD>
	MeshData<VertexType, IndexType> CreateCylinderNoCap(float radius = 1.0f, float height = 2.0f, UINT slices = 20,
		const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

	// 创建圆锥体网格数据，slices越大，精度越高。
	template<class VertexType = VertexPosNormalTex, class IndexType = WORD>
	MeshData<VertexType, IndexType> CreateCone(float radius = 1.0f, float height = 2.0f, UINT slices = 20,
		const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

	// 创建只有圆锥体侧面网格数据，slices越大，精度越高。
	template<class VertexType = VertexPosNormalTex, class IndexType = WORD>
	MeshData<VertexType, IndexType> CreateConeNoCap(float radius = 1.0f, float height = 2.0f, UINT slices = 20,
		const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

	// 创建一个覆盖NDC屏幕的面
	template<class VertexType = VertexPosTex, class IndexType = WORD>
	MeshData<VertexType, IndexType> Create2DShow(const DirectX::XMFLOAT2& center, const DirectX::XMFLOAT2& scale = { 1.0f, 1.0f },
		const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	template<class VertexType = VertexPosTex, class IndexType = WORD>
	MeshData<VertexType, IndexType> Create2DShow(float centerX = 0.0f, float centerY = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f,
		const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

	// 创建一个平面
	template<class VertexType = VertexPosNormalTex, class IndexType = WORD>
	MeshData<VertexType, IndexType> CreatePlane(const DirectX::XMFLOAT3& center, const DirectX::XMFLOAT2& planeSize = { 10.0f, 10.0f },
		const DirectX::XMFLOAT2& maxTexCoord = { 1.0f, 1.0f }, const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
	template<class VertexType = VertexPosNormalTex, class IndexType = WORD>
	MeshData<VertexType, IndexType> CreatePlane(float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f,
		float width = 10.0f, float depth = 10.0f, float texU = 1.0f, float texV = 1.0f,
		const DirectX::XMFLOAT4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
}





namespace Geometry
{
	namespace Internal
	{
		//
		// 以下结构体和函数仅供内部实现使用
		//

		struct VertexData
		{
			DirectX::XMFLOAT3 pos;
			DirectX::XMFLOAT3 normal;
			DirectX::XMFLOAT4 tangent;
			DirectX::XMFLOAT4 color;
			DirectX::XMFLOAT2 tex;
		};

		// 根据目标顶点类型选择性将数据插入
		template<class VertexType>
		inline void InsertVertexElement(VertexType& vertexDst, const VertexData& vertexSrc)
		{
			// semanticName 语义名
			static std::string semanticName;
			// 定义了语义的映射表，创建了这么一个映射，一个语义对应语段内存空间，自定义了一个语义对应的变量类型
			// 包括位置，法线，tangent切线，颜色，纹理坐标
			static const std::map<std::string, std::pair<size_t, size_t>> semanticSizeMap = {
				{"POSITION", std::pair<size_t, size_t>(0, 12)},
				{"NORMAL", std::pair<size_t, size_t>(12, 24)},
				{"TANGENT", std::pair<size_t, size_t>(24, 40)},
				{"COLOR", std::pair<size_t, size_t>(40, 56)},
				{"TEXCOORD", std::pair<size_t, size_t>(56, 64)}
			};

			for (size_t i = 0; i < ARRAYSIZE(VertexType::inputLayout); i++)
			{
				semanticName = VertexType::inputLayout[i].SemanticName;
				const auto& range = semanticSizeMap.at(semanticName);
				memcpy_s(reinterpret_cast<char*>(&vertexDst) + VertexType::inputLayout[i].AlignedByteOffset,
					range.second - range.first,
					reinterpret_cast<const char*>(&vertexSrc) + range.first,
					range.second - range.first);
			}
		}
	}
	
	//
	// 几何体方法的实现
	//

	// 新增
	// 名字模型的创建
	template<class VertexType, class IndexType>
	inline MeshData<VertexType, IndexType> CreateName(const DirectX::XMFLOAT4& color)
	{
		using namespace DirectX;

		MeshData<VertexType, IndexType> meshData;
		meshData.vertexVec.resize(96);
		Internal::VertexData vertexDataArr[96];
		// 存放名字顶点数据的结构体
		struct data {
			XMFLOAT3 pos;
			XMFLOAT4 color;
			XMFLOAT3 normal;
			XMFLOAT2 tex;
		};

		const data NAME[96] = {
			{XMFLOAT3(0.800000,-1.000000,2.16574),XMFLOAT4(1, 1, 1, 1.0f),XMFLOAT3(-0.0000,-1.0000,-0.0000),XMFLOAT2(0.125000,0.750000)},
			{XMFLOAT3(0.800000,1.000000,2.16574),XMFLOAT4(0, 1, 1, 1.0f),XMFLOAT3(-0.0000,1.0000,0.0004),XMFLOAT2(0.875000,0.750000)},
			{XMFLOAT3(0.800000,-1.000000,0.16574),XMFLOAT4(0, 0, 0, 1.0f),XMFLOAT3(-1.0000,-0.0000,0.0027),XMFLOAT2(0.375000,0.250000)},
			{XMFLOAT3(0.800000,1.000000,0.16574),XMFLOAT4(1, 0, 0, 1.0f),XMFLOAT3(-1.0000,-0.0000,0.0027),XMFLOAT2(0.625000,0.250000)},
			{XMFLOAT3(1.200000,-1.000000,2.16574),XMFLOAT4(1, 0, 0, 1.0f),XMFLOAT3(-0.0000,-1.0000,-0.0000),XMFLOAT2(0.375000,0.750000)},
			{XMFLOAT3(1.200000,1.000000,2.16574),XMFLOAT4(0, 0, 1, 1.0f),XMFLOAT3(1.0000,-0.0000,-0.0000),XMFLOAT2(0.625000,0.750000)},
			{XMFLOAT3(1.200000,-1.000000,0.16574),XMFLOAT4(0, 0, 1, 1.0f),XMFLOAT3(-0.0000,-1.0000,-0.0000),XMFLOAT2(0.375000,0.500000)},
			{XMFLOAT3(1.200000,1.000000,0.16574),XMFLOAT4(0, 0, 1, 1.0f),XMFLOAT3(1.0000,-0.0000,-0.0000),XMFLOAT2(0.625000,0.500000)},
			{XMFLOAT3(-1.200000,-1.000000,3.00000),XMFLOAT4(1, 0, 0, 1.0f),XMFLOAT3(-1.0000,-0.0000,-0.0034),XMFLOAT2(0.375000,0.000000)},
			{XMFLOAT3(-1.200000,1.000000,3.00000),XMFLOAT4(0, 1, 1, 1.0f),XMFLOAT3(-0.0000,0.0013,1.0000),XMFLOAT2(0.625000,1.000000)},
			{XMFLOAT3(-1.200000,-1.000000,0.00000),XMFLOAT4(1, 1, 1, 1.0f),XMFLOAT3(-0.0000,-1.0000,-0.0000),XMFLOAT2(0.125000,0.500000)},
			{XMFLOAT3(-1.200000,1.000000,0.00000),XMFLOAT4(1, 1, 1, 1.0f),XMFLOAT3(-0.0000,1.0000,-0.0000),XMFLOAT2(0.875000,0.500000)},
			{XMFLOAT3(-0.800000,-1.000000,3.00000),XMFLOAT4(1, 1, 1, 1.0f),XMFLOAT3(-0.0000,-1.0000,-0.0000),XMFLOAT2(0.375000,0.750000)},
			{XMFLOAT3(-0.800000,1.000000,3.00000),XMFLOAT4(1, 1, 1, 1.0f),XMFLOAT3(-0.0000,0.0013,1.0000),XMFLOAT2(0.625000,0.750000)},
			{XMFLOAT3(-0.800000,-1.000000,0.00000),XMFLOAT4(0, 1, 1, 1.0f),XMFLOAT3(-0.0000,-1.0000,-0.0000),XMFLOAT2(0.375000,0.500000)},
			{XMFLOAT3(-0.800000,1.000000,0.00000),XMFLOAT4(1, 0, 0, 1.0f),XMFLOAT3(-0.0000,1.0000,-0.0000),XMFLOAT2(0.625000,0.500000)},
			{XMFLOAT3(-3.194206,-1.000000,0.34822),XMFLOAT4(1, 1, 0, 1.0f),XMFLOAT3(-0.6615,-0.0000,-0.7499),XMFLOAT2(0.375000,0.000000)},
			{XMFLOAT3(-3.194206,1.000000,0.34822),XMFLOAT4(1, 0, 1, 1.0f),XMFLOAT3(-0.0009,1.0000,-0.0011),XMFLOAT2(0.875000,0.750000)},
			{XMFLOAT3(0.555463,-1.000000,-2.95933),XMFLOAT4(0, 0, 1, 1.0f),XMFLOAT3(-0.6615,-0.0000,-0.7499),XMFLOAT2(0.375000,0.250000)},
			{XMFLOAT3(0.555463,1.000000,-2.95933),XMFLOAT4(0, 0, 0, 1.0f),XMFLOAT3(-0.6614,-0.0004,-0.7500),XMFLOAT2(0.625000,0.250000)},
			{XMFLOAT3(-2.929601,-1.000000,0.64819),XMFLOAT4(1, 1, 0, 1.0f),XMFLOAT3(-0.7499,-0.0000,0.6615),XMFLOAT2(0.375000,0.750000)},
			{XMFLOAT3(-2.930224,1.000561,0.64757),XMFLOAT4(1, 1, 0, 1.0f),XMFLOAT3(-0.7516,0.0005,0.6596),XMFLOAT2(0.625000,0.750000)},
			{XMFLOAT3(0.820068,-1.000000,-2.65936),XMFLOAT4(0, 1, 0, 1.0f),XMFLOAT3(0.6611,-0.0014,0.7503),XMFLOAT2(0.375000,0.500000)},
			{XMFLOAT3(0.820068,1.000000,-2.65936),XMFLOAT4(0, 1, 1, 1.0f),XMFLOAT3(0.7218,0.0083,-0.6920),XMFLOAT2(0.625000,0.500000)},
			{XMFLOAT3(0.056728,-1.000000,-1.95902),XMFLOAT4(0, 1, 1, 1.0f),XMFLOAT3(-0.7135,-0.0000,0.7006),XMFLOAT2(0.375000,1.000000)},
			{XMFLOAT3(0.056728,1.000000,-1.95902),XMFLOAT4(0, 0, 1, 1.0f),XMFLOAT3(-0.7135,-0.0000,0.7006),XMFLOAT2(0.625000,1.000000)},
			{XMFLOAT3(0.342146,-1.000000,-2.23926),XMFLOAT4(1, 1, 0, 1.0f),XMFLOAT3(-0.0000,-1.0000,-0.0000),XMFLOAT2(0.125000,0.500000)},
			{XMFLOAT3(0.342146,1.000000,-2.23926),XMFLOAT4(0, 1, 0, 1.0f),XMFLOAT3(0.7267,-0.0000,-0.6869),XMFLOAT2(0.625000,0.250000)},
			{XMFLOAT3(2.859163,-1.000000,0.89515),XMFLOAT4(1, 0, 1, 1.0f),XMFLOAT3(-0.0000,-1.0000,-0.0000),XMFLOAT2(0.375000,0.750000)},
			{XMFLOAT3(2.859163,1.000000,0.89515),XMFLOAT4(1, 0, 1, 1.0f),XMFLOAT3(-0.7135,-0.0000,0.7006),XMFLOAT2(0.625000,0.750000)},
			{XMFLOAT3(3.144582,-1.000000,0.61491),XMFLOAT4(1, 1, 1, 1.0f),XMFLOAT3(-0.0000,-1.0000,-0.0000),XMFLOAT2(0.375000,0.500000)},
			{XMFLOAT3(3.103382,1.000373,0.65376),XMFLOAT4(0, 0, 1, 1.0f),XMFLOAT3(-0.0009,1.0000,0.0007),XMFLOAT2(0.625072,0.535383)},
			{XMFLOAT3(0.547587,1.000000,-2.94358),XMFLOAT4(1, 1, 1, 1.0f),XMFLOAT3(0.0001,1.0000,-0.0001),XMFLOAT2(0.875000,0.500000)},
			{XMFLOAT3(0.820068,1.000000,-2.65936),XMFLOAT4(0, 1, 1, 1.0f),XMFLOAT3(0.6615,-0.0000,0.7499),XMFLOAT2(0.625000,0.500000)},
			{XMFLOAT3(0.342146,1.000000,-2.23926),XMFLOAT4(0, 1, 1, 1.0f),XMFLOAT3(-0.7006,-0.0000,-0.7135),XMFLOAT2(0.625000,0.250000)},
			{XMFLOAT3(0.820068,-1.000000,-2.65936),XMFLOAT4(0, 1, 1, 1.0f),XMFLOAT3(-0.0000,-1.0000,-0.0000),XMFLOAT2(0.375000,0.500000)},
			{XMFLOAT3(0.555463,-1.000000,-2.95933),XMFLOAT4(1, 1, 0, 1.0f),XMFLOAT3(0.7218,0.0083,-0.6920),XMFLOAT2(0.375000,0.250000)},
			{XMFLOAT3(-2.929601,1.000000,0.64819),XMFLOAT4(0, 1, 1, 1.0f),XMFLOAT3(0.6611,-0.0014,0.7503),XMFLOAT2(0.625000,0.750000)},
			{XMFLOAT3(-2.930224,1.000561,0.64757),XMFLOAT4(0, 0, 0, 1.0f),XMFLOAT3(0.0001,1.0000,-0.0001),XMFLOAT2(0.625000,0.750000)},
			{XMFLOAT3(-3.195091,1.000000,0.34803),XMFLOAT4(1, 1, 1, 1.0f),XMFLOAT3(-0.6614,-0.0004,-0.7500),XMFLOAT2(0.625000,0.000000)},
			{XMFLOAT3(-3.193958,1.001371,0.34707),XMFLOAT4(0, 0, 1, 1.0f),XMFLOAT3(-0.7516,0.0005,0.6596),XMFLOAT2(0.625000,1.000000)},
			{XMFLOAT3(-3.194206,-1.000000,0.34822),XMFLOAT4(1, 1, 0, 1.0f),XMFLOAT3(-0.7499,-0.0000,0.6615),XMFLOAT2(0.375000,1.000000)},
			{XMFLOAT3(-3.194206,-1.000000,0.34822),XMFLOAT4(0, 1, 1, 1.0f),XMFLOAT3(-0.0000,-1.0000,-0.0000),XMFLOAT2(0.125000,0.750000)},
			{XMFLOAT3(-2.930887,-0.997088,0.64550),XMFLOAT4(0, 1, 1, 1.0f),XMFLOAT3(0.6611,-0.0014,0.7503),XMFLOAT2(0.375000,0.750000)},
			{XMFLOAT3(-2.929601,-1.000000,0.64819),XMFLOAT4(1, 0, 1, 1.0f),XMFLOAT3(-0.0000,-1.0000,-0.0000),XMFLOAT2(0.375000,0.750000)},
			{XMFLOAT3(0.555463,-1.000000,-2.95933),XMFLOAT4(1, 0, 0, 1.0f),XMFLOAT3(-0.0000,-1.0000,-0.0000),XMFLOAT2(0.125000,0.500000)},
			{XMFLOAT3(0.820068,-1.000000,-2.65936),XMFLOAT4(1, 1, 0, 1.0f),XMFLOAT3(0.7499,-0.0000,-0.6615),XMFLOAT2(0.375000,0.500000)},
			{XMFLOAT3(3.144582,-1.000000,0.61491),XMFLOAT4(0, 0, 1, 1.0f),XMFLOAT3(0.7130,0.0377,-0.7001),XMFLOAT2(0.375000,0.500000)},
			{XMFLOAT3(3.144582,-1.000000,0.61491),XMFLOAT4(0, 1, 1, 1.0f),XMFLOAT3(0.7006,0.0017,0.7135),XMFLOAT2(0.375000,0.500000)},
			{XMFLOAT3(2.859163,-1.000000,0.89515),XMFLOAT4(0, 0, 0, 1.0f),XMFLOAT3(0.7006,0.0017,0.7135),XMFLOAT2(0.375000,0.750000)},
			{XMFLOAT3(2.859163,-1.000000,0.89515),XMFLOAT4(0, 0, 1, 1.0f),XMFLOAT3(-0.7135,-0.0000,0.7006),XMFLOAT2(0.375000,0.750000)},
			{XMFLOAT3(3.083571,1.008121,0.66093),XMFLOAT4(1, 0, 0, 1.0f),XMFLOAT3(0.7267,-0.0000,-0.6869),XMFLOAT2(0.624142,0.494898)},
			{XMFLOAT3(2.862646,1.002550,0.88074),XMFLOAT4(1, 1, 0, 1.0f),XMFLOAT3(0.6911,0.0040,0.7227),XMFLOAT2(0.625000,0.750000)},
			{XMFLOAT3(2.865870,1.000000,0.88857),XMFLOAT4(0, 1, 1, 1.0f),XMFLOAT3(-0.0009,1.0000,0.0007),XMFLOAT2(0.625000,0.744125)},
			{XMFLOAT3(1.200000,-1.000000,2.16574),XMFLOAT4(0, 0, 1, 1.0f),XMFLOAT3(1.0000,-0.0000,-0.0000),XMFLOAT2(0.375000,0.750000)},
			{XMFLOAT3(1.200000,-1.000000,2.16574),XMFLOAT4(1, 0, 0, 1.0f),XMFLOAT3(-0.0021,-0.0000,1.0000),XMFLOAT2(0.375000,0.750000)},
			{XMFLOAT3(0.805328,-1.000000,2.16491),XMFLOAT4(1, 0, 1, 1.0f),XMFLOAT3(-0.0021,-0.0000,1.0000),XMFLOAT2(0.375000,1.000000)},
			{XMFLOAT3(1.200000,1.000000,2.16574),XMFLOAT4(1, 0, 1, 1.0f),XMFLOAT3(-0.0000,-0.0004,1.0000),XMFLOAT2(0.625000,0.750000)},
			{XMFLOAT3(1.200000,1.000000,2.16574),XMFLOAT4(0, 1, 0, 1.0f),XMFLOAT3(-0.0000,1.0000,0.0004),XMFLOAT2(0.625000,0.750000)},
			{XMFLOAT3(0.800000,1.000000,2.16574),XMFLOAT4(1, 0, 0, 1.0f),XMFLOAT3(-1.0000,-0.0027,-0.0000),XMFLOAT2(0.625000,0.000000)},
			{XMFLOAT3(0.800000,1.000000,2.16574),XMFLOAT4(0, 1, 0, 1.0f),XMFLOAT3(-0.0000,-0.0004,1.0000),XMFLOAT2(0.625000,1.000000)},
			{XMFLOAT3(0.800000,-1.000000,0.16574),XMFLOAT4(1, 1, 0, 1.0f),XMFLOAT3(-0.0000,-0.0002,-1.0000),XMFLOAT2(0.375000,0.250000)},
			{XMFLOAT3(0.800000,-1.000000,0.16574),XMFLOAT4(1, 1, 1, 1.0f),XMFLOAT3(-0.0000,-1.0000,-0.0000),XMFLOAT2(0.125000,0.500000)},
			{XMFLOAT3(1.200000,-1.000000,0.16574),XMFLOAT4(1, 0, 1, 1.0f),XMFLOAT3(-0.0000,-0.0002,-1.0000),XMFLOAT2(0.375000,0.500000)},
			{XMFLOAT3(1.200000,-1.000000,0.16574),XMFLOAT4(1, 0, 0, 1.0f),XMFLOAT3(1.0000,-0.0000,-0.0000),XMFLOAT2(0.375000,0.500000)},
			{XMFLOAT3(-0.800000,-1.000000,0.00000),XMFLOAT4(0, 1, 0, 1.0f),XMFLOAT3(-0.0200,-0.0000,-0.9998),XMFLOAT2(0.375000,0.500000)},
			{XMFLOAT3(-0.800000,-1.000000,0.00000),XMFLOAT4(0, 1, 1, 1.0f),XMFLOAT3(1.0000,-0.0000,-0.0000),XMFLOAT2(0.375000,0.500000)},
			{XMFLOAT3(-1.189768,-1.000000,0.00778),XMFLOAT4(1, 1, 0, 1.0f),XMFLOAT3(-1.0000,-0.0000,-0.0034),XMFLOAT2(0.375000,0.250000)},
			{XMFLOAT3(-1.189768,-1.000000,0.00778),XMFLOAT4(1, 1, 0, 1.0f),XMFLOAT3(-0.0200,-0.0000,-0.9998),XMFLOAT2(0.375000,0.250000)},
			{XMFLOAT3(0.800000,1.000000,0.16574),XMFLOAT4(1, 1, 0, 1.0f),XMFLOAT3(-0.0011,-0.0000,-1.0000),XMFLOAT2(0.625000,0.250000)},
			{XMFLOAT3(0.800000,1.000000,0.16574),XMFLOAT4(1, 1, 0, 1.0f),XMFLOAT3(-0.0021,1.0000,-0.0000),XMFLOAT2(0.875000,0.500000)},
			{XMFLOAT3(1.196897,1.001836,0.16528),XMFLOAT4(0, 0, 1, 1.0f),XMFLOAT3(-0.0011,-0.0000,-1.0000),XMFLOAT2(0.607943,0.467563)},
			{XMFLOAT3(0.805328,-1.000000,2.16491),XMFLOAT4(1, 0, 0, 1.0f),XMFLOAT3(-1.0000,-0.0000,0.0027),XMFLOAT2(0.375000,0.000000)},
			{XMFLOAT3(0.820068,1.000000,-2.65936),XMFLOAT4(0, 0, 1, 1.0f),XMFLOAT3(0.0001,1.0000,-0.0001),XMFLOAT2(0.625000,0.500000)},
			{XMFLOAT3(0.056728,-1.000000,-1.95902),XMFLOAT4(1, 1, 0, 1.0f),XMFLOAT3(-0.7006,-0.0000,-0.7135),XMFLOAT2(0.375000,0.000000)},
			{XMFLOAT3(0.056728,1.000000,-1.95902),XMFLOAT4(1, 1, 0, 1.0f),XMFLOAT3(-0.7006,-0.0000,-0.7135),XMFLOAT2(0.625000,0.000000)},
			{XMFLOAT3(0.056728,-1.000000,-1.95902),XMFLOAT4(1, 0, 0, 1.0f),XMFLOAT3(-0.0000,-1.0000,-0.0000),XMFLOAT2(0.125000,0.750000)},
			{XMFLOAT3(0.056728,1.000000,-1.95902),XMFLOAT4(1, 1, 0, 1.0f),XMFLOAT3(-0.0000,1.0000,-0.0000),XMFLOAT2(0.875000,0.750000)},
			{XMFLOAT3(0.342146,1.000000,-2.23926),XMFLOAT4(0, 1, 1, 1.0f),XMFLOAT3(-0.0009,1.0000,0.0007),XMFLOAT2(0.875000,0.500000)},
			{XMFLOAT3(-0.800000,-1.000000,3.00000),XMFLOAT4(1, 1, 0, 1.0f),XMFLOAT3(1.0000,-0.0000,-0.0000),XMFLOAT2(0.375000,0.750000)},
			{XMFLOAT3(-0.800000,-1.000000,3.00000),XMFLOAT4(1, 0, 0, 1.0f),XMFLOAT3(0.0068,-0.0000,1.0000),XMFLOAT2(0.375000,0.750000)},
			{XMFLOAT3(-0.800000,1.000000,3.00000),XMFLOAT4(0, 0, 1, 1.0f),XMFLOAT3(1.0000,-0.0000,-0.0000),XMFLOAT2(0.625000,0.750000)},
			{XMFLOAT3(-0.800000,1.000000,3.00000),XMFLOAT4(1, 1, 0, 1.0f),XMFLOAT3(-0.0000,1.0000,-0.0000),XMFLOAT2(0.625000,0.750000)},
			{XMFLOAT3(-1.200000,1.000000,3.00000),XMFLOAT4(1, 1, 1, 1.0f),XMFLOAT3(-0.0000,1.0000,-0.0000),XMFLOAT2(0.875000,0.750000)},
			{XMFLOAT3(-1.200000,1.000000,0.00000),XMFLOAT4(0, 1, 0, 1.0f),XMFLOAT3(-1.0000,-0.0051,-0.0000),XMFLOAT2(0.625000,0.250000)},
			{XMFLOAT3(-1.200000,1.000000,0.00000),XMFLOAT4(0, 1, 1, 1.0f),XMFLOAT3(-0.0000,-0.0039,-1.0000),XMFLOAT2(0.625000,0.250000)},
			{XMFLOAT3(-0.800000,1.000000,0.00000),XMFLOAT4(1, 0, 1, 1.0f),XMFLOAT3(-0.0000,-0.0039,-1.0000),XMFLOAT2(0.625000,0.500000)},
			{XMFLOAT3(-0.800000,1.000000,0.00000),XMFLOAT4(0, 1, 0, 1.0f),XMFLOAT3(1.0000,-0.0000,-0.0000),XMFLOAT2(0.625000,0.500000)},
			{XMFLOAT3(-1.200000,1.000000,3.00000),XMFLOAT4(1, 0, 0, 1.0f),XMFLOAT3(-1.0000,-0.0051,-0.0000),XMFLOAT2(0.625000,0.000000)},
			{XMFLOAT3(-1.181182,-1.000000,2.99852),XMFLOAT4(0, 1, 1, 1.0f),XMFLOAT3(-0.0000,-1.0000,-0.0000),XMFLOAT2(0.125000,0.750000)},
			{XMFLOAT3(0.547587,1.000000,-2.94358),XMFLOAT4(0, 1, 0, 1.0f),XMFLOAT3(0.7218,0.0083,-0.6920),XMFLOAT2(0.625000,0.250000)},
			{XMFLOAT3(0.342146,-1.000000,-2.23926),XMFLOAT4(0, 0, 1, 1.0f),XMFLOAT3(-0.7006,-0.0000,-0.7135),XMFLOAT2(0.375000,0.250000)},
			{XMFLOAT3(0.342146,-1.000000,-2.23926),XMFLOAT4(1, 0, 1, 1.0f),XMFLOAT3(0.7267,-0.0000,-0.6869),XMFLOAT2(0.375000,0.250000)},
			{XMFLOAT3(-1.193915,-0.993548,3.00266),XMFLOAT4(1, 0, 0, 1.0f),XMFLOAT3(-0.0000,0.0013,1.0000),XMFLOAT2(0.375000,1.000000)},
			{XMFLOAT3(3.103819,1.000370,0.65012),XMFLOAT4(1, 1, 0, 1.0f),XMFLOAT3(0.6911,0.0040,0.7227),XMFLOAT2(0.625000,0.536533)},
			{XMFLOAT3(1.198296,1.000829,0.16739),XMFLOAT4(0, 0, 0, 1.0f),XMFLOAT3(-0.0000,1.0000,0.0004),XMFLOAT2(0.625000,0.500000)}
		};

		for (UINT i = 0; i < 96; i++) 
		{
			vertexDataArr[i].pos = NAME[i].pos;
			vertexDataArr[i].normal = NAME[i].normal;
			vertexDataArr[i].color = NAME[i].color;
			vertexDataArr[i].tex = NAME[i].tex;
		}

		// 将数据传入
		for (UINT i = 0; i < 96; i++)
		{
			Internal::InsertVertexElement(meshData.vertexVec[i], vertexDataArr[i]);
		}

		meshData.indexVec = {
			// 顶点索引值
			72,3,2,
			61,71,63,
			7,54,64,
			57,56,55,
			6,0,62,
			95,1,58,
			88,67,8,
			68,86,65,
			87,79,66,
			13,93,80,
			14,89,10,
			11,82,15,
			16,19,18,
			36,23,46,
			22,37,43,
			21,41,20,
			45,44,42,
			32,38,73,
			75,91,74,
			92,51,47,
			94,49,48,
			29,24,50,
			26,28,76,
			78,53,31,
			72,59,3,
			61,69,71,
			7,5,54,
			57,60,56,
			6,4,0,
			95,70,1,
			88,84,67,
			68,85,86,
			87,81,79,
			13,9,93,
			14,12,89,
			11,83,82,
			16,39,19,
			36,90,23,
			22,33,37,
			21,40,41,
			45,35,44,
			32,17,38,
			75,34,91,
			92,27,51,
			94,52,49,
			29,25,24,
			26,30,28,
			78,77,53
		};

		return meshData;
	}

	template<class VertexType, class IndexType>
	inline MeshData<VertexType, IndexType> CreateSphere(float radius, UINT levels, UINT slices, const DirectX::XMFLOAT4 & color)
	{
		using namespace DirectX;

		MeshData<VertexType, IndexType> meshData;
		UINT vertexCount = 2 + (levels - 1) * (slices + 1);
		UINT indexCount = 6 * (levels - 1) * slices;
		meshData.vertexVec.resize(vertexCount);
		meshData.indexVec.resize(indexCount);

		Internal::VertexData vertexData;
		IndexType vIndex = 0, iIndex = 0;

		float phi = 0.0f, theta = 0.0f;
		float per_phi = XM_PI / levels;
		float per_theta = XM_2PI / slices;
		float x, y, z;

		// 放入顶端点
		vertexData = { XMFLOAT3(0.0f, radius, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(0.0f, 0.0f) };
		Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);

		for (UINT i = 1; i < levels; ++i)
		{
			phi = per_phi * i;
			// 需要slices + 1个顶点是因为 起点和终点需为同一点，但纹理坐标值不一致
			for (UINT j = 0; j <= slices; ++j)
			{
				theta = per_theta * j;
				x = radius * sinf(phi) * cosf(theta);
				y = radius * cosf(phi);
				z = radius * sinf(phi) * sinf(theta);
				// 计算出局部坐标、法向量、Tangent向量和纹理坐标
				XMFLOAT3 pos = XMFLOAT3(x, y, z), normal;
				XMStoreFloat3(&normal, XMVector3Normalize(XMLoadFloat3(&pos)));

				vertexData = { pos, normal, XMFLOAT4(-sinf(theta), 0.0f, cosf(theta), 1.0f), color, XMFLOAT2(theta / XM_2PI, phi / XM_PI) };
				Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);
			}
		}

		// 放入底端点
		vertexData = { XMFLOAT3(0.0f, -radius, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f),
			XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(0.0f, 1.0f) };
		Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);


		// 逐渐放入索引
		if (levels > 1)
		{
			for (UINT j = 1; j <= slices; ++j)
			{
				meshData.indexVec[iIndex++] = 0;
				meshData.indexVec[iIndex++] = j % (slices + 1) + 1;
				meshData.indexVec[iIndex++] = j;
			}
		}


		for (UINT i = 1; i < levels - 1; ++i)
		{
			for (UINT j = 1; j <= slices; ++j)
			{
				meshData.indexVec[iIndex++] = (i - 1) * (slices + 1) + j;
				meshData.indexVec[iIndex++] = (i - 1) * (slices + 1) + j % (slices + 1) + 1;
				meshData.indexVec[iIndex++] = i * (slices + 1) + j % (slices + 1) + 1;

				meshData.indexVec[iIndex++] = i * (slices + 1) + j % (slices + 1) + 1;
				meshData.indexVec[iIndex++] = i * (slices + 1) + j;
				meshData.indexVec[iIndex++] = (i - 1) * (slices + 1) + j;
			}
		}

		// 逐渐放入索引
		if (levels > 1)
		{
			for (UINT j = 1; j <= slices; ++j)
			{
				meshData.indexVec[iIndex++] = (levels - 2) * (slices + 1) + j;
				meshData.indexVec[iIndex++] = (levels - 2) * (slices + 1) + j % (slices + 1) + 1;
				meshData.indexVec[iIndex++] = (levels - 1) * (slices + 1) + 1;
			}
		}


		return meshData;
	}

	template<class VertexType, class IndexType>
	inline MeshData<VertexType, IndexType> CreateBox(float width, float height, float depth, const DirectX::XMFLOAT4 & color)
	{
		using namespace DirectX;

		MeshData<VertexType, IndexType> meshData;
		meshData.vertexVec.resize(24);


		Internal::VertexData vertexDataArr[24];
		float w2 = width / 2, h2 = height / 2, d2 = depth / 2;

		// 右面(+X面)
		vertexDataArr[0].pos = XMFLOAT3(w2, -h2, -d2);
		vertexDataArr[1].pos = XMFLOAT3(w2, h2, -d2);
		vertexDataArr[2].pos = XMFLOAT3(w2, h2, d2);
		vertexDataArr[3].pos = XMFLOAT3(w2, -h2, d2);
		// 左面(-X面)
		vertexDataArr[4].pos = XMFLOAT3(-w2, -h2, d2);
		vertexDataArr[5].pos = XMFLOAT3(-w2, h2, d2);
		vertexDataArr[6].pos = XMFLOAT3(-w2, h2, -d2);
		vertexDataArr[7].pos = XMFLOAT3(-w2, -h2, -d2);
		// 顶面(+Y面)
		vertexDataArr[8].pos = XMFLOAT3(-w2, h2, -d2);
		vertexDataArr[9].pos = XMFLOAT3(-w2, h2, d2);
		vertexDataArr[10].pos = XMFLOAT3(w2, h2, d2);
		vertexDataArr[11].pos = XMFLOAT3(w2, h2, -d2);
		// 底面(-Y面)
		vertexDataArr[12].pos = XMFLOAT3(w2, -h2, -d2);
		vertexDataArr[13].pos = XMFLOAT3(w2, -h2, d2);
		vertexDataArr[14].pos = XMFLOAT3(-w2, -h2, d2);
		vertexDataArr[15].pos = XMFLOAT3(-w2, -h2, -d2);
		// 背面(+Z面)
		vertexDataArr[16].pos = XMFLOAT3(w2, -h2, d2);
		vertexDataArr[17].pos = XMFLOAT3(w2, h2, d2);
		vertexDataArr[18].pos = XMFLOAT3(-w2, h2, d2);
		vertexDataArr[19].pos = XMFLOAT3(-w2, -h2, d2);
		// 正面(-Z面)
		vertexDataArr[20].pos = XMFLOAT3(-w2, -h2, -d2);
		vertexDataArr[21].pos = XMFLOAT3(-w2, h2, -d2);
		vertexDataArr[22].pos = XMFLOAT3(w2, h2, -d2);
		vertexDataArr[23].pos = XMFLOAT3(w2, -h2, -d2);


		for (UINT i = 0; i < 4; ++i)
		{
			// 右面(+X面)
			vertexDataArr[i].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);
			vertexDataArr[i].tangent = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
			vertexDataArr[i].color = color;
			// 左面(-X面)
			vertexDataArr[i + 4].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);
			vertexDataArr[i + 4].tangent = XMFLOAT4(0.0f, 0.0f, -1.0f, 1.0f);
			vertexDataArr[i + 4].color = color;
			// 顶面(+Y面)
			vertexDataArr[i + 8].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			vertexDataArr[i + 8].tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
			vertexDataArr[i + 8].color = color;
			// 底面(-Y面)
			vertexDataArr[i + 12].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);
			vertexDataArr[i + 12].tangent = XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f);
			vertexDataArr[i + 12].color = color;
			// 背面(+Z面)
			vertexDataArr[i + 16].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
			vertexDataArr[i + 16].tangent = XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f);
			vertexDataArr[i + 16].color = color;
			// 正面(-Z面)
			vertexDataArr[i + 20].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
			vertexDataArr[i + 20].tangent = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
			vertexDataArr[i + 20].color = color;
		}

		for (UINT i = 0; i < 6; ++i)
		{
			vertexDataArr[i * 4].tex = XMFLOAT2(0.0f, 1.0f);
			vertexDataArr[i * 4 + 1].tex = XMFLOAT2(0.0f, 0.0f);
			vertexDataArr[i * 4 + 2].tex = XMFLOAT2(1.0f, 0.0f);
			vertexDataArr[i * 4 + 3].tex = XMFLOAT2(1.0f, 1.0f);
		}

		for (UINT i = 0; i < 24; ++i)
		{
			Internal::InsertVertexElement(meshData.vertexVec[i], vertexDataArr[i]);
		}

		meshData.indexVec = {
			0, 1, 2, 2, 3, 0,		// 右面(+X面)
			4, 5, 6, 6, 7, 4,		// 左面(-X面)
			8, 9, 10, 10, 11, 8,	// 顶面(+Y面)
			12, 13, 14, 14, 15, 12,	// 底面(-Y面)
			16, 17, 18, 18, 19, 16, // 背面(+Z面)
			20, 21, 22, 22, 23, 20,	// 正面(-Z面)
		};

		return meshData;
	}

	template<class VertexType, class IndexType>
	inline MeshData<VertexType, IndexType> CreateCylinder(float radius, float height, UINT slices, const DirectX::XMFLOAT4 & color)
	{
		using namespace DirectX;

		auto meshData = CreateCylinderNoCap<VertexType, IndexType>(radius, height, slices, color);
		UINT vertexCount = 4 * (slices + 1) + 2;
		UINT indexCount = 12 * slices;
		meshData.vertexVec.resize(vertexCount);
		meshData.indexVec.resize(indexCount);

		float h2 = height / 2;
		float theta = 0.0f;
		float per_theta = XM_2PI / slices;

		IndexType vIndex = 2 * (slices + 1), iIndex = 6 * slices;
		IndexType offset = 2 * (slices + 1);
		Internal::VertexData vertexData;

		// 放入顶端圆心
		vertexData = { XMFLOAT3(0.0f, h2, 0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f),
			XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(0.5f, 0.5f) };
		Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);

		// 放入顶端圆上各点
		for (UINT i = 0; i <= slices; ++i)
		{
			theta = i * per_theta;
			vertexData = { XMFLOAT3(radius * cosf(theta), h2, radius * sinf(theta)), XMFLOAT3(0.0f, 1.0f, 0.0f),
				XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(cosf(theta) / 2 + 0.5f, sinf(theta) / 2 + 0.5f) };
			Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);
		}

		// 放入底端圆心
		vertexData = { XMFLOAT3(0.0f, -h2, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f),
			XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(0.5f, 0.5f) };
		Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);

		// 放入底部圆上各点
		for (UINT i = 0; i <= slices; ++i)
		{
			theta = i * per_theta;
			vertexData = { XMFLOAT3(radius * cosf(theta), -h2, radius * sinf(theta)), XMFLOAT3(0.0f, -1.0f, 0.0f),
				XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(cosf(theta) / 2 + 0.5f, sinf(theta) / 2 + 0.5f) };
			Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);
		}

		

		// 逐渐放入顶部三角形索引
		for (UINT i = 1; i <= slices; ++i)
		{
			meshData.indexVec[iIndex++] = offset;
			meshData.indexVec[iIndex++] = offset + i % (slices + 1) + 1;
			meshData.indexVec[iIndex++] = offset + i;
		}

		// 逐渐放入底部三角形索引
		offset += slices + 2;
		for (UINT i = 1; i <= slices; ++i)
		{
			meshData.indexVec[iIndex++] = offset;
			meshData.indexVec[iIndex++] = offset + i;
			meshData.indexVec[iIndex++] = offset + i % (slices + 1) + 1;
		}

		return meshData;
	}

	template<class VertexType, class IndexType>
	inline MeshData<VertexType, IndexType> CreateCylinderNoCap(float radius, float height, UINT slices, const DirectX::XMFLOAT4 & color)
	{
		using namespace DirectX;

		MeshData<VertexType, IndexType> meshData;
		UINT vertexCount = 2 * (slices + 1);
		UINT indexCount = 6 * slices;
		meshData.vertexVec.resize(vertexCount);
		meshData.indexVec.resize(indexCount);

		float h2 = height / 2;
		float theta = 0.0f;
		float per_theta = XM_2PI / slices;

		Internal::VertexData vertexData;

		// 放入侧面顶端点
		for (UINT i = 0; i <= slices; ++i)
		{
			theta = i * per_theta;
			vertexData = { XMFLOAT3(radius * cosf(theta), h2, radius * sinf(theta)), XMFLOAT3(cosf(theta), 0.0f, sinf(theta)),
				XMFLOAT4(-sinf(theta), 0.0f, cosf(theta), 1.0f), color, XMFLOAT2(theta / XM_2PI, 0.0f) };
			Internal::InsertVertexElement(meshData.vertexVec[i], vertexData);
		}

		// 放入侧面底端点
		for (UINT i = 0; i <= slices; ++i)
		{
			theta = i * per_theta;
			vertexData = { XMFLOAT3(radius * cosf(theta), -h2, radius * sinf(theta)), XMFLOAT3(cosf(theta), 0.0f, sinf(theta)),
				XMFLOAT4(-sinf(theta), 0.0f, cosf(theta), 1.0f), color, XMFLOAT2(theta / XM_2PI, 1.0f) };
			UINT vIndex = (slices + 1) + i;
			Internal::InsertVertexElement(meshData.vertexVec[vIndex], vertexData);
		}

		// 放入索引
		UINT iIndex = 0;

		for (UINT i = 0; i < slices; ++i)
		{
			meshData.indexVec[iIndex++] = i;
			meshData.indexVec[iIndex++] = i + 1;
			meshData.indexVec[iIndex++] = (slices + 1) + i + 1;

			meshData.indexVec[iIndex++] = (slices + 1) + i + 1;
			meshData.indexVec[iIndex++] = (slices + 1) + i;
			meshData.indexVec[iIndex++] = i;
		}


		return meshData;
	}

	template<class VertexType, class IndexType>
	MeshData<VertexType, IndexType> CreateCone(float radius, float height, UINT slices, const DirectX::XMFLOAT4& color)
	{
		using namespace DirectX;
		auto meshData = CreateConeNoCap<VertexType, IndexType>(radius, height, slices, color);

		UINT vertexCount = 3 * slices + 1;
		UINT indexCount = 6 * slices;
		meshData.vertexVec.resize(vertexCount);
		meshData.indexVec.resize(indexCount);
		
		float h2 = height / 2;
		float theta = 0.0f;
		float per_theta = XM_2PI / slices;
		UINT iIndex = 3 * slices;
		UINT vIndex = 2 * slices;
		Internal::VertexData vertexData;

		// 放入圆锥底面顶点
		for (UINT i = 0; i < slices; ++i)
		{
			theta = i * per_theta;
			vertexData = { XMFLOAT3(radius * cosf(theta), -h2, radius * sinf(theta)), XMFLOAT3(0.0f, -1.0f, 0.0f),
				XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(cosf(theta) / 2 + 0.5f, sinf(theta) / 2 + 0.5f) };
			Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);
		}
		vertexData = { XMFLOAT3(0.0f, -h2, 0.0f), XMFLOAT3(0.0f, -1.0f, 0.0f),
				XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(0.5f, 0.5f) };
		Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);

		// 放入索引
		UINT offset = 2 * slices;
		for (UINT i = 0; i < slices; ++i)
		{
			meshData.indexVec[iIndex++] = offset + slices;
			meshData.indexVec[iIndex++] = offset + i % slices;
			meshData.indexVec[iIndex++] = offset + (i + 1) % slices;
		}

		return meshData;
	}

	template<class VertexType, class IndexType>
	MeshData<VertexType, IndexType> CreateConeNoCap(float radius, float height, UINT slices, const DirectX::XMFLOAT4& color)
	{
		using namespace DirectX;

		MeshData<VertexType, IndexType> meshData;
		UINT vertexCount = 2 * slices;
		UINT indexCount = 3 * slices;
		meshData.vertexVec.resize(vertexCount);
		meshData.indexVec.resize(indexCount);

		float h2 = height / 2;
		float theta = 0.0f;
		float per_theta = XM_2PI / slices;
		float len = sqrtf(height * height + radius * radius);
		UINT iIndex = 0;
		UINT vIndex = 0;
		Internal::VertexData vertexData;

		// 放入圆锥尖端顶点(每个顶点包含不同的法向量和切线向量)
		for (UINT i = 0; i < slices; ++i)
		{
			theta = i * per_theta + per_theta / 2;
			vertexData = { XMFLOAT3(0.0f, h2, 0.0f), XMFLOAT3(radius * cosf(theta) / len, height / len, radius * sinf(theta) / len),
				XMFLOAT4(-sinf(theta), 0.0f, cosf(theta), 1.0f), color, XMFLOAT2(0.5f, 0.5f) };
			Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);
		}

		// 放入圆锥底面顶点
		for (UINT i = 0; i < slices; ++i)
		{
			theta = i * per_theta;
			vertexData = { XMFLOAT3(radius * cosf(theta), -h2, radius * sinf(theta)), XMFLOAT3(radius * cosf(theta) / len, height / len, radius * sinf(theta) / len),
				XMFLOAT4(-sinf(theta), 0.0f, cosf(theta), 1.0f), color, XMFLOAT2(cosf(theta) / 2 + 0.5f, sinf(theta) / 2 + 0.5f) };
			Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);
		}

		// 放入索引
		for (UINT i = 0; i < slices; ++i)
		{
			meshData.indexVec[iIndex++] = i;
			meshData.indexVec[iIndex++] = slices + (i + 1) % slices;
			meshData.indexVec[iIndex++] = slices + i % slices;
		}

		return meshData;
	}

	template<class VertexType, class IndexType>
	inline MeshData<VertexType, IndexType> Create2DShow(const DirectX::XMFLOAT2 & center, const DirectX::XMFLOAT2 & scale,
		const DirectX::XMFLOAT4 & color)
	{
		return Create2DShow<VertexType, IndexType>(center.x, center.y, scale.x, scale.y, color);
	}

	template<class VertexType, class IndexType>
	inline MeshData<VertexType, IndexType> Create2DShow(float centerX, float centerY, float scaleX, float scaleY, const DirectX::XMFLOAT4 & color)
	{
		using namespace DirectX;

		MeshData<VertexType, IndexType> meshData;
		meshData.vertexVec.resize(4);

		Internal::VertexData vertexData;
		IndexType vIndex = 0;

		vertexData = { XMFLOAT3(centerX - scaleX, centerY - scaleY, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f),
			XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(0.0f, 1.0f) };
		Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);

		vertexData = { XMFLOAT3(centerX - scaleX, centerY + scaleY, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f),
			XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(0.0f, 0.0f) };
		Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);

		vertexData = { XMFLOAT3(centerX + scaleX, centerY + scaleY, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f),
			XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(1.0f, 0.0f) };
		Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);

		vertexData = { XMFLOAT3(centerX + scaleX, centerY - scaleY, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f),
			XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(1.0f, 1.0f) };
		Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);

		meshData.indexVec = { 0, 1, 2, 2, 3, 0 };
		return meshData;
	}

	template<class VertexType, class IndexType>
	inline MeshData<VertexType, IndexType> CreatePlane(const DirectX::XMFLOAT3 & center, const DirectX::XMFLOAT2 & planeSize,
		const DirectX::XMFLOAT2 & maxTexCoord, const DirectX::XMFLOAT4 & color)
	{
		return CreatePlane<VertexType, IndexType>(center.x, center.y, center.z, planeSize.x, planeSize.y, maxTexCoord.x, maxTexCoord.y, color);
	}

	template<class VertexType, class IndexType>
	inline MeshData<VertexType, IndexType> CreatePlane(float centerX, float centerY, float centerZ, float width, float depth, float texU, float texV, const DirectX::XMFLOAT4 & color)
	{
		using namespace DirectX;

		MeshData<VertexType, IndexType> meshData;
		meshData.vertexVec.resize(4);

		Internal::VertexData vertexData;
		IndexType vIndex = 0;

		vertexData = { XMFLOAT3(centerX - width / 2, centerY, centerZ - depth / 2), XMFLOAT3(0.0f, 1.0f, 0.0f),
			XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(0.0f, texV) };
		Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);

		vertexData = { XMFLOAT3(centerX - width / 2, centerY, centerZ + depth / 2), XMFLOAT3(0.0f, 1.0f, 0.0f),
			XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(0.0f, 0.0f) };
		Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);

		vertexData = { XMFLOAT3(centerX + width / 2, centerY, centerZ + depth / 2), XMFLOAT3(0.0f, 1.0f, 0.0f),
			XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(texU, 0.0f) };
		Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);

		vertexData = { XMFLOAT3(centerX + width / 2, centerY, centerZ - depth / 2), XMFLOAT3(0.0f, 1.0f, 0.0f),
			XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), color, XMFLOAT2(texU, texV) };
		Internal::InsertVertexElement(meshData.vertexVec[vIndex++], vertexData);

		meshData.indexVec = { 0, 1, 2, 2, 3, 0 };
		return meshData;
	}

}



#endif

