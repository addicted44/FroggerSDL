//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <cmath>

using namespace std;

//Main game variables
    double foo_pos_render_x = 200,  foo_pos_render_y = 435,
    car1_render_x = rand()%400, car1_render_y = 403,
    car2_render_x = rand()%400,
    car2_render_y = 370,
    car3_render_x = rand()%400,
    car3_render_y = 330,
    car4_render_x = rand()%400,
    car4_render_y = 290,
    car5_render_x = car1_render_x+80,
    car5_render_y = car1_render_y,
    car6_render_x = car1_render_x+160,
    car6_render_y = car1_render_y,
    car7_render_x = rand()%400, car7_render_y = 255,
    tree_small_render_x = rand()%400, tree_small_render_y = 85,
    tree_med_render_x = rand()%400, tree_med_render_y = 145,
    tree_big_render_x = rand()%400, tree_big_render_y = 200,
    turtle1_render_x = rand()%400, turtle1_render_y = 175,
    turtle2_render_x = turtle1_render_x +30, turtle2_render_y = turtle1_render_y,
    turtle3_render_x = turtle1_render_x +60, turtle3_render_y = turtle1_render_y,
    turtle4_render_x = rand()%400, turtle4_render_y = 113,
    turtle5_render_x = turtle4_render_x + 30, turtle5_render_y = turtle4_render_y,
    turtle6_render_x = turtle4_render_x + 60, turtle6_render_y = turtle4_render_y,
    explosion_render_x = 1000, explosion_render_y = 1000, velocity=.75;

    int frame = 0, frames = 0 ,   michaelbay=0, changed = 0,
    gameover = 0, restart =0,
    score = 0, lives = 3, victoryhouse1=0,victoryhouse2=0,victoryhouse3 = 0,victoryhouse4=0,extralife=0,
    carFrame = 0, turtleFrame =0, explosionFrame=0;

//Music variables
Mix_Music *music = NULL;
Mix_Chunk *jump = NULL;
Mix_Chunk *explosion = NULL;
Mix_Chunk *dumbways = NULL;
Mix_Chunk *victory = NULL;

//Screen dimension constants
const int SCREEN_WIDTH = 441;
const int SCREEN_HEIGHT = 525;

SDL_Color textColor = { 255, 255, 255 };

//Texture wrapper class
class LTexture
{
    public:
        //Initializes variables
        LTexture();

        //Deallocates memory
        ~LTexture();

        //Loads image at specified path
        bool loadFromFile( std::string path );

                //Creates image from font string
        bool loadFromRenderedText( std::string textureText, SDL_Color textColor );

        //Deallocates texture
        void free();

        //Renders texture at given point
        void render1( int x, int y );
        void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );


        //Gets image dimensions
        int getWidth();
        int getHeight();

    private:
        //The actual hardware texture
        SDL_Texture* mTexture;

        //Image dimensions
        int mWidth;
        int mHeight;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
const int WALKING_ANIMATION_FRAMES = 8;
const int DRIVING_ANIMATION_FRAMES = 3;
const int SWIMMING_ANIMATION_FRANES = 5;
const int EXPLOSION_ANIMATION_FRAMES = 16;

LTexture gFrogTextureUP, gFrogTextureDOWN, gFrogTextureLEFT, gFrogTextureRIGHT,
gSpriteSheetTexture, gCar1SpriteSheetTexture, gTurtleSpriteSheetTexture, gBackgroundTexture, gExplosionSpriteSheetTexture,
gCar1Texture, gCar2Texture, gCar3Texture, gCar4Texture, gCar5Texture, gCar7Texture,
gTreeSmallTexture, gTreeMedTexture, gTreeBigTexture, gTurtle1Texture;

SDL_Rect gSpriteClips[ WALKING_ANIMATION_FRAMES ];
SDL_Rect gCarSpriteClips [ DRIVING_ANIMATION_FRAMES ];
SDL_Rect gTurtleSpriteClips[ SWIMMING_ANIMATION_FRANES ];
SDL_Rect gExplosionSpriteClips[ EXPLOSION_ANIMATION_FRAMES];


//Globally used font
TTF_Font *gFont = NULL;

//Rendered texture
LTexture gTextTexture;
LTexture gTextTexture1;
LTexture gTextTexture2;
LTexture gTextTexture3;
LTexture gTextTexture4;
LTexture gHeartTexture;
LTexture gHouseTexture1, gHouseTexture2, gHouseTexture3, gHouseTexture4;
LTexture gFullHouseTexture,gEmptyHouseTexture;

bool valueInRange(int value, int min, int max)
{ return (value >= min) && (value <= max); }

