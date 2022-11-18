/*******************************************************************************************
*
*   raylib [user] example - A Pomodoro timer Program, that uses uses 15 work min/ 30 relax seconds -> 4 sets, then 15 min break.
*
*   Welcome to raylib!
*
********************************************************************************************/

/* Change Log *******************************************************************************
*   - Shorter relax periods are shorter
********************************************************************************************/

#include "raylib.h"
#include "raymath.h"
#include <stddef.h>


//------------------------------------------------------------------------------------------
// Types and Structures Definition
//------------------------------------------------------------------------------------------
typedef enum GameScreen { LOGO = 0, WORK, RELAX, RELAX_BIG, SETTINGS } GameScreen;

// define a slider
typedef struct
{
    Rectangle Path;
    Rectangle Holder;
} GUI_Input_Slider;

void Draw_GUI_Input_Slider(GUI_Input_Slider slider){
    //Rectangle path
    
    DrawRectangleRec(slider.Path, RAYWHITE); 
    DrawRectangleRec(slider.Holder, GRAY); 
}

// define a timer
typedef struct
{
    float Lifetime;
}Timer;

// start or restart a timer with a specific lifetime
void StartTimer(Timer* timer, float lifetime)
{
    if (timer != NULL)  timer->Lifetime = lifetime;
}

// update a timer with the current frame time
void UpdateTimer(Timer* timer)
{
    // subtract this frame from the timer if it's not allready expired
    if (timer != NULL && timer->Lifetime > 0)   timer->Lifetime -= GetFrameTime();
}

