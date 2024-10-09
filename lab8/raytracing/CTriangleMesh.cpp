#include "stdafx.h"
#include "libgl/CIntersection.h"
#include "libgl/CRay.h"
#include "CTriangleMesh.h"
#include "libgl/VectorMath.h"

// ������������ �����������, ��������� ��� ��������������� ����������
// ��� ��������� ���������� ����� ����������� � �����
CTriangle::CTriangle(Vertex const& vertex0, Vertex const& vertex1, Vertex const& vertex2, bool flatShaded)
	: m_pVertex0(&vertex0)
	, m_pVertex1(&vertex1)
	, m_pVertex2(&vertex2)
	, m_flatShaded(flatShaded)
{
	const double EPSILON = 1e-10;

	// ���������� ������
	CVector3d const& p0 = vertex0.position;
	CVector3d const& p1 = vertex1.position;
	CVector3d const& p2 = vertex2.position;

	// �����
	CVector3d const e01 = p1 - p0;
	CVector3d const e12 = p2 - p1;
	CVector3d const e20 = p0 - p2;

	// ������� � ��������� ������������ � ��������� ��� ���������
	CVector3d const normal = Cross(e20, e01);
	m_planeEquation = CVector4d(normal, -Dot(normal, p0));

	// ��������� ��������� ������� � ������, ������� � ��������� ������������,
	// � �������������� ���������� ������������
	CVector3d edge01Normal = Normalize(Cross(normal, e01));
	CVector3d edge12Normal = Normalize(Cross(normal, e12));
	CVector3d edge20Normal = Normalize(Cross(normal, e20));

	// �������� ���� �����
	double edge01Square = Dot(e01, e01);
	double edge12Square = Dot(e12, e12);
	double edge20Square = Dot(e20, e20);

	// �������� ���� �����, ��������� �� ��������������� ����� �����������
	double edge01PerpSquare = (edge01Square > EPSILON)
		? Max(0.0, (edge01Square * edge20Square - Sqr(Dot(e01, e20))) / edge01Square)
		: 0.0;
	double edge12PerpSquare = (edge12Square > EPSILON)
		? Max(0.0, (edge12Square * edge01Square - Sqr(Dot(e12, e01))) / edge12Square)
		: 0.0;
	double edge20PerpSquare = (edge20Square > EPSILON)
		? Max(0.0, (edge20Square * edge12Square - Sqr(Dot(e20, e12))) / edge20Square)
		: 0.0;

	// ������� ��������������� (�����), ������������ �� ����� � �������������� ������� ������������
	m_edge01Perp = edge01Normal * sqrt(edge01PerpSquare);
	m_edge12Perp = edge12Normal * sqrt(edge12PerpSquare);
	m_edge20Perp = edge20Normal * sqrt(edge20PerpSquare);

	// �������� �������� ���� ���������������
	m_invEdge01PerpSquare = (edge01PerpSquare > EPSILON) ? (1.0 / edge01PerpSquare) : 0.0;
	m_invEdge12PerpSquare = (edge12PerpSquare > EPSILON) ? (1.0 / edge12PerpSquare) : 0.0;
	m_invEdge20PerpSquare = (edge20PerpSquare > EPSILON) ? (1.0 / edge20PerpSquare) : 0.0;
}

