/**
 * author: krille71 & antonlc
 */

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

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
    sf::RenderWindow window;

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
    sf::Texture bkgr;
    sf::Sprite background;
    sf::Texture texture;
    std::vector<sf::Sprite> sprites;

    // testLength and timer
    std::chrono::microseconds startupLength;
    std::chrono::microseconds testLength;
    float deltaTime;
    std::chrono::steady_clock::duration prevTime;
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock clock;

};


#endif //KEX_TEST_H
