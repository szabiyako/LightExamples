#pragma once

#include "Drawable/DrawableData/DrawableData.h"

#include "OpenGL/VertexArray.h"
#include "OpenGL/VertexBufferLayout.h"
#include "OpenGL/IndexBuffer.h"

//TMP
#include "LoadableData/ObjData/ObjData.h"
#include "Utils/NonCopyable.h"

namespace DrawableData {
class Default : public DrawableData, public NonCopyable
{
public:
	VertexArray m_vertexArray;
	VertexBuffer m_vertexBuffer;
	IndexBuffer m_indexBuffer;

	Default(const ObjData& objData);
	void loadFromObjData(const ObjData& objData);
	~Default();
};
}