// check if a timer is done.
bool TimerDone(Timer* timer)
{
    if (timer != NULL)  return timer->Lifetime <= 0;
    
	return false;
}


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main ()
{
	// Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    const int targetFPS = 15;
    InitWindow(screenWidth, screenHeight, "Work Timer");
	SetTargetFPS(targetFPS);
    SetExitKey(KEY_NULL); 
    
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    
    GameScreen currentScreen = LOGO;
    GameScreen previousScreen = currentScreen;
    
    float logoTimer = 2.0f;             // Time to wait in seconds.
    float workTimer = 900.0f;           // 15.0f * 60.0f = 900.0f sec
    float relaxTimer = 30.0f;
    
    SetRandomSeed(0);
    int relaxBigTimerMin = 900;         // 15 * 60 = 900 sec
    int relaxBigTimerMax = 1200;        // 20 * 60 = 1200 sec
    int countWorkTimer = 0;             // Count for number of work stages done, after four are done reset the counter back to zero.

    // A timer for the program
    Timer countdownTimer = { 0 };               // Initialise a timer
    StartTimer(&countdownTimer, logoTimer);     // Begin the timer
    
    // A GUI Input Slider
    GUI_Input_Slider Setting_FPS = {{180, 157, 50, 3}, {182, 150, 10, 20}};
    
    
	// App Loop
	while (!WindowShouldClose())
	{
        // Update
        //----------------------------------------------------------------------------------
        switch(currentScreen)
        {
            case LOGO:
            {
                // TODO: Update LOGO screen variables here!

                UpdateTimer(&countdownTimer);                   // Update timer
                
                // Wait for logoTimer seconds (2 seconds) before jumping to WORK screen
                if (TimerDone(&countdownTimer))
                {
                    currentScreen = WORK;                       // Set to screen to WORK
                    StartTimer(&countdownTimer, workTimer);     // Start workTimer
                    ClearWindowState(FLAG_WINDOW_TOPMOST);      // Clear windows state (Window can be ovelay with another app;ication)
                    SetWindowState(FLAG_WINDOW_HIDDEN);         // Set windows state to hidden
                 }
            } break;
            
            case WORK:
            {
                // TODO: Update WORK screen variables here!
                UpdateTimer(&countdownTimer);       // Update Timer

                // When timer is done change to RELAX screen
                if (TimerDone(&countdownTimer))
                {
                    countWorkTimer += 1;
                    printf("Current Work Stage: %d \r", countWorkTimer+1);
                    
                    if (countWorkTimer >= 4){
                        currentScreen = RELAX_BIG;                      // Set the screen to RELAX_BIG
                        StartTimer(&countdownTimer, GetRandomValue(relaxBigTimerMin, relaxBigTimerMax) * 1.0f);    // Start relaxBigTimer
                        ClearWindowState(FLAG_WINDOW_HIDDEN);       // Clear window state (Window is visible again)
                        SetWindowState(FLAG_WINDOW_TOPMOST);        // Set the windows to be always on top so user can see the window
                    }
                    else{
                        currentScreen = RELAX;                      // Set the screen to RELAX
                        StartTimer(&countdownTimer, relaxTimer);    // Start relaxTimer
                        ClearWindowState(FLAG_WINDOW_HIDDEN);       // Clear window state (Window is visible again)
                        SetWindowState(FLAG_WINDOW_TOPMOST);        // Set the windows to be always on top so user can see the window
                        SetWindowState(FLAG_FULLSCREEN_MODE);        // Set the windows to be fullscreen
                    }
                }
            } break;
            
            case RELAX:
            {
                // TODO: Update RELAX screen variables here!
                UpdateTimer(&countdownTimer);               // Update Timer
                
                // When relaxTimer is done change to WORK screen
                if (TimerDone(&countdownTimer))
                {
                    currentScreen = WORK;                       // Set to screen to WORK
                    StartTimer(&countdownTimer, workTimer);     // Start workTimer
                    ClearWindowState(FLAG_WINDOW_TOPMOST);      // Clear windows state (Window can be ovelay with another app;ication)
                    SetWindowState(FLAG_WINDOW_HIDDEN);         // Set windows state to hidden
                }
            } break;
            
            case RELAX_BIG:
            {
                // TODO: Update RELAX screen variables here!
                UpdateTimer(&countdownTimer);               // Update Timer
                
                // When relaxTimer is done change to WORK screen
                if (TimerDone(&countdownTimer))
                {   
                    countWorkTimer = 0;
                    currentScreen = WORK;                       // Set to screen to WORK
                    StartTimer(&countdownTimer, workTimer);     // Start workTimer
                    ClearWindowState(FLAG_WINDOW_TOPMOST);      // Clear windows state (Window can be ovelay with another app;ication)
                    SetWindowState(FLAG_WINDOW_HIDDEN);         // Set windows state to hidden
                }
            } break;
            
            case SETTINGS:
            {
                // TODO: Update SETTINGS screen variables here!
                

            } break;
            default: break;
        }
        
        if(IsKeyReleased(KEY_ESCAPE)){
            if (currentScreen != SETTINGS)    {
                previousScreen = currentScreen;
                currentScreen = SETTINGS;
            }
            else 
                currentScreen = previousScreen;
        }

        
        //----------------------------------------------------------------------------------
        
        
        // Draw
        //----------------------------------------------------------------------------------
        
            switch(currentScreen)
            {
                case LOGO:
                {
                    BeginDrawing();

                        ClearBackground(RAYWHITE);

                        // TODO: Draw LOGO screen here!
                        DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
                        DrawText("WAIT for 2 SECONDS...", 290, 220, 20, GRAY);
                    EndDrawing();

                } break;
                
                case WORK:
                {
                    //BeginDrawing();
                        
                        
                        
                    EndDrawing();
                }
                break;
                
                case RELAX:
                {
                    BeginDrawing();

                        ClearBackground(RAYWHITE);
                        // TODO: Draw RELAX screen here!
                        DrawRectangle(0, 0, screenWidth, screenHeight, PURPLE);
                        DrawText("RELAX SCREEN", 20, 20, 40, MAROON);
                        DrawText("LOOK OUTSIDE", 150, 100, 80, RAYWHITE);
                        DrawText(TextFormat("Time remaining: %0.3f", countdownTimer.Lifetime), 200, 80, 20, RAYWHITE);

                    EndDrawing();
                } break;
                
                case RELAX_BIG:
                {
                    BeginDrawing();

                        ClearBackground(RAYWHITE);
                        // TODO: Draw RELAX_BIG screen here!
                        DrawRectangle(0, 0, screenWidth, screenHeight, PURPLE);
                        DrawText("RELAX FOR LONG THIS TIME", 20, 20, 40, MAROON);
                        DrawText("LOOK OUTSIDE", 150, 100, 80, RAYWHITE);
                        DrawText(TextFormat("Time remaining: %0.3f", countdownTimer.Lifetime), 200, 80, 20, RAYWHITE);

                    EndDrawing();
                } break;
                
                case SETTINGS:
                {
                    BeginDrawing();

                        ClearBackground(RAYWHITE);

                        // TODO: Draw SETTINGS screen here!
                        DrawRectangle(0, 0, screenWidth, screenHeight, BLUE);
                        DrawText("SETTINGS SCREEN", 20, 20, 40, DARKBLUE);
                        DrawText("PRESS ESCAPE to return to previous screen", 120, 100, 20, DARKBLUE);
                        DrawText("FPS", 120, 150, 20, DARKBLUE);
                        Draw_GUI_Input_Slider(Setting_FPS);
                        DrawText(TextFormat("X: %d", GetMouseX()), 0, 0, 10, RAYWHITE);
                        DrawText(TextFormat("Y: %d", GetMouseY()), 0, 12, 10, RAYWHITE);
                    EndDrawing();
                } break;
                
                default: break;
            }

        //----------------------------------------------------------------------------------
	}

	// cleanup
	CloseWindow();
	return 0;
}
