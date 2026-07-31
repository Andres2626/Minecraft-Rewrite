#pragma once
#include "GameProperties.h"
#include "Player/Hitresult.h"
#include "Player/Selector.h"
#include "Player/Player.h"
#include "Level/Level.h"
#include "Chunk/Chunk.h"
#include "Character/Zombie.h"
#include "Renderer/ZombieRenderer.h"
#include "Model/ModelManager.h"
#include "Entity/EntityManager.h"
#include "Renderer/ParticleRenderer.h"
#include "gui/gui.h"

#include <core.h>

using namespace MC;
using namespace Graphics;
using namespace Utils;
using namespace Physics;
using namespace App;
using namespace Events;
using namespace Layers;
using namespace Math;
using namespace File;

class Minecraft : public Default 
{
private:
	vec2 m_WinSize;
	std::unique_ptr<gui> m_GUI;
	std::unique_ptr<Level> m_Level;
	std::unique_ptr<Player> m_Player;
	std::unique_ptr<Timer> m_Timer;
	std::unique_ptr<ZombieRenderer> m_ZombieRenderer;
	std::unique_ptr<ParticleRenderer> m_ParticleRenderer;
	std::unique_ptr<EntityManager> m_EntityManager;
	Texture m_TerrainAtlas;
	Texture m_CharAtlas;
	Window m_InternalWindow;
	WindowProperties m_Props;
	bool m_MouseLeft;
	bool m_MouseRight;
public:
	Minecraft();
	~Minecraft();
public:
	bool Init() override;
	void OnUpdate(Timestep& ts) override;
	void OnEvent(Event& ev);
	void OnKey(KeyboardButtonEvent &ev);
	void OnResize(WindowResizeEvent &ev);
	void OnCursorMove(CursorMotionEvent &ev);
	void OnCursorMoved(int& x, int& y);
	void OnRender(float alpha) override;
	void OnTick() override;
	void OnSuspended() override;
};