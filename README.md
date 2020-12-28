# EoBE - Eat or Be Eaten
A life simulator, where each critter has a neural network brain, a set of sensors and a set of effectors.

All entities within the simulation can be detected only via senses and interacted with only via effectors.

Critters eat food to survive, and can reproduce a-sexually or sexually.

The senses and abilities a critter has are entirely based upon their genome, and all behaviour based on their neural network brain.

Senses and effectors can have their own mini neural network, so that specific behaviour can be inherited along with them.

Requirements
-----
Developed with Qt 15.12.6 & MinGW 7.3.0

Should work with older Qt versions but a compiler supporting C++17 is necessary.

Build using qMake, so easiest way to build is to open project in QtCreator, select a compatible (C++17) toolchain and build through the QtCreator GUI.

TODO
-----
 - Tests
 - More GUI controlls
 - More effectors (mouthparts? body armor? spikes? sex organs? pheremone creators? poison creators? decoy creators? food creators? Egg layers?)
 - Genes that encode more than a single effector or sensor (Pair of antenna? pair of eyes? memory/flip-flop (loop output to input)?
 - Improve rendering (image based? decals? OpenGL?)
 - More descriptive brain inspector view
 - Genes inspector view
 - More graphs & configurable graphs
 - Matrix based coordinate system/collisions
 - More entities (pheremones? poisons? more food types?)
 - Add Descriptions for Senses and Effectors so that Inspector can label what the inputs and outputs represent
 - MAYBE Fixed number of "nodes" on a Swimmer, limiting the number of senses and effectors that can be attached to it?
 - tabs in left and right GUI to split up settings (add more settings per section) and add geneome inspector on right
 -  - Allow adding multiple swimmers at a time
 -  - clear all swimmers
 -  - Move feed dispenser
 -  - adjust feed dispenser pellet energy/frequency
 -  - Add feed dispenser
 -  - resize feed dispenser
 - Allow entities to be movable/draggable
 - add settings per graph on left of each graph (also make space on left of y-axis for label so it doesn't overlap numbers)
 - add new sense MoonPhase (Need a means of accessing Universe settings in tick for senses and mutation probability e.t.c.)
 - add new effector SpringTail that charges up energy and releases it as a burst of movement
 - add images for each sense and effector and body
 - add gene for image decals
 - add gene for egg size
 - allow laid eggs to be fertilised by passing swimmers (optional, let genetics decide)
 - allow swimmers to take damage and die, and cost energy to heal damage. dead swimmers to explode into edible meat chunks
 - brain inspector needs ability to adjust line culling (i.e. don't show connections below x strength...) etc
