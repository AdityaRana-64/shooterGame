This document highlights the data structures used in the making of this game and the reason they were particularly chpsen.

1. Overview:
     This game uses basic structs and fixed-size arrays to store data. Also, dynamic memory alloc is used throughout for the sake of simplicity.

2. Player:
     The player state is managed by a number of variables like:
       int player_x;             : stores the horizontal position of the player
       int player_hp;            : stores the value of the current health level of the player
       int gun_mode;             : keeps a track of the gun type in use currently
       int ammo;                 : keeps a track of the ammo left for the player

    Using multplie variables to describe the state of the player made it easy to understand the code and to debug it.

3. Bullet:
    A Struct is used for describing a bullet here:
      typedef struct {
        int x;                   : stores the position of the bullet
        int y;                   : stores the position of the bullet
        int active;              : keeps a track of the status of the bullet - in use or not
        int type;                : stores info regarding the type of the bullet - laser or shotgun
      } Bullet;

    The bullets are further stored in the form of a fixed size array:
      Bullet bullets[50];
    This was done to avoid any memory leakage. Also, the active feature of the bullet struct allows us to reuse the slots for the bullets by turning one off and the other on. Using this also made collision detection much easier.

4. Alien:
    A Struct is used for describing an alien here:
      typedef struct {
        int x;                   : stores the x position of the alien
        int y;                   : stores the y position of the alien
        int active;              : keeps a track of whether the alien is alive or not
        int dir;                 : stores whether the alien moves left to right (1) or right to left (-1)

    The aliens, too, are further stored in the form of a fixed size array:
      Alien aliens[20];
    This was done as fixed limit negates the possibility of screen overflow due to aliens. Also, this way, the horizontal motion is easier to implement.

5. Bomb:
    A Struct is used for describing a bomb here:
      typedef struct {
        int x;                   : stores the x position of the bomb
        int y;                   : stores the y position of the bomb
        int active;              : keeps a track of whether the bomb is currently falling or not

    The bombs are further stored in the form of a fixed size array:
      Bomb bombs[20];
    This allows us to reuse the bomb slots

6. Boss:
    A struct is used for describing a boss alien here:
      typedef struct {
        int x;                   : stores the x position of the boss alien
        int y;                   : stores the y position of the boss alien
        int active;              : keeps a track of whether the boss alien is present on the screen or not
        int hp;                  : keeps a track of the current health value of the boss alien
        int max_hp;              : stores the maximum health value of the boss alien
        int dir;                 : stores whether the boss left to right (1) or right to left (-1)

    The boss alien is stored as a single struct only, as only one boss alien is present on the screen at a time:
      Boss boss;

7. Screen Buffer:
    It represents one full frame of the game
      char buffer[HEIGHT][WIDTH + 1];

    WIDTH + 1 is used to accomodate the '\0' null terminator while printing each row as a string.

8. Global Variables:
    long ticks;                  : this helps keep track of time, thereby keeping a track of when to spawn the aliens and the boss alien, along with the powerups
    int score;                   : keeps a track of the score throughout the game, thus it has been declared as a global variable
    int game_running;            : this checks whether to end the game loop or to keep it running
    int difficulty;              : stores the difficulty level chosen by the player, and since difficulty does not change throughout the game, it has been declared as a global variable

## Why have fixed size arrays been used throughout the code?

--> This has been done keeping in mind the following accessibilities:
    (i) Simplicity - it makes debugging the code easier (if required)
    (ii) No allocation failures - no possibilty of allocation failures, since our code does not ask for memory allocation while the game loop is running, unlike dynamic memory allocation, whihc may return NULL under certain circumtances
    (iii) Beginner friendly - no pointers and hence no unnecessary complications :)
    (iv) Performance - using fixed size arrays allows fast access to game elements, thereby decreasing lags and time taken by the game loop to run
