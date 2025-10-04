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

#include "quakedef.h"
#include "common_draw.h"
#include "hud.h"
#include "hud_common.h"
#include "fonts.h"
#include "teamplay.h"
#include <time.h>

// Item respawn times (in seconds)
#define RESPAWN_POWERUP     120  // quad, pent, ring, suit
#define RESPAWN_MEGAHEALTH  20   // mega health
#define RESPAWN_ARMOR       20   // red, yellow, green armor
#define RESPAWN_WEAPON      30   // weapons
#define RESPAWN_HEALTH      15   // health boxes
#define RESPAWN_AMMO        15   // ammo boxes

// Maximum number of items to track
#define MAX_ITEM_TIMERS     32

typedef struct {
    unsigned int itemflag;      // Item type flag (it_quad, it_pent, etc.)
    vec3_t origin;              // Item location
    double pickup_time;         // When item was picked up
    int respawn_time;           // Respawn time in seconds
    qbool active;               // Is this timer slot active
    char name[32];              // Item name for display
} item_timer_t;

static item_timer_t item_timers[MAX_ITEM_TIMERS];
static int num_active_timers = 0;

// Debug logging function
static void ItemTimers_Log(const char *format, ...)
{
    static FILE *logfile = NULL;
    va_list args;
    time_t rawtime;
    struct tm *timeinfo;
    char timestamp[32];
    
    // Open log file on first use
    if (!logfile) {
        logfile = fopen("itemtimers_debug.log", "w");
        if (!logfile) {
            return;
        }
    }
    
    // Get timestamp
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(timestamp, sizeof(timestamp), "%H:%M:%S", timeinfo);
    
    // Write timestamp
    fprintf(logfile, "[%s] ", timestamp);
    
    // Write formatted message
    va_start(args, format);
    vfprintf(logfile, format, args);
    va_end(args);
    
    fprintf(logfile, "\n");
    fflush(logfile); // Ensure immediate write
}

// CVars for configuration (hud_itemtimers_show is created automatically by HUD system)
cvar_t hud_itemtimers_powerups = {"hud_itemtimers_powerups", "1"};
cvar_t hud_itemtimers_armor = {"hud_itemtimers_armor", "1"};
cvar_t hud_itemtimers_weapons = {"hud_itemtimers_weapons", "1"};
cvar_t hud_itemtimers_health = {"hud_itemtimers_health", "1"};
cvar_t hud_itemtimers_ammo = {"hud_itemtimers_ammo", "1"};
cvar_t hud_itemtimers_backpack = {"hud_itemtimers_backpack", "1"};

// Get respawn time for an item type
static int GetItemRespawnTime(unsigned int itemflag)
{
    if (itemflag & (it_quad | it_pent | it_ring | it_suit)) {
        return RESPAWN_POWERUP;
    }
    else if (itemflag & it_mh) {
        return RESPAWN_MEGAHEALTH;
    }
    else if (itemflag & (it_ra | it_ya | it_ga)) {
        return RESPAWN_ARMOR;
    }
    else if (itemflag & (it_lg | it_rl | it_gl | it_sng | it_ng | it_ssg)) {
        return RESPAWN_WEAPON;
    }
    else if (itemflag & it_health) {
        return RESPAWN_HEALTH;
    }
    else if (itemflag & (it_shells | it_nails | it_rockets | it_cells)) {
        return RESPAWN_AMMO;
    }
    else if (itemflag & it_pack) {
        return RESPAWN_AMMO; // Backpack uses ammo respawn time
    }
    
    return RESPAWN_HEALTH; // Default
}

// Get item name for display
static const char* GetItemDisplayName(unsigned int itemflag)
{
    if (itemflag & it_quad) return "QUAD";
    if (itemflag & it_pent) return "PENT";
    if (itemflag & it_ring) return "RING";
    if (itemflag & it_suit) return "SUIT";
    if (itemflag & it_mh) return "MH";
    if (itemflag & it_ra) return "RA";
    if (itemflag & it_ya) return "YA";
    if (itemflag & it_ga) return "GA";
    if (itemflag & it_lg) return "LG";
    if (itemflag & it_rl) return "RL";
    if (itemflag & it_gl) return "GL";
    if (itemflag & it_sng) return "SNG";
    if (itemflag & it_ng) return "NG";
    if (itemflag & it_ssg) return "SSG";
    if (itemflag & it_health) return "HEALTH";
    if (itemflag & it_shells) return "SHELLS";
    if (itemflag & it_nails) return "NAILS";
    if (itemflag & it_rockets) return "ROCKETS";
    if (itemflag & it_cells) return "CELLS";
    if (itemflag & it_pack) return "PACK";
    
    return "ITEM";
}

