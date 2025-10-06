---
description: Repository Information Overview
alwaysApply: true
---

# ezQuake SoftAim Information

## Summary
ezQuake SoftAim is a modified version of the ezQuake QuakeWorld client that adds aim assistance features. It's designed to enhance gameplay with intelligent target selection and smooth tracking capabilities, along with advanced input handling systems like randomized and deterministic SOCD (Simultaneous Opposite Cardinal Directions) cleaning.

## Structure
- **docs/**: Website files and documentation for GitHub Pages
- **src/**: Source code directory (appears to be minimal in this fork)
- **vcpkg/**: Package manager for C++ libraries

## Language & Runtime
**Language**: C/C++
**Platform Support**: Windows, Linux, MacOSX, FreeBSD (via SDL2)
**Build System**: Not explicitly defined in this repository

## Features
**Main Features**:
- SoftAim System: Advanced aim assistance with intelligent target selection
- cl_rDrive: Randomized SOCD cleaning with configurable timing and key pairs
- cl_iDrive: Deterministic SOCD cleaning where most recent key takes priority
- Modern graphics with QuakeTV support
- Rich menus and multiview support
- Built-in server browser and MP3 player control
- Recorded games browser
- Customizable HUD and graphics elements

## SoftAim Configuration
```
cl_softaim 1                    // Enable/disable (0 or 1)
cl_softaim_strength 1.0         // Assistance strength (0.5-2.0)
cl_softaim_maxdist 512          // Maximum targeting distance
cl_softaim_fov 90               // Field of view for targeting
cl_softaim_smooth 0.3           // Smoothing factor (0.01-1.0)
cl_softaim_autoswitch 1         // Auto weapon switching (0 or 1)
cl_softaim_lg_range 350         // Lightning Gun optimal range
```

## Input Systems
**cl_rDrive (Randomized SOCD)**:
```
cl_rDrive 1                                    // Enable randomized SOCD
cl_rDrive_ms 5                                 // Max delay in milliseconds
cl_rDrive_pairs "forward,back;left,right"      // Configure key pairs
```

**cl_iDrive (Deterministic SOCD)**:
```
cl_iDrive 1    // Enable deterministic SOCD
```

## Installation
**Minimal Installation**:
1. Create a new directory
2. Extract the ezQuake executable into this directory
3. Create a subdirectory called `id1`
4. Copy `pak0.pak` and `pak1.pak` from original Quake into the `id1` subdirectory

**Existing Installation**:
- Extract the ezQuake executable into an existing Quake directory
- Remove `opengl32.dll` if you encounter `glide2x.dll` missing errors

## Website
**Structure**:
- GitHub Pages deployment via `/docs` folder
- Responsive design with feature documentation
- Download counter badges via GitHub API
- Direct download links to latest releases

**Customization**:
- CSS styling in `style.css`
- Background color set to RGB(0,0,0) (solid black)
- Color scheme using CSS variables
```