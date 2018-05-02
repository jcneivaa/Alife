#include "Animation.h"

Animation::Animation()
{
    //ctor
}

Animation::~Animation()
{
    //dtor
}

void Animation::LoadContent(ALLEGRO_BITMAP *image, std::pair<float, float> position)
{
    this->image = image;
    this->position = position;

    alpha=255;
}

void Animation::UnloadContent()
{
    al_destroy_bitmap(image);
    alpha = NULL;
    position = std::pair<float,float>(0,0);
}

void Animation::Update(InputManager input)
{

}

void Animation::Draw(ALLEGRO_DISPLAY *display)
{
   if (image!=NULL){
       al_draw_tinted_bitmap(image,al_map_rgba(0,0,0,alpha),position.first,position.second,NULL);
   }
}

void Animation::SetAlpha(float alpha)
{
    this->alpha =alpha;
}

float Animation::GetAlpha()
{
    return alpha;
}
