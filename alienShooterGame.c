//Author: Aditya Rana (MFSDSAI, 250150003)
//Project: Alien Shooter Game


//Made as a project to understand structs
//This game runs on terminal using ASCII characters only
//Controls: Arrow keys to move left/right; Spacebar to shoot; R to switch weapons; L to reload; Q to quit early


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>

//Game configurations
#define WIDTH 100
#define HEIGHT 30 

//Player configurations
#define PLAYER_HP_MAX 10
#define MAX_AMMO 30

//ASCII art used for players and aliens
const char* PLAYER_SPRITE[] = { "  ^  ", " /|\\ ", "/_|_\\" }; 
const char* ALIEN_SPRITE  = "<[o]>"; 
const char* BOSS_SPRITE[] = {
    "  /--!@!@!--\\  ",
    " =[  oooo  ]= ",
    "  \\__/^\\__/  "
};

//Functions to control terminal cursor and colour
void gotoxy(int x, int y) { printf("\033[%d;%dH", y, x); }                //Moves the cursor to (x,y) position
void hide_cursor() { printf("\033[?25l"); }
void show_cursor() { printf("\033[?25h"); }
void clear_screen() { printf("\033[2J\033[H"); }
void set_color_red() { printf("\033[1;31m"); }
void set_color_cyan() { printf("\033[1;36m"); }
void set_color_reset() { printf("\033[0m"); }

//Draws a sprite to the buffer at specified coordinates
//Makes sure drawing does not go outside the screen boundaries
void draw_to_buffer(char buffer[HEIGHT][WIDTH+1], int x, int y, const char* sprite) {
    if (y < 0 || y >= HEIGHT) return; 
    int len = strlen(sprite);
    for(int i=0; i<len; i++) {
        int draw_x = x + i;
        if(draw_x >= 0 && draw_x < WIDTH) {
            buffer[y][draw_x] = sprite[i];
        }
    }
}

void flash_arena() {
    printf("\033[41m"); 
    for(int y=1; y<=HEIGHT; y++) {
        gotoxy(1, y);
        for(int x=0; x<WIDTH; x++) printf(" ");
    }
    gotoxy(WIDTH/2 - 5, HEIGHT/2);
    printf("\033[1;37m!!! CRITICAL !!!"); 
    fflush(stdout);
    usleep(50000); 
    printf("\033[0m"); 
}

//Checks if a key is pressed without blocking program execution, needed for real time movement
int kbhit(void) {
    struct termios oldt, newt;
    int ch, oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if(ch != EOF) { ungetc(ch, stdin); return 1; }
    return 0;
}

//Game object structs
//Using structs to store position and state of the game objects
//Bullet fired by the player
typedef struct {
    int x, y;
    int active;                                                           //1 if bullet is currently on screen, 0 else
    int type;                                                             //0 = laser, 1 = shotgun
} Bullet;

//Bomb dropped by aliens or boss
typedef struct {
    int x, y;
    int active;
} Bomb;

//Normal alien enemy
typedef struct {
    int x, y;
    int active;
    int dir;                                                              //Direction of movement
} Alien;

//Boss alien with more health
typedef struct {
    int x, y;
    int active;
    int hp;
    int max_hp;
    int dir;
} Boss;

