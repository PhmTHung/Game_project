#include "xulygame.h"
#include "baseobj.h"
#include "basefunc.h"
#include "map.h"
#include "mainobj.h"
#include <iostream>

BaseObject gBackground;
GameMap Map;
MainObject player;

Game::Game(){};
Game::~Game(){};
void Game::InitData()
{
    //bool flag=true;
    //thiet lap mt ban dau
    int ret=SDL_Init(SDL_INIT_VIDEO);
    if(ret<0) std::cout<<"ERROR->Init"<<std::endl;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1");
    gWindow=SDL_CreateWindow("Game Window",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN);
     if(gWindow)
    {   std::cout<<"Create gWindow success"<<std::endl;
        gScreen=SDL_CreateRenderer(gWindow,-1,SDL_RENDERER_ACCELERATED);
        if(gScreen)
        {
            std::cout<<"Create gScreen success"<<std::endl;
            SDL_SetRenderDrawColor(gScreen,RENDER_DRAW_COLOR,RENDER_DRAW_COLOR,RENDER_DRAW_COLOR,RENDER_DRAW_COLOR);
            int imgFlags=IMG_INIT_PNG;
            if((IMG_Init(imgFlags)&&imgFlags)) std::cout<<"Create init image.PNG"<<std::endl;
            isRunning=true;
        }
    }
    //gBackground.LoadImage("image/dirt_03.png",gScreen);

    Map.LoadMap("map.txt");
    Map.LoadTiles(gScreen);
    player.LoadImage("image/left.png",gScreen);
    //player.LoadImage("player_right.png",gScreen);
    player.set_clips();
}
void Game::handleEvents()
{
    player.HandleInputAction(gEvent,gScreen);
}
void Game::render()
{
    SDL_RenderClear(gScreen);
    //Map.DrawMap(gScreen);
    gBackground.Render(gScreen);
    //player.FrameShow(gScreen);
    SDL_RenderPresent(gScreen);
}
void Game::close()
{
    SDL_DestroyWindow(gWindow);
    gWindow=NULL;

    SDL_DestroyRenderer(gScreen);
    gScreen=NULL;

    IMG_Quit();
    SDL_Quit();
    std::cout<<"game cleaned"<<std::endl;;
}

