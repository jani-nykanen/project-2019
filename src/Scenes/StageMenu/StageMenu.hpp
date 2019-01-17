// Stage menu scene
// (c) 2019 Jani Nykänen

#ifndef __STAGE_MENU_H__
#define __STAGE_MENU_H__

#include "../../Core/Scene.hpp"
#include "../../Core/Bitmap.hpp"
#include "../../Core/Tilemap.hpp"

#include "../Game/Stage.hpp"

#include "Grid.hpp"

// To be passed to player
struct StageInfo {
    Tilemap* tmap;
    int stageIndex;
};

// Stage menu class
class StageMenu : public Scene {

private:

    // Transition
    Transition* trans;

    // Bitmaps
    Bitmap* bmpFont;

    // Grid
    Grid stageGrid;

    // Maps
    std::vector<Tilemap> maps;
    // Map names
    std::vector<std::string> mapNames;
    // Map difficulties
    std::vector<int> mapDiff;
    // Map completion
    std::vector<int> completion;

    // Stage target
    int stageTarget;

    // Draw stage info
    void drawStageInfo(Graphics* g);

public: 

    // Go to the selected stage
    void goToStage();
    // Set stage target
    inline void setStageTarget(int index) {
        stageTarget = index;
    }
    // Fade to something
    void fadeToTarget(TransitionCallback cb);

    // Initialize scene
    void init();
    // Update scene
    void update(float tm);
    // Draw scene
    void draw(Graphics* g);
    // Dispose scene
    void dispose();
    // Called when the scene is changed
    // to this scene
    void onChange(void* param=NULL);

    // Get name
    inline std::string getName() {

        return "stageMenu";
    }
};

#endif // __STAGE_MENU_H__
