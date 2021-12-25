#include "map.h"

#include "globals.h"
#include "graphics.h"

/**
 * The Map structure. This holds a HashTable for all the MapItems, along with
 * values for the width and height of the Map.
 * In this file you need to define how the map will be structured. IE how will
 * you put values into the map, pull them from the map. Remember a "Map" variable
 * is a hashtable plus two ints (see below) 
 * You will have more than one map variable, one will be the main map with it's own hashtable.
 * Then you'll have a second map with another hashtable
 * You should store objects into the hashtable with different properties (spells
 * etc)
 */
struct Map
{
    HashTable *items;
    int w, h;
};

unsigned map_hash(unsigned key);

/**
 * Storage area for the maps.
 * This is a global variable, but can only be access from this file because it
 * is static.
 */
static Map *map[2];
static int active_map;

/**
 * The first step in HashTable access for the map is turning the two-dimensional
 * key information (x, y) into a one-dimensional unsigned integer.
 * This function should uniquely map (x,y) onto the space of unsigned integers.
 */
static unsigned XY_KEY(int X, int Y)
{
    return ((X + Y) * (X + Y + 1)) / 2 + Y;
}

/**
 * This is the hash function actually passed into createHashTable. It takes an
 * unsigned key (the output of XY_KEY) and turns it into a hash value (some
 * small non-negative integer).
 */
unsigned map_hash(unsigned key)
{
    return key % 50;
}

void maps_init()
{
    Map *newMap = (Map *)malloc(sizeof(Map));
    newMap->w = 50;
    newMap->h = 50;
    newMap->items = createHashTable(map_hash, 50);
    
    Map *newMap1 = (Map *)malloc(sizeof(Map));
    newMap1->w = 50;
    newMap1->h = 50;
    newMap1->items = createHashTable(map_hash, 50);
    
    map[0] = newMap;
    map[1] = newMap1;
}

Map *get_active_map()
{
    return map[active_map];
}

Map *set_active_map(int m)
{
    active_map = m;
    return map[active_map];
}


void print_map()
{
    // As you add more types, you'll need to add more items to this array.
    char lookup[] = {'W', 'P', 'N', 'G', 'S', 'M', 'C', 'D'};
    for (int y = 0; y < map_height(); y++)
    {
        for (int x = 0; x < map_width(); x++)
        {
            MapItem *item = get_here(x, y);
            if (item)
                pc.printf("%c", lookup[item->type]);
            else
                pc.printf(" ");
        }
        pc.printf("\r\n");
    }
}

int map_width()
{
    return map[active_map]->w;
}

int map_height()
{
    return map[active_map]->h;
}

int map_area()
{
    return (map[active_map]->h) * (map[active_map]->w);
}

MapItem* get_north(int x, int y)
{
    return (MapItem*)getItem(map[active_map]->items, XY_KEY(x, y - 1));;
}

MapItem *get_south(int x, int y)
{
    return (MapItem*)getItem(map[active_map]->items, XY_KEY(x, y + 1));
}

MapItem *get_east(int x, int y)
{
    return (MapItem*)getItem(map[active_map]->items, XY_KEY(x + 1, y));
}

MapItem *get_west(int x, int y)
{
    return (MapItem*)getItem(map[active_map]->items, XY_KEY(x - 1, y));
}

MapItem *get_here(int x, int y)
{
    return (MapItem*)getItem(map[active_map]->items, XY_KEY(x, y));
}

void map_erase(int x, int y)
{
    deleteItem(map[active_map]->items, XY_KEY(x, y));
}

void add_wall(int x, int y, int dir, int len)
{
    for (int i = 0; i < len; i++)
    {
        MapItem *w1 = (MapItem *)malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall;
        w1->walkable = false;
        w1->data = NULL;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x + i, y) : XY_KEY(x, y + i);
        void *val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}

void add_plant(int x, int y)
{
    MapItem *w1 = (MapItem *)malloc(sizeof(MapItem));
    w1->type = PLANT;
    w1->draw = draw_plant;
    w1->walkable = true;
    w1->data = NULL;
    void *val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_npc(int x, int y)
{
    MapItem *npc1 = (MapItem *)malloc(sizeof(MapItem));
    npc1->type = NPC;
    npc1->draw = draw_npc;
    npc1->walkable = false;
    npc1->data = false;
    void *val = insertItem(get_active_map()->items, XY_KEY(x, y), npc1);
    if (val) free(val);
}

void add_gbMan(int x, int y)
{
    MapItem *gbMan = (MapItem *)malloc(sizeof(MapItem));
    gbMan->type = GBMAN;
    gbMan->draw = draw_gbMan;
    gbMan->walkable = false;
    gbMan->data = false;
    void *val = insertItem(get_active_map()->items, XY_KEY(x, y), gbMan);
    if (val) free(val);
}
void add_stairs(int x, int y)
{
    MapItem *stairs = (MapItem *)malloc(sizeof(MapItem));
    stairs->type = STAIRS;
    stairs->draw = draw_stairs;
    stairs->walkable = true;
    stairs->data = (void *) active_map;
    void *val = insertItem(get_active_map()->items, XY_KEY(x, y), stairs);
    if (val) free(val);
}

void add_milk(int x, int y)
{
    MapItem *milk = (MapItem *)malloc(sizeof(MapItem));
    milk->type = MILK;
    milk->draw = draw_milk;
    milk->walkable = false;
    milk->data = NULL;   
    void *val = insertItem(get_active_map()->items, XY_KEY(x, y), milk);
    if (val)
        free(val);
}

void add_cookie(int x, int y)
{
    MapItem *cookie = (MapItem *)malloc(sizeof(MapItem));
    cookie->type = COOKIE;
    cookie->draw = draw_cookie;
    cookie->walkable = false;
    cookie->data = NULL;   
    void *val = insertItem(get_active_map()->items, XY_KEY(x, y), cookie);
    if (val) free(val);   
}

void add_door(int x, int y)
{
    MapItem *door = (MapItem *)malloc(sizeof(MapItem));
    door->type = DOOR;
    door->draw = draw_door;
    door->walkable = false;
    door->data = NULL;   
    void *val = insertItem(get_active_map()->items, XY_KEY(x, y), door);
    if (val) free(val);   
}