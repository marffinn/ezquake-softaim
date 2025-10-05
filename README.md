# ezQuake — Modern QuakeWorld Client
Homepage: [https://ezquake.com][homepage]

Community discord: [http://discord.quake.world][discord]

![Downloads](https://img.shields.io/github/downloads/marffinn/ezquake-softaim/total?style=for-the-badge&logo=github&label=Total%20Downloads&color=blue)
![Latest Release Downloads](https://img.shields.io/github/downloads/marffinn/ezquake-softaim/latest/total?style=for-the-badge&logo=download&label=Latest%20Release&color=green)
![EXE Downloads](https://img.shields.io/github/downloads/marffinn/ezquake-softaim/latest/ezquake.exe?style=for-the-badge&label=ezquake.exe&color=orange)

This is the right place to start playing QuakeWorld&reg; — the fastest first
person shooter action game ever.

Combining the features of all modern QuakeWorld® clients, ezQuake makes
QuakeWorld&reg; easier to start and play. The immortal first person shooter
Quake&reg; in the brand new skin with superb graphics and extremely fast
gameplay.

## Features

 * Modern graphics
 * [QuakeTV][qtv] support
 * Rich menus
 * Multiview support
 * Tons of features to serve latest pro-gaming needs
 * Built in server browser & MP3 player control
 * Recorded games browser
 * Customization of all possible graphics elements of the game including Heads Up Display
 * All sorts of scripting possibilities
 * **Advanced Input Systems**: 
   - `cl_iDrive`: Deterministic SOCD (Simultaneous Opposite Cardinal Directions) cleaning
   - `cl_rDrive`: Randomized SOCD cleaning with configurable timing and key pairs
 * **SoftAim System**: Advanced aim assistance with target prioritization, line-of-sight validation, and auto weapon switching
 * Windows, Linux, MacOSX and FreeBSD platforms supported (SDL2).

Our client comes only with bare minimum of game media. If you want to
experience ezQuake with modern graphics and other additional media including
custom configurations, maps, textures and more, try using the [nQuake][nQuake]-installer.

## Support

Need help with using ezQuake? Try #dev-corner on [discord][discord]

Or (less populated these days) visit us on IRC at QuakeNet, channel #ezQuake: [webchat][webchat] or [IRC][IRC].

Sometimes help from other users of ezQuake might be more useful to you so you
can also try visiting the [quakeworld.nu Client Talk-forums][forum].

If you have found a bug, please report it [here][issues]

## Installation guide

To play Quakeworld you need the files *pak0.pak* and *pak1.pak* from the original Quake-game.

### Install ezQuake to an existing Quake-installation
If you have an existing Quake-installation simply extract the ezQuake executable into your Quake-directory.

A typical error message when installing ezQuake into a pre-existing directory is about *glide2x.dll* missing.
To get rid of this error, remove the file *opengl32.dll* from your Quake directory.

### Upgrade an nQuake-installation
If you have a version of [nQuake][nQuake] already installed you can upgrade ezQuake by extracting the new executable into the nQuake-directory.

### Minimal clean installation
If you want to make a clean installation of ezQuake you can do this by following these steps:

1. Create a new directory
2. Extract the ezQuake-executable into this directory
3. Create a subdirectory called *id1*
4. Copy *pak0.pak* and *pak1.pak* into this subdirectory

## Advanced Features

### SoftAim - Advanced Aim Assistance

The `cl_softaim` system provides intelligent aim assistance with natural micro-adjustments during combat.

**Configuration:**
```
cl_softaim 1                    // Enable SoftAim
cl_softaim_strength 1.0         // Assistance strength (0.5-2.0)
cl_softaim_maxdist 512          // Maximum targeting distance
cl_softaim_fov 90               // Field of view for targeting
cl_softaim_smooth 0.3           // Smoothing factor (0.01-1.0)
cl_softaim_autoswitch 1         // Auto weapon switching (LG/SG)
cl_softaim_lg_range 350         // Lightning gun optimal range
```

**Features:**
- Smart target selection prioritizing closest enemies within FOV
- Line-of-sight validation to prevent targeting through walls
- Distance-based strength scaling for natural feel
- Only activates when attacking (requires +attack)
- Automatic weapon switching between Lightning Gun and Shotgun based on range
- Smooth interpolation that blends with player input
- Ignores teammates, spectators, and dead players

### cl_rDrive - Randomized SOCD System

The `cl_rDrive` system provides randomized input clearing for competitive play, offering an alternative to the deterministic `cl_iDrive` system.

**Configuration:**
```
cl_rDrive 1                                    // Enable randomized SOCD
cl_rDrive_ms 5                                 // Max delay in milliseconds  
cl_rDrive_pairs "forward,back;left,right"      // Configure key pairs
```

**Supported Keys:** `forward`, `back`, `left`, `right`, `moveleft`, `moveright`, `up`, `down`, `lookup`, `lookdown`, `attack`, `attack2`, `jump`, `use`, `speed`, `strafe`

**How it works:**
- When conflicting keys are pressed simultaneously, randomly chooses which input to clear
- Applies random delay (0 to `cl_rDrive_ms` milliseconds) before clearing
- Fully configurable key pairs for maximum flexibility

### cl_iDrive - Deterministic SOCD System

The `cl_iDrive` system provides traditional SOCD cleaning where the most recently pressed key takes priority.

**Configuration:**
```
cl_iDrive 1                     // Enable deterministic SOCD
```

**Note:** Only enable one SOCD system at a time (either `cl_iDrive` OR `cl_rDrive`, not both).

## Compiling

On Linux, `./build-linux.sh` produces an ezQuake binary in the top directory. 

For a more in-depth description of how to build on all platforms, have a look at 
[BUILD.md](BUILD.md).

## Nightly builds

Nightly builds can be found [here][nightly]

 [nQuake]: http://nquake.com/
 [webchat]: http://webchat.quakenet.org/?channels=#ezquake
 [IRC]: irc://irc.quakenet.org/#ezquake
 [forum]: http://www.quakeworld.nu/forum/8
 [qtv]: http://qtv.quakeworld.nu/
 [nightly]: https://builds.quakeworld.nu/ezquake/snapshots/
 [releases]: https://github.com/ezQuake/ezquake-source/releases
 [issues]: https://github.com/ezQuake/ezquake-source/issues
 [homepage]: https://ezquake.com
 [discord]: http://discord.quake.world/
