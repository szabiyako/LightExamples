#pragma once

#include "Drawable/DrawableData/DrawableData.h"

#include "OpenGL/VertexArray.h"
#include "OpenGL/VertexBufferLayout.h"
#include "OpenGL/IndexBuffer.h"

//#include "OpenGL/Texture.h"

//TMP
#include "LoadableData/ObjData/ObjData.h"
#include "Utils/NonCopyable.h"

namespace DrawableData {
class Geometry : public DrawableData, public NonCopyable
{
public:
	Geometry(const ObjData& objData);
	void loadFromObjData(const ObjData& objData);
	~Geometry();

	bool hasUVs() const;
//private:
	VertexArray m_vertexArray;
	VertexBuffer m_vertexBuffer;
	IndexBuffer m_indexBuffer;

	bool m_hasUVs = false;
};
}
