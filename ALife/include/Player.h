#pragma once

#include <allegro5/allegro.h>

#include"InputManager.h"

class Player
{
    public:
        Player();
        ~Player();

        void LoadContent();
        void UnloadContent();
        void Update(ALLEGRO_EVENT ev,InputManager input);
        void Draw (ALLEGRO_DISPLAY *display);

    private:
        ALLEGRO_BITMAP *playerImage;
        std::pair<float,float> position;
        float moveSpeed;
};

