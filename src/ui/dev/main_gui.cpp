#include "main_gui.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include "../../deps/raylib/raylib.h"
// #include "../../deps/raylib/raudio.h"
// #include "../../audio.hpp"

// Music music;
void DevGui::imguiInit(dgScene *s, GameState *gs)
{
    scene = s;
    gameState = gs;

    _saveGameState();

    // music = LoadMusicStream("assets/audio/spawn.mp3");
    // PlayMusicStream(music);
    // AudioStream ss = music.stream;
    // RigmocAudio::rAudioBuffer *ab = ss.buffer;

    renderTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    // Setup Platform/Renderer bindings
    GLFWwindow *window = (GLFWwindow *)GetWindow();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(/*glsl_version*/);
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
}

void DevGui::imguiUpdate()
{
    // feed inputs to dear imgui, start new frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::AlignTextToFramePadding();

    // render your GUI
    _displayRenderWindow();
    _displayPlayerInfo(0);
    _displayPlayerInfo(1);
    _displayPlayerInputHistory(0);
    // _displayPlayerInputHistory(1);
    _displayStateManipButtons();

    // Render dear imgui into screen
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void DevGui::beginRenderTexture()
{
    BeginTextureMode(renderTexture);
}

void DevGui::endRenderTexture()
{
    EndTextureMode();
}

void DevGui::imguiShutdown()
{
    UnloadTexture(renderTexture.texture);
    UnloadRenderTexture(renderTexture);

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void DevGui::_displayPlayerInfo(int playerId)
{
    ImGui::Begin(std::string("Player ").append(std::to_string(playerId)).append(" info").c_str());

    ImGui::Text("Action Face: %i", gameState->playerData[playerId].actionFace);
    ImGui::Text("Side Face: %i", gameState->playerData[playerId].sideFace);
    ImGui::Text("Physical:");
    ImGui::Text("       X: %i", gameState->playerData[playerId].physical.x);
    ImGui::Text("       Y: %i", gameState->playerData[playerId].physical.y);
    ImGui::Text("  HSpeed: %i", gameState->playerData[playerId].physical.HSpeed);
    ImGui::Text("  VSpeed: %i", gameState->playerData[playerId].physical.VSpeed);
    char buf[254] = "";
    ImGui::InputText("Test", buf, 254);

    ImGui::End();
}

void DevGui::_displayStateManipButtons()
{
    ImGui::Begin("State Manipulation");
    ImGui::Columns(4);

    if(ImGui::Button("Save State"))
    {
        _saveGameState();
    }

    ImGui::NextColumn();

    if (ImGui::Button("Load State"))
    {
        _loadGameState();
    }

    ImGui::NextColumn();

    if (ImGui::Button("Toggle Update"))
    {
        _toggleUpdate();
    }

    ImGui::NextColumn();

    if (ImGui::Button("Step Update"))
    {
        _stepUpdate();
    }

    ImGui::End();
}

void DevGui::_displayPlayerInputHistory(int playerId)
{
    ImGui::Begin(std::string("Player ").append(std::to_string(playerId)).append(" Input History").c_str());

    for (int i = 0; i < INPUT_HISTORY_MAX; i++)
    {
        ImGui::Text(std::to_string(scene->players[playerId].inputHistory[i]).c_str());
    }

    ImGui::End();
}

void DevGui::_displayRenderWindow()
{
    ImGui::SetNextWindowSizeConstraints({50.f, 50.f}, {(float)GetScreenWidth(), (float)GetScreenHeight()}, _SixteenNineAspectRatio);
    ImGui::Begin("Render window");

    renderWindowIsFocused = ImGui::IsWindowFocused();
    ImVec2 wp = ImGui::GetWindowPos();

    if (!ImGui::IsWindowCollapsed())
    {
        DrawTexturePro(renderTexture.texture,
            {
                0.f,
                0.f,
                (float)renderTexture.texture.width,
                (float)-renderTexture.texture.height
            },
            {
                wp.x,
                wp.y + 20.f,
                (float)ImGui::GetWindowWidth(),
                (float)ImGui::GetWindowHeight() - 20
            },
            {
                0,
                0
            },
            0.f,
            WHITE);
    }

    ImGui::End();
}

void DevGui::_saveGameState()
{
    std::cout << "Clicked: Save State" << std::endl;
    gsLen = sizeof(*gameState);
    gsBuffer = (unsigned char *)malloc(gsLen);
    if (!*gsBuffer)
    {
        return;
    }
    memcpy(gsBuffer, gameState, gsLen);
    gameState->playerData[0].vitality -= 10;
    // std::ofstream file;
    // file.open("buffer.txt", std::ofstream::out | std::ofstream::binary);
    // file << (void *)*gsBuffer;
    // file.close();
}

void DevGui::_loadGameState()
{
    std::cout << "Clicked: Load State" << std::endl;

    std::ifstream file;
    // file.open("buffer.txt", std::ofstream::in | std::ofstream::binary);
    // int dataSize = file.tellg();
    // char *data = new char[dataSize];
    // file.read(data, dataSize);
    // file.close();
    memcpy(gameState, gsBuffer, gsLen);
}

void DevGui::_toggleUpdate()
{
    scene->willStep = !scene->willStep;
}

void DevGui::_stepUpdate(int allowance)
{
    scene->willStep = false;
    scene->stepAllowance = allowance;
}
