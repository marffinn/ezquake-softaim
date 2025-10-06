# ezQuake Soft Aim - Enhanced QuakeWorld Client

![Downloads](https://img.shields.io/github/downloads/marffinn/ezquake-softaim/total?style=for-the-badge&logo=github&label=Total%20Downloads&color=blue)
![Latest Release Downloads](https://img.shields.io/github/downloads/marffinn/ezquake-softaim/latest/total?style=for-the-badge&logo=download&label=Latest%20Release&color=green)
![EXE Downloads](https://img.shields.io/github/downloads/marffinn/ezquake-softaim/latest/ezquake.exe?style=for-the-badge&label=ezquake.exe&color=orange)

Advanced QuakeWorld client with intelligent aim assistance and sophisticated input systems for competitive gaming.

## 🎯 Softaim System

Advanced aim assistance with intelligent target selection and smooth tracking.

**Configuration:**
```
cl_softaim 1                    // Enable/disable (0 or 1)
cl_softaim_strength 1.0         // Assistance strength (0.5-2.0)
cl_softaim_maxdist 512          // Maximum targeting distance
cl_softaim_fov 90               // Field of view for targeting
cl_softaim_smooth 0.3           // Smoothing factor (0.01-1.0)
cl_softaim_autoswitch 1         // Auto weapon switching (0 or 1)
cl_softaim_lg_range 350         // Lightning Gun optimal range
```

**Features:**
- Smart target selection (prioritizes closest enemies within FOV)
- Line-of-sight validation (no targeting through walls)
- Distance-based strength scaling
- Only activates when attacking (+attack pressed)
- Auto weapon switching (LG for close range, SG for long range)
- Smooth interpolation that blends with player input
- Ignores teammates, spectators, and dead players

**Presets:**
- **Subtle:** `strength 0.7, smooth 0.5, fov 60`
- **Balanced:** `strength 1.0, smooth 0.3, fov 90`
- **Aggressive:** `strength 1.5, smooth 0.15, fov 120`

## 🎲 cl_rDrive - Randomized SOCD System

Randomized input clearing for unpredictable competitive play.

**Configuration:**
```
cl_rDrive 1                                    // Enable randomized SOCD
cl_rDrive_ms 5                                 // Max delay in milliseconds
cl_rDrive_pairs "forward,back;left,right"      // Key pair configuration
```

**How It Works:**
1. Detects when conflicting keys are pressed simultaneously
2. Randomly chooses which key to clear (50/50 chance)
3. Applies random delay (0 to cl_rDrive_ms milliseconds)
4. Clears the selected key's input
5. Resets when both keys are released

**Supported Keys:** `forward, back, left, right, moveleft, moveright, up, down, lookup, lookdown, attack, attack2, jump, use, speed, strafe`

**Examples:**
```
Basic:    cl_rDrive_pairs "forward,back;left,right"
Strafe:   cl_rDrive_pairs "moveleft,moveright"
Advanced: cl_rDrive_pairs "forward,back;left,right;lookup,lookdown"
```

## ⚡ cl_iDrive - Deterministic SOCD System

Traditional SOCD cleaning where most recent key takes priority.

**Configuration:**
```
cl_iDrive 1    // Enable deterministic SOCD
```

**Features:**
- Last input priority (newest key always wins)
- Instant response (no delays)
- Fixed key pairs (predefined)
- 100% predictable behavior

⚠️ **WARNING:** Only enable ONE SOCD system at a time! Use either `cl_iDrive` OR `cl_rDrive`, never both simultaneously.

## 📊 Comparison: cl_iDrive vs cl_rDrive

| Feature | cl_iDrive | cl_rDrive |
|---------|-----------|----------|
| Clearing Logic | Deterministic (newest wins) | Random selection (50/50) |
| Timing | Immediate (0ms) | Random delay (0-N ms) |
| Configuration | Fixed key pairs | Fully configurable pairs |
| Predictability | 100% predictable | Unpredictable behavior |
| Use Case | Consistent gameplay | Competitive advantage |

## Download

[**Download ezquake.exe**](https://github.com/marffinn/ezquake-softaim/releases/latest/download/ezquake.exe) | [**View All Files**](https://github.com/marffinn/ezquake-softaim/releases/latest)

---

[Website](https://marffinn.github.io/ezquake-softaim/) | [GitHub](https://github.com/marffinn/ezquake-softaim)
