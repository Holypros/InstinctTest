# InstinctTest
 
This is a simple game made entirely using C++ and unreal (except for the UI being made using Blueprints)

Map of the game is a grid of cells, the size of the grid is dynamic and being read from a file

The player is controlling a sphere ball. It's being moved using physics forces for it's movement

A number of rotating turrets will be distributed through the map (grid). Each turret occupies one cell from within the grid. The number of turrets are also read from a file

Each turret has a limited coverage area as shown in the video. If the sphere gets in the turret’s coverage area, the turret will start firing at the sphere. 

The player objective is to collect the coins to get score points without being detected by the turrets.

The game ends when the sphere health reaches zero.


You can watch the Gameplay Video here: https://drive.google.com/file/d/1KTc-huBZFKWsh-wq6E5h8te1jvakiAQZ/view?usp=share_link
