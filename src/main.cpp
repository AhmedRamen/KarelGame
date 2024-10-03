#include <iostream>
#include "game.h"
#include <raymath.h>
#include "tileset.h"
#include "audio.h"
#include "player.h"
#include "sol/sol.hpp"
#include "timer.h"

#pragma region Lua Script implementation

//Init variables
bool running = false;
Timer ExecutionTimer;
float tilExecution = 0.15f;
std::vector<std::string> scripts;
int script_inc = 0;
enum class ScriptState {
    Idle,
    Executing
};
ScriptState currentState = ScriptState::Idle;

bool can_stop_executing;

//Init functions

//Runs the script from the text file
void runScript();

//What actually execute the script via lua.
void executeScript(sol::string_view code);

void getFile() {
    //Open file
    std::ifstream file("workers/MausWorker.txt");
    //As a fall back in case the file got deleted
    if (!file) {
        failed = true;
        std::ofstream nofile;
        nofile.open("workers/MausWorker.txt");
        nofile << "";
        nofile.close();
        return;
    }
    //Append the lines to the script vector
    std::string line;
    while (getline(file, line)) {
        scripts.push_back(line);
    }
    file.close();
    running = true;
    runScript();
}

void loadFile() {
    //Open file
    std::ifstream file("workers/MausWorker.txt");
    //Apparently file doesn't exist, Create it.
    if (!file) {
        failed = true;
        std::ofstream nofile;
        nofile.open("workers/MausWorker.txt");
        nofile << "";
        nofile.close();
        return;
    }
    std::cout << "File exists" << std::endl;
    file.close();

}

//Stops executing the script
void StopScript() {
    if (currentState == ScriptState::Executing && can_stop_executing) {
        std::cout << "Stopping execution..." << std::endl;
        can_stop_executing = false;
        running = false;
    }
}

//Run the script based on timer
void runScript() {
    //Check if scripts vector is actually empty
    if (scripts.empty()) {
        empty = true;
        running = false;
    }

    if (currentState == ScriptState::Idle && script_inc < scripts.size()) {
        currentState = ScriptState::Executing;
        StartTimer(&ExecutionTimer, tilExecution);
    }
    while (running) {
        if (TimerDone(ExecutionTimer)) {
            //Finally we can execute the script
            if (script_inc < scripts.size()) {
                can_stop_executing = true;
                std::cout << "Script line " << script_inc << ": " << scripts.at(script_inc) << std::endl;
                executeScript(scripts.at(script_inc));
                script_inc++; //Increment

                //Check if there's more scripts in the text file
                if (script_inc < scripts.size()) {
                    StartTimer(&ExecutionTimer, tilExecution);
                }
                else {
                    currentState = ScriptState::Idle;
                    running = false;
                    break;
                }
            }
        }
        //This is the most fucked up way of actually not locking up the game
        GameUpdate();
        BeginDrawing();
        GameRender();
        EndDrawing();
    }
    //Clean up
    can_stop_executing = false;
    script_inc = 0;
    scripts.clear();
    currentState = ScriptState::Idle; //if it was interrupted by user
};

//Basic panic handling from the sol2 doc (shouldn't really run but if it does, it'll crash)
inline void my_panic(sol::optional<std::string> maybe_msg) {
    std::cerr << "Lua is in a panic state and will now abort() the application" << std::endl;
    if (maybe_msg) {
        const std::string& msg = maybe_msg.value();
        std::cerr << "\terror message: " << msg << std::endl;
    }
    // When this function exits, Lua will exhibit default behavior and abort()
}

//Try to execute the script
void executeScript(sol::string_view code) {
    sol::state lua;
    lua.open_libraries(sol::lib::base);
    //Get functions
    lua.set_function("MoveForward", &MoveForward);
    lua.set_function("TurnLeft", &TurnLeft);
    lua.set_function("TurnRight", &TurnRight);
    lua.set_function("TurnAround", &TurnAround);

    std::cout << "Executing script..." << std::endl;
    //Execute the script here
    sol::protected_function_result pfr = lua.safe_script(code, sol::script_pass_on_error);
    //Error happened, stop running
    if (!pfr.valid()) {
        running = false;
        failed = true;
        sol::error err = pfr;
        std::cout << "Failed to execute code: " << err.what() << std::endl;
        return;
    }
}

