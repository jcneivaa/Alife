#pragma once

#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <cmath>
#include <vector>

class Fish
{
    private:
        std::pair <float, float> position, speed,acceleration;
        ALLEGRO_BITMAP* image;
        int vision;
        void SpeedLimit();
        std::pair<float,float> SteerForceLimit(std::pair <float,float> valor);
        void Move();


    public:
        Fish(float x, float y,ALLEGRO_BITMAP* image, float speedx,float speedy);
        ~Fish();

        void Draw(ALLEGRO_DISPLAY* display);
        void Behavior(std::vector <Fish> flock);

};


