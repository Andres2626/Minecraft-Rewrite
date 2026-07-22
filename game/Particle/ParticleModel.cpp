#include "Particle/ParticleModel.h"

ParticleModel::ParticleModel()
{
	m_VAO = std::make_unique<VertexArray>();
	m_VBO = std::make_unique<VertexBuffer>(GL_STATIC_DRAW);
	m_IBO = std::make_unique<IndexBuffer>();
	m_InstanceBuffer = std::make_unique<VertexBuffer>(GL_DYNAMIC_DRAW);
	Build();
}

ParticleModel::~ParticleModel()
{

}

ParticleModel &ParticleModel::Get()
{
	static ParticleModel instance;
	return instance;
}

void ParticleModel::Build()
{
	std::vector<vec3> vertices;
	std::vector<unsigned int> indices;

	int count = (int)vertices.size();
	vertices.push_back({ -0.5f, -0.5f, 0.0f });
	vertices.push_back({ 0.5f, -0.5f, 0.0f });
	vertices.push_back({ 0.5f,  0.5f, 0.0f });
	vertices.push_back({ -0.5f,  0.5f, 0.0f });
	
	unsigned int ind[] = {
		0, 1, 2,
		2, 3, 0
	};

	for (int i = 0; i < 6; i++) {
		indices.push_back(count + ind[i]);
	}

	/* build mesh */
	VertexLayout VL, IVL;
	m_VAO->Bind();
	m_VBO->Build(vertices.size() * sizeof(vec3), vertices.data());
	m_IBO->Build(indices.size(), indices.data());
	VL.AddAttribute<vec3>(SHADER_VERTEX_BIT, sizeof(vec3), (void*)offsetof(Vertex, pos));
	m_VBO->SetVertexLayout(VL);
	
	m_InstanceBuffer->Resize(1000 * sizeof(ParticleInstance));
	IVL.AddAttribute<vec3>(5, sizeof(ParticleInstance), (void*)offsetof(ParticleInstance, pos), 1);
	IVL.AddAttribute<u32t>(6, sizeof(ParticleInstance), (void*)offsetof(ParticleInstance, tex), 1);
	IVL.AddAttribute<vec2>(7, sizeof(ParticleInstance), (void*)offsetof(ParticleInstance, texOffset), 1);

	m_InstanceBuffer->SetVertexLayout(IVL);
	m_InstanceBuffer->Unbind();
	m_VAO->Unbind();
}

void ParticleModel::UpdateInstances(const std::vector<ParticleInstance>& instances)
{
	if (instances.empty())
		return;

	m_InstanceBuffer->Bind();
	m_InstanceBuffer->Update(0, instances.size() * sizeof(ParticleInstance), instances.data());
}

void ParticleModel::Bind() const
{
	m_VAO->Bind();
}

void ParticleModel::Unbind() const
{
	m_VAO->Unbind();
}
