#pragma once
#include "Player/Hitresult.h"
#include "Player/Selector.h"
#include "Player/Player.h"
#include "Block/Tile.h"
#include "Level/Level.h"
#include "Chunk/Chunk.h"

#include <mc.h>

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
	vec2 m_Last;
	Shader* m_CShader, *m_SShader;
	Selector* m_Selector;
	Level* m_Level;
	Player* m_Player;
	Texture m_TerrainTexture;
	Window m_InternalWindow;
	WindowProperties m_Props;
	Timer* m_Timer;
	Image m_Icon;
	bool m_MouseLeft, m_MouseRight;
public:
	Rubydung();
	~Rubydung();
public:
	void Init() override;
	void OnUpdate(Timestep& ts) override;
	void OnKeyPressed(int key);
	void OnCursorMoved(int& x, int& y);
	void OnEvent(Event& ev) override;
	void OnRender() override;
	void OnTick() override;
	void OnSuspended() override;
private:
	bool Raycast(const vec3& org, const vec3& dir, Hitresult& ret);
	void PlayerPick();
};