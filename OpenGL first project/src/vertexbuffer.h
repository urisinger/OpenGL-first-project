#pragma once

#include <GL/glew.h>
class VertexBuffer
{
private:
	unsigned int BufferID;
public:
	VertexBuffer(int a);
	~VertexBuffer();

	void AddData(const void* data, unsigned int size);
	void Bind() const;
	void UnBind() const;

};