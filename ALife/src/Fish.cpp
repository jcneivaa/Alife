#include "Fish.h"


Fish::Fish(float x, float y, ALLEGRO_BITMAP* image,float speedx,float speedy)
{
   this->position.first = x;
   this->position.second = y;
   this->image = image;
   this->speed.first=speedx;
   this->speed.second=speedy;
   this->vision = 85;
}

Fish::~Fish()
{
    //dtor
}

void Fish::Draw(ALLEGRO_DISPLAY *display, int comida[1500][780])
{
    Fish::Move();
    al_draw_bitmap(image,position.first,position.second,0);
    if(comida[int(position.first)][int(position.second)]>0){
        comida[int(position.first)][int(position.second)]=0;
    }
    //std::cout<<position.first<<"  "<<position.second<<speed.first<<"   "<<speed.second<<std::endl;
}

void Fish::Behavior(std::vector<Fish> flock, int comida[1500][780], std::vector<std::pair <float,float>> predators)
{

    int boidCount = 0;
    int foodCount =0;
    int predatorCount =0;

    std::pair <float,float> alignment (0,0);
    std::pair <float,float> cohesion (0,0);
    std::pair <float,float> separation (0,0);
    std::pair <float,float> food (0,0);
    std::pair <float,float> survive(0,0);
    std::pair <float,float> repulse;

    for (int x=vision/2; x>-vision/2; --x){
        for (int y=vision/2; y>-vision/2;--y){
            if (comida[int((position.first+x+1500))%1500][int((position.second+y+780))%780]>0){
                food.first+=position.first+x;
                food.second+=position.second+y;
                foodCount++;
            }
        }
    }

    if (foodCount>0){
        food.first/=foodCount;
        food.second/=foodCount;
        food.first-=position.first;
        food.second-=position.second;
        food.first*=0.1;
        food.second*=0.1;

        speed.first += (food.first);
        speed.second+= (food.second);
    }

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

            separation.first -= repulse.first;
            separation.second -= repulse.second;


        }
    }

     for (int i=0;i<predators.size();++i){
        float dist;

        float dx = predators[i].first - position.first;
        float dy = predators[i].second - position.second;

        dist = std::sqrt(dx*dx + dy*dy);

        if (dist>0 && dist<=vision){

            survive.first += predators[i].first;
            survive.second += predators[i].second;
            predatorCount++;

        }
    }

    if (predatorCount>0){
        survive.first/=predatorCount;
        survive.second/=predatorCount;
        survive.first-=position.first;
        survive.second-=position.second;
        survive.first*=0.1;
        survive.second*=0.1;

        speed.first -= (survive.first);
        speed.second-= (survive.second);
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
        speed.first += alignment.first + cohesion.first *3 + separation.first;
        speed.second += alignment.second + cohesion.second*3 + separation.second;
    }

}

void Fish::Move()
{

    Fish::SpeedLimit();
    position.first = int((position.first + speed.first + 1500))%1500;
    position.second = int((position.second + speed.second + 780))%780;
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

std::pair<float,float> Fish::getPosition()
{
    return position;
}
