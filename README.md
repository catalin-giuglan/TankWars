# TankWars - 2D Tank Battle Game

## Description

**TankWars** is a 2D tank battle game implemented in C++ using OpenGL. The game involves two players who control tanks positioned on a procedurally generated terrain and battle each other.

## Gameplay and Controls

### Player 1 Tank

- **Movement:** `A/D`
- **Adjust Cannon Angle:** `W/S`
- **Fire Projectile:** `SPACE`
- **Reset Health:** `R`

### Player 2 Tank

- **Movement:** `Left/Right Arrows`
- **Adjust Cannon Angle:** `Up/Down Arrows`
- **Fire Projectile:** `ENTER`
- **Reset Health:** `Right CTRL`

## Code Architecture

- **lab3.cpp and lab3.h:** Main game implementation, including terrain, projectile logic, and collisions.
- **lab3_vis2D.cpp and lab3_vis2D.h:** Functions for 2D rendering and viewport.
- **object2D.h:** Functions for generating graphical objects (tanks, projectiles, terrain, etc.).

## Implemented Features

- ✅ Terrain generation with sinusoidal functions for a dynamic environment
- ✅ Gravitational simulation for projectiles, affecting their trajectory
- ✅ Collisions between projectiles and tanks, decreasing health
- ✅ Terrain modification on projectile impact, creating craters
- ✅ Health bar for each tank, visible during gameplay
- ✅ Simultaneous control for two players, offering a competitive experience
- ✅ Predictable projectile trajectory for strategic gameplay
- ✅ Ability to reset health for each player
