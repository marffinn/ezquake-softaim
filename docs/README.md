# ezQuake SoftAim - Enhanced QuakeWorld Client

![Downloads](https://img.shields.io/github/downloads/marffinn/ezquake-softaim/total?style=for-the-badge&logo=github&label=Total%20Downloads&color=blue)
![Latest Release Downloads](https://img.shields.io/github/downloads/marffinn/ezquake-softaim/latest/total?style=for-the-badge&logo=download&label=Latest%20Release&color=green)
![Latest Release](https://img.shields.io/github/release/marffinn/ezquake-softaim?style=for-the-badge&logo=github&label=Latest%20Release&color=orange)

## 🚀 Download

**[📥 Download Latest Release](https://github.com/marffinn/ezquake-softaim/releases/latest/download/ezquake.exe)**

## ✨ Features

### 🎯 SoftAim System
Subtle aim assistance that helps with micro-adjustments when firing. Natural feeling assistance that doesn't feel like a traditional aimbot.

```
cl_softaim 1                    // Enable SoftAim
cl_softaim_strength 1.0         // Assistance strength (0.5-2.0)
cl_softaim_maxdist 512          // Maximum targeting distance
cl_softaim_fov 90               // Field of view for targeting
```

### 🎲 cl_rDrive - Randomized SOCD System
Revolutionary randomized SOCD (Simultaneous Opposite Cardinal Directions) input clearing system that provides unpredictable input behavior for competitive play.

```
cl_rDrive 1                                    // Enable randomized SOCD
cl_rDrive_ms 5                                 // Max delay in milliseconds
cl_rDrive_pairs "forward,back;left,right"      // Configure key pairs
```

**How it works:**
1. **Conflict Detection:** When both keys in a configured pair are pressed simultaneously
2. **Random Selection:** The system randomly chooses which key to clear (50/50 chance)
3. **Random Timing:** A random delay (0 to cl_rDrive_ms milliseconds) is applied
4. **Input Clearing:** The selected key's input is cleared after the delay
5. **State Reset:** When both keys are released, the system resets

### ⚡ cl_iDrive - Deterministic SOCD
Traditional SOCD cleaning where the most recently pressed key takes priority. Predictable and consistent behavior.

```
cl_iDrive 1                     // Enable deterministic SOCD
```

## 📊 Download Statistics

The download counter badges above show:
- **Total Downloads:** All-time downloads across all releases
- **Latest Release Downloads:** Downloads of the most recent release only

These counters are updated automatically via GitHub's API and show exactly how many people have downloaded your ezQuake client.

## 🔧 Supported Keys

Both SOCD systems support these keys:
- **Movement:** `forward`, `back`, `left`, `right`, `moveleft`, `moveright`, `up`, `down`
- **Look:** `lookup`, `lookdown`
- **Actions:** `attack`, `attack2`, `jump`, `use`, `speed`, `strafe`

## ⚠️ Important Notes

- Only enable one SOCD system at a time: use either `cl_iDrive` OR `cl_rDrive`, not both
- cl_rDrive provides unpredictable behavior - perfect for competitive scenarios
- cl_iDrive provides consistent, predictable behavior
- SoftAim can be used alongside either SOCD system

## 🔗 Links

- [GitHub Repository](https://github.com/marffinn/ezquake-softaim)
- [All Releases](https://github.com/marffinn/ezquake-softaim/releases)
- [Report Issues](https://github.com/marffinn/ezquake-softaim/issues)
- [Official ezQuake](https://ezquake.com)
- [QuakeWorld Discord](http://discord.quake.world)

---

*Based on ezQuake - The Modern QuakeWorld Client*  
*Enhanced with SoftAim and cl_rDrive systems*