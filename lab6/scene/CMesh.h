#pragma once
#include <boost/noncopyable.hpp>
#include "CBoundingBox.h"
#include "common_inc.h"
#include <vector>

class CMesh : public boost::noncopyable
{
public:
	// —труктура, описывающа€ подсетку
	struct SubMesh
	{
		// Ќачало подсетки в массиве индексов сетки
		unsigned startIndex;
		//  оличество индексов в подсетке
		unsigned indexCount;
	};

	CMesh(
		unsigned int vertexBufferOffset, // смещение в буфере вершин
		unsigned int indexBufferOffset, // смещение в буфере индексов
		unsigned vertexCount, // количество вершин
		unsigned indexCount, // количество индексов
		bool hasTextureCoords, // наличие текстурных координат
		CBoundingBox const& boundingBox, // ограничивающий блок
		GLenum primitiveType, // тип примитивов сетки
		GLenum indexType // тип индексов сетки
	);

	// ≈сть ли в сетке текстурные координаты?
	bool HasTextureCoords() const;

	// ¬озвращаем смещение относительно начала буфера вершин
	unsigned int GetVertexBufferOffset() const;

	// ¬озвращаем смещение относительно начала буфера индексов
	unsigned int GetIndexBufferOffset() const;

	// ¬озвращаем количество индексов
	unsigned int GetIndexCount() const;

	// ¬озвращаем количество вершин
	unsigned int GetVertexCount() const;

	// ¬озвращаем ограничивающий блок
	CBoundingBox const& GetBoundingBox() const;

	// ¬озвращаем тип примитивов (GL_TRIANGLES, GL_TRIANGLE_STRIP и т.п.)
	GLenum GetPrimitiveType() const;

	// ¬озвращаем тип данных дл€ хранени€ индексов (GL_UNSIGNED_SHORT и т.п.)
	GLenum GetIndexType() const;

	// ƒобавл€ем подсетку, диапазон индексов внутри сетки, объединенных
	// по некоторому признаку (например, по общему материалу)
	// ¬озвращаетс€ индекс добавленной подсетки
	unsigned AddSubMesh(unsigned startIndex, unsigned subMeshIndexCount);

	//  оличество подсеток
	unsigned GetSubMeshCount() const;

	// ¬озвращаем информацию о подсетке с заданным индексом
	SubMesh GetSubMesh(unsigned index) const;

	// «адаем индекс подсетки, грани которой имеют материал materialIndex
	void SetMaterialSubMesh(int materialIndex, unsigned subMeshIndex);

	// ѕолучаем индекс подсетки, грани которой имеют материал materialIndex
	int GetMaterialSubMesh(int materialIndex) const;

	// ѕолучаем количество материалов, используемых в сетке
	unsigned GetMaterialsCount() const;

	// ѕолучаем индес материала по его пор€дковому номеру в сетке
	int GetMaterial(unsigned index) const;

	// ѕолучаем количество байт, требуемых дл€ хранени€ одного индекса
	unsigned GetIndexSize() const;


private:
	// ћассив подсеток внутри данной сетки
	std::vector<SubMesh> m_subMeshes;
	// ћассив индексов материалов, используемых данной сеткой
	std::vector<int> m_materials;
	// ћассив, задающий индекс подсетки дл€ каждого материала
	std::vector<int> m_materialSubMesh;
	unsigned int m_vertexBufferOffset;
	unsigned int m_indexBufferOffset;
	unsigned int m_vertexCount;
	unsigned int m_indexCount;
	bool m_hasTexture;
	CBoundingBox const m_boundingBox;
	GLenum m_primitiveType;
	GLenum m_indexType;
};