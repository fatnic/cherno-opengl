#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer {
public:
	Renderer();
	~Renderer();
    void clear() const;
    void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
};
