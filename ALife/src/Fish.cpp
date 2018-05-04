#include "Fish.h"


Fish::Fish(float x, float y, ALLEGRO_BITMAP* image,float speedx,float speedy)
{
   this->position.first = x;
   this->position.second = y;
   this->image = image;
   this->speed.first=speedx;
   this->speed.second=speedy;
   this->vision = 85;
   this->acceleration.first =0;
   this->acceleration.second =0;
}

Fish::~Fish()
{
    //dtor
}

void Fish::Draw(ALLEGRO_DISPLAY *display)
{
    Fish::Move();
    al_draw_bitmap(image,position.first,position.second,0);
    std::cout<<position.first<<"  "<<position.second<<speed.first<<"   "<<speed.second<<std::endl;
}

void Fish::Behavior(std::vector<Fish> flock)
{

    int boidCount = 0;

    std::pair <float,float> alignment (0,0);
    std::pair <float,float> cohesion (0,0);
    std::pair <float,float> separation (0,0);
    std::pair <float,float> repulse;


    for (int i=0;i<flock.size();++i){
        float dist;

        float dx = flock[i].position.first - position.first;
        float dy = flock[i].position.second - position.second;

        dist = std::sqrt(dx*dx + dy*dy);

        if (dist>0 && dist<=vision){
            alignment.first += flock[i].speed.first;
            alignment.second += flock[i].speed.second;

            cohesion.first += flock[i].position.first;
            cohesion.second += flock[i].position.second;

            boidCount++;

            repulse.first = dx/dist;
            repulse.second = dy/dist;

            repulse.first /=dist;
            repulse.second/=dist;

            separation.first += repulse.first;
            separation.second += repulse.second;


        }
    }

    if(boidCount >0){
        float aux;
        alignment.first /= boidCount;
        alignment.second /= boidCount;
        aux = std::sqrt(alignment.first*alignment.first + alignment.second*alignment.second);
        alignment.first /=aux;
        alignment.second/=aux;
        alignment.first*=0.1;
        alignment.second*=0.1;

        cohesion.first /= boidCount;
        cohesion.second /= boidCount;
        cohesion.first-=position.first;
        cohesion.second-=position.second;
        aux = std::sqrt(cohesion.first*cohesion.first + cohesion.second*cohesion.second);
        cohesion.first /=aux;
        cohesion.second/=aux;
        cohesion.first*=0.1;
        cohesion.second*=0.1;

        //acceleration.first += alignment.first + cohesion.first *3 + separation.first;
        //acceleration.second += alignment.second + cohesion.second*3 + separation.second;
        speed.first += alignment.first + cohesion.first *2 + separation.first;
        speed.second += alignment.second + cohesion.second*2 + separation.second;
    }

}

void Fish::Move()
{

    Fish::SpeedLimit();
    position.first = int((position.first + speed.first))%1460;
    position.second = int((position.second + speed.second))%770;
    if (position.first < 0){
        position.first =1450;
    }else if (position.first > 1450){
        position.first=0;
    }
    if (position.second < 0){
        position.second = 760;
    }else if(position.second>760){
        position.second=0;
    }
}

void Fish::SpeedLimit()
{
    float aux;
    aux = std::sqrt(speed.first*speed.first + speed.second*speed.second);
    speed.first /=aux;
    speed.second/=aux;
    speed.first*=4;
    speed.second*=4;
    if (std::isnan(speed.first)){
        speed.first  = -1;
    }
    if (std::isnan(speed.second)){
        speed.second = -1;
    }
}

std::pair <float,float> Fish::SteerForceLimit(std::pair <float,float> valor)
{
    float aux=0.1;
    if(valor.first > aux){
        valor.first =aux;
    }else if(valor.first<-aux){
        valor.first =-aux;
    }
    if (valor.second>aux){
        valor.second =aux;
    }else if(valor.second<-aux){
        valor.second =-aux;
    }
    return valor;
}

