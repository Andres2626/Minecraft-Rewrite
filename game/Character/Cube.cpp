#include "Character/Cube.h"

#include <Graphics/Renderer.h>

Cube::Cube(vec2 uv)
	: m_UV(uv), pos(0.0f), rot(0.0f)
{

}

Cube::~Cube()
{

}

void Cube::Push(std::vector<float>& vertices, vec3 p, vec2 uv)
{
	vec2 u = { uv.x / 64.0f, 1.0f - (uv.y / 32.0f) };
	vertices.push_back(p.x);
	vertices.push_back(p.y);
	vertices.push_back(p.z);
	vertices.push_back(u.x);
	vertices.push_back(u.y);
}

void Cube::PushQuad(vec3 p0, vec3 p1, vec3 p2, vec3 p3, vec2 uv0, vec2 uv1, std::vector<float>& vertices, std::vector<unsigned int>& indices)
{
	int count = (int)(vertices.size() / 5);
	Push(vertices, p0, { uv1.x, uv0.y });
	Push(vertices, p1, { uv0.x, uv0.y });
	Push(vertices, p2, { uv0.x, uv1.y });
	Push(vertices, p3, { uv1.x, uv1.y });
	PushIndices(indices, count);
}

void Cube::AddBox(vec3 start, vec3 size, std::vector<float>& vertices, std::vector<unsigned int>& indices)
{
	vec3 end = start + size;
	float w = size.x;
	float h = size.y;
	float d = size.z;

	vec3 u0 = { start.x, start.y, start.z };
	vec3 u1 = { end.x, start.y, start.z };
	vec3 u2 = { end.x, end.y, start.z };
	vec3 u3 = { start.x, end.y, start.z };
	vec3 l0 = { start.x, start.y, end.z };
	vec3 l1 = { end.x, start.y, end.z };
	vec3 l2 = { end.x, end.y, end.z };
	vec3 l3 = { start.x, end.y, end.z };

	vec2 uv0 = { m_UV.x + d + w, m_UV.y + d  };
	vec2 uv1 = { m_UV.x + d + w + d, m_UV.y + d + h };
	vec2 uv2 = { m_UV.x, m_UV.y + d };
	vec2 uv3 = { m_UV.x + d, m_UV.y + d + h };
	vec2 uv4 = { m_UV.x + d, m_UV.y };
	vec2 uv5 = { m_UV.x + d + w, m_UV.y + d };
	vec2 uv6 = { m_UV.x + d + w, m_UV.y};
	vec2 uv7 = { m_UV.x + d + w + w, m_UV.y + d };
	vec2 uv8 = { m_UV.x + d, m_UV.y + d };
	vec2 uv9 = { m_UV.x + d + w, m_UV.y + d + h };
	vec2 uv10 = { m_UV.x + d + w + d, m_UV.y + d };
	vec2 uv11 = { m_UV.x + d + w + d + w, m_UV.y + d + h };

	PushQuad(l1, u1, u2, l2, uv0, uv1, vertices, indices);
	PushQuad(u0, l0, l3, u3, uv2, uv3, vertices, indices);
	PushQuad(l1, l0, u0, u1, uv4, uv5, vertices, indices);
	PushQuad(u2, u3, l3, l2, uv6, uv7, vertices, indices);
	PushQuad(u1, u0, u3, u2, uv8, uv9, vertices, indices);
	PushQuad(l0, l1, l2, l3, uv10, uv11, vertices, indices);
}

void Cube::PushIndices(std::vector<unsigned int>& indices, int count)
{
	indices.push_back(count);
	indices.push_back(count + 1);
	indices.push_back(count + 2);
	indices.push_back(count + 2);
	indices.push_back(count + 3);
	indices.push_back(count);
}

mat4 Cube::GetModelMatrix()
{
	mat4 m = mat4(1.0f);
	m = translate(m, pos);
	m = rotate(m, rot.z, vec3(0.0f, 0.0f, 1.0f));
	m = rotate(m, rot.y, vec3(0.0f, 1.0f, 0.0f));
	m = rotate(m, rot.x, vec3(1.0f, 0.0f, 0.0f));
	return m;
}