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

    Character& operator[](size_t index) {
        return font[index];
    }

    // const Character& operator[](size_t index) const {
    //     return font[index];
    // }
};

struct Defaults {
    Vector3 backgroundColor;
};

void writeFontToBitset(CharacterFont &characterFont) {
    int fontLetterSizeBytes = 31;
    int numberOfFontLetters = 2; // letters in the font
    FILE* file = fopen("../font.txt", "r");

    char* letterBits = (char*)malloc(sizeof(char)*(fontLetterSizeBytes+1));

    for (int i = 0; i < numberOfFontLetters; i++) {
        fread(letterBits, sizeof(char), fontLetterSizeBytes, file);
        int bytesSeen = 0;
        for (int j = 0; j < fontLetterSizeBytes; j++) {
            if (letterBits[j] == '1' || letterBits[j] == '0') {
                if (letterBits[j] == '1') {
                    characterFont[i].bit.set(bytesSeen); // writes each 1 and 0 to each point in each character
                }
                bytesSeen += 1;
            }
        }
    }

    for (int i = 0; i < characterFont[0].bit.size(); i++) {
        std::cout << characterFont[0].bit[i];
    }

    fclose(file);
    free(letterBits);
}

std::bitset<5*5> characterToBitmap(char character, CharacterFont &characterFont) {
    switch (character) {
        case ('A'):
            return characterFont[0].bit;
            break;
        case ('F'):
            return characterFont[1].bit;
            break;
        default:
            // perror("Unsupported character's bitset requested");
            std::bitset<5*5> bitset("111111111111111111111111");
            return bitset;
    }
}


int main() {

    InitWindow(1280, 720, "font renderer");
    rlImGuiSetup(true);
    SetExitKey(0);
    SetWindowState(FLAG_VSYNC_HINT);
    // SetTargetFPS(2);

    std::array<float, 30> fpsBuffer = {}; // 30 or 60 is a good number to use generally It's best to go with what you know will be the worst case scenario for performance
    int microsecondsPerFrame;
    int mspf;
    float fps = 0;
    unsigned long long frameNumber = 0;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator (seed);

    float backgroundColor[3] = {80.0f / 255.0f, 80.0f / 255.0f, 80.0f / 255.0f};

    Defaults defaultsList = {backgroundColor[0], backgroundColor[1], backgroundColor[2]};

    CharacterFont characterFont;
    writeFontToBitset(characterFont);

    int characterWidth = 5;
    int characterHeight = 5;
    int characterRows = 10;
    int characterColumns = 4;
    int characterHorizontalOffset = 20;
    int characterVerticalOffset = 40;
    int characterScale = 10;
    int characterHorizontalGap = 1;
    int characterVerticalGap = 3;
    char* sentence = new char[128+1];
    // strcpy(sentence, "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz, 1234567890 The quick brown fox jumps over the lazy dog. !-=?:~\'\""); // 120 characters currently

    strcpy(sentence, "AFAFFAAAFFAAFAFFAAAFFAAFAFFAAAFFAAFAFFAAAFFA");

    char* bitsetToDraw = new char [25+1];
    strcpy(bitsetToDraw, "0101010101010101010101010");

    int rectsToDraw = 0;

    while (!WindowShouldClose()) {
        std::chrono::time_point<std::chrono::high_resolution_clock> clockStart = std::chrono::high_resolution_clock::now();

        BeginDrawing();
        ClearBackground(ColorFromNormalized({backgroundColor[0], backgroundColor[1], backgroundColor[2], 255.0f}));

        DrawRectangleRounded(Rectangle(0,0,640,480), 0.1, 2, Color(255,255,255, 255));

        int rectsDrawn = 0;
        for (int i = 0; i < characterColumns; i++) {
            for (int j = 0; j < characterRows; j++) {
                // std::bitset<5*5> bitset = characterToBitmap(sentence[j*i+j], characterFont);
                std::bitset<5*5> bitset(bitsetToDraw);
                for (int ch = 0; ch < characterHeight; ch++) { // ch = character height
                    for (int cw = 0; cw < characterWidth; cw++) { // cw = character width
                        // std::cout << "0";
                        if (bitset[cw*ch+cw] == 1 && rectsDrawn < rectsToDraw) {
                            // std::cout<< "1";
                            DrawRectangle((j*characterHorizontalGap*characterScale)+(j*characterWidth*characterScale)+(cw*characterScale)+characterHorizontalOffset, (i*characterVerticalGap*characterScale)+(i*characterHeight*characterScale)+(ch*characterScale)+characterVerticalOffset, characterScale, characterScale, Color(0,0,0, 255));
                        }
                        rectsDrawn++;
                    }
                    // std::cout << std::endl;
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
                ImGui::SliderInt("Scale", &characterScale, 1, 25);
                ImGui::InputText("Text Input", sentence, 128);
                ImGui::SliderInt("Rects to draw", &rectsToDraw, 0, 32);
                ImGui::InputText("Bitset", bitsetToDraw, 25+1);
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