bool checkCollision()
{
    bool xOverlap, yOverlap;
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;
    int leftC, rightC, topC, bottomC; // Променливи използвани за нещата, които движат жабата

    // Координати на жабата
    leftA = foo_pos_render_x+5;
    rightA = foo_pos_render_x + 30 ;
    topA = foo_pos_render_y;
    bottomA = foo_pos_render_y + 35;

    // Координати на колите
    leftB = car1_render_x +5;
    rightB = car1_render_x + 25;
    topB = car1_render_y + 5;
    bottomB = car1_render_y + 25;

    //Координати на голямото дърво
    leftC = tree_big_render_x ;
    rightC = tree_big_render_x + 190;
    topC = tree_big_render_y -10;
    bottomC = tree_big_render_y + 15;

    {xOverlap = valueInRange(leftA, leftC, rightC) ||
                    valueInRange(leftB, leftC, rightC);

    yOverlap = valueInRange(topA,topC, bottomC) ||
                    valueInRange(topB, topC, bottomC);
    if (xOverlap == true && yOverlap == true) {
            if (foo_pos_render_x <= 415){
                foo_pos_render_x += 0.5 * velocity;
            }
            return true;
        }
    }
    // Координати на средното дърво
        leftC = tree_med_render_x ;
    rightC = tree_med_render_x + 130;
    topC = tree_med_render_y -10;
    bottomC = tree_med_render_y + 15;

    {xOverlap = valueInRange(leftA, leftC, rightC) ||
                    valueInRange(leftB, leftC, rightC);

    yOverlap = valueInRange(topA,topC, bottomC) ||
                    valueInRange(topB, topC, bottomC);
    if (xOverlap == true && yOverlap == true) {
            if (foo_pos_render_x <= 415){
                foo_pos_render_x += 0.5 * velocity;
            }
            return true;
        }
    }
    //Координати на малкото дърво
        leftC = tree_small_render_x ;
    rightC = tree_small_render_x + 95;
    topC = tree_small_render_y -10;
    bottomC = tree_small_render_y + 15;

    {xOverlap = valueInRange(leftA, leftC, rightC) ||
                    valueInRange(leftB, leftC, rightC);

    yOverlap = valueInRange(topA,topC, bottomC) ||
                    valueInRange(topB, topC, bottomC);
    if (xOverlap == true && yOverlap == true) {
            if (foo_pos_render_x <= 415){
                foo_pos_render_x += 0.5 * velocity;
            }
            return true;
        }
    }
    // TURTLE COLLISION
        leftC = turtle1_render_x ;
    rightC = turtle1_render_x + 30;
    topC = turtle1_render_y -10;
    bottomC = turtle1_render_y  + 15;

    {xOverlap = valueInRange(leftA, leftC, rightC) ||
                    valueInRange(leftB, leftC, rightC);

    yOverlap = valueInRange(topA,topC, bottomC) ||
                    valueInRange(topB, topC, bottomC);

    if (xOverlap == true && yOverlap == true) {
                if (foo_pos_render_x <= 415){
                    foo_pos_render_x -= 0.75*velocity;
                }
                return true;
            }
    }
            leftC = turtle2_render_x ;
    rightC = turtle2_render_x + 30;
    topC = turtle2_render_y -10;
    bottomC = turtle2_render_y  + 15;

    {xOverlap = valueInRange(leftA, leftC, rightC) ||
                    valueInRange(leftB, leftC, rightC);

    yOverlap = valueInRange(topA,topC, bottomC) ||
                    valueInRange(topB, topC, bottomC);
    if (xOverlap == true && yOverlap == true) {
            if (foo_pos_render_x <= 415){
                foo_pos_render_x -= 0.75*velocity;
            }
            return true;
        }
    }
            leftC = turtle3_render_x ;
    rightC = turtle3_render_x + 30;
    topC = turtle3_render_y -10;
    bottomC = turtle3_render_y  + 15;

    {xOverlap = valueInRange(leftA, leftC, rightC) ||
                    valueInRange(leftB, leftC, rightC);

    yOverlap = valueInRange(topA,topC, bottomC) ||
                    valueInRange(topB, topC, bottomC);
    if (xOverlap == true && yOverlap == true) {
            if (foo_pos_render_x <= 415){
                foo_pos_render_x -= 0.75*velocity;
            }
            return true;
        }
    }

    // SECOND TURTLE COLLISION
    // CAREFUL !!!
            leftC = turtle4_render_x ;
    rightC = turtle4_render_x + 30;
    topC = turtle4_render_y -10;
    bottomC = turtle4_render_y  + 15;

    {xOverlap = valueInRange(leftA, leftC, rightC) ||
                    valueInRange(leftB, leftC, rightC);

    yOverlap = valueInRange(topA,topC, bottomC) ||
                    valueInRange(topB, topC, bottomC);
    if (xOverlap == true && yOverlap == true) {
            if (foo_pos_render_x <= 415){
                foo_pos_render_x -= 0.75*velocity;
            }
            return true;
        }
    }
            leftC = turtle5_render_x ;
    rightC = turtle5_render_x + 30;
    topC = turtle5_render_y -10;
    bottomC = turtle5_render_y  + 15;

    {xOverlap = valueInRange(leftA, leftC, rightC) ||
                    valueInRange(leftB, leftC, rightC);

    yOverlap = valueInRange(topA,topC, bottomC) ||
                    valueInRange(topB, topC, bottomC);
    if (xOverlap == true && yOverlap == true) {
            if (foo_pos_render_x <= 415){
                foo_pos_render_x -= 0.75*velocity;
            }
            return true;
        }
    }
            leftC = turtle6_render_x ;
    rightC = turtle6_render_x + 30;
    topC = turtle6_render_y -10;
    bottomC = turtle6_render_y  + 15;

    {xOverlap = valueInRange(leftA, leftC, rightC) ||
                    valueInRange(leftB, leftC, rightC);

    yOverlap = valueInRange(topA,topC, bottomC) ||
                    valueInRange(topB, topC, bottomC);
    if (xOverlap == true && yOverlap == true) {
            if (foo_pos_render_x <= 415){
                foo_pos_render_x -= 0.75*velocity;
            }
            return true;
        }
    }


    {xOverlap = valueInRange(leftA, leftB, rightB) ||
                    valueInRange(leftB, leftA, rightA);

    yOverlap = valueInRange(topA,topB, bottomB) ||
                    valueInRange(topB, topA, bottomA);
    if (xOverlap == true && yOverlap == true)
        return false;}

            leftB = car5_render_x +5;
    rightB = car5_render_x + 25;
    topB = car5_render_y+5;
    bottomB = car5_render_y + 25;

    {xOverlap = valueInRange(leftA, leftB, rightB) ||
                    valueInRange(leftB, leftA, rightA);

    yOverlap = valueInRange(topA,topB, bottomB) ||
                    valueInRange(topB, topA, bottomA);
    if (xOverlap == true && yOverlap == true)
        return false;}

    leftB = car6_render_x +5;
    rightB = car6_render_x + 25;
    topB = car6_render_y+5;
    bottomB = car6_render_y + 25;

    {xOverlap = valueInRange(leftA, leftB, rightB) ||
                    valueInRange(leftB, leftA, rightA);

    yOverlap = valueInRange(topA,topB, bottomB) ||
                    valueInRange(topB, topA, bottomA);
    if (xOverlap == true && yOverlap == true)
        return false;}

    leftB = car2_render_x +5;
    rightB = car2_render_x + 25;
    topB = car2_render_y +5;
    bottomB = car2_render_y + 25;

    {xOverlap = valueInRange(leftA, 57, 100);
     yOverlap = valueInRange(topA, 25, 85);
        if (xOverlap == true && yOverlap == true&&victoryhouse1==0) {
            gHouseTexture1 = gFullHouseTexture;
            score+=10000;
            victoryhouse1 =1;
            extralife=1;
        }
    }

    {xOverlap = valueInRange(leftA, 153, 196);
     yOverlap = valueInRange(topA, 25, 85);
        if (xOverlap == true && yOverlap == true&&victoryhouse2==0) {
            gHouseTexture2 = gFullHouseTexture;
            score+=10000;
            victoryhouse2=1;
            extralife=1;
        }
    }

    {xOverlap = valueInRange(leftA, 245, 295);
     yOverlap = valueInRange(topA, 25, 85);
        if (xOverlap == true && yOverlap == true&&victoryhouse3==0) {
            gHouseTexture3 = gFullHouseTexture;
            score+=10000;
            victoryhouse3=1;
            extralife=1;
        }
    }

    {xOverlap = valueInRange(leftA, 340, 390);
     yOverlap = valueInRange(topA, 25, 85);
        if (xOverlap == true && yOverlap == true&&victoryhouse4==0) {
            gHouseTexture4 = gFullHouseTexture;
            score+=10000;
            victoryhouse4=1;
            extralife=1;
        }
    }

    {xOverlap = valueInRange(leftA, leftB, rightB) ||
                    valueInRange(leftB, leftA, rightA);

    yOverlap = valueInRange(topA,topB, bottomB) ||
                    valueInRange(topB, topA, bottomA);
    if (xOverlap == true && yOverlap == true)
        return false;}


    leftB = car3_render_x +5;
    rightB = car3_render_x + 25;
    topB = car3_render_y +5;
    bottomB = car3_render_y + 25;

    {xOverlap = valueInRange(leftA, leftB, rightB) ||
                    valueInRange(leftB, leftA, rightA);

    yOverlap = valueInRange(topA,topB, bottomB) ||
                    valueInRange(topB, topA, bottomA);
    if (xOverlap == true && yOverlap == true)
        return false;}


    leftB = car4_render_x +5;
    rightB = car4_render_x + 25;
    topB = car4_render_y +5;
    bottomB = car4_render_y + 25;

    {xOverlap = valueInRange(leftA, leftB, rightB) ||
                    valueInRange(leftB, leftA, rightA);

    yOverlap = valueInRange(topA,topB, bottomB) ||
                    valueInRange(topB, topA, bottomA);
    if (xOverlap == true && yOverlap == true)
        return false;}

    leftB = car7_render_x +5;
    rightB = car7_render_x + 25;
    topB = car7_render_y +15;
    bottomB = car7_render_y + 25;

    {xOverlap = valueInRange(leftA, leftB, rightB) ||
                    valueInRange(leftB, leftA, rightA);

    yOverlap = valueInRange(topA,topB, bottomB) ||
                    valueInRange(topB, topA, bottomA);
    if (xOverlap == true && yOverlap == true)
        return false;}

        //opasna voda
        /*if (water_y > topA){
                return false;
        }*/

    //If none of the sides from A are outside B
    return true;

}

