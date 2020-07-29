#include "pch.h"
#include "Weapon.h"
#include "GameData.h"
#include "DrawData2D.h"
#include "TeamsManager.h"
#include "Rocket.h"
//#include "Bullet.h"
#include "grenade.h"
//#include "Dynamite.h"

Weapon::Weapon(ID3D11Device* _GD) :
	ImageGO2D("weapon", _GD)
{
	m_rotation = angle;
	pickColour();
	hud_weaponlist.SetColour(Color((float*)&Colors::OrangeRed));
	hud_weaponlist.SetScale(0.5);
}

void Weapon::Tick(GameData* _GD)
{
	updateWepListHudElement(&_GD->m_Teams); // TODO: Move to Turn Manager

	if (list_display_timer)
	{
		list_display_timer -= _GD->m_dt;
		if (list_display_timer < 0)
		{
			list_display_timer = 0;
		}
	}


	if (charge > 0 && !projectile_released)
	{
		chargeWeapon(_GD);
		return;
	}

	Unit* current_unit = _GD->m_Teams.getCurrentUnit();
	m_pos = current_unit->GetPos();
	bool still = current_unit->getPhysCmp()->isStill();
	if (has_fired || !still)
	{
		return;
	}
	// Player must be stood still, and not have already fired this turn

	if (_GD->m_Input.checkKey(InputManager::IMP_NEXT))
	{
		switchWep(_GD, true);
	}
	if (_GD->m_Input.checkKey(InputManager::IMP_PREV))
	{
		switchWep(_GD, false);
	}
	if (_GD->m_Input.checkKey(InputManager::IN_UP))
	{
		changeAngle(_GD, true);
	};
	if (_GD->m_Input.checkKey(InputManager::IN_DOWN))
	{
		changeAngle(_GD, false);
	}
	if (_GD->m_Input.checkKey(InputManager::IN_FIRE))
	{
		list_display_timer = HUD_LIST_DECAY_TIME + WEP_MAX_CHARGE_TIME;
		has_fired = true;
		current_unit->getPhysCmp()->setLocked(true);
		if (current_weptype == WEP_DYNAMITE)
		{
			fire(_GD);
			_GD->m_Input.releaseKey(InputManager::IN_FIRE);
			return;
		}
		list_display_timer = HUD_LIST_DECAY_TIME + WEP_MAX_CHARGE_TIME;
		chargeWeapon(_GD);
	}
}

void Weapon::Draw(DrawData2D* _DD)
{
	if (!has_fired)
	{
		_DD->m_Sprites->Draw(m_pTextureRV, m_pos, nullptr, m_colour, m_rotation, m_origin, m_scale, SpriteEffects_None);
	}
	if (list_display_timer)
	{
		hud_weaponlist.Draw(_DD);
	}
}

void Weapon::updateWepListHudElement(TeamsManager* _TM)
{
	// This could be optimized with a lot of small functions
	// But it seems like overkill given the time constraint
	int amount;
	string str = "";
	if (current_weptype == 0)
		str += ">";
	str += "BAZOOKA: ";
	amount = _TM->ammoCount(0);
	str += amount < 0 ? "INFINITE" : to_string(amount);
	str += "\n";

	if (current_weptype == 1)
		str += ">";
	str += "PISTOL: ";
	amount = _TM->ammoCount(1);
	str += amount < 0 ? "INFINITE" : to_string(amount);
	str += "\n";

	if (current_weptype == 2)
		str += ">";
	str += "GRENADE: ";
	amount = _TM->ammoCount(2);
	str += amount < 0 ? "INFINITE" : to_string(amount);
	str += "\n";

	if (current_weptype == 3)
		str += ">";
	str += "DYNAMITE: ";
	amount = _TM->ammoCount(3);
	str += amount < 0 ? "INFINITE" : to_string(amount);

	hud_weaponlist.SetString(str);
}

void Weapon::switchWep(GameData* _GD, bool forward)
{
	int wep_id = static_cast<int>(current_weptype);
	do
	{
		wep_id += forward ? 1 : -1;
		// secure bounds
		if (wep_id >= WEP_NUMWEPS)
		{
			wep_id = 0;
		}
		else if (wep_id < 0)
		{
			wep_id = WEP_NUMWEPS - 1;
		}
		// check if the current weapon has ammo
		// -1 (infinite ammo) is cast to true
	} while (!_GD->m_Teams.ammoCount(wep_id));
	// set Wep List display timer
	list_display_timer = HUD_LIST_DECAY_TIME;
	// update the weapon and sprite colour
	current_weptype = static_cast<Weapon::WepType>(wep_id);
	pickColour();
}

void Weapon::chargeWeapon(GameData* _GD)
{
	charge += _GD->m_dt;
	if (charge > WEP_MAX_CHARGE_TIME ||
		!_GD->m_Input.checkKey(InputManager::IN_FIRE))
	{
		fire(_GD);
	}
}

void Weapon::fire(GameData* _GD)
{
	list_display_timer = HUD_LIST_DECAY_TIME;
	_GD->m_Teams.consumeAmmo(current_weptype);
	_GD->m_Teams.getCurrentUnit()->getPhysCmp()->setLocked(false);

	GameObject2D* new_projectile = nullptr;
	switch (current_weptype)
	{
	case WEP_ROCKET: new_projectile =
		new Rocket(_GD->p_Device, _GD->m_Teams.getCurrentUnit()->GetPos(), generateAimVector() * WEP0_CHARGE_MULT);
		break;
	case WEP_PISTOL: //new_projectile = new Bullet(_GD->p_Device, Vector2(100, 0));
		break;
	case WEP_GRENADE: //new_projectile = new Grenade(_GD->p_Device, Vector2(100, 0));
		break;
	case WEP_DYNAMITE: //new_projectile = new Dynamite(_GD->p_Device, Vector2(100, 0));
		break;
	}
	_GD->creation_list.emplace_back(new_projectile);
	charge = 0;
}

void Weapon::changeAngle(GameData* _GD, bool is_up)
{
	bool flipped = _GD->m_Teams.getCurrentUnit()->isFlipped();
	int modi = 1;
	// bool XOR
	if (is_up != flipped)
	{
		modi = -1;
		// we only invert the increment if doing down when
		// facing right, or when going up when facing left
	}
	angle += WEP_ROT_SPEED * _GD->m_dt * modi;

	if (flipped)
	{
		if (angle > 0)
		{
			angle *= -1;
		}
		if (angle > -AIM_MIN_ANGLE)
			angle = -AIM_MIN_ANGLE;
		else if (angle < -AIM_MAX_ANGLE)
			angle = -AIM_MAX_ANGLE;
	}
	else
	{
		if (angle < 0)
		{
			angle *= -1;
		}
		if (angle < AIM_MIN_ANGLE)
			angle = AIM_MIN_ANGLE;
		else if (angle > AIM_MAX_ANGLE)
			angle = AIM_MAX_ANGLE;
	}
	m_rotation = angle;
}

void Weapon::pickColour()
{
	switch (current_weptype)
	{
	case WEP_ROCKET:
		m_colour = Color((float*)&Colors::DarkOliveGreen);
		break;
	case WEP_PISTOL:
		m_colour = Color((float*)&Colors::Yellow);
		break;
	case WEP_GRENADE:
		m_colour = Color((float*)&Colors::Orange);
		break;
	case WEP_DYNAMITE:
		m_colour = Color((float*)&Colors::Maroon);
		break;
	}
}

Vector2 Weapon::generateAimVector()
{
	float x = sin(angle);
	float y = -cos(angle);
	return Vector2(x, y) * charge;
}


// TODO:
// explosions wiping map
// charge indicator