#include <SDL2/SDL.h>
#include <glm/glm.hpp>

#include <iostream>
#include "circle.h"
//define window dimensions
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;

const float ANIMATION_SPEED = 200.0f;

SDL_Window *window = nullptr;
SDL_Renderer *windowRenderer;
SDL_Event currentEvent;

bool quit = false;
int mouseX, mouseY;

glm::vec4 backgroundColor;

Circle circleFull;
bool animationRunning = false;
bool goingRight = true;
float elapsedTime;
bool initWindow() {

    bool success = true;

    // Try to initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {

        std::cout << "SDL initialization failed" << std::endl;
        success = false;
    } else {

        // Try to create the window
        window = SDL_CreateWindow("SDL2 Hello Window",
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  WINDOW_WIDTH,
                                  WINDOW_HEIGHT,
                                  SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);

        if(window == nullptr) {

            std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
            success = false;
        } else {

            // Create a renderer for the current window
            windowRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

            if(windowRenderer == nullptr) {

                std::cout << "Failed to create the renderer: " << SDL_GetError() << std::endl;
                success = false;
            }
        }
    }

    return success;
}

void destroyWindow() {

    // Destroy window
    SDL_DestroyWindow(window);
    window = nullptr;

    // Quit SDL
    SDL_Quit();
}

void initScene() {

    backgroundColor = glm::vec4(255.0f, 255.0f, 255.0f, 255.0f);
    circleFull.center.x = 100.0f;
    circleFull.center.y = 100.0f;
    circleFull.radius = 20.0f;
    circleFull.color = glm::vec4(0.0f, 255.0f, 0.0f, 255.0f);

}

void processEvent(SDL_Event event) {

    // User requests quit
    if(event.type == SDL_QUIT) {

        quit = true;
    }

    // Mouse event -> pressed button
    if(event.type == SDL_MOUSEBUTTONDOWN) {

        if(event.button.button == SDL_BUTTON_LEFT) {

            SDL_GetMouseState(&mouseX, &mouseY);
            std::cout << "Left mouse button pressed at (" << mouseX << ", " << mouseY << ")" << std::endl;
        }

        if(event.button.button == SDL_BUTTON_RIGHT) {

            SDL_GetMouseState(&mouseX, &mouseY);
            std::cout << "Right mouse button pressed at (" << mouseX << ", " << mouseY << ")" << std::endl;
        }
    }

    // Mouse event -> mouse movement
    if(event.type == SDL_MOUSEMOTION) {

        if(SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON_LMASK) {

            SDL_GetMouseState(&mouseX, &mouseY);
            std::cout << "Left mouse drag at (" << mouseX << ", " << mouseY << ")" << std::endl;
        }

        if(SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON_RMASK) {

            SDL_GetMouseState(&mouseX, &mouseY);
            std::cout << "Right mouse drag at (" << mouseX << ", " << mouseY << ")" << std::endl;
        }
    }

    // Keyboard event
    if(event.type == SDL_KEYDOWN) {

        switch(event.key.keysym.sym) {

            case SDLK_ESCAPE:
                quit = true;
                break;
            case SDLK_r:
                std::cout << "R key pressed" << std::endl;
         
                break;

            default:
                break;
        }
    }
}
void animate() {
    glm::vec2 mousePos{ mouseX, mouseY };
    glm::vec2 dir = glm::normalize(mousePos - circleFull.center);
    circleFull.center = circleFull.center + dir * ANIMATION_SPEED * elapsedTime;
    
    /*if (goingRight) {
        if (circleFull.center.x < 600.0f) {
            circleFull.center.x += ANIMATION_SPEED * elapsedTime;
        }
        else
        {
            goingRight = false;
        }
    }
    else
    {
        if (circleFull.center.x > 100.0f) {
            circleFull.center.x -= ANIMATION_SPEED * elapsedTime;
        }
        else
        {
            goingRight = true;
        }
    }
    for (int i = 0; i < 100000000; i++) {

    }*/
}
void drawFrame() {
    Uint32 start = SDL_GetTicks();
    // clear background
    SDL_SetRenderDrawColor(windowRenderer,
                           static_cast<Uint8>(backgroundColor.r),
                           static_cast<Uint8>(backgroundColor.g),
                           static_cast<Uint8>(backgroundColor.b),
                           static_cast<Uint8>(backgroundColor.a));
    SDL_RenderClear(windowRenderer);

    // draw the scene
    //TODO
    if (animationRunning) {
        animate();
    }
    SDL_FillCircle(windowRenderer, circleFull);
    
  

    // render to window
    SDL_RenderPresent(windowRenderer);
    Uint32 end = SDL_GetTicks();
    elapsedTime = (end - start) / 1000.0f;
}

int main(int argc, char* args[]) {

    // unused parameters
    (void)argc;
    (void)args;

    if(!initWindow()) {

        std::cout << "Failed to initialize SDL" << std::endl;
        SDL_Quit();
        return -1;
    }

    initScene();

    // game loop
    while (!quit) {

        // Handle events in queue (non-blocking)
        if(SDL_PollEvent(&currentEvent) != 0) {

            // process any event that has occurred
            processEvent(currentEvent);
        }

        // render the frame
        drawFrame();
    }

    destroyWindow();
    return 0;
}
