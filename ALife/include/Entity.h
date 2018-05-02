#pragma once

#include <allegro5/allegro.h>
#include <vector>
#include <string>

class Entity
{
    public:
        Entity(void);
        ~Entity(void);

        void LoadContent(std::vector<std::string> attributes, std::vector<std::string> contents);
        void UnloadContent();
        //void Update(ALLEGRO_EVENT ev, InputManager inputManager);
        void Draw (ALLEGRO_DISPLAY *display);

        std::pair<float,float> position, prevPosition, velocity;

    protected:

        ALLEGRO_BITMAP *image;
};

