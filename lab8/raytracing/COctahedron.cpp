#include "stdafx.h"
#include "COctahedron.h"

COctahedron::COctahedron(CMatrix4d const& transform)
	: m_mesh(nullptr)
{
	std::vector<Vertex> vertices{
		Vertex({ 1, 0, 0 }),
		Vertex({ -1, 0, 0 }),
		Vertex({ 0, 1, 0 }),
		Vertex({ 0, -1, 0 }),
		Vertex({ 0, 0, 1 }),
		Vertex({ 0, 0, -1 }),
	};

	std::vector<Face> faces{
		{ 0, 2, 4 },
		{ 0, 4, 3 },
		{ 0, 3, 5 },
		{ 0, 5, 2 },
		{ 1, 4, 2 },
		{ 1, 3, 4 },
		{ 1, 5, 3 },
		{ 1, 2, 5 },
	};

	auto const pMeshData = std::make_shared<CTriangleMeshData>(vertices, faces);
	m_mesh = CTriangleMesh(pMeshData, transform);
}

bool COctahedron::Hit(CRay const& ray, CIntersection& intersection) const
{
	return m_mesh.Hit(ray, intersection);
}
