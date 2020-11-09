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
 - Genes that encode more than a single effector or sensor (Pair of antenna? pair of eyes? memory (loop output to input)?
 - Improve rendering (image based? decals? OpenGL?)
 - More descriptive brain inspector view
 - Genes inspector view
 - More graphs
 - Matrix based coordinate system/collisions
 - More entities (pheremones? poisons? more food types?)
 - FPS/TPS readout
 - Add Descriptions for Senses and Effectors so that Inspector can label what the inputs and outputs represent
 - Fixed number of "nodes" on a Swimmer, limiting the number of senses and effectors that can be attached to it?
 - Metabolic cost to owning sensors and effectors based on complexity (to discourage huge numbers of complex senses)
 - Slight metabolic cost for size of Genome (to discourage huge genomes)
