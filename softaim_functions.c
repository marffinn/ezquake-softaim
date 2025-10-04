
/*
 * SoftAim System for EzQuake
 * 
 * This is my implementation of a subtle aim assistance system.
 * It's not a full aimbot - just helps with micro-adjustments when firing.
 * Should feel natural and not too obvious.
 * 
 * TODO: Maybe add prediction for moving targets?
 * FIXME: Line of sight check might be too strict sometimes
 */

#include "quakedef.h"

// Configuration variables - tweaked these values through lots of testing
cvar_t cl_softaim = { "cl_softaim", "0" };                           // Master switch - off by default
cvar_t cl_softaim_strength = { "cl_softaim_strength", "1.0" };       // How strong the assist is (0.5-2.0 seems good)
cvar_t cl_softaim_maxdist = { "cl_softaim_maxdist", "512" };         // Don't assist beyond this distance
cvar_t cl_softaim_fov = { "cl_softaim_fov", "90" };                  // Only assist targets in this FOV cone
cvar_t cl_softaim_smooth = { "cl_softaim_smooth", "0.3" };           // Smoothing factor - prevents jerky movement
cvar_t cl_softaim_autoswitch = { "cl_softaim_autoswitch", "0" };     // Auto weapon switching (experimental)
cvar_t cl_softaim_lg_range = { "cl_softaim_lg_range", "350" };       // LG effective range for auto-switch

// Function prototypes - organized by functionality
void CL_SoftAim_ApplyAimAssist(float *mouse_x, float *mouse_y);              // Main entry point
static qbool CL_SoftAim_FindBestTarget(vec3_t target_origin, float *target_distance);  // Target selection
static void CL_SoftAim_CalculateAngles(vec3_t target_origin, float *yaw_diff, float *pitch_diff);  // Math stuff
static void CL_SoftAim_CheckAutoSwitch(float target_distance);               // Weapon switching logic
static qbool CL_SoftAim_IsLightningGun(void);                               // Weapon detection helpers
static qbool CL_SoftAim_HasShotgun(void);
static qbool CL_SoftAim_HasLightningGun(void);
static qbool CL_SoftAim_IsShotgun(void);
static qbool CL_SoftAim_HasLineOfSight(vec3_t target_origin);                // LOS checking


/*
 * Calculate the angle differences between current view and target
 * This was a pain to get right - had to brush up on my trigonometry!
 */
static void CL_SoftAim_CalculateAngles(vec3_t target_origin, float *yaw_diff, float *pitch_diff)
{
	vec3_t dir, forward, right, up;
	float distance;
	
	// Get direction vector to target
	VectorSubtract(target_origin, cl.simorg, dir);
	distance = VectorLength(dir);
	
	// Sanity check - don't divide by zero or work with tiny distances
	if (distance < 1.0f) {
		*yaw_diff = 0;
		*pitch_diff = 0;
		return;
	}
	
	VectorNormalize(dir);
	
	// Get current view direction
	AngleVectors(cl.viewangles, forward, right, up);
	
	// Calculate target angles
	float target_yaw = atan2(dir[1], dir[0]) * 180.0f / M_PI;
	float current_yaw = atan2(forward[1], forward[0]) * 180.0f / M_PI;
	
	*yaw_diff = target_yaw - current_yaw;
	
	// Normalize angles to [-180, 180] range - this took me forever to debug
	while (*yaw_diff > 180.0f) *yaw_diff -= 360.0f;
	while (*yaw_diff < -180.0f) *yaw_diff += 360.0f;
	
	float target_pitch = -asin(dir[2]) * 180.0f / M_PI;
	*pitch_diff = target_pitch - cl.viewangles[PITCH];
	
	// Same normalization for pitch
	while (*pitch_diff > 180.0f) *pitch_diff -= 360.0f;
	while (*pitch_diff < -180.0f) *pitch_diff += 360.0f;
}

/*
 * Find the best target to assist aiming towards
 * Uses a scoring system based on distance and angle - closer and more centered = better
 */
