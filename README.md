# Cub-3d

<div align="center">

<img width="150" height="150" alt="cub3dm" src="https://github.com/user-attachments/assets/627a2b83-49db-42a5-a328-0e16e7a3a5fe" />
</div>

</br>

**Cub-3D** is a project completed as part of the 42 School curriculum.  
The goal is to recreate a simplified 3D game engine using **ray-casting**, inspired by the rendering techniques of early FPS games like *Wolfenstein 3D*.  
The program loads a `.cub` map, interprets textures, colors, and player orientation, and renders a real-time first-person view using the **DDA algorithm** for efficient wall collision detection.

> _"The objective of this project is to build a minimal 3D engine and understand the fundamental mathematics behind ray-casting, projections, and grid-based world navigation, while ensuring strict parsing, memory safety, and stable real-time rendering."_

<div align="center">
  
[![Co-author: DanielFonsecaa](https://img.shields.io/badge/Co--author-DanielFonsecaa-blue)](https://github.com/DanielFonsecaa)
[![Co-author: NukyYuki](https://img.shields.io/badge/Co--author-NukyYuki-pink)](https://github.com/NukyYuki)
</div>

## Features

- First-person 3D rendering using **ray-casting**.  
- **DDA (Digital Differential Analyzer)** algorithm used to detect wall collisions along each ray.  
- Wall textures vary depending on wall orientation (North, South, East, West).  
- Configurable floor and ceiling colors (RGB).  
- Smooth player movement (forward, backward, strafe) and view rotation.  
- Parsing, validation, and handling of `.cub` formatted map files.  
- Map support for walls, empty spaces, and player start/orientation.  

## Ray-Casting & DDA Overview

Cub-3d uses the classical technique of projecting rays from the player's position to compute what is visible on screen.

### Ray Setup
For each vertical stripe of the window:
- Compute the ray direction from the camera plane.
- Determine the step direction and distance to the next grid boundary.

### DDA (Digital Differential Analyzer)
The DDA algorithm marches along the ray grid-by-grid until it hits a wall:
- At each iteration, move to the next X or Y boundary depending on which is closer.  
- Stop when a cell containing `1` (wall) is found.  

This produces:
- Wall hit position  
- Correct perpendicular distance to avoid fisheye effect  
- Wall orientation for selecting the correct texture  

### Wall Projection
Based on the distance, the wall slice height is calculated and drawn with texture mapping.

### Summary
This combination of ray-casting + DDA allows efficient real-time 3D rendering using only integer grid navigation and simple math.

## Map Format (`.cub` file)

The `.cub` file describes the map, wall textures, floor/ceiling colors, and player start. Example structure:

```text
NO ./textures/north_texture.xpm
SO ./textures/south_texture.xpm
WE ./textures/west_texture.xpm
EA ./textures/east_texture.xpm
F 220,100,0     # Floor color (R,G,B)
C 225,30,0      # Ceiling color (R,G,B)

1111111111
1000000001
10000N0001
1000000001
1111111111
```
1 = wall

0 = empty space / floor

N / S / E / W = player start position + orientation

Walls must fully enclose the map (no open boundaries), and there should be no invalid characters.
Build & Run

## Clone the repository:

git clone https://github.com/DanielFonsecaa/Cub-3d.git
cd Cub-3d

Compile the project using the included Makefile:

    make

Run the executable with a .cub map file:

    ./cub3d path/to/your_map.cub

Controls
Key / Input	Action
W / S	Move forward / backward
A / D	Strafe left / right
← / →	Rotate view left / right
ESC / Window close	Exit the program

(Update this table if you have different key bindings in your implementation.)
Dependencies / Requirements

- C compiler (e.g. gcc)
- Graphics library: MiniLibX
- Standard C libraries (e.g. math library via -lm)

## Project Structure (suggested)

Cub-3d/

├── mandatory/        # Core code for minimal requirements

├── bonus/            # (Optional) Bonus / extra features

├── lib/              # Utility libraries (e.g. custom libft)

├── map/              # Example maps (.cub files)

├── textures/         # Wall textures (e.g. .xpm files)

├── Makefile  

└── README.md

## Enhancements

- Collision detection to prevent walking through walls.
- Minimap overlay for navigation.
- Mouse support for smoother view rotation.
- More map elements: doors, sprites, interactive objects.
- Additional map validation and error handling.

## Authors

- [DanielFonsecaa](https://github.com/DanielFonsecaa)
- [NukyYuki](https://github.com/NukyYuki)
