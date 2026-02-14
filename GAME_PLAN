GOAL OF THE GAME:
  
  The goal of the player playing this game is to survive as long as possible with the limited health that he/she has.
  The player ust optimize the ammo, movement, and health with the enemies becoming stronger in terms of speed and power every passing moment.


OVERALL GAME FLOW:
  
  Game starts
  Player choses the difficulty level
  Game loop starts
  Player interacts with the UI of the game
  Aliens attack and spawn back
  Boss appears after sometime
  Player defeats the boss or gets killed
  Game ends with the 'GAME OVER' msg


DEVELOPMENT:

  Terminal Setup:
    Clear the terminal screen
    Hide the cursor
    Fast screen updates using buffer
    Implementation of cursor positioning by ANSI escape codes, which are special characters that start with 'ESC' and enable to do some non printable tasks on terminal

  Player Implementation:
    Place the player at the bottom of the screen
    Render the player using ASCII chars
    Allow left and right movements using left and right arrow keys
    Bound the movement within the game window

  Input Handling:
    Implement non blocking keyboard inputs using 'termios'
    Detect arrow key inputs and spacebar inputs
    Handle quit and reload and change weapon triggers

  Bullet System:
    Create a bullet struct
    Allow bullets to spawn when spacebar is pressed
    Move the bullets up with every frame
    Deactivate the bullet when it goes out of the screen

  Alien System:
    Create an alien struct
    Spawn aliens randomaly
    Move them horizontally
    Remove the alien as soon as it gets out of the game window
    Detect collision with a bullet

  Bomb System:
    Allow aliens and bosses to drop the bombs
    Bombs fall vertically down to the player
    Detect collision with player
    Reduce the health of the player on coming in contact with the bomb

  Boss Enemy:
    Spawn boss after some time
    Grant the boss higher health and bigger sprite
    Allow boss too to move left and right only
    Allow boss to drop bombs more frequently
    End fight with boss as soon as the health equals zero

  Difficulty Levels:
    Adjust the spawn rate according to the difficulty level chosen by the user
    Incresase the movement speed according to the level
    Make the incoming boss even stronger than the previous one

  Score Bar:
    Display health, score, ammo count
    Show weapon currently yielded
    Indicate when the boss is active
    Flash blood on the screen when the player gets hit by a bomb

  Game Over:
    Game ends when the health of the player becomes zero
    Show the final
    Restore the terminal back to the original settings


GAME LOOP DESIGN:

  Process user input
  Update the game objects:
    Player
    Bullets
    Aliens
    Bombs
    Boss
  Detect any collisions
  Draw everything in a buffer
  Render the buffer to the terminal
  Sleep briefly to control the refresh rate

This loop repeats itself until the game ends.


DESIGN DECISIONS:

  Fixed size arrays are used instead of dynamic memory alloc for the sake of simplicity
  ASCII characters are used for graphics to avois any external dependence
  Time is handled using a tick countr and 'usleep'
  One boss at a time to avoid overcomplexity


LEARNINGS:

  This project helps one revise the following C concepts:
    Structs and arrays
    Game loops and management
    Non blocking input handling
    Collision detection
    Terminal based rendering


//HOPE YOU ENJOY THIS PROJECT :)