static qbool CL_SoftAim_FindBestTarget(vec3_t target_origin, float *target_distance)
{
	int i;
	player_info_t *info;
	centity_t *cent;
	vec3_t dir, forward;
	float distance, angle_diff;
	float best_score = 999999.0f;  // Start with impossibly high score
	qbool found_target = false;
	float max_dist = cl_softaim_maxdist.value;
	float max_fov = cl_softaim_fov.value * 0.5f;  // Convert to half-angle
	static double last_debug_time = 0;  // For debug output throttling
	int active_players = 0, valid_enemies = 0;  // Stats for debugging
	
	// Basic sanity checks
	if (max_dist <= 0 || max_fov <= 0) {
		return false;
	}
	
	AngleVectors(cl.viewangles, forward, NULL, NULL);
	
	// Loop through all potential targets
	for (i = 0; i < MAX_CLIENTS; i++) {
		if (i == cl.playernum) {
			continue;  // Don't target ourselves, obviously
		}
		
		info = &cl.players[i];
		cent = &cl_entities[i + 1];
		
		if (info->name[0]) {
			active_players++;
		}
		
		// Skip invalid targets - no name, spectators, teammates, dead players
		if (!info->name[0] || info->spectator || info->teammate || info->dead) {
			continue;
		}
		
		valid_enemies++;
		
		// Calculate distance to target
		VectorSubtract(cent->lerp_origin, cl.simorg, dir);
		distance = VectorLength(dir);
		
		// Fallback to current origin if lerp_origin is zero
		if (distance < 1.0f) {
			VectorSubtract(cent->current.origin, cl.simorg, dir);
			distance = VectorLength(dir);
		}
		
		// Skip if too far or too close
		if (distance > max_dist || distance < 1.0f) {
			continue;
		}
		
		VectorNormalize(dir);
		
		// Check if target is within FOV cone
		float dot = DotProduct(forward, dir);
		angle_diff = acos(bound(-1.0f, dot, 1.0f)) * 180.0f / M_PI;
		
		if (angle_diff > max_fov) {
			continue;  // Outside FOV
		}
		
		// Use the better position (lerp vs current)
		vec3_t target_pos;
		VectorCopy(cent->lerp_origin[0] != 0 ? cent->lerp_origin : cent->current.origin, target_pos);
		
		// Make sure we can actually see the target
		if (!CL_SoftAim_HasLineOfSight(target_pos)) {
			continue;  // Wall in the way
		}
		
		// Scoring: distance + angle penalty (angle weighted more heavily)
		float score = distance + (angle_diff * 10.0f);
		
		if (score < best_score) {
			best_score = score;
			VectorCopy(cent->lerp_origin[0] != 0 ? cent->lerp_origin : cent->current.origin, target_origin);
			*target_distance = distance;
			found_target = true;
		}
	}
	
	return found_target;
}

/*
 * Main aim assist function - this gets called from the mouse input processing
 * Only works when actually firing and not in menus/console
 */
void CL_SoftAim_ApplyAimAssist(float *mouse_x, float *mouse_y)
{
	vec3_t target_origin;
	float target_distance;
	float yaw_diff, pitch_diff;
	float strength, distance_factor, smooth_factor;
	float aim_x, aim_y;
	
	// Early exits for disabled/invalid states
	if (!cl_softaim.value || cl_softaim_strength.value <= 0) {
		return;  // Disabled or zero strength
	}
	
	if (key_dest != key_game || cl.spectator) {
		return;  // In menu/console or spectating
	}
	
	if (!(in_attack.state & 1)) {
		return;  // Not firing - only assist when actually shooting
	}
	
	// Find a target to assist towards
	if (!CL_SoftAim_FindBestTarget(target_origin, &target_distance)) {
		return;  // No valid targets found
	}
	
	// Calculate how much we need to adjust our aim
	CL_SoftAim_CalculateAngles(target_origin, &yaw_diff, &pitch_diff);
	
	// Scale strength based on distance - closer targets get more assistance
	distance_factor = 1.0f - (target_distance / cl_softaim_maxdist.value);
	distance_factor = bound(0.0f, distance_factor, 1.0f);
	
	strength = cl_softaim_strength.value * distance_factor;
	
	// Convert angle differences to mouse movement
	// Negative yaw because mouse X is inverted relative to yaw
	aim_x = -yaw_diff / sensitivity.value;
	aim_y = pitch_diff / sensitivity.value;
	
	// Apply smoothing to prevent jerky movements
	smooth_factor = bound(0.01f, cl_softaim_smooth.value, 1.0f);
	
	float final_x = aim_x * strength * smooth_factor;
	float final_y = aim_y * strength * smooth_factor;
	
	// Add our assistance to the mouse input
	*mouse_x += final_x;
	*mouse_y += final_y;
	
	// Optional auto weapon switching based on range
	if (cl_softaim_autoswitch.value) {
		CL_SoftAim_CheckAutoSwitch(target_distance);
	}
}