void update()
{
    if(victoryhouse1 == 1 && victoryhouse2 == 1 && victoryhouse3 == 1 && victoryhouse4 == 1)
        gameover=-1;
    bool checker = checkCollision();
    if (checker == false)
    {
    explosion_render_x = foo_pos_render_x;
    explosion_render_y = foo_pos_render_y;
    michaelbay = 1;
    Mix_PlayChannel( -1, explosion, 0 );
    if (extralife == 1 && lives == 3) {
        extralife = 0;
    }
    lives--;
    if(extralife == 1 && lives < 3)
    {lives++; extralife = 0;}
    foo_pos_render_x = 200;
    foo_pos_render_y = 435;
    frames = 0;
    car1_render_x = rand()%400;
    car2_render_x = rand()%400;
    car3_render_x = rand()%400;
    car4_render_x = rand()%400;
    car5_render_x = car1_render_x+80;
    car6_render_x = car1_render_x+160;
    car7_render_x = rand()%400;
    car7_render_y= 255;
    turtle1_render_x = rand()%400;
    turtle2_render_x = turtle1_render_x + 30;
    turtle3_render_x = turtle1_render_x + 60;
    turtle4_render_x = rand()%400;
    turtle4_render_y = 113;
    turtle5_render_x = turtle4_render_x + 30;
    turtle6_render_x = turtle4_render_x + 60;
    }
    if(gameover == 1)
    {
    Mix_PlayChannel( 3, dumbways, 0 );
    Mix_PauseMusic();
    foo_pos_render_x = 200;
    foo_pos_render_y = 435;
    tree_small_render_x = rand()%400;
    tree_small_render_y = 85;
    tree_med_render_x = rand()%400;
    tree_med_render_y = 145;
    tree_big_render_x = rand()%400;
    tree_big_render_y = 200;
    turtle1_render_x = rand()%400;
    turtle2_render_x = turtle1_render_x + 30;
    turtle3_render_x = turtle1_render_x + 60;
    turtle4_render_x = rand()%400;
    turtle5_render_x = turtle4_render_x + 30;
    turtle6_render_x = turtle4_render_x + 60;

    victoryhouse1 =0;
    victoryhouse2 =0;
    victoryhouse3 =0;
    victoryhouse4 =0;
    gHouseTexture1=gEmptyHouseTexture;
    gHouseTexture2=gEmptyHouseTexture;
    gHouseTexture3=gEmptyHouseTexture;
    gHouseTexture4=gEmptyHouseTexture;

    frames = 0;
    car1_render_x = rand()%400;
    car2_render_x = rand()%400;
    car3_render_x = rand()%400;
    car4_render_x = rand()%400;
    car5_render_x = car1_render_x+80;
    car6_render_x = car1_render_x+160;
    car7_render_x = rand()%400;
    car7_render_y = 255;
    SDL_Delay(5000);
    lives = 3;
    gameover = 0;
    }

    if(gameover == -1)
    {
    Mix_PlayChannel( 3, victory, 0 );
    Mix_PauseMusic();
    foo_pos_render_x = 200;
    foo_pos_render_y = 435;
    tree_small_render_x = rand()%400;
    tree_small_render_y = 85;
    tree_med_render_x = rand()%400;
    tree_med_render_y = 145;
    tree_big_render_x = rand()%400;
    tree_big_render_y = 200;
    turtle1_render_x = rand()%400;
    turtle2_render_x = turtle1_render_x + 30;
    turtle3_render_x = turtle1_render_x + 60;
    turtle4_render_x = rand()%400;
    turtle5_render_x = turtle4_render_x + 30;
    turtle6_render_x = turtle4_render_x + 60;

    victoryhouse1 =0;
    victoryhouse2 =0;
    victoryhouse3 =0;
    victoryhouse4 =0;
    gHouseTexture1=gEmptyHouseTexture;
    gHouseTexture2=gEmptyHouseTexture;
    gHouseTexture3=gEmptyHouseTexture;
    gHouseTexture4=gEmptyHouseTexture;

    frames = 0;
    car1_render_x = rand()%400;
    car2_render_x = rand()%400;
    car3_render_x = rand()%400;
    car4_render_x = rand()%400;
    car5_render_x = car1_render_x+80;
    car6_render_x = car1_render_x+160;
    car7_render_x = rand()%400;
    car7_render_y = 255;
    SDL_Delay(5000);
    lives = 3;
    gameover = 0;
    }

    if (frames > 250)
    Mix_ResumeMusic();
    if(frames!=0)
    if(frames % 60)
    {
       char buffer [5];

        gTextTexture1.loadFromRenderedText( itoa(frames/60,buffer,10), textColor );
    }
    if(car1_render_x >= 0)
     car1_render_x -= 0.5*velocity;
     else car1_render_x = 441+rand()%20;

         if(car5_render_x >= 0)
     car5_render_x -= 0.5*velocity;
     else car5_render_x = car1_render_x+80;

         if(car6_render_x >= 0)
     car6_render_x -= 0.5*velocity;
     else car6_render_x = car1_render_x+160;

    if(car2_render_x >= 0)
     car2_render_x -= 0.6*velocity;
     else car2_render_x = 441+rand()%20;

     if(car3_render_x <= 441)
     car3_render_x += 0.8*velocity;
     else car3_render_x = -20-rand()%20;

     if(car4_render_x >= 0)
     car4_render_x -= 1*velocity;
     else car4_render_x = 441+rand()%20;

      if(car7_render_x <= 441)
     car7_render_x += 2*velocity;
     else car7_render_x = -50-rand()%20;

     if(tree_small_render_x <= 441)
     tree_small_render_x += 0.5*velocity;
     else tree_small_render_x = -170-rand()%20;

      if(tree_med_render_x <= 441)
     tree_med_render_x += 0.5*velocity;
     else tree_med_render_x = -170-rand()%20;

     if(tree_big_render_x <= 441)
     tree_big_render_x += 0.5*velocity;
     else tree_big_render_x = -170-rand()%20;

     if(turtle1_render_x >= 0)
     turtle1_render_x -= 0.75*velocity;
     else turtle1_render_x = 441+rand()%20;

       if(turtle2_render_x >= 0)
     turtle2_render_x -= 0.75*velocity;
     else turtle2_render_x = turtle1_render_x + 30;

      if(turtle3_render_x >= 0)
     turtle3_render_x -= 0.75*velocity;
     else turtle3_render_x = turtle1_render_x + 60;

     if(turtle4_render_x >= 0)
     turtle4_render_x -= 0.75*velocity;
     else turtle4_render_x = 441+rand()%20;

     if(turtle5_render_x >= 0)
     turtle5_render_x -= 0.75*velocity;
     else turtle5_render_x = turtle4_render_x+30;

     if(turtle6_render_x >= 0)
     turtle6_render_x -= 0.75*velocity;
     else turtle6_render_x = turtle4_render_x+60;

      frames++;
    if(frames == 6000 || lives == 0)
    {
    gameover = 1;
    score = 0;
    }

}

