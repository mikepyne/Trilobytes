#include "FoodPellet.h"

#include "FeedDispenser.h"

FoodPellet::~FoodPellet()
{
    spawner_.PelletEaten();
}
