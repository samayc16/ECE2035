// Project includes
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "speech.h"

// Functions in this file
int get_action (GameInputs inputs);
int update_game (int action);
void draw_game (int init);
void init_main_map ();
int main ();
// Added Helper Functions
int go_up(int x, int y);
int go_down(int x, int y);
int go_right(int x, int y);
int go_left(int x, int y);
int check_object(int x, int y);
void init_gingerbread_map();

int omni_mode = 0;
int start_game = 1;
int stairs = -1;

GameInputs in;
int action;
int update;

const char *EntranceLine[3] =      {"Hello! Welcome tothe game! To your",
                                    "right is an NPC. Interact with the",
                                    "NPC to find out  your quest!      "};
                                    
const char *GingerMapLine[5] =     {"Hello! Welcome tothe lair!        ",
                                    "You must defeat  the gingerbread  ",
                                    "man in order to  retrieve the key.",
                                    "There are many   attacks to choose",
                                    ", but rememember,he hates milk!   "};
                                    
const char *CookieChose[4] =       {"Oh no! You chose the cookie spell!",
                                    "Cookies make the gingerbread man  "
                                    "stronger. see if you can find     "
                                    "another spell!                    "};
                                    
const char *NoKey[1] =             {"Get the key to   unlock the door! "};

const char *YesKey[2] =            {"You have found   the key!         ",
                                    "Go through the   door to finish!  ",};     
                                    
const char *MilkChose[4] =         {"Good job, you    chose the milk!  ",
                                    "Watch the monsterdisappear!       ",
                                    "Go back up to theNPC and you will ",
                                    "get the key!                      "};

const char *MonsterAliveLine[8] = {"Hello, welcome toyour quest! You  ",
                                    "must defeat the  gingerbread man &",
                                    "retrieve the key.You are able to  ",
                                    "use spells in hislair, but I must ",
                                    "warn you, he is  powerful. He may ",
                                    "be powerful, but he is very weak  ",
                                    "against milk.    Come back here   ", 
                                    "when you defeat  the monster!     "};

const char *NotFirstNPC[2] =       {"Follow the stairsat (9, 14) to get",
                                    "your key!                         "};                                    

const char *DeadNoKey[2] =         {"Thank you for    helping me by    ",
                                    "killing the      gingerbread man! "};
                                    
const char *DeadKey[2] =           {"Take this key to the locked door  ",
                                    "near coordinates (25, 5)!         "};


/**
 * The main game state. Must include Player locations and previous locations for
 * drawing to work properly. Other items can be added as needed.
 */
struct {
    int x,y;    // Current locations
    int px, py; // Previous locations
    int has_key;
    int defeatMonster;
    int stairs;
    int win;
    int talkNPC;
    // You can add other properties for the player here
} Player;

/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possbile return values are defined below.
 */
#define NO_ACTION 0
#define ACTION_BUTTON 1
#define MENU_BUTTON 2
#define GO_LEFT 3
#define GO_RIGHT 4
#define GO_UP 5
#define GO_DOWN 6
#define END_GAME 7
#define OMNI_MODE 8
int get_action(GameInputs inputs)
{
    if (!inputs.b2) omni_mode = !omni_mode;
    if (!inputs.b1) return ACTION_BUTTON;
    if (inputs.ax < -0.4) return GO_LEFT;
    if (inputs.ax > 0.4) return GO_RIGHT;
    if (inputs.ay < -0.4) return GO_DOWN;
    if (inputs.ay > 0.4) return GO_UP;
    if ((23 <= Player.x && Player.x <= 29) && (1 <= Player.y && Player.y <= 5)) return END_GAME;
    else return NO_ACTION;
}

/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the Player position accordingly.
 *
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the player has not moved.
 */
#define NO_RESULT 0
#define GAME_OVER 4
#define FULL_DRAW 2
int update_game(int action)
{

    // Save player previous location before updating
    Player.px = Player.x;
    Player.py = Player.y;

    // Do different things based on the each action.
    // You can define functions like "go_up()" that get called for each case.
    switch(action)
    {
        case GO_UP:
        if(go_up(Player.px, Player.py)) return FULL_DRAW;
        else break;
        case GO_LEFT:
        if(go_left(Player.px, Player.py)) return FULL_DRAW;
        else break;
        case GO_DOWN:
        if(go_down(Player.px, Player.py)) return FULL_DRAW;
        else break;
        case GO_RIGHT:
        if(go_right(Player.px, Player.py)) return FULL_DRAW;
        else break;
        case ACTION_BUTTON:
        if(check_object(Player.px, Player.py)) return FULL_DRAW;
        else break;
        case MENU_BUTTON:
        break;
        case END_GAME:
        Player.win = 1;
        return GAME_OVER;
        default:
        break;
    }
    return NO_RESULT;
}

