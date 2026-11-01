#include "gui/gui.h"

#include "Block/BlockManager.h"

#include "Model/ModelManager.h"
#include "Model/BlockModel.h"
#include "Model/BushModel.h"

#include <Utils/Profiler.h>
#include <Utils/Util.h>

#ifdef MC_PLATFORM_WEB
# define MAX_TEXTURE_UNITS 16
#else
# define MAX_TEXTURE_UNITS 32
#endif

gui::gui(Player *player, const vec2 &winsize)
	: m_FPS(0), m_CUPS(0)
{
	m_Player = player;
	m_GUISize.x = winsize.x * 240 / winsize.y;
	m_GUISize.y = winsize.y * 240 / winsize.y;
	m_Cam = std::make_unique<OrthographicCamera>(0.0f, m_GUISize.x, 0.0f, m_GUISize.y, 100.0f, 300.0f);
	m_Shader = &ShaderManager::Get("hud");
    m_Font = std::make_unique<DefaultFont>();

	int samplers[MAX_TEXTURE_UNITS];
    for (int i = 0; i < MAX_TEXTURE_UNITS; i++)
		samplers[i] = i;

	m_Shader->Bind();
	m_Shader->SetUniformArray("s_Textures", MAX_TEXTURE_UNITS, samplers);
    m_Shader->SetUniformArray("s_Textures", MAX_TEXTURE_UNITS, samplers);

	Build();
    BuildCrossHair();
}

gui::~gui()
{

}

void gui::BuildCrossHair()
{
	float x = m_GUISize.x / 2;
	float y = m_GUISize.y / 2;

	m_Renderer.FillRect({ x, y - 4.0f }, { 1.0f, 9.0f }, { 1.0f, 1.0f, 1.0f });
	m_Renderer.FillRect({ x - 4.0f, y }, { 9.0f, 1.0f }, { 1.0f, 1.0f, 1.0f });
}

void gui::Build()
{
	m_BP.Build(m_Player);
}

void gui::Render(Texture *tex)
{
	PROFILE_FUNCTION();
	m_Cam->Update();

	Renderer::Clear(GL_DEPTH_BUFFER_BIT);

	m_Shader->Bind();
    
	glm::mat4 VM = glm::mat4(1.0f);
	VM = translate(VM, { 0.0f, 0.0f, -200.0f });
	m_Shader->SetUniform("s_VP", m_Cam->GetProjection() * VM);
	m_Shader->SetUniform("s_M", glm::mat4(1.0f));
	
	m_Renderer.Begin();
	BuildCrossHair();
	m_Renderer.DrawFormatedTextWS(m_Font.get(), { 2.0f, m_GUISize.y - 2.0f }, { 1.0f, 1.0f, 1.0f }, "0.0.2a");
	m_Renderer.DrawFormatedTextWS(m_Font.get(), { 2.0f, m_GUISize.y - 12.0f }, { 1.0f, 1.0f, 1.0f }, "%i fps, %i chunk updates", m_FPS, m_CUPS);
	m_Renderer.End();

	Renderer::Enable(GL_BLEND);
	Renderer::BlendFunc(BlendValue::SRC_ALPHA, BlendValue::ONE_MINUS_SRC_ALPHA);
	m_Renderer.Render();
	m_BP.Render(m_GUISize, tex, m_Cam.get());
	Renderer::Disable(GL_BLEND);
}

void gui::OnTick(int fps, int cups)
{
	m_FPS = fps;
	m_CUPS = cups;
}
