#pragma once

#include "CTriangleMesh.h"

class CTetrahedron : public CGeometryObjectImpl
{
public:
	// передать только 4 вершины
	CTetrahedron(CMatrix4d const& transform = CMatrix4d(), std::vector<Vertex> const& vertices = {
															   Vertex({ -1, 0, 1 }),
															   Vertex({ +1, 0, 1 }),
															   Vertex({ 0, 0, -1 }),
															   Vertex({ 0, 2, 0 }),
														   });

	virtual bool Hit(CRay const& ray, CIntersection& intersection) const;

private:
	CTriangleMesh m_mesh;
};