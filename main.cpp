﻿// main.cpp : Defines the entry point for the application.
//

#include "foosiespp.h"

using namespace std;

Game::Game()
{
    Game::grid = Grid();
    Game::scene = Scene();
    Game::gameState = GameState();
    Game::controllers = std::vector<Controller>();
}

int Game::init()
{
    int screenWidth = 800;
    int screenHeight = 450;

    scene.targetFPS = 60;

    scene.cam = Camera3D();
    scene.cam.position = Vector3{0.f, 10.f, 8.f};
    scene.cam.target = Vector3{0.f, 0.f, 0.f};
    scene.cam.up = Vector3{0.f, 1.f, 0.f};
    scene.cam.fovy = 60.f;
    scene.cam.type = CAMERA_PERSPECTIVE;

    InitWindow(screenWidth, screenHeight, "foosiespp");
    SetTargetFPS(scene.targetFPS);

    while (!WindowShouldClose())
    {
        // update the state
        update();
        // draw the state
        render();
    }

    CloseWindow();
    return 0;
}

void Game::update()
{
    std::vector<InputNormalization> inputList = _aggregateGamepadInputs();
}

void Game::render()
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    _drawScene();
    _drawUI();

    EndDrawing();
}

void Game::startMultiplayerSession()
{
    Game::session = Session();

    /* Start a new session */
    GGPOErrorCode result = session.start();
}

void Game::_drawScene()
{
    BeginMode3D(scene.cam);

    DrawGrid(10, 1.f);

    EndMode3D();
}

void Game::_drawUI()
{
    GGPOPlayerType type = GGPOPlayerType::GGPO_PLAYERTYPE_REMOTE;
    std::string x = "This is a raylib->Foosies example, involving: ";
    std::string y = std::to_string((int)type);
    x.append(y);

    DrawText(x.c_str(), 10, 40, 20, DARKGRAY);
    DrawFPS(10, 10);
};

std::vector<InputNormalization> Game::_aggregateGamepadInputs()
{
    unsigned int maxPads = 4;
    std::vector<InputNormalization> inputList;
    int padsAvailable = 0;

    for (unsigned int padId = 0; padId < maxPads; padId++)
    {
        if (IsGamepadAvailable(padId))
        {
            padsAvailable++;

            if (padId < controllers.size())
            {
                if (GetGamepadName(padId) != controllers.at(padId).name)
                {
                    controllers.erase(controllers.begin() + padId);
                    controllers.emplace(controllers.begin() + padId, Controller(padId, GetGamepadName(padId)));
                }
            }
            else
            {
                controllers.push_back(Controller(padId, GetGamepadName(padId)));
            }

            inputList.push_back(controllers.at(padId).getNormalizedInputs());
        }
    }

    if (padsAvailable > 0)
    {
        controllers.erase(controllers.begin() + padsAvailable - 1, controllers.end());
    }

    return inputList;
}

int main()
{
    Game game = Game();
    return game.init();
}
