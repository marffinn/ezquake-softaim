/*
Copyright (C) 2024 EzQuake team

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __HUD_ITEMTIMERS_H__
#define __HUD_ITEMTIMERS_H__

// CVars for configuration (hud_itemtimers_show is created automatically by HUD system)
extern cvar_t hud_itemtimers_powerups;
extern cvar_t hud_itemtimers_armor;
extern cvar_t hud_itemtimers_weapons;
extern cvar_t hud_itemtimers_health;
extern cvar_t hud_itemtimers_ammo;
extern cvar_t hud_itemtimers_backpack;

// Initialize the item timers system
void HUD_ItemTimers_Init(void);

// Initialize the HUD module (called from CommonDraw_Init)
void ItemTimers_HudInit(void);

// Add a timer for an item pickup
void HUD_ItemTimers_AddTimer(unsigned int itemflag, vec3_t origin);

// Reset all timers (called on map change)
void HUD_ItemTimers_Reset(void);

#endif // __HUD_ITEMTIMERS_H__