#pragma once
#include "GameProperties.h"
#include "Player/Hitresult.h"
#include "Player/Selector.h"
#include "Player/Player.h"
#include "Level/Level.h"
#include "Chunk/Chunk.h"
#include "Character/Zombie.h"
#include "Entity/EntityManager.h"
#include "Entity/EntityRenderer.h"
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

class Rubydung : public Default 
{
private:
	vec2 m_WinSize;
	vec2 m_Last;
	std::unique_ptr<gui> m_GUI;
	std::unique_ptr<Level> m_Level;
	std::unique_ptr<Player> m_Player;
	std::unique_ptr<Timer> m_Timer;
	std::unique_ptr <EntityManager> m_Entities;
	std::unique_ptr <EntityRenderer> m_EntityRenderer;
	Texture m_TerrainAtlas;
	Texture m_CharAtlas;
	Window m_InternalWindow;
	WindowProperties m_Props;
	bool m_MouseLeft;
	bool m_MouseRight;
public:
	Rubydung();
	~Rubydung();
public:
	void Init() override;
	void OnUpdate(Timestep& ts) override;
	void OnKeyPressed(int key);
	void OnCursorMoved(int& x, int& y);
	void OnEvent(Event& ev) override;
	void OnRender(float alpha) override;
	void OnTick() override;
	void OnSuspended() override;
};