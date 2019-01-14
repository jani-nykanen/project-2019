// Game scene
// (c) 2019 Jani Nykänen

#include "Game.hpp"

#include "../../Core/MathExt.hpp"
#include "../../Core/Tilemap.hpp"
#include "../../Core/SceneManager.hpp"

#include <cstdio>
#include <cmath>

#include <GLFW/glfw3.h>

// Reference to this
static Game* gref;


// Callbacks
static void cb_Resume() { gref->resume(); }
static void cb_Reset() { gref->reset(); }
static void cb_Settings() { gref->activateSettings();}
static void cb_Quit() { gref->quit(); }

static void cb_SFX() {gref->toggleSFX();}
static void cb_Music() {gref->toggleMusic();}
static void cb_Fullscreen() {gref->toggleFullscreen();}
static void cb_Back() {gref->reactivatePause();}


// Draw "Stage clear"
void Game::drawStageClear(Graphics* g) {

    const float END_MENU_Y = 128.0f;
    const float SHADOW_X = 4.0f;
    const float SHADOW_Y = 6.0f;
    const float SHADOW_ALPHA = 0.5f;
    const float TEXT_Y = -96;
    const float XOFF = -32.0f;
    const float BASE_SCALE = 1.5f;
    const float SCALE_MOD = 0.25f;
    const float COLOR_MOD = 0.25f;

    Vector2 view = g->getViewport();

    // Draw end menu
    endMenu.draw(g, 0, END_MENU_Y);

    // Compute size & color mods
    float s = sinf(endTimer);
    float scale = BASE_SCALE + s*SCALE_MOD;
    float cmod = (s+1.0f)/2.0f * COLOR_MOD;

    // Draw "Stage clear"
    if(hud.isPerfectClear())
        g->setColor(1.5f+cmod,1.5f+cmod,0.5f+cmod);
    else
        g->setColor(0.75f+cmod,0.75f+cmod,0.75f+cmod);

    g->drawText(bmpFont, "STAGE CLEAR!", 
        view.x/2, view.y/2 + TEXT_Y*(1+s*SCALE_MOD/2), XOFF, 0, 
        SHADOW_X, SHADOW_Y, SHADOW_ALPHA,
        scale, true);
}


// Hard reset
void Game::hardReset(Tilemap* tmap) {

    // (Re)initialize stage
    stage = Stage(tmap);
    // Parse map for objects
    workers = std::vector<Worker> ();
    stage.parseMap(comm);

    // Reset hud
    hud.reset();
    // Pass data to hud
    hud.setMoveTarget(stage.getMoveTarget());

    // Disable pause
    pause.deactivate();
    endMenu.deactivate();
}


// Reset the current game state
void Game::reset() {

    // Reset stage
    stage.reset();
    // Reset hud
    hud.reset();

    // Reset workers
    workers.clear();
    stage.parseMap(comm);

    // Disable pause
    pause.deactivate();
    endMenu.deactivate();
}


// Resume game
void Game::resume() {

    pause.deactivate();
}


// Terminate
void Game::quit() {
    
    sceneMan->changeActiveScene("stageMenu");
}


// Reactivate pause
void Game::reactivatePause() {

    settings.deactivate();
    pause.activate(-1);
}


// Activate settings
void Game::activateSettings() {

    settings.activate(3);
    pause.deactivate();
}


// Toggle SFX
void Game::toggleSFX() {

    AudioManager* audio = evMan->getAudioManager();
    audio->toggleSfx();

    std::string text = "SFX: " + 
        (audio->isSfxEnabled() ? std::string("On") : std::string("Off"));

    settings.setButtonText(0, text);
}


// Toggle music
void Game::toggleMusic() {

    AudioManager* audio = evMan->getAudioManager();
    audio->toggleMusic();

    std::string text = "Music: " + 
        (audio->isMusicEnabled() ? std::string("On") : std::string("Off"));

    settings.setButtonText(1, text);
}