//Game starts here
int main() {
    srand(time(NULL));

    int player_x = WIDTH / 2;
    int player_hp = PLAYER_HP_MAX;
    int score = 0;
    int game_running = 1;
    long ticks = 0;
    int gun_mode = 0; 
    int ammo = MAX_AMMO;
    
    int bosses_killed = 0;
    long boss_spawn_timer = 300; 

    Bullet bullets[50] = {0};
    Alien aliens[20] = {0};
    Bomb bombs[20] = {0};
    Boss boss = {0}; 

    clear_screen();
    
    //Initial menu to choose difficulty level; difficulty affects speed and spawn rate
    printf("\n\tALIEN SHOOTER GAME\n\n");
    printf("\t1. Easy\n\t2. Medium\n\t3. Difficult\n\n\tChoice: ");
    int difficulty = 1;
    char c; scanf(" %c", &c);
    if(c=='2') difficulty=2; if(c=='3') difficulty=3;

    clear_screen(); 
    hide_cursor();                                                        //Hiding cursor for cleaner UI
    setvbuf(stdout, NULL, _IOFBF, 4096);

    //This main loop runs until the player dies or quits
    while (game_running) {
        int move_speed = 9 - difficulty;
        if (move_speed < 2) move_speed = 2; 

        char buffer[HEIGHT][WIDTH + 1];
        for(int y=0; y<HEIGHT; y++) {
            memset(buffer[y], ' ', WIDTH);
            buffer[y][WIDTH] = '\0';
        }

        if (player_x < 2) player_x = 2;
        if (player_x > WIDTH - 3) player_x = WIDTH - 3;

        //Drawing player at the bottom of the screen
        for(int r=0; r<3; r++) {
            draw_to_buffer(buffer, player_x - 2, HEIGHT - 4 + r, PLAYER_SPRITE[r]);
        }
        
        //Boss appears after some time and has higher health, and drops bombs
        if (!boss.active && ticks > boss_spawn_timer) {
            boss.active = 1;
            boss.hp = 50 + (bosses_killed * 20); 
            boss.max_hp = boss.hp;
            boss.x = WIDTH / 2;
            boss.y = 2;
            boss.dir = 1;
        }

        if (boss.active) {
            if (ticks % (move_speed + 2) == 0) {
                boss.x += boss.dir;
                if (boss.x > WIDTH - 8) boss.dir = -1;
                if (boss.x < 8) boss.dir = 1;
            }
            for(int r=0; r<3; r++) {
                draw_to_buffer(buffer, boss.x - 7, boss.y + r, BOSS_SPRITE[r]);
            }
            if (ticks % 60 == 0) {
                for(int k=0; k<20; k++) {
                    if(!bombs[k].active) {
                        bombs[k].active = 1; bombs[k].x = boss.x; bombs[k].y = boss.y + 3;
                        break;
                    }
                }
            }
            for (int i=0; i<50; i++) {
                if (bullets[i].active && abs(bullets[i].x - boss.x) < 7 && abs(bullets[i].y - boss.y) < 3) {
                    bullets[i].active = 0;
                    boss.hp--;
                    if (boss.hp <= 0) {
                        boss.active = 0;
                        score += 1000;
                        bosses_killed++;
                        player_hp += 3;
                        if(player_hp > PLAYER_HP_MAX) player_hp = PLAYER_HP_MAX;
                        boss_spawn_timer = ticks + 400; 
                    }
                }
            }
        }

        if (ticks % (50 - (difficulty * 2)) == 0) { 
            for(int i=0; i<20; i++) {
                if(!aliens[i].active) {
                    aliens[i].active = 1;
                    aliens[i].y = (rand() % (HEIGHT/3));
                    if(rand()%2) { aliens[i].x = 0; aliens[i].dir = 1; }
                    else { aliens[i].x = WIDTH-1; aliens[i].dir = -1; }
                    break;
                }
            }
        }

        //Spawning and moving aliens, and checking for bullet hit
        for(int i=0; i<20; i++) {
            if(aliens[i].active) {
                if (ticks % move_speed == 0) aliens[i].x += aliens[i].dir;
                
                draw_to_buffer(buffer, aliens[i].x, aliens[i].y, ALIEN_SPRITE);

                if (rand() % 300 == 0) { 
                     for(int k=0; k<20; k++) {
                        if(!bombs[k].active) {
                            bombs[k].active = 1; bombs[k].x = aliens[i].x + 2; bombs[k].y = aliens[i].y + 1;
                            break;
                        }
                    }
                }
                
                if(aliens[i].x < -5 || aliens[i].x > WIDTH + 5) aliens[i].active = 0;

                for(int b=0; b<50; b++) {
                    if(bullets[b].active && abs(bullets[b].x - (aliens[i].x+2)) < 3 && bullets[b].y == aliens[i].y) {
                        bullets[b].active = 0;
                        aliens[i].active = 0;
                        score += 10;
                    }
                }
            }
        }

        //Handling bombs dropped by aliens and boss
        for(int k=0; k<20; k++) {
            if(bombs[k].active) {
                if (ticks % (move_speed - 1) == 0) bombs[k].y++;
                if (bombs[k].y >= HEIGHT - 1) {
                    bombs[k].active = 0; 
                } else {
                    draw_to_buffer(buffer, bombs[k].x, bombs[k].y, "*");
                    if (bombs[k].y >= HEIGHT - 4 && abs(bombs[k].x - player_x) <= 3) {
                        bombs[k].active = 0;
                        player_hp--;
                        fflush(stdout); 
                        flash_arena();
                        if (player_hp <= 0) game_running = 0;
                    }
                }
            }
        }

        //Handling bullets fired by the player
        for(int i=0; i<50; i++) {
            if(bullets[i].active) {
                bullets[i].y--; 
                if(bullets[i].y < 0) bullets[i].active = 0;
                else {
                    const char* b_char = (bullets[i].type == 1) ? ":" : "|";
                    draw_to_buffer(buffer, bullets[i].x, bullets[i].y, b_char);
                }
            }
        }

        //Rendering everything to the screen
        //Using buffer to avoid flickering
        gotoxy(1, 1); 
        
        printf("+"); for(int x=0; x<WIDTH; x++) printf("-"); printf("+\n");
        for(int y=0; y<HEIGHT; y++) {
            printf("|%s|\n", buffer[y]);
        }
        printf("+"); for(int x=0; x<WIDTH; x++) printf("-"); printf("+\n");

        //Displaying score, health, ammo and weapon
        set_color_cyan();
        printf(" SCORE: %-6d  LEVEL: %d    ", score, difficulty);
        if (ammo == 0) { set_color_red(); printf(" [ RELOAD (L) ]   "); set_color_cyan(); }
        else { printf(" AMMO: %-2d/%-2d     ", ammo, MAX_AMMO); }

        set_color_red();
        printf("HP: [");
        for(int i=0; i<PLAYER_HP_MAX; i++) printf("%c", (i < player_hp) ? '#' : ' ');
        printf("] \n");
        set_color_reset();
        
        printf(" GUN: %s (R)  %s \n", (gun_mode==0?"LASER  ":"SHOTGUN"), (boss.active?"[BOSS]":"      "));

        fflush(stdout); 

        //Keyboard input handling for movement and shooting
        if (kbhit()) {
            char ch = getchar();
            if (ch == '\033') { 
                getchar();
                switch(getchar()) {
                    case 'C': player_x += 2; break;
                    case 'D': player_x -= 2; break;
                }
            } else if (ch == ' ') {
                int cost = (gun_mode == 0) ? 1 : 3;
                if (ammo >= cost) {
                    ammo -= cost;
                    if (gun_mode == 0) { 
                         for(int i=0; i<50; i++) if(!bullets[i].active) {
                            bullets[i].active = 1; bullets[i].x = player_x; bullets[i].y = HEIGHT - 5; bullets[i].type=0; break;
                         }
                    } else { 
                        int c=0; for(int i=0; i<50 && c<3; i++) if(!bullets[i].active) {
                            bullets[i].active = 1; bullets[i].y = HEIGHT - 5; bullets[i].type=1;
                            bullets[i].x = player_x + ((c==0)?0:((c==1)?-2:2)); c++;
                        }
                    }
                }
            } else if (ch == 'r' || ch == 'R') gun_mode = !gun_mode; 
            else if (ch == 'l' || ch == 'L') ammo = MAX_AMMO; 
            else if (ch == 'q') game_running = 0;
        }
        usleep(30000); 
        ticks++;
    }

    show_cursor();
    printf("\nGAME OVER\n");                                              //GAME OVER message
    printf("Final Score: %d\n", score);
    return 0;
}
