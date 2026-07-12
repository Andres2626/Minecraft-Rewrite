#pragma once

#include <Graphics/Shader/Shader.h>
#include <Graphics/Buffers/IndexBuffer.h>
#include <Graphics/Buffers/VertexArray.h>
#include <Graphics/Buffers/VertexBuffer.h>
#include <Math/Math.h>

using namespace MC;
using namespace Math;
using namespace Graphics;

class Cube
{
public:
	vec2 m_UV;
	vec3 pos;
	vec3 rot;
public:
	Cube(vec2 uv);
	~Cube();
private:
	void Push(std::vector<float>& vertices, vec3 p, vec2 uv);
	void PushQuad(vec3 p0, vec3 p1, vec3 p2, vec3 p3, vec2 uv0, vec2 uv1, std::vector<float>& vertices, std::vector<unsigned int>& indices);
	void PushIndices(std::vector<unsigned int>& indices, int count);
public:
	void AddBox(vec3 start, vec3 size, std::vector<float> &vertices, std::vector<unsigned int> &indices);
};