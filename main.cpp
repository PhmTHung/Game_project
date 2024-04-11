#include "basefunc.h"
#include "baseobj.h"
#include "mainobj.h"
#include "map.h"
#include "Threats.h"
#include "vukhi.h"
#include "FPS.h"
#include<iostream>


std::vector<Threats*> MakeThreatsList()
{
    std::vector<Threats*>list_threats;
    Threats* threat_obj=new Threats[5];
    for(int i=0;i<5;i++)
    {
        Threats* p_threat=(threat_obj+i);
        if(p_threat!=NULL)
        {
            p_threat->LoadImage("image/Threats/threatleft.png", gScreen);
            p_threat->LoadImage("image/Threats/threatright.png", gScreen);
            p_threat->LoadImage("image/Threats/threatforward.png", gScreen);
            p_threat->LoadImage("image/Threats/threatback.png", gScreen);
            p_threat->set_clips();
            p_threat->set_x_pos(i*100+50);
            p_threat->set_y_pos(i*120);

            Weapon* p_bullet=new Weapon();
            p_threat->InitBullet(p_bullet,gScreen);

            list_threats.push_back(p_threat);
        }
    }
    return list_threats;
}
//Game *game = NULL;
bool InitData()
{
    bool flag=true;
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
            flag=true;
        }
    }
    return flag;
}

int main (int argc,char* argv[])
{

    ImpTimer fps_timer;
    if(InitData()==false) return -1;
    GameMap G_Map;
    G_Map.LoadMap("map.txt");
    G_Map.LoadTiles(gScreen);

    MainObject player;
    player.LoadImage("image/MainCharacter/right.png",gScreen);
    player.LoadImage("image/MainCharacter/left.png",gScreen);
    player.LoadImage("image/MainCharacter/ahead.png",gScreen);
    player.LoadImage("image/MainCharacter/behind.png",gScreen);
    player.set_clips();

    std::vector<Threats*> threats_list = MakeThreatsList();

    //const int FPS = 30;
    //const int framDelay=1000/FPS;

    Uint32 frameStart;
    int frameTime;

    bool is_quit=false;
     while(!is_quit)
    {
        fps_timer.start();
        while(SDL_PollEvent(&gEvent)!=0)
        {
            if(gEvent.type==SDL_QUIT)
            {
                is_quit=true;
            }
            player.HandleInputAction(gEvent,gScreen);
        }

        SDL_SetRenderDrawColor(gScreen,RENDER_DRAW_COLOR,RENDER_DRAW_COLOR,RENDER_DRAW_COLOR,RENDER_DRAW_COLOR);
        SDL_RenderClear(gScreen);

        G_Map.DrawMap(gScreen);
        Map map_data=G_Map.getMap();

        player.HandleWeapon(gScreen);
        player.player_move(map_data);
        player.PlayerGPS(map_data);
        player.FrameShow(gScreen);
        player.DrawHPBar(gScreen);

       for (int i=0;i<threats_list.size();i++)
        {
            Threats* p_threat=threats_list.at(i);
            if(p_threat!=NULL)
            {
                p_threat->Threat_Move(map_data);
                p_threat->SetMapXY(map_data.start_x,map_data.start_y);
                p_threat->MakeBullet(gScreen,SCREEN_WIDTH,SCREEN_HEIGHT);
                p_threat->FrameShow(gScreen);
                p_threat->DrawHPBar(gScreen);
            }
        }

        std::vector<Weapon*> bullet_arr=player.get_weapon_list();
        for(int r=0;r<bullet_arr.size();r++)
        {
            Weapon* p_weapon=bullet_arr.at(r);
            if(p_weapon!=NULL)
            {
                for(int t=0;t<threats_list.size();t++)
                {
                    Threats* obj_threat=threats_list.at(t);
                    if(obj_threat!=NULL)
                    {
                        SDL_Rect tRect;
                        tRect.x=obj_threat->GetRect().x;
                        tRect.y=obj_threat->GetRect().y;
                        tRect.w=obj_threat->get_width_frame();
                        tRect.h=obj_threat->get_height_frame();

                        SDL_Rect bRect=p_weapon->GetRect();

                        bool bCol=SDLBaseFunc::CheckCollision(bRect,tRect);
                        if(bCol)
                        {
                            player.DeleteBullet(r);
                            obj_threat->Free();
                            threats_list.erase(threats_list.begin()+t);
                        }
                    }
                }
            }
        }

        SDL_RenderPresent(gScreen);

        int real_imp_time=fps_timer.get_ticks();
        int time_one_frame=1000/FRAME_PER_SECOND;

        if(real_imp_time<time_one_frame)
        {
            int delay_time=time_one_frame-real_imp_time;
            if(delay_time>=0)
            {
                 SDL_Delay(delay_time);
            }
        }
    }
    SDL_DestroyWindow(gWindow);
    gWindow=NULL;

    SDL_DestroyRenderer(gScreen);
    gScreen=NULL;

    IMG_Quit();
    SDL_Quit();

    return 0;
}

