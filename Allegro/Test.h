/**
 * author: krille71 & antonlc
 */

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

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

#ifndef ALLEGROKEX_TEST_H
#define ALLEGROKEX_TEST_H

struct Sprite {
    ALLEGRO_BITMAP* bmp;
    ALLEGRO_COLOR color = al_map_rgba_f(1,1,1,1);
    float x;
    float y;
    float ox;
    float oy;
    float rotation;
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
    ALLEGRO_DISPLAY* window;

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
    ALLEGRO_BITMAP* background;
    ALLEGRO_BITMAP* bitmap;
    std::vector<Sprite> sprites;

    // testLength and timer
    std::chrono::microseconds startupLength;
    std::chrono::microseconds testLength;
    float deltaTime;
    std::chrono::steady_clock::duration prevTime;
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock clock;

};


#endif //ALLEGROKEX_TEST_H
