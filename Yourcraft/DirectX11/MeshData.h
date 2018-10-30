#pragma once

#include <Windows.h>
#include <DirectXMath.h>
#include <Geometry.h>
#include <vector>

struct MeshData
{
	DirectX::VertexCollection vertexVec;				// 顶点数组（位置，法向量，贴图位置）
	DirectX::IndexCollection indexVec;					// 索引数组

	static MeshData CreateBox(float width = 2.0f, float height = 2.0f, float depth = 2.0f, bool rhcoords = false , bool invertn = false);
	static MeshData CreateSphere(float diameter = 2.0f, size_t tessellation = 20, bool rhcoords = false, bool invertn = false);
	static MeshData CreateGeoSphere(float diameter = 2.0f, size_t tessellation = 20, bool rhcoords = false);
	static MeshData CreateCylinder(float height = 2.0f, float diameter = 2.0f, size_t tessellation = 20, bool rhcoords = false);
	static MeshData CreateCone(float diameter, float height, size_t tessellation = 20, bool rhcoords = false);
	static MeshData CreateTorus(float diameter, float thickness, size_t tessellation = 20, bool rhcoords = false);
	static MeshData CreateTetrahedron(float size, bool rhcoords = false);
	static MeshData CreateOctahedron(float size, bool rhcoords = false);
	static MeshData CreateDodecahedron(float size, bool rhcoords = false);
	static MeshData CreateIcosahedron(float size, bool rhcoords = false);
	static MeshData CreateTeapot(float size, size_t tessellation = 20, bool rhcoords = false);
};