bool CTriangle::HitTest(CVector3d const& rayStart, CVector3d const& rayDirection, double& hitTime, CVector3d& hitPoint, double& vertex0Weight, double& vertex1Weight, double& vertex2Weight, double const& EPSILON) const
{
	//////////////////////////////////////////////////////////////////////////
	// �������� �� ����������� ���� � ���������� ������������
	//////////////////////////////////////////////////////////////////////////
	{
		// ��������� ������������ ������� ������� � ������������ � ����������� ����
		double normalDotDirection = m_planeEquation.x * rayDirection.x + m_planeEquation.y * rayDirection.y + m_planeEquation.z * rayDirection.z;

		// �������� �� �������������� ���� � ��������� ������������
		if (abs(normalDotDirection) < EPSILON)
		{
			return false;
		}

		// ����������� ��������� ����� � ��������� ����
		double playEquationAtRayStart = m_planeEquation.x * rayStart.x + m_planeEquation.y * rayStart.y + m_planeEquation.z * rayStart.z + m_planeEquation.w;

		// ��������� ����� ���������� ���� � ���������� ������������
		hitTime = -playEquationAtRayStart / normalDotDirection;

		// ���� ����� ���������� � �������, �� ������������ ���
		if (hitTime < EPSILON)
		{
			return false;
		}

		// ����� ����������� ���� � ���������� ������������
		hitPoint = rayStart + hitTime * rayDirection;
	}

	//////////////////////////////////////////////////////////////////////////
	// �������� �������������� ����� ����������� ���������� ������� ������������
	//////////////////////////////////////////////////////////////////////////
	{
		// ������ �� 0 ������� � ����� ����������
		CVector3d p0Hit = hitPoint - m_pVertex0->position;

		// ���������, ��� ����� ���������� ����� �� �� �� ������� �� ����� 01, ��� � ������������� � �����
		double p0HitDotEdge01Perp = Dot(p0Hit, m_edge01Perp);
		if (p0HitDotEdge01Perp < 0)
		{
			return false;
		}

		// ���������, ��� ����� ���������� ����� �� �� �� ������� �� ����� 20, ��� � ������������� � �����
		double p0HitDotEdge20Perp = Dot(p0Hit, m_edge20Perp);
		if (p0HitDotEdge20Perp < 0)
		{
			return false;
		}

		// ������ �� 1 ������� � ����� ����������
		CVector3d p1Hit = hitPoint - m_pVertex1->position;

		// ���������, ��� ����� ���������� ����� �� �� �� ������� �� ����� 12, ��� � ������������� � �����
		double p1HitDotEdge12Perp = Dot(p1Hit, m_edge12Perp);
		if (p1HitDotEdge12Perp < 0)
		{
			return false;
		}

		// ������� ������������ ��� ������������ ��������� ������
		vertex0Weight = p1HitDotEdge12Perp * m_invEdge12PerpSquare;
		vertex1Weight = p0HitDotEdge20Perp * m_invEdge20PerpSquare;
		vertex2Weight = 1 - vertex0Weight - vertex1Weight;
	}

	// ����� ����� ������ ������������, ���� ��� 3 ������� ������������
	// ��������� � ��������� [0; 1], � �� ����� ����� 1.
	// ��� ���������� ����� ������� �������� ��������� ������� �����������
	// ��� ������� �2 �� �����������������
	return vertex2Weight >= 0;
}

/*
������������ ������� ������������� ����� �� ������ ���������� ���������� � �� �������� � ������
*/
CTriangleMeshData::CTriangleMeshData(std::vector<Vertex> vertices, std::vector<Face> const& faces, bool normalize)
	: m_vertices(std::move(vertices))
{
	size_t const numVertices = m_vertices.size();
	if (normalize)
	{
		// ��� ������������� ��������� ������������ �������� � ��������
		for (size_t i = 0; i < numVertices; ++i)
		{
			m_vertices[i].normal.Normalize();
		}
	}

	// �������� ������ ��� �������� ���� ����������� ������
	m_triangles.reserve(faces.size());

	// ��������� ������ ����������� ������
	size_t const numFaces = faces.size();
	for (size_t i = 0; i < numFaces; ++i)
	{
		Face const& face = faces[i];

		size_t i0 = size_t(face.vertex0);
		size_t i1 = size_t(face.vertex1);
		size_t i2 = size_t(face.vertex2);
		assert(i0 < numVertices && i1 < numVertices && i2 < numVertices);

		CTriangle triangle(m_vertices[i0], m_vertices[i1], m_vertices[i2], face.isFlat);
		m_triangles.push_back(triangle);
	}
}

CTriangleMesh::CTriangleMesh(std::shared_ptr<CTriangleMeshData const> pMeshData, CMatrix4d const& transform)
	: CGeometryObjectImpl(transform)
	, m_pMeshData(std::move(pMeshData))
{
}

