#pragma once

#include "Drawable/DrawableData/DrawableData.h"

#include "OpenGL/VertexArray.h"
#include "OpenGL/VertexBufferLayout.h"
#include "OpenGL/IndexBuffer.h"

//TMP
#include "LoadableData/ObjData/ObjData.h"

namespace DrawableData {
class Geometry : public DrawableData
{
public:
	VertexArray m_vertexArray;
	VertexBuffer m_vertexBuffer;
	IndexBuffer m_indexBuffer;

	Geometry(const ObjData& objData);
	void loadFromObjData(const ObjData& objData);
	~Geometry();
};
}

