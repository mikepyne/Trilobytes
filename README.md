# EoBE - Eat or Be Eaten
A life simulator, where each critter has a neural network brain, a set of sensors and a set of effectors.

All entities within the simulation can be detected only via senses and interacted with only via effectors.

Critters eat food to survive, and can reproduce a-sexually or sexually.

The senses and abilities a critter has are entirely based upon their genome, and all behaviour based on their neural network brain.

Senses and effectors can have their own mini neural network, so that specific behaviour can be inherited along with them.

Requirements
-----
Developed with Qt 15.12.6 & MinGW 7.3.0, using C++17 features

Should work with older Qt versions but a compiler supporting C++17 is necessary.

TODO
-----
 - Tests
 - Controlls GUI & more options
 - More senses (own status? fixed value? occilating value? low pass filtered random value?)
 - More effectors (mouthparts? body armor? spikes? sex organs? pheremone creators? poison creators? decoy creators? food creators?)
 - Genes that encode more than a single effector or sensor (Pair of antenna? pair of eyes? memory (loop output to input)?
 - Improve rendering (image based? decals?)
 - Ability to mutate number of hidden layers
 - Ability to mutate number of inputs/outputs of senses/effectors
 - More interactive brain inspector view
 - Genes inspector view
 - More/better graphs
 - Matrix based coordinate system/collisions
 - More entities (pheremones? poisons? more food types?)
 - FPS/TPS readout
