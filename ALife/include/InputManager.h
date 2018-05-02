#pragma once

#include <allegro5/allegro.h>
#include <allegro5/keyboard.h>
#include <vector>

class InputManager
{
    public:
        InputManager();
        ~InputManager();

        bool IsKeyPressed(ALLEGRO_EVENT ev, int key);
        //void IsKeyPressed(ALLEGRO_EVENT ev, std::vector<int> keys);
        bool IsKeyReleased(ALLEGRO_EVENT ev, int key);


    private:
        ALLEGRO_KEYBOARD_STATE keyState;
};
