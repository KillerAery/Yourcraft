#include "MeshData.h"
#include "Geometry.h"

MeshData MeshData::CreateBox(float width, float height, float depth, bool rhcoords, bool invertn)
{
	MeshData data;
	DirectX::ComputeBox(data.vertexVec, data.indexVec, DirectX::XMFLOAT3(width, height, depth), rhcoords, invertn);
	return data;
}

MeshData MeshData::CreateSphere(float diameter, size_t tessellation, bool rhcoords, bool invertn)
{
	MeshData data;
	DirectX::ComputeSphere(data.vertexVec, data.indexVec, diameter, tessellation, rhcoords, invertn);
	return data;
}

MeshData MeshData::CreateGeoSphere(float diameter, size_t tessellation, bool rhcoords)
{
	MeshData data;
	DirectX::ComputeGeoSphere(data.vertexVec, data.indexVec, diameter, tessellation, rhcoords);
	return data;
}

MeshData MeshData::CreateCylinder(float height, float diameter, size_t tessellation, bool rhcoords)
{
	MeshData data;
	DirectX::ComputeCylinder(data.vertexVec, data.indexVec, height,diameter,tessellation,rhcoords);
	return data;
}

MeshData MeshData::CreateCone(float diameter, float height, size_t tessellation, bool rhcoords)
{
	MeshData data;
	DirectX::ComputeCone(data.vertexVec, data.indexVec, diameter, height,tessellation, rhcoords);
	return data;
}

MeshData MeshData::CreateTorus(float diameter, float thickness, size_t tessellation, bool rhcoords)
{
	MeshData data;
	DirectX::ComputeTorus(data.vertexVec, data.indexVec, diameter,thickness,tessellation, rhcoords);
	return data;
}

MeshData MeshData::CreateTetrahedron(float size, bool rhcoords)
{
	MeshData data;
	DirectX::ComputeTetrahedron(data.vertexVec, data.indexVec, size, rhcoords);
	return data;
}

MeshData MeshData::CreateOctahedron(float size, bool rhcoords)
{
	MeshData data;
	DirectX::ComputeOctahedron(data.vertexVec, data.indexVec, size , rhcoords);
	return data;
}

MeshData MeshData::CreateDodecahedron(float size, bool rhcoords)
{
	MeshData data;
	DirectX::ComputeDodecahedron(data.vertexVec, data.indexVec, size , rhcoords);
	return data;
}

MeshData MeshData::CreateIcosahedron(float size, bool rhcoords)
{
	MeshData data;
	DirectX::ComputeIcosahedron(data.vertexVec, data.indexVec, size, rhcoords);
	return data;
}

MeshData MeshData::CreateTeapot(float size, size_t tessellation, bool rhcoords)
{
	MeshData data;
	DirectX::ComputeTeapot(data.vertexVec, data.indexVec, size, tessellation, rhcoords);
	return data;
}