//Restart the level
void RestartLevel() {
    sound_manager::PlaySoundFromList(SOUND_RESTART_LEVEL);
    StartLevel();
    failed = false;
    started = false;
}

void StartMaus() {
    empty = false;
    attempts += 1;
    total_attempts += 1;
    started = true;
    getFile(); //Start to try and get the worker file
}

#pragma endregion

#pragma region UI

//Simple hack (Sorry mac and linux users)
/*
void onOpenScript() {
    system(".\\workers\\MausWorker.txt");
    return;
}
*/


void RenderUI() {
    DrawRectangle(screenWidth - 260, 0, 320, screenHeight, ColorAlpha(BLACK, 0.5f));
    DrawRectangle(screenWidth - 250, 0, 245, 240, BLUE);
    DrawRectangle(screenWidth - 245, 5, 235, 230, DARKBLUE);

    //How to control the Maus Tutorial
    DrawText("How to Control:", screenWidth - 245, 5, 28, WHITE);
    DrawText("MoveForward()", screenWidth - 245, 35, 30, YELLOW); //\nMoves the Maus Forward!
    DrawText("Moves the Maus Forward", screenWidth - 245, 65, 17, WHITE);
    DrawText("TurnLeft()", screenWidth - 245, 85, 35, YELLOW); //\nTurns the Maus left!
    DrawText("Turns the Maus left", screenWidth - 245, 115, 17, WHITE);
    DrawText("TurnRight()", screenWidth - 245, 135, 30, YELLOW); //\nTurns the Maus right!
    DrawText("Turns the Maus right", screenWidth - 245, 165, 17, WHITE);
    DrawText("TurnAround()", screenWidth - 245, 185, 30, YELLOW); //\nTurns the Maus Around!
    DrawText("Turns the Maus around", screenWidth - 245, 215, 17, WHITE);
    
    //Level and Attempt counter
    DrawText(TextFormat("Level %d", level), 10, 10, 48, WHITE);
    if (attempts > max_attempts)
        DrawText(TextFormat("Attempt: %d", attempts), 10, 55, 26, RED);
    else
        DrawText(TextFormat("Attempt: %d", attempts), 10, 55, 26, RAYWHITE);
    DrawText(TextFormat("Score: %d", score), screenWidth - 245, 250, 26, RAYWHITE);

    //This freezes the game due to how bad the implementation is
    //OpenScript.Render(onOpenScript);

    if (empty) {
        DrawText("Script is empty!", screenWidth / 2 + 155, screenHeight / 2 + 155, 28, YELLOW);
        DrawText("Add a function to move Maus.", screenWidth / 2 + 155, screenHeight / 2 + 185, 18, YELLOW);
    }


    if (running == true) {
        DrawText("Running...", screenWidth / 2 + 160, screenHeight / 2 + 155, 48, WHITE);
        StopExec.Render(StopScript);
    }
    else {
        //Player has touched the cheese
        if (win == true && started == false && failed == false) {
            DrawText("Congratulations!", screenWidth / 3 - 100, screenHeight / 2 - 40, 48, WHITE);
            DrawText("You got the cheese.", screenWidth / 3 - 100, screenHeight / 2, 48, WHITE);
            DrawText(TextFormat("Total attempts: %d", attempts), screenWidth / 3 - 40, screenHeight / 2 + 50, 36, WHITE);
            //Only prints if we're past the maximum amount
            if (attempts > max_attempts)
                DrawText(TextFormat("Maximum attempts: %d", max_attempts), screenWidth / 3 - 50, screenHeight / 2 + 80, 36, RED);
            NextLevel.Render(StartNextLevel);

        }
        //Didn't start with executing yet
        else if (started == false)
            Execute.Render(StartMaus);
        else if (started == true || failed == true) {
            //Should happen if you make a mistake.
            if (failed)
                DrawText("Script Error!", screenWidth / 2 + 155, screenHeight / 2 + 165, 36, RED);
            Restart.Render(RestartLevel);
        }

    }

}

