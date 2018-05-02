#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <string>

#include "InputManager.h"

class Animation
{
    public:
        Animation();
        ~Animation();

        virtual void LoadContent(ALLEGRO_BITMAP *image, std::pair<float,float> position);
        virtual void UnloadContent();
        virtual void Update(InputManager input);
        void Draw(ALLEGRO_DISPLAY *display);

        virtual void SetAlpha(float value);
        float GetAlpha();

    protected:
        ALLEGRO_BITMAP *image;
        std::string text;
        std::pair<float,float> position;
        float alpha;

    private:
};

