#include "Test.h"

Test::Test() {

    // Initialize
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(
            "sdl",
            0,
            0,
            1920,
            1080,
            SDL_WINDOW_FULLSCREEN
    );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    IMG_Init(IMG_INIT_PNG);

    // Load textures
    background = IMG_LoadTexture(renderer, "Background.png");
    texture = IMG_LoadTexture(renderer, "Icon.png");
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    // Test variables
    INCREMENTS = 20;
    NUM_TESTS = 4;
    numFrames = 0;
    currentTest = 1;
    testIncrement = 0;
    currentSpriteNumber = 0;
    testSizes = {50, 100, 200, 500, 1000, 2000, 5000, 10000};

    testFrames = std::vector<std::vector<std::vector<int> > >(NUM_TESTS, std::vector<std::vector<int> >(testSizes.size(), std::vector<int>(INCREMENTS+1,0)));

    seed = 12345678;
    srand(seed);
    startup = true;
    fullResult = "";

    // testLength and timer
    startupLength = std::chrono::microseconds(10000000);
    testLength = std::chrono::microseconds(1000000);
    startTime = clock.now();
}

void Test::fillSpriteVector(){

    int size = testSizes.at(currentSpriteNumber);
    sprites.clear();
    if(currentTest == 4){
        dirXs.clear();
        dirYs.clear();
        dirXs.reserve(size);
        dirYs.reserve(size);
    }
    for(int i = 0; i < size; i++){

        // Random position
        float x = rand() % 1792;
        float y = rand() % 952;

        // Create sprite
        Sprite sprite = {{static_cast<int>(x), static_cast<int>(y), SPRITE_SIZE, SPRITE_SIZE}, x, y, 255, 0};

        if(currentTest == 2){
            int alpha = rand() % 256;
            sprite.alpha = alpha;
        }
        else if(currentTest == 4){
            float dirX;
            float dirY;
            rand() % 2 == 0 ? dirX = 0.0001f:dirX = -0.0001f;
            rand() % 2 == 0 ? dirY = 0.0001f:dirY = -0.0001f;
            dirXs[i] = dirX;
            dirYs[i] = dirY;
        }

        sprites.push_back(sprite);
    }
}

void Test::run() {
    while (true){

        // End program when all tests are done
        if(currentTest > NUM_TESTS){
            std::cout << "Increment " << testIncrement << " done!\n";
            if (testIncrement < INCREMENTS){
                testIncrement++;
                std::cout << "Increment " << testIncrement << " begins!\n";
                currentTest = 1;
            } else{
                end();
                break;
            }
        }

        // Next frame
        next();

        // Render
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background, nullptr, nullptr);
        for(auto &sprite:sprites){
            SDL_SetTextureAlphaMod(texture, static_cast<Uint8>(sprite.alpha));
            SDL_RenderCopyEx(renderer, texture, nullptr, &sprite.rect, sprite.rotation, nullptr, SDL_FLIP_NONE);
        }
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(background);
    SDL_DestroyTexture(texture);
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Test::next() {

    auto elapsed = clock.now() - startTime;

    // Allow some seconds for startup
    if(startup && std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() > startupLength.count()){
        startup = false;
        numFrames = 0;
        fillSpriteVector();
        startTime = clock.now();
        prevTime = std::chrono::microseconds::zero();
    }

        // Time for next test
    else if(!startup && std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() > testLength.count()){

        // Frames
        testFrames.at(currentTest-1).at(currentSpriteNumber).at(testIncrement) = numFrames;
        numFrames = 0;

        // Next sprite size
        if(currentSpriteNumber < testSizes.size() - 1){
            currentSpriteNumber++;
            fillSpriteVector();
        }

        // Current test done
        else {
            // Next test
            currentTest++;
            currentSpriteNumber = 0;
            fillSpriteVector();
            std::cout << "Test " << currentTest-1 << " done!\n";
        }

        prevTime = std::chrono::microseconds::zero();
        startTime = clock.now();
    }

    // Continue with the test
    else if(!startup){

        // Move or rotate
        if(currentTest > 2){

            deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(elapsed - prevTime).count();
            prevTime = elapsed;

            // Rotate sprite if on test 3
            if(currentTest == 3)
                for(auto &sprite:sprites)
                    sprite.rotation += 0.00036f*deltaTime;

            // Move sprite if on test 4
            else if(currentTest == 4){
                for(int i = 0; i < testSizes.at(currentSpriteNumber); i++){
                    sprites.at(i).x += dirXs[i]*deltaTime;
                    sprites.at(i).y += dirYs[i]*deltaTime;
                    sprites.at(i).rect.x = static_cast<int>(sprites.at(i).x);
                    sprites.at(i).rect.y = static_cast<int>(sprites.at(i).y);
                }
            }
        }

        numFrames++;
    }
}

void Test::end() {
    std::ofstream out("results.txt");
    out << std::fixed;
    out << std::setprecision(2);

    std::ofstream out2("full_results.txt");

    // Save results
    for (int k = 0; k < 4; k++) {
        switch (k+1) {
            case 1:out << "Standard draw\nSprites\tFrames\tDelta\tDeviation\n"; fullResult+="Standard draw\n";break;
            case 2:out << "Alpha draw\nSprites\tFrames\tDelta\tDeviation\n"; fullResult += "Alpha draw\n"; break;
            case 3:out << "Rotating draw\nSprites\tFrames\tDelta\tDeviation\n"; fullResult += "Rotating draw\n"; break;
            case 4:out << "Moving draw\nSprites\tFrames\tDelta\tDeviation\n"; fullResult += "Moving draw\n"; break;
        }
        for (int i = 0; i < testFrames.at(0).size(); i++) {
            double frameAverage = 0;
            for (int j = 1; j <= INCREMENTS; j++) {
                frameAverage += testFrames.at(k).at(i).at(j);
                fullResult += std::to_string(testSizes.at(i)) + "\t" + std::to_string(testFrames.at(k).at(i).at(j)) + "\n";
            }
            frameAverage /= INCREMENTS;
            int frameDifference = 0;
            double frameDeviation = 0;
            for (int j = 1; j <= INCREMENTS; j++) {
                testFrames.at(k).at(i).at(j) -= frameAverage;
                if (testFrames.at(k).at(i).at(j) < 0)
                    testFrames.at(k).at(i).at(j) *= -1;
                frameDifference += testFrames.at(k).at(i).at(j);
                frameDeviation += std::pow(testFrames.at(k).at(i).at(j), 2);
            }
            frameDifference *= 100 / INCREMENTS;
            frameDifference /= frameAverage;

            // NOTE: this is UNCORRECTED sample standard deviation
            // For corrected, divide by INCREMENTS-1 instead
            frameDeviation /= INCREMENTS;
            frameDeviation = std::sqrt(frameDeviation);

            out << std::to_string(testSizes.at(i)) << "\t" <<
                frameAverage << "\t" <<
                frameDifference << "%\t" <<
                frameDeviation << "\n";
        }
        out << "\n";
    }

    out.close();
    out2 << fullResult;
    out2.close();
    std::cout << "closing down...\n";
}