// Simple weapon detection functions - these check both weapon ownership and ammo

static qbool CL_SoftAim_IsLightningGun(void)
{
	return (cl.stats[STAT_ACTIVEWEAPON] & IT_LIGHTNING) != 0;
}

static qbool CL_SoftAim_HasShotgun(void)
{
	return (cl.stats[STAT_ITEMS] & IT_SHOTGUN) && (cl.stats[STAT_SHELLS] >= 1);
}

static qbool CL_SoftAim_IsShotgun(void)
{
	return (cl.stats[STAT_ACTIVEWEAPON] & IT_SHOTGUN) != 0;
}

static qbool CL_SoftAim_HasLightningGun(void)
{
	return (cl.stats[STAT_ITEMS] & IT_LIGHTNING) && (cl.stats[STAT_CELLS] >= 1);
}

/*
 * Auto weapon switching logic - experimental feature
 * Switches between LG (close range) and shotgun (far range) based on target distance
 * Has a cooldown to prevent rapid switching
 */
static void CL_SoftAim_CheckAutoSwitch(float target_distance)
{
	static double last_switch_time = 0;
	
	// Prevent rapid weapon switching - 0.5 second cooldown
	if (curtime - last_switch_time < 0.5) {
		return;
	}
	
	float lg_range = cl_softaim_lg_range.value;
	qbool target_in_lg_range = (target_distance <= lg_range);
	qbool is_using_lg = CL_SoftAim_IsLightningGun();
	qbool is_using_shotgun = CL_SoftAim_IsShotgun();
	
	// Switch from LG to shotgun if target is too far
	if (is_using_lg && !target_in_lg_range) {
		if (!CL_SoftAim_HasShotgun()) {
			return;  // Don't have shotgun or ammo
		}
		
		in_impulse = 2;  // Switch to shotgun
		last_switch_time = curtime;
		return;
	}
	
	// Switch from shotgun to LG if target is close
	if (is_using_shotgun && target_in_lg_range) {
		if (!CL_SoftAim_HasLightningGun()) {
			return;  // Don't have LG or cells
		}
		
		in_impulse = 8;  // Switch to lightning gun
		last_switch_time = curtime;
		return;
	}
}

/*
 * Line of sight checking using ray tracing
 * Returns true if we have a clear shot to the target (no walls in the way)
 */
static qbool CL_SoftAim_HasLineOfSight(vec3_t target_origin)
{
	trace_t trace;
	vec3_t start, end;
	
	// Start from our eye position
	VectorCopy(cl.simorg, start);
	start[2] += ((cl.z_ext & Z_EXT_VIEWHEIGHT) || cls.nqdemoplayback) ? cl.stats[STAT_VIEWHEIGHT] : DEFAULT_VIEWHEIGHT;
	VectorCopy(target_origin, end);
	
	// Trace a line from our eyes to the target
	trace = PM_TraceLine(start, end);
	
	// If fraction is 1.0, we hit nothing (clear line of sight)
	return (trace.fraction >= 1.0f);
}

/*
 * Register all the softaim console variables
 * This should be called from CL_InitInput() during startup
 */
void CL_SoftAim_RegisterCvars(void)
{
	// Register all our configuration variables with the console system
	Cvar_Register(&cl_softaim);           // Master enable/disable
	Cvar_Register(&cl_softaim_strength);  // Assistance strength
	Cvar_Register(&cl_softaim_maxdist);   // Maximum targeting distance
	Cvar_Register(&cl_softaim_fov);       // Field of view for targeting
	Cvar_Register(&cl_softaim_smooth);    // Smoothing factor
	Cvar_Register(&cl_softaim_autoswitch); // Auto weapon switching
	Cvar_Register(&cl_softaim_lg_range);  // LG optimal range
}