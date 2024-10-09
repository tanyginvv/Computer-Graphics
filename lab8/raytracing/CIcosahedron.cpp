#include "stdafx.h"
#include "CIcosahedron.h"
#include <cmath>

CIcosahedron::CIcosahedron(CMatrix4d const& transform)
	: m_mesh(nullptr)
{
	float t = (sqrtf(5) - 1) / 2;

	std::vector<Vertex> vertices = {
		Vertex({ 0, +1, t }), 
		Vertex({ 0, +1, -t }), 
		Vertex({ +1, t, 0 }), 
		Vertex({ +1, -t, 0 }), 
		Vertex({ 0, -1, -t }), 
		Vertex({ 0, -1, t }), 
		Vertex({ t, 0, +1 }),
		Vertex({ -t, 0, +1 }), 
		Vertex({ t, 0, -1 }), 
		Vertex({ -t, 0, -1 }), 
		Vertex({ -1, t, 0 }), 
		Vertex({ -1, -t, 0 }),
	};

	std::vector<Face> faces{
		{ 11, 4, 5 },
		{ 7, 11, 5 },
		{ 7, 5, 6 },
		{ 0, 7, 6 },
		{ 0, 6, 2 },
		{ 1, 0, 2 },
		{ 1, 2, 8 },
		{ 9, 1, 8 },
		{ 9, 8, 4 },
		{ 11, 9, 4 },
		{ 11, 7, 10 },
		{ 7, 0, 10 },
		{ 0, 1, 10 },
		{ 1, 9, 10 },
		{ 9, 11, 10 },
		{ 5, 4, 3 },
		{ 6, 5, 3 },
		{ 2, 6, 3 },
		{ 8, 2, 3 },
		{ 4, 8, 3 }
	};

	auto const pMeshData = std::make_shared<CTriangleMeshData>(vertices, faces);
	m_mesh = CTriangleMesh(pMeshData, transform);
}

bool CIcosahedron::Hit(CRay const& ray, CIntersection& intersection) const
{
	return m_mesh.Hit(ray, intersection);
}
