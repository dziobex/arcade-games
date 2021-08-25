## Snakey
![](https://i.imgur.com/7GPsRL9.gif)</br></br>
Snakey indeed, because I mispelled word and left it.
Simple, classic Snake clone, written in c++ using SFML library.

## Pong
![](https://i.imgur.com/ZbBH9cU.gif)</br></br>
Classic Pong Clone, written in C++ with SFML library.</br>
I used an algorithm that takes 2 positions on a regular basis and after detecting a collision, sets the next direction.

## Conway's Game of Life (CGOL)
![](https://i.imgur.com/7JKb7RP.gif)</br></br>
Simple simulator of CGOL, devised by John Horton Conway in 1970. You can observe how different configurations of cells evolve and how they detroy themselves (: ]).</br>
I did it regaring to the 4 basics rules ([source](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)):
1. Any live cell with fewer than two live neighbours dies, as if by underpopulation.
2. Any live cell with two or three live neighbours lives on to the next generation.
3. Any live cell with more than three live neighbours dies, as if by overpopulation.
4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.

## Tetris
![](https://i.imgur.com/RfkX0jw.gif)</br></br>
Classic Tetris Clone. Original Tetris was created by Russian software engineer Alexey Pajitnov in 1984.</br>
I used several algorithms: all blocks are in a 2D bool array (board) and tetrominoes rotations depends on the small, single arrays rotation. Tetrominoes structures, resources have been placed in the files.

## Breakout
![](https://i.imgur.com/YeN3hsb.gif)</br></br>
From Wikipedia: Breakout (also known as Arkanoid) is an arcade game developed and published by Atari Inc.</br>
I used almost the same technique in bouncing ball (with slight changes in the collision detection) as with Pong ([look above](https://github.com/dziobex/arcade-games#pong)).

## Doge Invaders
[It looks like that.](https://i.imgur.com/Apkyk0f.gifv)</br></br>
Game based on the classic [Space Invaders](https://en.wikipedia.org/wiki/Space_Invaders).</br>
Nothing special - a simple algorithm for collision detection with bullets and a bunch of silly assets.
