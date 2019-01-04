// Global scene
// (c) 2019 Jani Nykänen

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "../Core/Scene.hpp"

// Game scene
class Global : public Scene {

private:

    // ...

public:

    // Initialize scene
    void init();
    // Update scene
    void update(int steps);
    // Draw scene
    void draw(Graphics* g);
    // Dispose scene
    void dispose();
    
    // Get name
    inline std::string getName() {

        return "global";
    }
};



#endif // __GLOBAL_H__
