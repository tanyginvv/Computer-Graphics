#include "stdafx.h"
#include "CCube.h"

CCube::CCube(CMatrix4d const& transform)
	: m_mesh(nullptr)
{
	std::vector<Vertex> vertices{
		Vertex({ -1, 0, -1 }),
		Vertex({ +1, 0, -1 }),
		Vertex({ +1, +2, -1 }),
		Vertex({ -1, +2, -1 }),
		Vertex({ -1, 0, +1 }),
		Vertex({ +1, 0, +1 }),
		Vertex({ +1, +2, +1 }),
		Vertex({ -1, +2, +1 }),
	};

	std::vector<Face> faces{
		{ 4, 7, 3 },
		{ 4, 3, 0 },
		{ 5, 2, 6 },
		{ 5, 1, 2 },
		{ 4, 0, 1 },
		{ 4, 1, 5 },
		{ 3, 6, 2 },
		{ 3, 7, 6 },
		{ 2, 1, 0 },
		{ 2, 0, 3 },
		{ 5, 6, 7 },
		{ 5, 7, 4 },
	};

	auto const pMeshData = std::make_shared<CTriangleMeshData>(vertices, faces);
	m_mesh = CTriangleMesh(pMeshData, transform);
}

bool CCube::Hit(CRay const& ray, CIntersection& intersection) const
{
	return m_mesh.Hit(ray, intersection);
}
