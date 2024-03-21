# Robo Runner Game Roadmap

The objective of the game will be to set a high score based on number of enemies defeated and distance flown. The game will have a user visible and modifiable seed for randomly generating the game map, enemy spawns, and power ups. This makes replaying games possible but none will be hard coded.

## Primary Game Elements

### Player Entity
The player will control a ship automatically flying from the left to the right represented by a scrolling background. The player will be able to fully move the ship in two axis, but may be limited to only the left half of the screen.

### Weapons and Power Ups
The player will start with a single shot non-auto laser with a cool down meter. Weapon power ups will increase cooling capacity, add auto fire, multiply shots fires, change spread, and increase damage.

The player will have a permanent health pool that can never be restored, but will have a small number of "shields" that can prevent them from taking permanent damage for one hit. The maximum number of carried shields may be increased as the player progresses.

Powerups will be dropped from defeated enemies.

### Enemies
Enemies will have three base sprite designs for light, medium, and heavy opponents. Lighter enemies will move faster from the right to the left and get slower as they increase in weight class. The maximum spawn limit for enemies will be higher for lighter classes. Flight patterns will be based on simple formulas ([example](https://en.wikipedia.org/wiki/B%C3%A9zier_curve#Linear_B.C3.A9zier_curves)) that take only a `t` value from 0-100.

Once they reach 100 they should be off screen on the left, the player will loose points equivalent to the enemies health if this happens.

Enemy weapon types will be fixed, but upon reaching specific point values the "level" of the enemies will increase multiplying their health and damage output. Different class enemies will increase in level at different point values.

Enemy levels will be indicated by manipulating their sprites to change accent colors.

### Scenery

The main background will be space with stars.

At the top and bottom of the screen, tiles representing a floor and ceiling will be able to encroach on the area the player can move. Touching one of these will damage the player and bounce them away from the direction they hit. There will be a minimum required distance from the floor and ceiling that will decrease as the player score increases. This will add difficulty. The floor and ceiling will be able to randomly move up and down but be slightly more likely to move towards the center of the screen.

A separate layer of decorative background objects like galaxies, space debris, or dust may be added for visual distinction.

### Boss Enemy
A single boss enemy will appear after the player has scored enough points.

The boss will have different forms each time it is encountered to add to its threat as the player progresses:  

1. A single sprite and spread shot attack
2. Adds a "wing" that can shoot a straight laser beam when not firing spread shot
3. Summons light class enemies with heavy flight paths between attacks
4. Adds another "wing" that can shoot a straight laser beam, both fire at same time.
5. Two attacks at once with random rolls
6. All attacks at once with random rolls


Defeating the boss will instantly push the floor and ceiling away from the center giving the feeling of moving on to a new space. Scenery tiles will have their accent colors adjusted. The decorative background will also change if it is available.