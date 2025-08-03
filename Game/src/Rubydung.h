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
using namespace Layer;
using namespace Math;

class Rubydung : public DefaultLayer {
private:
	vec2 last;
	Shader* sh_chunk, * sh_sel;
	Selector* py_sel;
	Level* lev;
	Player* steve;
	Texture tex;
	Window InternalWindow;
	WindowProperties props;
	Timer* tm;
	Image icon;
	bool last_mouse_left, last_mouse_right;
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