#pragma endregion

#pragma region Button Clicks
void onStartGame() {
    previous = currentScreen;
    score = 0;
    total_attempts = 0;
    currentScreen = GAME;

    StartGame();
}

void onHighScores() {
    previous = currentScreen;
    currentScreen = MENU;
}

void onReturnMenu() {
    currentScreen = previous;
    previous = currentScreen;
    if (!IsMusicStreamPlaying(musics[MUSIC_TITLE_SCREEN]))
        sound_manager::playMusicFromList(MUSIC_TITLE_SCREEN);
}

void onQuitGame() {
    exitWindow = true;
}

#pragma endregion

#pragma region Levels
void StartNextLevel() {
    level++;
    if (level == 4) {
        currentScreen = ENDING;
        sound_manager::playMusicFromList(MUSIC_ENDING);
        return;
    }

    loadLevel("levels/level" + std::to_string(level));
    StartLevel();
    win = false;
    
    attempts = 0;
    return;
}

//There could have been  a even better way to do this
//But Hardcoding will do for now, seems to work better anyway.
void StartLevel() {
    win = false;
    switch (level) {
    case 1:
        cheese.x = 13 * TILE_WIDTH;
        cheese.y = 5 * TILE_HEIGHT;
        player.x = 3 * TILE_WIDTH;
        player.y = 5 * TILE_HEIGHT;
        player.dir = EAST;
        max_attempts = 1;
        break;
    case 2:
        cheese.x = 16 * TILE_WIDTH;
        cheese.y = 5 * TILE_HEIGHT;
        player.x = 8 * TILE_WIDTH;
        player.y = 11 * TILE_HEIGHT;
        player.dir = NORTH;
        max_attempts = 2;
        break;
    case 3:
        cheese.x = 10 * TILE_WIDTH;
        cheese.y = 18 * TILE_HEIGHT;
        player.x = 11 * TILE_WIDTH;
        player.y = 3 * TILE_HEIGHT;
        player.dir = SOUTH;
        max_attempts = 4;
        break;
    case 4:
        break;
    }

    //Set the camera to the player
    camera.offset = { screenWidth / 2, screenHeight / 2 };
    camera.target.x = player.x;
    camera.target.y = player.y;

}
#pragma endregion

#pragma region Game Start
void GameStartup() {
    InitAudioDevice();

    //Load the image and store as a texture
    Image image = LoadImage("assets/tileDebug.png");
    textures[TEXTURE_TILEMAP] = LoadTextureFromImage(image);
    //Delete the image
    UnloadImage(image);

    //Load the textures
    background = LoadTexture("assets/Background.png");
    title = LoadTexture("assets/sprTitle.png");
    icon = LoadImage("assets/icon.ico");


    sound_manager::LoadSounds();
    sound_manager::LoadMusic();

    sound_manager::playMusicFromList(MUSIC_TITLE_SCREEN);

    //Set the windows icon (Doesn't really work for no reason)
    SetWindowIcon(icon);

    //Load file
    loadFile();

}

void StartGame() {
    //Load level
    loadLevel("levels/level" + std::to_string(level));
    //Set camera to player after loading the level (if it does load).
    camera.target = Vector2{ (float)player.x, (float)player.y };
    camera.offset = { screenWidth / 2, screenHeight / 2 };
    camera.rotation = 0.0f;
    camera.zoom = 2.0f;

    sound_manager::playMusicFromList(MUSIC_GAME);
    StartLevel();
}

#pragma endregion

