# Trilobytes - Evolution Simulator
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
 - Genes inspector view
 - More graphs & configurable graphs
 - Matrix based coordinate system/collisions
 - More entities (pheremones? poisons? more food types?)
 - MAYBE Fixed number of "nodes" on a Swimmer, limiting the number of senses and effectors that can be attached to it?
 - tabs in left and right GUI to split up settings (add more settings per section) and add geneome inspector on right
 -  - Allow adding multiple swimmers at a time
 -  - Move feed dispenser
 -  - adjust feed dispenser pellet energy/frequency
 -  - Add feed dispenser
 -  - resize feed dispenser
 - Allow entities to be movable/draggable
 - add images for each sense and effector and body
 - add gene for image decals
 - add gene for egg size
 - allow laid eggs to be fertilised by passing swimmers (optional, let genetics decide)
 - brain inspector needs ability to adjust line culling (i.e. don't show connections below x strength...) etc
 - Would be very insightful to record which mutations have been accumulated over the generations (could be useful to refine mutations and detect anything too lethal to be useful)
 - MAYBE "Aerodynamics" aspect of entities to control how they interact with friction, and a seperate momentum transform from location transform, so that facing & moving direction aren't always the same
 - - Allows for effectors that improve cornering etc
 - - Allows for certain effectors to change the gliding ability
 - - Allows for certain effectors to transition the swimmer to a walker

Next Steps
----------
Entity needs a pure virtual GetType() which returns Animal, Vegetable, or Mineral
Entity needs a pure virtual GetAlive() (Animal, Egg & MeatChunk are all animal, but MeatChunks are not alive)
Entities should not be removed based on Energy content, but instead a seperate existence flag

QuadTree needs to be more intelligent about the area it covers (like downscaling when root node is too big, and expanding in the direction of the entitiy that triggered expansion)

Need to create a logo / an Icon

Upgrade to Qt 6, with Qt 3D (for realtime 2D graphical aspect)

Add 3 mouth types,
 - DONE FilterMouth (wide oval touching body somewhere, small entities only, adds drag to swimmer but 100% efficient),
 - Proboscis (needle point protruding from body, living target only (i.e. swimmers and eggs), reduced feeding rate but 100% efficient)
 - ToothedMouth (circle touching body somewhere, pellets @ ~50% & eggs/meat chunks at ~75%, can bite swimmers for damage) perhaps it can only eat a fixed ammount per tick, with smaller bites being more energy efficient but doing less damage?

Add another tail type
 - DONE SpringTail

Add some effectors
 - Vortex generator, moves entities near to swimmer in a vortex pattern
 - DONE Spike, damages entities that collide with it

Add adjustable friction cooeficient (speed *= friction) so friction is between 0.0 and 1.0

Ability to add/remove spikes

DefaultGenome could do with some variation in terms of content OR RandomGenome could do with some tighter restrictions on what it contains?
perhaps split genes into effectors, sensors, cosmetic etc so a random genome can contain a fixed number of each, but still random?
maybe noew category for swimmer genome that is more varied instead????