int go_up(int x, int y) {
    bool walk = get_north(x, y)->walkable;
    if (walk || omni_mode) {
       --Player.y;
       return 1;
    } else return 0;
}
int go_down(int x, int y) {
    bool walk = get_south(x, y)->walkable;
    if (walk || omni_mode) {
        ++Player.y;
        return 1;
    } else return 0;
}
int go_left(int x, int y) {
    bool walk = get_west(x, y)->walkable;
    if (walk || omni_mode) {
        --Player.x;
        return 1;
    } else return 0;
}
int go_right(int x, int y) {
    bool walk = get_east(x, y)->walkable;
    if(walk || omni_mode) {
        ++Player.x;
        return 1;
    } else return 0;
}
int check_object(int x, int y)
{
    int northItem = get_north(x, y)->type;
    int southItem = get_south(x, y)->type;
    int eastItem = get_east(x, y)->type;
    int westItem = get_west(x, y)->type;
    if (NPC == northItem  || NPC == southItem || NPC == eastItem || NPC == westItem)
    {
        if (Player.defeatMonster == 0 && Player.talkNPC == 0) 
        {
            long_speech(MonsterAliveLine, 8);
            Player.talkNPC = 1;
        }
        if (Player.defeatMonster == 0 && Player.talkNPC == 1) long_speech(NotFirstNPC, 2);
        if (Player.defeatMonster == 1 && Player.has_key == 0) 
        {
            long_speech(DeadNoKey, 2);
            Player.has_key = 1;
        }
        if (Player.defeatMonster == 1 && Player.has_key == 1) long_speech(DeadKey, 2);
        return 1;
    }
    if (COOKIE == northItem  || COOKIE == southItem || COOKIE == eastItem || COOKIE == westItem)
    {
        long_speech(CookieChose, 4);
        map_erase(14, 14);
        return 1;
    }
    if (MILK == northItem  || MILK == southItem || MILK == eastItem || MILK == westItem)
    {
        long_speech(MilkChose, 4);
        map_erase(16, 15);
        map_erase(12, 12);
        Player.defeatMonster = 1;
        return 1;
    }
    if (STAIRS == get_here(x,y)->type)
    {
        
        stairs++;
        if (stairs == 1)                      stairs++;
        if (stairs != 1 & stairs % 2 == 1)    set_active_map(0);
        if (stairs != 2 & stairs % 2 == 0)    set_active_map(1);
        return 1;
    }
    if (DOOR == get_north(x,y)->type)
    {
        if (Player.has_key == 0) long_speech(NoKey, 1);
        else
        {
            long_speech(YesKey, 2);
            for (int i = 23; i < 30; i++)
            {
                map_erase(i, 5);
            }  
        }
        return 1;
    }
    else return 0;
}
/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status
 * bars. Unless init is nonzero, this function will optimize drawing by only
 * drawing tiles that have changed from the previous frame.
 */
void draw_game(int init)
{
    if (Player.win == 1)
    {
        uLCD.filled_rectangle(0, 0, 127, 127, 0x000000);
        uLCD.locate(2,4);
        uLCD.color(0xFFFFFF);
        uLCD.printf("YOU WIN!");
        wait(1000000000000000);
    }
    // Draw game border first
    if(init) draw_border();

    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) // Iterate over columns of tiles
    {
        for (int j = -4; j <= 4; j++) // Iterate over one column of tiles
        {
            // Here, we have a given (i,j)

            // Compute the current map (x,y) of this tile
            int x = i + Player.x;
            int y = j + Player.y;

            // Compute the previous map (px, py) of this tile
            int px = i + Player.px;
            int py = j + Player.py;

            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;

            // Figure out what to draw
            DrawFunc draw = NULL;
            if (init && i == 0 && j == 0) // Only draw the player on init
            {
                draw_player(u, v, Player.has_key);
                continue;
            }
            else if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) // Current (i,j) in the map
            {
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (init || curr_item != prev_item) // Only draw if they're different
                {
                    if (curr_item) // There's something here! Draw it
                    {
                        draw = curr_item->draw;
                    }
                    else // There used to be something, but now there isn't
                    {
                        draw = draw_nothing;
                    }
                }
            }
            else if (init) // If doing a full draw, but we're out of bounds, draw the walls.
            {
                draw = draw_wall;
            }

            // Actually draw the tile
            if (draw) draw(u, v);
        }
    }
    if (stairs == 0) long_speech(GingerMapLine, 5);
    
    // Draw status bars
    draw_upper_status(Player.x, Player.y);
    draw_lower_status(Player.has_key, Player.defeatMonster);
    
}


/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion. Note: using the similar
 * procedure you can init the secondary map(s).
 */
void init_main_map()
{
    Map *map = set_active_map(0);
    add_npc(8, 5);
    add_stairs(9, 14);
    // "Random" plants
    for(int i = map_width() + 3; i < map_area(); i += 39)
    {
        add_plant(i % map_width(), i / map_width());
    }
    pc.printf("plants\r\n");

    pc.printf("Adding walls!\r\n");
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    pc.printf("Walls done!\r\n");

    // Make walled room that is locked until a key is there
    add_wall(22,              0,              VERTICAL,   6);
    add_wall(30,              0,              VERTICAL,   6);

    for (int i = 23; i < 30; i++)
    {
        add_door(i, 5);
    }

    pc.printf("Door walls done!\r\n");

    print_map();

}
void init_gingerbread_map()
{
    Map *map = set_active_map(1);
    add_stairs(9, 14);
    add_gbMan(16, 15);
    add_milk(12, 12);
    add_cookie(14, 14);
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height() - 1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width() - 1,0,              VERTICAL,   map_height());
    print_map();
}

/**
 * Program entry point! This is where it all begins.
 * This function orchestrates all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
int main()
{
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
    // Initialize the maps
    maps_init();
    init_main_map();
    init_gingerbread_map();

    // Initialize game state
    set_active_map(0);
    Player.x = Player.y = 5;
    Player.has_key = 0;
    Player.defeatMonster = 0;
    // Initial drawing
    draw_game(true);
    long_speech(EntranceLine, 3);

    // Main game loop
    while(1)
    {
        // Timer to measure game update speed
        Timer t; t.start();

        // Actually do the game update:
        // 1. Read inputs
        in = read_inputs();
        // 2. Determine action (get_action)
        action = get_action(in);
        // 3. Update game (update_game)
        update = update_game(action);
        // 3b. Check for game over
        // 4. Draw frame (draw_game)
        draw_game(update);
        // 5. Frame delay
        t.stop();
        int dt = t.read_ms();
        if (dt < 100) wait_ms(100 - dt);
    }
}