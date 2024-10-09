#pragma once
#include "libgl/CGeometryObjectImpl.h"

/*
���������, �������� ���������� � ������� ������������� �����
*/
struct Vertex
{
	// ����������� �� ���������
	Vertex() {}

	// ����������� � ��������� �����������
	Vertex(CVector3d const& p, CVector3d const& n = CVector3d())
		: position(p)
		, normal(n)
	{
	}

	CVector3d position; // ���������� ������� � ������������
	CVector3d normal; // ���������� ������� �������
};

/*
���������, �������� ���������� � ����������� ����� ������������� �����
*/
struct Face
{
	// �����������
	Face(unsigned i0, unsigned i1, unsigned i2, bool flat = true)
		: vertex0(i0)
		, vertex1(i1)
		, vertex2(i2)
		, isFlat(flat)
	{
	}

	// ������� ������ �����
	unsigned vertex0, vertex1, vertex2;

	// �������� �� ����� �������
	// ������� � ����� ����� ������� ����� �������� ����������
	// � �� ������� �� �������� �������� �� ������
	bool isFlat;
};

/*
����� "�����������". ������ ��������� ���������� � ����������� �����
*/
class CTriangle
{
public:
	// ������������
	CTriangle(Vertex const& vertex0, Vertex const& vertex1, Vertex const& vertex2, bool flatShaded = true);

	// ������ �� ������� ������������
	Vertex const& GetVertex0() const { return *m_pVertex0; }
	Vertex const& GetVertex1() const { return *m_pVertex1; }
	Vertex const& GetVertex2() const { return *m_pVertex2; }

	// ���������� ������������ ��������� ��������� ������������
	CVector4d const& GetPlaneEquation() const
	{
		return m_planeEquation;
	}

	// ���������� �� ����������� "�������" ��������� ��� �� ���
	// ���������� ������� ������������ ������������ �������� ��� ������
	bool IsFlatShaded() const { return m_flatShaded; }

	// �������� �� ������������ ���� � �������������
	bool HitTest(
		CVector3d const& rayStart, // ����� ���������� ����
		CVector3d const& rayDirection, // ����������� ����
		double& hitTime, // ����� ������������ ���� � �������������
		CVector3d& hitPoint, // ����� ������������
		double& vertex0Weight, // ������� ����������� 0 ������� � ����� ������������
		double& vertex1Weight, // ������� ����������� 1 ������� � ����� ������������
		double& vertex2Weight, // ������� ����������� 2 ������� � ����� ������������
		double const& EPSILON = 1e-10 // ���������� ����������� ���������� ����� �����������
	) const;

private:
	CVector4d m_planeEquation; // ��������� ���������

	// �������������� � ������ ������������
	CVector3d m_edge01Perp;
	CVector3d m_edge12Perp;
	CVector3d m_edge20Perp;

	// �������� �������� ���� ���������������
	double m_invEdge01PerpSquare;
	double m_invEdge12PerpSquare;
	double m_invEdge20PerpSquare;

	// ��������� �� ������� ������������ (� ������� ������)
	Vertex const* m_pVertex0;
	Vertex const* m_pVertex1;
	Vertex const* m_pVertex2;

	// ���������� �� ����������� ������� ���������
	bool m_flatShaded;
};

/*
�����-��������� ������ ������������� ����� (������� ������ � ����������� ������).

������ �������� �������� �� ������������ �� �����, ��� ���������
��������� ������ �� �������� ������: ��������� ������������� �����
����� ��������� �� ���� � �� �� ������, �� ����� ������ �������������.
*/
class CTriangleMeshData
{
public:
	CTriangleMeshData(
		std::vector<Vertex> vertices, // �������
		const std::vector<Face>& faces, // �����
		bool normalize = false // ��������� �� ������������ �������� ������?
	);

	// ���������� ���������� ������
	size_t GetVertexCount() const { return m_vertices.size(); }
	// ����� ������� ������
	Vertex const* GetVertices() const { return &m_vertices[0]; }

	// ���������� �������������
	size_t GetTriangleCount() const { return m_triangles.size(); }
	// ����� ������� �������������
	CTriangle const* GetTriangles() const { return &m_triangles[0]; }

private:
	std::vector<Vertex> m_vertices; // �������
	std::vector<CTriangle> m_triangles; // ����������� �����
};

/*
����� "����� �� �������������"
*/
class CTriangleMesh : public CGeometryObjectImpl
{
public:
	CTriangleMesh(std::shared_ptr<CTriangleMeshData const> pMeshData, CMatrix4d const& transform = CMatrix4d());

	// ����� ����������� ���� � ������������� ������
	virtual bool Hit(CRay const& ray, CIntersection& intersection) const;

private:
	// ����� ������ ������������� �����
	std::shared_ptr<CTriangleMeshData const> m_pMeshData;
};