// Check if we should track this item type
static qbool ShouldTrackItem(unsigned int itemflag)
{
    // Note: HUD visibility is handled by the HUD system itself via hud->show->value
    // This function only checks if the specific item type should be tracked
    
    if ((itemflag & (it_quad | it_pent | it_ring | it_suit)) && hud_itemtimers_powerups.integer) {
        return true;
    }
    if ((itemflag & (it_mh | it_ra | it_ya | it_ga)) && hud_itemtimers_armor.integer) {
        return true;
    }
    if ((itemflag & (it_lg | it_rl | it_gl | it_sng | it_ng | it_ssg)) && hud_itemtimers_weapons.integer) {
        return true;
    }
    if ((itemflag & it_health) && hud_itemtimers_health.integer) {
        return true;
    }
    if ((itemflag & (it_shells | it_nails | it_rockets | it_cells)) && hud_itemtimers_ammo.integer) {
        return true;
    }
    if ((itemflag & it_pack) && hud_itemtimers_backpack.integer) {
        return true;
    }
    
    return false;
}

// Add or update an item timer
void HUD_ItemTimers_AddTimer(unsigned int itemflag, vec3_t origin)
{
    int i;
    item_timer_t *timer = NULL;
    
    // Debug output
    ItemTimers_Log("AddTimer called with flag %u", itemflag);
    Com_Printf("ItemTimers: AddTimer called with flag %u\n", itemflag);
    
    if (!ShouldTrackItem(itemflag)) {
        ItemTimers_Log("Item not tracked (powerups=%d, armor=%d, weapons=%d, health=%d, ammo=%d, backpack=%d)", 
                      hud_itemtimers_powerups.integer, hud_itemtimers_armor.integer,
                      hud_itemtimers_weapons.integer, hud_itemtimers_health.integer, hud_itemtimers_ammo.integer, hud_itemtimers_backpack.integer);
        Com_Printf("ItemTimers: Item not tracked (powerups=%d, armor=%d, weapons=%d, health=%d, ammo=%d, backpack=%d)\n", 
                  hud_itemtimers_powerups.integer, hud_itemtimers_armor.integer,
                  hud_itemtimers_weapons.integer, hud_itemtimers_health.integer, hud_itemtimers_ammo.integer, hud_itemtimers_backpack.integer);
        return;
    }
    
    ItemTimers_Log("Adding timer for %s", GetItemDisplayName(itemflag));
    Com_Printf("ItemTimers: Adding timer for %s\n", GetItemDisplayName(itemflag));
    
    // Look for existing timer at this location
    for (i = 0; i < MAX_ITEM_TIMERS; i++) {
        if (item_timers[i].active && 
            item_timers[i].itemflag == itemflag &&
            VectorDistance(item_timers[i].origin, origin) < 32) {
            timer = &item_timers[i];
            break;
        }
    }
    
    // If not found, create new timer
    if (!timer) {
        for (i = 0; i < MAX_ITEM_TIMERS; i++) {
            if (!item_timers[i].active) {
                timer = &item_timers[i];
                num_active_timers++;
                break;
            }
        }
    }
    
    if (timer) {
        timer->itemflag = itemflag;
        VectorCopy(origin, timer->origin);
        timer->pickup_time = cls.realtime;
        timer->respawn_time = GetItemRespawnTime(itemflag);
        timer->active = true;
        strlcpy(timer->name, GetItemDisplayName(itemflag), sizeof(timer->name));
    }
}

// Remove expired timers
static void CleanupExpiredTimers(void)
{
    int i;
    double current_time = cls.realtime;
    
    for (i = 0; i < MAX_ITEM_TIMERS; i++) {
        if (item_timers[i].active) {
            double elapsed = current_time - item_timers[i].pickup_time;
            if (elapsed >= item_timers[i].respawn_time) {
                item_timers[i].active = false;
                num_active_timers--;
            }
        }
    }
}