#pragma region Game Update Frame
void GameUpdate() {
    //Stop if it fails to load
    if (failedtoLoad == true)
        return;

    //Music stream
    UpdateMusicStream(current_song);

    switch (currentScreen) {
    case TITLE:
        break;
    case MENU:
        break;
    case ENDING:
        if (frameCount == 0) {
            sound_manager::stopMusic();
            sound_manager::PlaySoundFromList(SOUND_UT_NOISE);
        }
        frameCount++;

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !skipped) {
            frameCount = 600;
            skipped = true;
        }

        if (frameCount == 100)
            sound_manager::PlaySoundFromList(SOUND_UT_NOISE);
        if (frameCount == 200)
            sound_manager::PlaySoundFromList(SOUND_UT_NOISE);
        if (frameCount == 300)
            sound_manager::PlaySoundFromList(SOUND_DRUMROLL);
        if (frameCount == 400) {
            if (score < 0 || score == 0) {
                sound_manager::PlaySoundFromList(SOUND_WOMPWOMP);
            }
            else if (score > 1 && score < 3000) {
                sound_manager::PlaySoundFromList(SOUND_AVERAGE);
            }
            else if (score > 3000 && score < 10000) {
                sound_manager::PlaySoundFromList(SOUND_TADA);
            }
        }

        if (frameCount == 600) {
            sound_manager::playMusicFromList(MUSIC_ENDING);
            skipped = true; //if we're bored I guess
        }
        break;
    case GAME:
        float x = player.x;
        float y = player.y;

        speedX -= 0.004f;
        speedY -= 0.003f;

        //Check if player is on the cheese
        if (cheese.x == x && cheese.y == y) {
            if (attempts < max_attempts && !win)
                score += 5000;
            else if (attempts == max_attempts && !win)
                score += 1000;
            else if (!win)
                score -= 500 * attempts;
            win = true;
            started = false;
            failed = false;

            //If script is still running
            StopScript();
        }

#pragma region Camera control
        float wheel = GetMouseWheelMove();

        // Translate based on mouse right click
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
        {
            Vector2 delta = GetMouseDelta();
            delta = Vector2Scale(delta, -1.0f / camera.zoom);
            camera.target = Vector2Add(camera.target, delta);
        }

        //Zoom in & out
        if (wheel != 0) {
            Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
            const float zoomIncrement = 0.5f;
            float targetZoom = camera.zoom + (wheel * zoomIncrement);

            camera.offset = GetMousePosition();
            camera.target = mouseWorldPos;

            //Clamp camera zoom
            if (targetZoom > 4.0f) targetZoom = 4.0f;
            if (targetZoom < 1.0f) targetZoom = 1.0f;

            camera.zoom = targetZoom;
        }

        //Reset camera to player
        if (IsKeyPressed(KEY_R)) {
            camera.target = Vector2{ (float)player.x, (float)player.y };
            camera.offset = { screenWidth / 2, screenHeight / 2 };
            camera.target.x = player.x;
            camera.target.y = player.y;
        }
        break;
    }

#pragma endregion
}
#pragma endregion

