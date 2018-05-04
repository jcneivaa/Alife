#include "Predator.h"

Predator::Predator(float x, float y,ALLEGRO_BITMAP* image, float speedx,float speedy)
{
    this->position.first = x;
    this->position.second = y;
    this->image = image;
    this->speed.first=speedx;
    this->speed.second=speedy;
    this->vision = 85;
}

Predator::~Predator()
{
    //dtor
}


void Predator::Draw(ALLEGRO_DISPLAY* display)
{
    Predator::Move();
    al_draw_bitmap(image,position.first,position.second,0);

}
void Predator::Behavior(std::vector <std::pair<float,float>> flock)
{
    int boidCount = 0;
    std::pair <float,float> boids (0,0);

     for (int i=0;i<flock.size();++i){
        float dist;

        float dx = flock[i].first - position.first;
        float dy = flock[i].second - position.second;

        dist = std::sqrt(dx*dx + dy*dy);

        if (dist>0 && dist<=vision){

            boids.first += flock[i].first;
            boids.second += flock[i].second;
            boidCount++;

        }
    }

    if (boidCount>0){
        boids.first/=boidCount;
        boids.second/=boidCount;
        boids.first-=position.first;
        boids.second-=position.second;
        boids.first*=0.1;
        boids.second*=0.1;

        speed.first += (boids.first);
        speed.second+= (boids.second);
    }


}

void Predator::Move()
{

    Predator::SpeedLimit();
    position.first = int((position.first + speed.first + 1500))%1500;
    position.second = int((position.second + speed.second + 780))%780;
}

void Predator::SpeedLimit()
{
    float aux;
    aux = std::sqrt(speed.first*speed.first + speed.second*speed.second);
    speed.first /=aux;
    speed.second/=aux;
    speed.first*=4;
    speed.second*=4;
    if (speed.first <0){
        speed.first-=1;
    }else{
        speed.first+=1;
    }
    if (speed.second<0){
        speed.second-=1;
    }else{
        speed.second+=1;
    }

    if (std::isnan(speed.first)){
        speed.first  = -1;
    }
    if (std::isnan(speed.second)){
        speed.second = -1;
    }
}

int Predator::Eat(std::vector <std::pair<float,float>> flock)
{
    for (int x=0; x<flock.size();++x){
        float dist;

        float dx = flock[x].first - position.first;
        float dy = flock[x].second - position.second;

        dist = std::sqrt(dx*dx + dy*dy);

        if (dist>0 && dist<=20){

            return x;

        }
    }
    return -1;
}

std::pair<float,float> Predator::getPosition()
{
    return position;
}