LTexture::LTexture()
{
    //Initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture()
{
    //Deallocate
    free();
}
void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };

    //Set clip rendering dimensions
    if( clip != NULL )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

bool LTexture::loadFromFile( std::string path )
{
    //Get rid of preexisting texture
    free();

    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Color key image
        SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    //Return success
    mTexture = newTexture;
    return mTexture != NULL;
}

bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
    //Get rid of preexisting texture
    free();

    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
        if( mTexture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }

    //Return success
    return mTexture != NULL;
}

void LTexture::free()
{
    //Free texture if it exists
    if( mTexture != NULL )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::render1( int x, int y )
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };
    SDL_RenderCopy( gRenderer, mTexture, NULL, &renderQuad );
}

int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}

bool init()
{
    //Initialization flag
    bool success = true;
    // Initialize SDL.
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
        return -1;

    //Initialize SDL_mixer
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 4, 4096 ) == -1 )
        return -1;

                         //Initialize SDL_ttf
                if( TTF_Init() == -1 )
                {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                    success = false;
                }
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }


    else
    {
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }

        //Create window
        gWindow = SDL_CreateWindow( "Frogger 2.3", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create renderer for window
            //Create vsynced renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
            if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 255, 0, 0, 255 );

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;


    //Open the font
    gFont = TTF_OpenFont( "game_over.ttf", 75 );

    if( gFont == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    else
    {
        //Render text

        if( !gTextTexture.loadFromRenderedText( "GAME OVER", textColor ) )
        {
            printf( "Failed to render text texture!\n" );
            success = false;
        }
    }


//Load sprite sheet texture
    if( !gSpriteSheetTexture.loadFromFile( "frogsprites.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    else
    {
        //Set sprite clips
        gSpriteClips[ 0 ].x =   0;
        gSpriteClips[ 0 ].y =   0;
        gSpriteClips[ 0 ].w =  33;
        gSpriteClips[ 0 ].h =  35;

        gSpriteClips[ 1 ].x =  33;
        gSpriteClips[ 1 ].y =   0;
        gSpriteClips[ 1 ].w =  33;
        gSpriteClips[ 1 ].h =  35;

        gSpriteClips[ 2 ].x = 66;
        gSpriteClips[ 2 ].y =   0;
        gSpriteClips[ 2 ].w =  33;
        gSpriteClips[ 2 ].h = 35;

        gSpriteClips[ 3 ].x = 99;
        gSpriteClips[ 3 ].y =   0;
        gSpriteClips[ 3 ].w =  33;
        gSpriteClips[ 3 ].h = 35;

        gSpriteClips[ 4 ].x =   0;
        gSpriteClips[ 4 ].y =  35;
        gSpriteClips[ 4 ].w =  33;
        gSpriteClips[ 4 ].h =  35;

        gSpriteClips[ 5 ].x =  33;
        gSpriteClips[ 5 ].y =   35;
        gSpriteClips[ 5 ].w =  33;
        gSpriteClips[ 5 ].h =  35;

        gSpriteClips[ 6 ].x = 66;
        gSpriteClips[ 6 ].y =  35;
        gSpriteClips[ 6 ].w =  33;
        gSpriteClips[ 6 ].h = 35;

        gSpriteClips[ 7 ].x = 99;
        gSpriteClips[ 7 ].y =   35;
        gSpriteClips[ 7 ].w =  33;
        gSpriteClips[ 7 ].h = 35;
    }

        if( !gExplosionSpriteSheetTexture.loadFromFile( "explosionsprites.png" ) )
    {
        printf( "Failed to load explosion animation texture!\n" );
        success = false;
    }
    else
    {
        //Set sprite clips
        gExplosionSpriteClips[ 0 ].x =   0;
        gExplosionSpriteClips[ 0 ].y =   0;
        gExplosionSpriteClips[ 0 ].w =  32;
        gExplosionSpriteClips[ 0 ].h =  32;

        gExplosionSpriteClips[ 1 ].x =  32;
        gExplosionSpriteClips[ 1 ].y =   0;
        gExplosionSpriteClips[ 1 ].w =  32;
        gExplosionSpriteClips[ 1 ].h =  32;

        gExplosionSpriteClips[ 2 ].x = 64;
        gExplosionSpriteClips[ 2 ].y =   0;
        gExplosionSpriteClips[ 2 ].w = 32;
        gExplosionSpriteClips[ 2 ].h = 32;

        gExplosionSpriteClips[ 3 ].x = 96;
        gExplosionSpriteClips[ 3 ].y =   0;
        gExplosionSpriteClips[ 3 ].w =  32;
        gExplosionSpriteClips[ 3 ].h = 32;

        gExplosionSpriteClips[ 4 ].x =   0;
        gExplosionSpriteClips[ 4 ].y =  32;
        gExplosionSpriteClips[ 4 ].w =  32;
        gExplosionSpriteClips[ 4 ].h =  32;

        gExplosionSpriteClips[ 5 ].x =  32;
        gExplosionSpriteClips[ 5 ].y =   32;
        gExplosionSpriteClips[ 5 ].w =  32;
        gExplosionSpriteClips[ 5 ].h =  32;

        gExplosionSpriteClips[ 6 ].x = 64;
        gExplosionSpriteClips[ 6 ].y =  32;
        gExplosionSpriteClips[ 6 ].w =  32;
        gExplosionSpriteClips[ 6 ].h = 32;

        gExplosionSpriteClips[ 7 ].x = 96;
        gExplosionSpriteClips[ 7 ].y =   32;
        gExplosionSpriteClips[ 7 ].w =  32;
        gExplosionSpriteClips[ 7 ].h = 32;

        gExplosionSpriteClips[ 8 ].x =   0;
        gExplosionSpriteClips[ 8 ].y =   64;
        gExplosionSpriteClips[ 8 ].w =  32;
        gExplosionSpriteClips[ 8 ].h =  32;

        gExplosionSpriteClips[ 9 ].x =  32;
        gExplosionSpriteClips[ 9 ].y =   64;
        gExplosionSpriteClips[ 9 ].w =  32;
        gExplosionSpriteClips[ 9 ].h =  32;

        gExplosionSpriteClips[ 10 ].x = 64;
        gExplosionSpriteClips[ 10 ].y =   64;
        gExplosionSpriteClips[ 10 ].w =  32;
        gExplosionSpriteClips[ 10 ].h = 32;

        gExplosionSpriteClips[ 11 ].x = 96;
        gExplosionSpriteClips[ 11 ].y =   64;
        gExplosionSpriteClips[ 11 ].w =  32;
        gExplosionSpriteClips[ 11 ].h = 32;

        gExplosionSpriteClips[ 12 ].x =   0;
        gExplosionSpriteClips[ 12 ].y =  96;
        gExplosionSpriteClips[ 12 ].w =  32;
        gExplosionSpriteClips[ 12 ].h =  32;

        gExplosionSpriteClips[ 13 ].x =  32;
        gExplosionSpriteClips[ 13 ].y =   96;
        gExplosionSpriteClips[ 13 ].w =  32;
        gExplosionSpriteClips[ 13 ].h =  32;

        gExplosionSpriteClips[ 14 ].x = 64;
        gExplosionSpriteClips[ 14 ].y =  96;
        gExplosionSpriteClips[ 14 ].w =  32;
        gExplosionSpriteClips[ 14 ].h = 32;

        gExplosionSpriteClips[ 15 ].x = 96;
        gExplosionSpriteClips[ 15 ].y =   96;
        gExplosionSpriteClips[ 15 ].w =  32;
        gExplosionSpriteClips[ 15 ].h = 32;
    }

    if( !gTurtleSpriteSheetTexture.loadFromFile( "turtle_spritesheets.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    else
    {
        //Set sprite clips
        gTurtleSpriteClips[ 0 ].x =   0;
        gTurtleSpriteClips[ 0 ].y =   0;
        gTurtleSpriteClips[ 0 ].w =  33;
        gTurtleSpriteClips[ 0 ].h =  23;

        gTurtleSpriteClips[ 1 ].x =  40;
        gTurtleSpriteClips[ 1 ].y =   0;
        gTurtleSpriteClips[ 1 ].w =  33;
        gTurtleSpriteClips[ 1 ].h =  23;

        gTurtleSpriteClips[ 2 ].x = 80;
        gTurtleSpriteClips[ 2 ].y =   0;
        gTurtleSpriteClips[ 2 ].w =  33;
        gTurtleSpriteClips[ 2 ].h = 23;

        gTurtleSpriteClips[ 3 ].x = 120;
        gTurtleSpriteClips[ 3 ].y =   0;
        gTurtleSpriteClips[ 3 ].w =  33;
        gTurtleSpriteClips[ 3 ].h = 23;

        gTurtleSpriteClips[ 4 ].x =   164;
        gTurtleSpriteClips[ 4 ].y =  0;
        gTurtleSpriteClips[ 4 ].w =  33;
        gTurtleSpriteClips[ 4 ].h =  23;
    }

    if( !gCar1SpriteSheetTexture.loadFromFile( "carsprites.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    else
    {
        //Set sprite clips
        gCarSpriteClips[ 0 ].x = 6;
        gCarSpriteClips[ 0 ].y = 0;
        gCarSpriteClips[ 0 ].w = 31;
        gCarSpriteClips[ 0 ].h = 24;

        gCarSpriteClips[ 1 ].x = 43;
        gCarSpriteClips[ 1 ].y = 0;
        gCarSpriteClips[ 1 ].w = 31;
        gCarSpriteClips[ 1 ].h = 24;

        gCarSpriteClips[ 2 ].x = 77;
        gCarSpriteClips[ 2 ].y = 0;
        gCarSpriteClips[ 2 ].w = 31;
        gCarSpriteClips[ 2 ].h = 24;

    }



    //Load background texture
    if( !gBackgroundTexture.loadFromFile( "background_frogger.png" ) )
    {
        printf( "Failed to load background texture image!\n" );
        success = false;
    }
        if( !gTreeSmallTexture.loadFromFile( "tree_small.png" ) )
    {
        printf( "Failed to load  texture image!\n" );
        success = false;
    }
        if( !gTreeMedTexture.loadFromFile( "tree_med.png" ) )
    {
        printf( "Failed to load  texture image!\n" );
        success = false;
    }
        if( !gTreeBigTexture.loadFromFile( "tree_big.png" ) )
    {
        printf( "Failed to load  texture image!\n" );
        success = false;
    }

    if( !gCar1Texture.loadFromFile( "car1.png" ) )
    {
        printf( "Failed to load  texture image!\n" );
        success = false;
    }
    if( !gCar2Texture.loadFromFile( "car2.png" ) )
    {
        printf( "Failed to load  texture image!\n" );
        success = false;
    }
    if( !gCar3Texture.loadFromFile( "car3.png" ) )
    {
        printf( "Failed to load  texture image!\n" );
        success = false;
    }
    if( !gCar4Texture.loadFromFile( "car4.png" ) )
    {
        printf( "Failed to load  texture image!\n" );
        success = false;
    }
    if( !gCar7Texture.loadFromFile( "golf.png" ) )
    {
        printf( "Failed to load  texture image!\n" );
        success = false;
    }

    if( !gHeartTexture.loadFromFile( "heart.png" ) )
    {
        printf( "Failed to load  texture image!\n" );
        success = false;
    }

    if( !gHouseTexture1.loadFromFile( "Empty_house3.png" ) )
    {
        printf( "Failed to load  texture image!\n" );
        success = false;
    }
    else{
        gHouseTexture2 = gHouseTexture1;
        gHouseTexture3 = gHouseTexture1;
        gHouseTexture4 = gHouseTexture1;
    }

    if( !gFullHouseTexture.loadFromFile( "Full_house3.png" ) )
    {
        printf( "Failed to load  texture image!\n" );
        success = false;
    }

        if( !gEmptyHouseTexture.loadFromFile( "Empty_house3.png" ) )
    {
        printf( "Failed to load  texture image!\n" );
        success = false;
    }

    music = Mix_LoadMUS("test.ogg");
    jump = Mix_LoadWAV("jump_sound.wav");
    explosion = Mix_LoadWAV("explosion.wav");
    dumbways = Mix_LoadWAV("dumbways.wav");
    victory = Mix_LoadWAV("victory.wav");
    if (music == NULL)
        return -1;

            if ( Mix_PlayMusic( music, -1) == -1 )
        return -1;
    return success;
}

void close()
{
    //Free loaded images
    gFrogTextureUP.free();
    gBackgroundTexture.free();
    gCar1Texture.free();
    gCar2Texture.free();
    gCar3Texture.free();
    gCar4Texture.free();
    gCar5Texture.free();
    gCar7Texture.free();
    gTreeSmallTexture.free();
    gTreeMedTexture.free();
    gTreeBigTexture.free();
    gTurtle1Texture.free();
        //Free global font
    TTF_CloseFont( gFont );
    gFont = NULL;

    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    Mix_FreeMusic(music);
    Mix_CloseAudio();

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

int main( int argc, char* args[] )
{

    //Start up SDL and create window
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            //Main loop flag
            bool quit = false;

            //Event handler
            SDL_Event e;

            //While application is running
            while( !quit )
            {
                update();
                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {

//                  if(!quit){

                  //  }
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }

                    if (e.type == SDL_KEYDOWN) {

                        switch (e.key.keysym.sym) {

                            case SDLK_a:
                                if (foo_pos_render_x >= 0) {
                                    foo_pos_render_x -= 10;
                                    ++frame;
                                    if (frame > 3 ||frame < 2) {
                                        frame = 2;
                                    }
                                    Mix_PlayChannel( -1, jump, 0 );
                                }
                                break;

                            case SDLK_s:
                                if (foo_pos_render_y <= 435) {
                                    foo_pos_render_y += 10;
                                    ++frame;
                                    if (frame > 7 || frame < 6) {
                                        frame = 6;
                                    }
                                    Mix_PlayChannel( -1, jump, 0 );
                                    if(score > 0)
                                    score -=10;
                                }
                                break;

                            case SDLK_d:
                                if (foo_pos_render_x <= 415){
                                    foo_pos_render_x += 10;
                                    ++frame;
                                    if (frame > 1) {
                                        frame = 0;
                                    }
                                    Mix_PlayChannel( -1, jump, 0 );
                                }
                                break;

                            case SDLK_w:
                                if (foo_pos_render_y > 5) {
                                   {
                                       foo_pos_render_y -= 10;
                                       score +=10;
                                   }
                                    ++frame;
                                    if (frame > 5 || frame < 4) {
                                        frame = 4;
                                    }
                                    Mix_PlayChannel( -1, jump, 0 );
                                }
                                break;

                        }
                    }
                }

                //Clear screen
                SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 255 );
                SDL_RenderClear( gRenderer );

                //Render background texture to screen
                gBackgroundTexture.render1( 0, 0 );

                gHouseTexture1.render(57, 25);
                gHouseTexture2.render(153, 25);
                gHouseTexture3.render(245, 25);
                gHouseTexture4.render(340, 25);


                //Render Foo' to the screen
                //gCar1Texture.render( car1_render_x, car1_render_y );
                gTreeSmallTexture.render(tree_small_render_x, tree_small_render_y);
                gTreeMedTexture.render(tree_med_render_x, tree_med_render_y);
                gTreeBigTexture.render(tree_big_render_x, tree_big_render_y);
               // gTurtle1Texture.render(turtle1_render_x, turtle1_render_y);
                gCar2Texture.render( car2_render_x, car2_render_y );
                gCar3Texture.render( car3_render_x, car3_render_y );
                gCar4Texture.render( car4_render_x, car4_render_y );
                gCar7Texture.render( car7_render_x, car7_render_y );
                //Render current frame


                SDL_Rect* currentCarClip = &gCarSpriteClips[ carFrame%3 ];
                gCar1SpriteSheetTexture.render( car1_render_x, car1_render_y, currentCarClip );
                if(frames %30 == 0)
                carFrame++;

                currentCarClip = &gCarSpriteClips[ (carFrame+1)%3 ];
                gCar1SpriteSheetTexture.render( car5_render_x, car5_render_y, currentCarClip );

                currentCarClip = &gCarSpriteClips[ (carFrame+2)%3 ];
                gCar1SpriteSheetTexture.render( car6_render_x, car6_render_y, currentCarClip );

                SDL_Rect* currentTurtleClip = &gTurtleSpriteClips[ turtleFrame%5 ];
                gTurtleSpriteSheetTexture.render( turtle1_render_x, turtle1_render_y, currentTurtleClip );
                if(frames %30 == 0)
                turtleFrame++;

                currentTurtleClip = &gTurtleSpriteClips[ (turtleFrame)%5 ];
                gTurtleSpriteSheetTexture.render(turtle2_render_x, turtle2_render_y, currentTurtleClip);

                currentTurtleClip = &gTurtleSpriteClips[ (turtleFrame)%5 ];
                gTurtleSpriteSheetTexture.render(turtle3_render_x, turtle3_render_y, currentTurtleClip);

                currentTurtleClip = &gTurtleSpriteClips[ (turtleFrame+1)%5 ];
                gTurtleSpriteSheetTexture.render(turtle4_render_x, turtle4_render_y, currentTurtleClip);

                currentTurtleClip = &gTurtleSpriteClips[ (turtleFrame+1)%5 ];
                gTurtleSpriteSheetTexture.render(turtle5_render_x, turtle5_render_y, currentTurtleClip);

                currentTurtleClip = &gTurtleSpriteClips[ (turtleFrame+1)%5 ];
                gTurtleSpriteSheetTexture.render(turtle6_render_x, turtle6_render_y, currentTurtleClip);

                if(michaelbay == 1)
                {
                SDL_Rect* currentClip = &gSpriteClips[ frame % 8 ];
                gSpriteSheetTexture.render( foo_pos_render_x+1000, foo_pos_render_y+1000, currentClip );
                }
                else
                {
                SDL_Rect* currentClip = &gSpriteClips[ frame % 8 ];
                gSpriteSheetTexture.render( foo_pos_render_x, foo_pos_render_y, currentClip );
                }

                if(gameover == 1)
                {
                        gTextTexture.loadFromRenderedText( "GAME OVER", textColor );
                        gTextTexture.render( ( SCREEN_WIDTH - gTextTexture.getWidth() ) / 2, ( SCREEN_HEIGHT - gTextTexture.getHeight() ) / 2 );
                        restart =1;

                }
                if(gameover == -1)
                {
                        gTextTexture.loadFromRenderedText( "VICTORY", textColor );
                        gTextTexture.render( ( SCREEN_WIDTH - gTextTexture.getWidth() ) / 2, ( SCREEN_HEIGHT - gTextTexture.getHeight() ) / 2 );
                        restart =1;

                }
                if(michaelbay == 1)
                {
                SDL_Rect* currentExplosion = &gSpriteClips[ explosionFrame % 16 ];
                gExplosionSpriteSheetTexture.render(explosion_render_x,explosion_render_y,currentExplosion);
                explosionFrame++;
                if(explosionFrame == 16) {michaelbay = 0;explosion_render_x = 1000; explosion_render_y = 1000; explosionFrame=0; changed = 0;}
                }


                char buffer[100];
                gTextTexture1.render( 210, 470);
                gTextTexture2.loadFromRenderedText(itoa(score,buffer,10),textColor);
                gTextTexture2.render( SCREEN_WIDTH-gTextTexture2.getWidth()-5, 470);

                if (lives == 3)
                {
                gHeartTexture.render(-5, 470);
                gHeartTexture.render(25, 470);
                gHeartTexture.render(55, 470);
                }
                                if (lives == 2)
                {
                gHeartTexture.render(-5, 470);
                gHeartTexture.render(25, 470);
                }
                                if (lives == 1)
                {

                gHeartTexture.render(-5, 470);
                }
                                if (lives == 0)
                {
                gTextTexture3.loadFromRenderedText("DEAD",textColor);
                gTextTexture3.render( 5, gTextTexture1.getHeight()+5);
                }

                //Update screen
                SDL_RenderPresent( gRenderer );


                //Update screen
                SDL_RenderPresent( gRenderer );
            }
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}
