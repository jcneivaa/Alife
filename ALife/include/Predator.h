#pragma once

#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <cmath>
#include <vector>
#include"Fish.h"

class Predator
{
    private:
        std::pair <float, float> position, speed;
        ALLEGRO_BITMAP* image;
        int vision;
        void Move();
        void SpeedLimit();

    public:
        Predator(float x, float y,ALLEGRO_BITMAP* image, float speedx,float speedy);
        ~Predator();

        void Draw(ALLEGRO_DISPLAY* display);
        void Behavior(std::vector <std::pair<float,float>> flock);
        int Eat(std::vector <std::pair<float,float>> flock);
        std::pair <float,float> getPosition();
};
