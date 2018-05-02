#include "InputManager.h"

InputManager::InputManager()
{
    //ctor
}

InputManager::~InputManager()
{
    //dtor
}

bool InputManager::IsKeyPressed(ALLEGRO_EVENT ev, int key)
{
    if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
        if (ev.keyboard.keycode == key){
            return true;
        }
    }
    return false;
}

bool InputManager::IsKeyReleased(ALLEGRO_EVENT ev, int key)
{
    if (ev.type == ALLEGRO_EVENT_KEY_UP){
        if (ev.keyboard.keycode == key){
            return true;
        }
    }
    return false;
}
