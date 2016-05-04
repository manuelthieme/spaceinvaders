#include <stdlib.h>
#include "../inc/entity.h"
#include "../inc/io.h"

// Returns a new entity on the stack.
struct entity newEntity(unsigned type, unsigned char symbol, unsigned health)
{
    if (type > 3 || symbol > 127 || health > 7)
    {
        terminate();
    }

    struct entity newEntity;
    newEntity.type = type;
    newEntity.symbol = symbol;
    newEntity.health = health;
    newEntity.flag1 = newEntity.flag2 = newEntity.flag3 = newEntity.flag4 = 0;

    return newEntity;
}

// Converts two coordinates into a pos struct.
struct pos getPos(unsigned x, unsigned y)
{
    struct pos coords = { x, y };
    return coords;
}
