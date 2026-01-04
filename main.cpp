#include "raylib.h"
#include "rlImGui.h"
#include "rlImGuiColors.h"
#include "bitset"
#include "random"
#include "imgui.h"
#include <iostream>
#include <chrono>


struct Character {
    std::bitset<5*5> bit;
};

struct CharacterFont {
    std::array<Character, 26> font;
};

struct Defaults {
    Vector3 backgroundColor;
};



int main() {

    InitWindow(1280, 720, "font renderer");
    rlImGuiSetup(true);
    SetExitKey(0);
    SetWindowState(FLAG_VSYNC_HINT);

    std::array<float, 30> fpsBuffer = {}; // 30 or 60 is a good number to use generally It's best to go with what you know will be the worst case scenario for performance
    int microsecondsPerFrame;
    int mspf;
    float fps = 0;
    unsigned long long frameNumber = 0;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator (seed);

    float backgroundColor[3] = {80.0f / 255.0f, 80.0f / 255.0f, 80.0f / 255.0f};

    Defaults defaultsList = {backgroundColor[0], backgroundColor[1], backgroundColor[2]};

    int characterWidth = 5;
    int characterHeight = 5;
    int characterHorizontalOffset = 20;
    int characterVerticalOffset = 40;
    int characterScale = 10;
    int characterHorizontalGap = 1;
    int characterVerticalGap = 3;

    while (!WindowShouldClose()) {
        std::chrono::time_point<std::chrono::high_resolution_clock> clockStart = std::chrono::high_resolution_clock::now();

        BeginDrawing();
        ClearBackground(ColorFromNormalized({backgroundColor[0], backgroundColor[1], backgroundColor[2], 255.0f}));

        DrawRectangleRounded(Rectangle(0,0,640,480), 0.1, 2, Color(255,255,255, 255));


        for (int i = 0; i < 10; i++) { // rows of characters
            for (int j = 0; j < 4; j++) { // columns of characters
                for (int cw = 0; cw < characterWidth; cw++) { // cw = character width
                    for (int ch = characterHeight; ch > 0; ch--) { // ch = character height
                        if (generator() % 2) {
                            DrawRectangle((i*characterHorizontalGap*characterScale)+(i*characterWidth*characterScale)+(cw*characterScale)+characterHorizontalOffset, (j*characterVerticalGap*characterScale)+(j*characterHeight*characterScale)+(ch*characterScale)+characterVerticalOffset, characterScale, characterScale, Color(0,0,0, 255));
                        }
                    }
                }
            }
        }

        rlImGuiBegin();

        if (ImGui::Begin("Settings")) {
            ImVec4 textColor;
            if (mspf <= 8.33f) { // 120 fps
                textColor = ImColor(0,255,0,255);
            }
            else if (mspf <= 16.66f) { // 60 fps
                textColor = ImColor(255,255,0,255);
            }
            else {
                textColor = ImColor(255,0,0,255);
            }

            ImGui::Text("milliseconds per frame: ");
            ImGui::PushStyleColor(ImGuiCol_Text, textColor);
            ImGui::SameLine();
            ImGui::Text("%d", mspf);
            ImGui::PopStyleColor();

            ImGui::Text("microseconds per frame: ");
            ImGui::PushStyleColor(ImGuiCol_Text, textColor);
            ImGui::SameLine();
            ImGui::Text("%d", microsecondsPerFrame % 1000);
            ImGui::PopStyleColor();

            ImGui::Text("fps: ");
            ImGui::PushStyleColor(ImGuiCol_Text, textColor);
            ImGui::SameLine();
            ImGui::Text("%.0f", fps);
            ImGui::PopStyleColor();

            if (ImGui::Button("Reset all parameters")) { // incredibly bad way to reset all params maybe make this a function later on
                backgroundColor[0] = defaultsList.backgroundColor.x;
                backgroundColor[1] = defaultsList.backgroundColor.y;
                backgroundColor[2] = defaultsList.backgroundColor.z;
            }

            if (ImGui::CollapsingHeader("Text", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::SliderInt("Scale: %d", &characterScale, 1, 25);
            }
        }
        ImGui::End();



        rlImGuiEnd();
        EndDrawing();
        frameNumber++;
        std::chrono::time_point<std::chrono::high_resolution_clock> clockEnd = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> deltaTime = clockEnd - clockStart;
        microsecondsPerFrame = static_cast<int>(std::chrono::duration_cast<std::chrono::microseconds>(deltaTime).count());
        mspf = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(deltaTime).count());
        fpsBuffer[frameNumber % std::size(fpsBuffer)] = 1000000.0f / static_cast<float>(microsecondsPerFrame);

        float fpsSum = 0.0f;
        for (float fpsNum : fpsBuffer) {
            fpsSum += fpsNum;

            fps = fpsSum / static_cast<float>(std::size(fpsBuffer));
        }
    }

    CloseWindow();
    rlImGuiShutdown();

    return 0;
}