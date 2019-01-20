// Title screen scene
// (c) 2019 Jani Nykänen

#ifndef __TITLE_H__
#define __TITLE_H__

#include "../../Core/Scene.hpp"
#include "../../Core/Transition.hpp"

#include "../Game/PauseMenu.hpp"

#include "../../Menu.hpp"

// Ending class
class Title : public Scene {

private:

    // Bitmaps
    Bitmap* bmpFont;
    Bitmap* bmpLogo;
    // Samples
    Sample* sPause;
    Sample* sReject;

    // Transition
    Transition* trans;
    // Menus
    Menu menu;
    PauseMenu settings;
    PauseMenu confirmMenu;

    // Logo float timer
    float logoFloat;
    // Logo scale
    float logoScale;
    // Has the logo stopped moving
    bool logoStopped;

    // Phase
    int phase;
    // "Press enter" timer
    float enterTimer;

    // Was data removed
    bool dataRemoved;

public:

    // Terminate
    inline void terminate() {
        evMan->terminate();
    }
    // Go to the stage menu
    void goToStageMenu();
    // Toggle fullscreen
    inline void toggleFullscreen() {
        evMan->toggleFullscreen();
    }
    // Toggle SFX
    void toggleSFX();
    // Toggle music
    void toggleMusic();
    // Disable settings
    inline void disableSettings() {
        settings.deactivate();
    }
    // Activate settings
    void activateSettings();
    // Remove data
    void removeData();
    // Disable confirmation menu
    inline void disableConfirmMenu() {
        confirmMenu.deactivate();
    }
    // Activate confirm menu
    void activateConfirmMenu();

    // Initialize scene
    void init();
    // Update scene
    void update(float tm);
    // Draw scene
    void draw(Graphics* g);
    // Destroy scene
    void dispose();
    // On change
    void onChange(void* param=NULL);

    // Get name
    inline std::string getName() {
        return "title";
    }

};

#endif // __TITLE_H__