#pragma region Game Render Frame
//Main Game Render
void GameRender() {
    ClearBackground(BLACK);

    if (failedtoLoad == true) {
        DrawText("Failed to load this level.", screenWidth / 4 - 100, screenHeight / 2 - 40, 48, RED);
        DrawText("Please check your files.", screenWidth / 4 - 100, screenHeight / 2, 48, WHITE);
        return;
    }

    //Terrible scene Switcher
    switch (currentScreen) {
        //Title screen
    case TITLE:
        DrawTexture(title, 150, 50, WHITE);
        DrawTexture(title, 150, 50, WHITE);
        StartGameButton.Render(onStartGame);
        Highscores.Render(onHighScores);
        QuitGame.Render(onQuitGame);
        break;
        //Nevermind running out of time (High score leaderboard)
    case MENU:
        DrawText("Work in Progress.", screenWidth / 2 - 200, screenHeight / 2 - 80, 48, WHITE);
        DrawText("Come back later", screenWidth / 2 - 200, screenHeight / 2 - 40, 48, WHITE);
        ReturnMenu.Render(onReturnMenu);
        break;
    case GAME:
        //Render the scene in the Camera
        BeginMode2D(camera);
        //Render Background
        DrawTextureEx(background, Vector2{ ((sinf((float)speedX) * 1000.0f) - 10.0f) - 2048, (cosf((float)speedY) * 1024.0f) - 2048 }, 0.0f, 1.0f, WHITE);

        //Draw the level
        RenderWorld();

        //Draw player
        drawPlayerDirection();

        EndMode2D();
        //UI
        RenderUI();
        break;
    case ENDING:
        if (frameCount > 100)
            DrawText(TextFormat("Total attempts taken: %d", total_attempts), screenWidth / 2 - 280, screenHeight / 2 - 150, 48, WHITE);
        if (frameCount > 200)
            DrawText(TextFormat("Total Score: %d", score), screenWidth / 2 - 230, screenHeight / 2 - 100, 48, WHITE);
        if (frameCount > 400) {
            //If our score is below 0
            if (score < 0 || score == 0) {
                DrawText("Game over.", screenWidth / 2 - 150, screenHeight / 2 - 20, 48, WHITE);
            }
            //If our score is between 1 to 3000
            else if (score > 1 && score < 3000) {
                DrawText("Good game!", screenWidth / 2 - 150, screenHeight / 2 - 20, 48, WHITE);
            }
            //If our score is between 3000 to 10,000
            else if (score > 3000 && score < 10000) {
                DrawText("Great game!", screenWidth / 2 - 150, screenHeight / 2 - 20, 48, WHITE);
            }
            //It's it more than 10,000!
            else if (score > 10000) {
                DrawText("WOWZERS!!!", screenWidth / 2 - 150, screenHeight / 2 - 20, 48, WHITE);
            }
        }

        //Tips
        if (frameCount > 600) {
            if (score == 0) {
                DrawText("You need to step up your game.", screenWidth / 2 - 200, screenHeight / 2 + 40, 28, WHITE);
            }
            else if (score > 1) {
                DrawText("Try doing less attempts for a higher score!", screenWidth / 2 - 300, screenHeight / 2 + 40, 28, WHITE);
            }
            else if (score > 10000) {
                DrawText("I'm amazed! You don't even need a tip from me.", screenWidth / 2 - 300, screenHeight / 2 + 40, 28, WHITE);
            }
            else if (score < 0) {
                DrawText("You need to do your attempts better next time.", screenWidth / 2 - 330, screenHeight / 2 + 40, 28, WHITE);
            }
            ReturnMenu.Render(onReturnMenu);
        }
        break;
    }

}
#pragma endregion

#pragma region Game Shut Down
void GameShutdown() {

    //Delete all textures
    for (int i = 0; i < MAX_TEXTURES; i++)
        UnloadTexture(textures[i]);

    UnloadTexture(background);
    UnloadImage(icon);

    //Delete all sounds
    for (int i = 0; i < MAX_SOUNDS; i++)
        UnloadSound(sounds[i]);

    //Delete all music
    for (int i = 0; i < MAX_MUSIC; i++) {
        StopMusicStream(musics[i]);
        UnloadMusicStream(musics[i]);
    }



    CloseAudioDevice();
}
#pragma endregion


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{   
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "Annoying Mouse Maze DELUXE");

    //60 FPS
    SetTargetFPS(60);
    
    //Initalise everything
    GameStartup();

    // Main game loop
    while (!exitWindow)
    {

        if (IsKeyPressed(KEY_ESCAPE) || WindowShouldClose()) exitWindow = true;     // Detect window close button or ESC key

        //Update the game
        GameUpdate();

        BeginDrawing();

        GameRender();

        EndDrawing();
    }

    //Stop the game and clean everything up
    GameShutdown();

    //Close the window
    CloseWindow();
    
    return 0;
}