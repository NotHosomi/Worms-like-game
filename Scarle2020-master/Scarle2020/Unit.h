#pragma once

#include "ImageGO2D.h"
#include "TextGO2D.h"
#include "PhysicsComponent.h"

class Unit : public ImageGO2D
{
public:
	Unit() = delete;
	Unit(ID3D11Device* _GD, const Vector2& location, int team);
	Unit(const Unit& other);
	Unit(Unit&& other) noexcept;

	void Tick(GameData* _GD) override;
	void Draw(DrawData2D* _DD) override;

	// TeamsManager controls
	void setAwake(bool _awake);
	int getTeam();
	bool isAlive();
	bool isFlipped();

	void addDamage(float amount);
	void applyDamages();

	// debugging
	PhysicsComponent* getPhysCmp() { return &PhysCmp; };

private:
	static constexpr float UNIT_WIDTH = 16;
	static constexpr float UNIT_HEIGHT = 26; // golden ratio 1 : 1.62
	static constexpr float UNIT_HP_BAR_OFFSET = -5;
	static constexpr float UNIT_HP_BAR_SCALE = 0.35;

	static constexpr float UNIT_WEIGHT = 60;
	static constexpr float UNIT_ELASTICITY = 0.1;

	static constexpr float MV_ACCELERATION = 12;
	static constexpr float MV_AIRACCELERATION = 1;
	static constexpr float MV_MAXAIRCONTROL = 10;

	static constexpr float MV_JUMPFORCE_V = 60;
	static constexpr float MV_JUMPFORCE_H = 40;
	static constexpr float MV_JUMPCOOLDOWN = 2;

	void playerMove(GameData* _GD);

	PhysicsComponent PhysCmp;
	bool awake = false;
	bool alive = true;

	int health = 100;
	int accumulated_damage = 0;
	TextGO2D hp_text = TextGO2D("100");

	int team_id = 0;
	bool facing_right = true;
};