bool CTriangleMesh::Hit(CRay const& ray, CIntersection& intersection) const
{
	// ��������� ������� ��������������� ��� (������ ��������� ������� �������������� �������)
	CRay invRay = Transform(ray, GetInverseTransform());
	CVector3d const& invRayStart = invRay.GetStart();
	CVector3d const& invRayDirection = invRay.GetDirection();

	//////////////////////////////////////////////////////////////////////////
	// ����� ������� ��������� �������� �� ����������� ���� � ��������������
	// ������� (bounding volume) ������������� �����.
	// ��� ���������� ������ ����������� ��� �������������� �� ����� ����������
	// �� ���� �� ������ �����, ��� �������� �������� ������ ��������:
	// if (!ray_intesects_bounding_volume)
	//     return false;
	//////////////////////////////////////////////////////////////////////////

	// �������� ���������� � ������� ������������� �����
	CTriangle const* const triangles = m_pMeshData->GetTriangles();
	const size_t numTriangles = m_pMeshData->GetTriangleCount();

	// ���������� � ����������� ���� � ������ �����
	struct FaceHit
	{
		CVector3d hitPointInObjectSpace; // ����� �����������
		double w0, w1, w2; // ������� ������������ ������ � ����� �����������
		double hitTime; // ����� �����������
		size_t faceIndex; // ������ �����
	};

	// ������ ��������� ����������� ���� � ������� �����.
	std::vector<FaceHit> faceHits;

	//////////////////////////////////////////////////////////////////////////
	// ������������ ����� ����������� ���� �� ����� ������� �����.
	// ������ ������ �������� ����� ������������� �� ������������� ������,
	// ���������� ����� ���������� �������� ������, �, ��� �����, ����� � ������ ������.
	//
	// ��� ������������ ������ ������������ ������� ����������� ������������� �����
	// �� � ���� ������� ������, � � ���� ����������� ��������� (Oct-Tree ��� BSP-Tree),
	// ��� �������� �������������� ��������� ������ ������������ � O(N) �� O(log N)
	//////////////////////////////////////////////////////////////////////////
	FaceHit hit;
	for (size_t i = 0; i < numTriangles; ++i)
	{
		CTriangle const& triangle = triangles[i];

		// �������� �� ����������� ���� � ����������� ������
		if (triangle.HitTest(invRayStart, invRayDirection, hit.hitTime, hit.hitPointInObjectSpace, hit.w0, hit.w1, hit.w2))
		{
			// ��������� ������ ����� � ��������� ���������� � ������ ��������� �����������
			hit.faceIndex = i;

			if (faceHits.empty())
			{
				// ��� ���������� ������� ����������� �����������
				// ������ ����� ��� 8 ����������� (��� ���������� ���������� �������� ��������� ������)
				faceHits.reserve(8);
			}
			faceHits.push_back(hit);
		}
	}

	// ��� ���������� ����������� �������
	if (faceHits.empty())
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	// ������������� ��������� ����������� �� ����������� ������� ������������
	//////////////////////////////////////////////////////////////////////////
	size_t const numHits = faceHits.size();
	std::vector<FaceHit const*> hitPointers(numHits);
	{
		// �������������� ������ ���������� �� ����� �����������
		for (size_t i = 0; i < numHits; ++i)
		{
			hitPointers[i] = &faceHits[i];
		}

		// ��������� ������ ���������� �� ����������� ������� ������������
		// ����������� ���������, � �� ���� �������, ����� ���������
		// �������� �� ����� ��������� �������:
		// �� 32 ������ ��������� ������ ��������� FaceHit ����� 64 ������,
		// � ������ ��������� - ����� 4 �����.
		if (numHits > 1)
		{
			struct HitPointerComparator
			{
				bool operator()(FaceHit const* p0, FaceHit const* p1) const
				{
					return (p0->hitTime < p1->hitTime);
				}
			};
			std::sort(hitPointers.begin(), hitPointers.end(), HitPointerComparator());
			// ������ ��������� � ������� hitPointers �������������
			// � ������� ����������� ������� ������������ ���� � �������
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// ���������� ���������� � ��������� ������������
	//////////////////////////////////////////////////////////////////////////
	for (size_t i = 0; i < numHits; ++i)
	{
		// �������� ���������� � ������������
		FaceHit const& faceHit = *hitPointers[i];

		// ����� ������������ � ������� ������� ���������
		CVector3d hitPoint = ray.GetPointAtTime(faceHit.hitTime);

		// �����, � ������� ��������� ������������
		CTriangle const& triangle = triangles[faceHit.faceIndex];

		// ������� "������� �����" �� ���� ������ ������������ ����� ������� ����� �����
		CVector3d normalInObjectSpace = triangle.GetPlaneEquation();

		if (!triangle.IsFlatShaded())
		{
			// ��� ��������� ������ ����������� ������������ �������� ������ ������������
			// � ������ �� ������� ������������� � ����� �����������
			Vertex const& v0 = triangle.GetVertex0();
			Vertex const& v1 = triangle.GetVertex1();
			Vertex const& v2 = triangle.GetVertex2();

			// ���������� ������ �������
			normalInObjectSpace = faceHit.w0 * v0.normal + faceHit.w1 * v1.normal + faceHit.w2 * v2.normal;
		}

		// ������� � ������� ������� ���������
		CVector3d normal = GetNormalMatrix() * normalInObjectSpace;

		// ��������� ���������� � ����� ����������� � ������ intersection
		intersection.AddHit(
			CHitInfo(
				faceHit.hitTime, *this,
				hitPoint,
				faceHit.hitPointInObjectSpace,
				normal, normalInObjectSpace));
	}

	return true;
}
