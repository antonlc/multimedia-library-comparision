/**
 * author: krille71 & antonlc
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <vector>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <fstream>
#include <chrono>
#include <cmath>

#include "windows.h"
#include "psapi.h"

#ifndef KEX_TEST_H
#define KEX_TEST_H

#define SPRITE_SIZE 128

struct Sprite{
    SDL_Rect rect;
    float x;
    float y;
    int alpha;
    double rotation;
};

class Test {
public:

    /**
     * Constructor
     */
    Test();

    /**
     * Run Tests
     */
    void run();

private:

    /**
     * Fills vector with sprites
     */
    void fillSpriteVector();

    /**
     * Next frame
     */
    void next();

    /**
     * Ends the program
     */
    void end();

    // Window
    SDL_Renderer *renderer;
    SDL_Window *window;

    // Test variables
    int NUM_TESTS;
    int INCREMENTS;

    int numFrames;
    int currentTest;
    int testIncrement;
    int currentSpriteNumber;
    std::vector<int> testSizes;
    std::vector<std::vector<std::vector<int>>> testFrames;

    unsigned int seed;
    bool startup;
    std::vector<float> dirXs;
    std::vector<float> dirYs;
    std::string fullResult;

    // Texture
    SDL_Texture *background;
    SDL_Texture *texture;
    std::vector<Sprite> sprites;

    // testLength and timer
    std::chrono::microseconds startupLength;
    std::chrono::microseconds testLength;
    float deltaTime;
    std::chrono::steady_clock::duration prevTime;
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock clock;

};


#endif //KEX_TEST_H
