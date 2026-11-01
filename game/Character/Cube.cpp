#include "Character/Cube.h"

#include <Graphics/Renderer.h>

Cube::Cube(vec2 uv)
	: m_UV(uv), pos(0.0f), rot(0.0f)
{

}

Cube::~Cube()
{

}

void Cube::Push(MeshData &data, vec3 p, vec2 uv)
{
	vec2 u = { uv.x / 64.0f, uv.y / 32.0f };
	MeshFactory::AddVertex(data, { p, { 0.0f, 0.0f, 0.0f }, u });
}

void Cube::PushQuad(vec3 p0, vec3 p1, vec3 p2, vec3 p3, vec2 uv0, vec2 uv1, MeshData &data)
{
	int count = (int)data.vertices.size();
	Push(data, p0, { uv1.x, uv0.y });
	Push(data, p1, { uv0.x, uv0.y });
	Push(data, p2, { uv0.x, uv1.y });
	Push(data, p3, { uv1.x, uv1.y });
	MeshFactory::AddIndices(data, count);
}

void Cube::AddBox(vec3 start, vec3 size, MeshData &data)
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

	PushQuad(l1, u1, u2, l2, uv0, uv1, data);
	PushQuad(u0, l0, l3, u3, uv2, uv3, data);
	PushQuad(l1, l0, u0, u1, uv4, uv5, data);
	PushQuad(u2, u3, l3, l2, uv6, uv7, data);
	PushQuad(u1, u0, u3, u2, uv8, uv9, data);
	PushQuad(l0, l1, l2, l3, uv10, uv11, data);
}