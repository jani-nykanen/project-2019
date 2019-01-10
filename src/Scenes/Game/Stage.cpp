// Game stage
// (c) 2019 Jani Nykänen

#include "Stage.hpp"

// Constants
static const float BASE_TILE_SIZE = 128.0f;

// Bitmaps
static Bitmap* bmpWall;
static Bitmap* bmpBorders;
static Bitmap* bmpCog;


// Initialize global data
void initGlobalStage(AssetPack* assets) {

    // Get assets
    bmpWall = assets->getBitmap("wall");
    bmpBorders = assets->getBitmap("borders");
    bmpCog = assets->getBitmap("cog");
}


// Get a tile
int Stage::getTile(int x, int y) {

    if(x < 0 || y < 0 || x >= width || y >= height)
        return 1;

    return data[y*width +x];
}


// Draw walls
void Stage::drawWalls(Graphics* g) {

    const int s = BASE_TILE_SIZE;
    const int BORDER = 8;
    const float SHADOW = 16.0f;

    int x, y;
    int px, py;
    for(int i = 0; i < width*height; ++ i) {

        if(data[i] != 1) 
            continue;;

        x = i % width;
        y = i / width;

        px = x*s;
        py = y*s;

        // Draw shadow
        g->setColor(0.25f,0.125f,0.075f);
        g->fillRect(px+SHADOW, py+SHADOW,s,s);

        // Draw wall tile
        g->setColor();
        g->drawBitmap(bmpWall, px, py);
        
        // Draw black borders
        g->setColor(0, 0, 0);
        // Right
        if(getTile(x+1, y) != 1)
             g->fillRect(px+s-BORDER, py, BORDER, s);
        // Left
        if(getTile(x-1, y) != 1)
             g->fillRect(px, py, BORDER, s);
        // Bottom
        if(getTile(x, y+1) != 1)
             g->fillRect(px, py+s-BORDER, s, BORDER);
        // Top
        if(getTile(x, y-1) != 1)
             g->fillRect(px, py, s, BORDER);    

        // Corners
        // Bottom-right 
        if(getTile(x+1, y+1) != 1)
             g->fillRect(px+s-BORDER, py+s-BORDER, 
             BORDER, BORDER);
        // Bottom-left 
        if(getTile(x-1, y+1) != 1)
             g->fillRect(px, py+s-BORDER, 
             BORDER, BORDER);
        // Top-right 
        if(getTile(x+1, y-1) != 1)
             g->fillRect(px+s-BORDER, py, 
             BORDER, BORDER);
        // Top-left 
        if(getTile(x-1, y-1) != 1)
             g->fillRect(px, py, 
             BORDER, BORDER);
    }
}


// Draw borders
void Stage::drawBorders(Graphics *g) {

    const float borderSize = bmpBorders->getWidth() / 3;

    // Draw corners
    for(int x = 0; x <= 1; ++ x) {
        for(int y = 0; y <= 1; ++ y) {

            g->drawBitmap(bmpBorders, 0, 0, borderSize, borderSize,
                -borderSize + (baseWidth+borderSize)*x, 
                -borderSize + (baseHeight+borderSize)*y);
        }
    }

    // Draw horizontal
    g->drawBitmap(bmpBorders, borderSize+1, 0, 
                borderSize-2, borderSize,
                0, -borderSize, baseWidth, borderSize);
    g->drawBitmap(bmpBorders, borderSize+1, borderSize*2, 
                borderSize-2, borderSize,
                0, baseHeight, baseWidth, borderSize);

    // Draw vertical
    g->drawBitmap(bmpBorders, 0, borderSize+1, 
                borderSize, borderSize-2,
                -borderSize, 0, borderSize, baseHeight);
    g->drawBitmap(bmpBorders, borderSize*2, borderSize+1, 
                borderSize, borderSize-2,
                baseWidth, 0, borderSize, baseHeight);
}


// Draw shadow
void Stage::drawShadow(Graphics* g) {

    const float SHADOW_OFF = 32.0f;
    const float borderSize = bmpBorders->getWidth() / 3;

    g->setColor(0, 0, 0, 0.5f);
    g->fillRect(-borderSize + SHADOW_OFF, 
                -borderSize+SHADOW_OFF,
                baseWidth+borderSize*2, 
                baseHeight+borderSize*2);
}


// Draw a single cog
void Stage::drawCog(Graphics* g, float x, float y, 
    float scale, float angle) {

    g->push();
    g->translate(x, y);
    g->rotate(angle);
    g->scale(scale, scale);
    g->useTransf();
    g->drawBitmap(bmpCog,-128,-128,256,256);
    g->pop();
}


// Draw background cogs
void Stage::drawCogs(Graphics* g) {

    const float BOTTOM_RIGHT = 1.25f;
    const float BOTTOM_LEFT = 1.0f;
    const float TOP_RIGHT = 1.0f;
    const float SHADOW_X = 16.0f;
    const float SHADOW_Y = 16.0f;
    const float SHADOW_ALPHA = 0.5f;

    Vector2 view = g->getViewport();

    for(int i = 1; i >= 0; -- i) {

        if(i == 0)
            g->setColor();
        else
            g->setColor(0, 0, 0, SHADOW_ALPHA);

        // Bottom right
        drawCog(g, view.x + i *SHADOW_X, view.y + i*SHADOW_Y, 
            BOTTOM_RIGHT, cogAngle);
        // Bottom left
        drawCog(g,  SHADOW_X, view.y +  i *SHADOW_Y, 
            BOTTOM_LEFT, -cogAngle);
        // Top right
        drawCog(g, view.x+ i *SHADOW_X,  i *SHADOW_X, 
            TOP_RIGHT, -cogAngle);
    }

}


// Constructor
Stage::Stage(std::string mapPath) {

    // Load tilemap
    tmap = new Tilemap(mapPath);

    // Get information
    data = tmap->copyData();
    width = tmap->getWidth();
    height = tmap->getHeight();

    // Compute scale
    baseWidth = width * BASE_TILE_SIZE;
    baseHeight = height * BASE_TILE_SIZE;
    scale = 1.0f / ( ((height+2)*BASE_TILE_SIZE) / VIEW_HEIGHT );
    scaledWidth = scale * baseWidth;
    scaledHeight = scale * baseHeight;

    // Set defaults
    cogAngle = 0.0f;
}
// Desctructor
Stage::~Stage() {

    delete tmap;
}


// Update
void Stage::update(EventManager* evMan, float tm) {

    const float COG_SPEED = 0.05f;

    // Rotate cogs
    cogAngle += COG_SPEED * tm;
}


// Draw
void Stage::draw(Graphics* g) {

    // Get viewport
    Vector2 view = g->getViewport();

    // Draw cogs
    drawCogs(g);

    // Set view
    g->push();
    g->translate(view.x/2, view.y/2);
    g->scale(scale, scale);
    g->translate(-baseWidth/2, -baseHeight/2);
    g->useTransf();

    // Draw shadow
    drawShadow(g);

    // Clear to black
    g->setColor(0.45f, 0.25f, 0.15f);
    g->fillRect(0, 0, baseWidth, baseHeight);

    // Draw walls
    g->setColor();
    drawWalls(g);

    // Draw borders
    g->setColor();
    drawBorders(g);

    g->pop();
    g->useTransf();
}
