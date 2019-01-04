// Game scene
// (c) 2019 Jani Nykänen

#include "Game.hpp"

#include <cstdio>

#include <GLFW/glfw3.h>


// Initialize scene
void Game::init() {

    printf("Initializing...\n");
}


// Update scene
void Game::update(int steps) {

    if(evMan->getKeyState(GLFW_KEY_ESCAPE) == State::Released) {

        evMan->terminate();
    }
}


// Draw scene
void Game::draw(Graphics* g) {

}


// Dispose scene
void Game::dispose() {

    printf("Terminating...\n");
}


// Called when the scene is changed
// to this scene
void Game::onChange() {

}
