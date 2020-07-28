#pragma once
#include "Projectile.h"

class Rocket :
	public Projectile
{
public:
	Rocket(ID3D11Device* _GD, Vector2 velocity);
private:
	static constexpr float ROCKET_DIMS = 12;
	static constexpr float ROCKET_WEIGHT = 10;
	static constexpr float ROCKET_EXP_RADIUS = 10;
	static constexpr float ROCKET_EXP_DMG = 60;

};