// Initialize scene
void Game::init() {

    const float PAUSE_WIDTH = 384.0f;
    const float PAUSE_HEIGHT = 288.0f;
    const float PAUSE_SCALE = 1.0f;

    const float END_WIDTH = 320.0f;
    const float END_HEIGHT = 160.0f;
    const float END_SCALE = 0.75f;

    const float SETTINGS_WIDTH = 400.0f;
    const float SETTINGS_HEIGHT = 288.0f;
    const float SETTINGS_SCALE = 1.0f;

    printf("Initializing...\n");

    gref = this;

    // Get bitmaps
    bmpFont = assets->getBitmap("font");

    // Get transition
    trans = evMan->getTransition();

    // Create communicator
    comm = Communicator(this);

    // Initialize global data
    initGlobalStage(assets);
    initGlobalWorker(assets);

    // Initialize hud
    hud = Hud(assets);

    // Not really necessary
    stage = Stage();

    // Create pause menu
    std::vector<MenuButton> buttons;
    buttons.push_back(MenuButton("Resume", cb_Resume));
    buttons.push_back(MenuButton("Restart", cb_Reset, true, 2.0f));
    buttons.push_back(MenuButton("Settings", cb_Settings));
    buttons.push_back(MenuButton("Quit", cb_Quit, true, 2.0f));
    pause = PauseMenu(buttons, 
        PAUSE_WIDTH, PAUSE_HEIGHT, PAUSE_SCALE);

    // Create end menu
    buttons.clear();
    buttons.push_back(MenuButton("Next stage", cb_Reset, true, 2.0f));
    buttons.push_back(MenuButton("Retry", cb_Reset, true, 2.0f));
    buttons.push_back(MenuButton("Stage menu", cb_Quit, true, 2.0f));
    endMenu = PauseMenu(buttons, 
        END_WIDTH, END_HEIGHT, END_SCALE);

    // Create settings
    buttons.clear();
    buttons.push_back(MenuButton("SFX: On", cb_SFX));
    buttons.push_back(MenuButton("Music: On", cb_Music));
    buttons.push_back(MenuButton("Fullscreen", cb_Fullscreen));
    buttons.push_back(MenuButton("Back", cb_Back));
    settings = PauseMenu(buttons, 
        SETTINGS_WIDTH, SETTINGS_HEIGHT, 
        SETTINGS_SCALE);
}


// Update scene
void Game::update(float tm) {

    const float END_TIMER_SPEED = 0.05f;

    // If fading, wait until it's over
    if(trans->isActive())
        return;

    GamePad* vpad = evMan->getController();
    // Check end menu
    if(endMenu.isActive()) {

        // Update end menu
        endMenu.update(evMan);

        // Update end timer
        endTimer += END_TIMER_SPEED * tm;
        endTimer = fmodf(endTimer, M_PI *2.0f);

        return;
    }
    // TEMP
    /*
    else if(vpad->getButton("debug") == State::Pressed) {

        endMenu.activate();
    }
    */

    // Check settings
    if(settings.isActive()) {

        settings.update(evMan, false);
        return;
    }

    // Check pause
    if(pause.isActive()) {

        pause.update(evMan, true);
        return;
    }
    else {

        if(vpad->getButton("start") == State::Pressed ||
           vpad->getButton("cancel") == State::Pressed) {

            pause.activate();
            return;
        }
    }

    // Reset
    if(vpad->getButton("reset") == State::Pressed) {

        trans->activate(FadeIn, 2.0f, cb_Reset);
    }

    // "Pre-update"
    bool anyMoving = false;
    int aliveCount = 0;
    for(int i = 0; i < workers.size(); ++ i) {

        // Check cog collisions
        workers[i].checkCogCollision(&stage);

        // Is moving
        if(!anyMoving && workers[i].isActive()) {

            anyMoving = true;
        }

        // If "alive"
        if(workers[i].isAlive()) {

            ++ aliveCount;
        }
    }

    // Check if victory
    if(aliveCount == 0 && !anyMoving) {

        endMenu.activate();
        endTimer = 0.0f;
        return;
    } 

    // Update workers
    bool anyStartedMoving = false;
    for(int i = 0; i < workers.size(); ++ i) {

        workers[i].update(evMan, &stage, anyMoving, tm);
        if(!anyStartedMoving && workers[i].hasStartedMoving())
            anyStartedMoving = true;
    }
    // Increase turns, if moved
    if(anyStartedMoving)
        hud.addMove();

    // Update stage
    stage.update(evMan, tm);

    // Update HUD
    hud.update();
}


// Draw scene
void Game::draw(Graphics* g) {

    g->clearScreen(0.1f, 0.60f, 1.0f);

    // Set transform
    g->setView(VIEW_HEIGHT);
    g->identity();
    g->useTransf();

    // Draw stage
    stage.draw(g, comm);

    // Draw hud
    hud.draw(g);

    // Draw pause menus
    pause.draw(g);
    settings.draw(g);
    if(endMenu.isActive()) {

        drawStageClear(g);
    }
}


// Dispose scene
void Game::dispose() {

    printf("Terminating...\n");
}


// Called when the scene is changed
// to this scene
void Game::onChange(void* param) {

    hardReset((Tilemap*)param);    
}


// Draw workers
void Game::drawWorkers(Graphics* g) {

    // Draw workers
    for(int i = 0; i < workers.size(); ++ i) {

        workers[i].draw(g);
    }
}


// Add a worker
void Game::addWorker(Point p, int color,  
    bool sleeping, bool isCog) {

    workers.push_back(Worker(p, color, sleeping, isCog));
}