// Draw the item timers HUD element
static void SCR_HUD_DrawItemTimers(hud_t *hud)
{
    static cvar_t *scale = NULL, *proportional;
    int x, y, width, height;
    int line_height, max_width;
    int i, line = 0;
    double current_time;
    char timestr[32];
    
    if (scale == NULL) {
        scale = HUD_FindVar(hud, "scale");
        proportional = HUD_FindVar(hud, "proportional");
    }
    
    // Debug output
    ItemTimers_Log("Draw called (show=%d, active=%d)", (int)hud->show->value, num_active_timers);
    Com_Printf("ItemTimers: Draw called (show=%d, active=%d)\n", (int)hud->show->value, num_active_timers);
    
    if (!hud->show->value) {
        return;
    }
    
    // Always show a test message if no timers are active
    if (num_active_timers == 0) {
        Draw_String(10, 100, "ItemTimers: No active timers");
        return;
    }
    
    CleanupExpiredTimers();
    
    current_time = cls.realtime;
    line_height = 8 * scale->value;
    max_width = 0;
    
    // Calculate required width and height
    for (i = 0; i < MAX_ITEM_TIMERS; i++) {
        if (item_timers[i].active) {
            double elapsed = current_time - item_timers[i].pickup_time;
            int remaining = (int)(item_timers[i].respawn_time - elapsed);
            
            if (remaining > 0) {
                snprintf(timestr, sizeof(timestr), "%s: %ds", item_timers[i].name, remaining);
                int text_width = strlen(timestr) * FontFixedWidth(1, scale->value, false, proportional->integer);
                if (text_width > max_width) {
                    max_width = text_width;
                }
                line++;
            }
        }
    }
    
    if (line == 0) {
        return;
    }
    
    width = max_width;
    height = line * line_height;
    
    if (!HUD_PrepareDraw(hud, width, height, &x, &y)) {
        return;
    }
    
    // Draw the timers
    line = 0;
    for (i = 0; i < MAX_ITEM_TIMERS; i++) {
        if (item_timers[i].active) {
            double elapsed = current_time - item_timers[i].pickup_time;
            int remaining = (int)(item_timers[i].respawn_time - elapsed);
            
            if (remaining > 0) {
                snprintf(timestr, sizeof(timestr), "%s: %ds", item_timers[i].name, remaining);
                
                // Color code based on time remaining
                if (remaining <= 5) {
                    Draw_SColoredStringBasic(x, y + line * line_height, timestr, 1, scale->value, proportional->integer); // Red
                } else if (remaining <= 10) {
                    Draw_SColoredStringBasic(x, y + line * line_height, timestr, 3, scale->value, proportional->integer); // Yellow
                } else {
                    Draw_SString(x, y + line * line_height, timestr, scale->value, proportional->integer); // White
                }
                
                line++;
            }
        }
    }
}

// Initialize item timers system
void HUD_ItemTimers_Init(void)
{
    int i;
    
    ItemTimers_Log("HUD_ItemTimers_Init called");
    
    // Clear all timers
    for (i = 0; i < MAX_ITEM_TIMERS; i++) {
        item_timers[i].active = false;
    }
    num_active_timers = 0;
    
    ItemTimers_Log("Registering CVars");
    
    // Register CVars (note: hud_itemtimers_show is created automatically by HUD_Register)
    Cvar_Register(&hud_itemtimers_powerups);
    Cvar_Register(&hud_itemtimers_armor);
    Cvar_Register(&hud_itemtimers_weapons);
    Cvar_Register(&hud_itemtimers_health);
    Cvar_Register(&hud_itemtimers_ammo);
    Cvar_Register(&hud_itemtimers_backpack);
    
    ItemTimers_Log("Registering HUD element");
    
    // Register HUD element
    HUD_Register(
        "itemtimers", NULL, "Shows countdown timers for item respawns.",
        0, ca_active, 0, SCR_HUD_DrawItemTimers,
        "1", "screen", "center", "left", "10", "100", "0", "0 0 0", NULL,
        "scale", "1",
        "proportional", "0",
        NULL
    );
    
    ItemTimers_Log("HUD element registered successfully");
    
    // Try to find our HUD element to verify it was registered
    hud_t* our_hud = HUD_Find("itemtimers");
    if (our_hud) {
        ItemTimers_Log("HUD element found after registration: show=%d", (int)our_hud->show->value);
        Com_Printf("ItemTimers: HUD element found after registration: show=%d\n", (int)our_hud->show->value);
    } else {
        ItemTimers_Log("ERROR: HUD element not found after registration!");
        Com_Printf("ItemTimers: ERROR: HUD element not found after registration!\n");
    }
}

// HUD module initialization (called from CommonDraw_Init)
void ItemTimers_HudInit(void)
{
    ItemTimers_Log("HudInit called");
    Com_Printf("ItemTimers: HudInit called\n");
    // Initialize the item timers system
    HUD_ItemTimers_Init();
}

// Reset all timers (called on map change)
void HUD_ItemTimers_Reset(void)
{
    int i;
    
    for (i = 0; i < MAX_ITEM_TIMERS; i++) {
        item_timers[i].active = false;
    }
    num_active_timers = 0;
}