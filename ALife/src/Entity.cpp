#include "Entity.h"

Entity::Entity()
{
    //ctor
}

Entity::~Entity()
{
    //dtor
}

void Entity::LoadContent(std::vector<std::string> attributes, std::vector<std::string> contents)
{
    for (int x=0; x<attributes.size();++x){
        if (attributes[x]=="Image"){
            image = al_load_bitmap(contents[x].c_str());
        }else if (attributes[x]=="Position"){
            std::string tileString = contents[x];
            std::pair<int,int> tile;
            tile.first = atoi(tileString.substr(0,tileString.find(',')).c_str());
            tile.second = atoi(tileString.substr(tileString.find(',')+1).c_str());
            position= tile;
        }
    }

}

void Entity::UnloadContent()
{
    al_destroy_bitmap(image);
}
/*
void Entity::Update(ALLEGRO_EVENT ev, InputManager inputManager)
{

}
*/
void Entity::Draw (ALLEGRO_DISPLAY *display)
{
    al_draw_bitmap(image,position.first,position.second,0);
}
