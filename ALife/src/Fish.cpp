#include "Fish.h"


Fish::Fish(float x, float y, float speedx,float speedy, std::vector <bool> dna)
{
   this->position.first = x;
   this->position.second = y;
   this->speed.first=speedx;
   this->speed.second=speedy;
   this->dna = dna;
   this->edad =0;
   this->libido=0;
   this->reserva=0;
   this->hambre=0;
   this->marry=false;


    for (int x=0; x<6;++x){
        std::vector <bool> aux;
        for (int y=0; y<8;++y){
            aux.push_back(dna[y+x]);
            //cout<<aux[y]<<endl;
        }
            //cout<<getNumber(aux)<<endl;
        color.push_back(Fish::getNumber(aux));
    }

    std::vector <bool> aux;

    for (int x=48;x<56;++x){
        aux.push_back(dna[x]);
    }

    rule = Fish::getNumber(aux);

    aux.clear();
    aux.push_back(dna[56]);
    aux.push_back(dna[57]);
    transformacion = Fish::getNumber(aux);

    aux.clear();
    for (int x=58;x<64;++x){
        aux.push_back(dna[x]);
    }
    vida= Fish::getNumber(aux);

    aux.clear();
    for (int x=64;x<70;++x){
        aux.push_back(dna[x]);
    }
    vision= 70 + Fish::getNumber(aux);

    aux.clear();
    for (int x=70;x<78;++x){
        aux.push_back(dna[x]);
    }
    celo= 500 + Fish::getNumber(aux);

    aux.clear();
    for (int x=78;x<80;++x){
        aux.push_back(dna[x]);
    }
    velocidad= Fish::getNumber(aux);

    aux.clear();
    for (int x=80;x<87;++x){
        aux.push_back(dna[x]);
    }
    resistencia= 300 + Fish::getNumber(aux);

    aux.clear();
    for (int x=87;x<89;++x){
        aux.push_back(dna[x]);
    }

    metabolismo=1+Fish::getNumber(aux);
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
        reserva+=comida[int(position.first)][int(position.second)];
        comida[int(position.first)][int(position.second)]=0;
    }
    if (reserva<=0){
        hambre++;
    }else{
        reserva--;
        hambre-=metabolismo*4;
    }
    if (hambre>resistencia){
        vida=0;
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

        //food.first*=(hambre/10);
        //food.second*=(hambre/10);

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

        if (dist>0 && dist<50 && libido>=celo){
                figlio.clear();
                figlio = Fish::reproducir(flock[i].getDNA());
                marry = true;
                libido=0;
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

        speed.first -= (survive.first)*1,4;
        speed.second-= (survive.second)*1,4;
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
    edad++;
    libido++;
    if (edad== 100){
        edad=0;
        vida--;
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
    if (speed.first <0){
        speed.first-=velocidad;
    }else{
        speed.first+=velocidad;
    }
    if (speed.second<0){
        speed.second-=velocidad;
    }else{
        speed.second+=velocidad;
    }
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

std::vector<int> Fish::getColor()
{
    return color;
}

int Fish::getTransformacion(){
    return transformacion;
}

int Fish::getRule(){
    return rule;
}

int Fish::getVida(){
    return vida;
}

void Fish::setImage(ALLEGRO_BITMAP* image){
    this->image = image;
}

void Fish::divorce(){
    figlio.clear();
    marry=false;
}


bool Fish::getMarry(){
    return marry;
}

std::vector<bool> Fish::getFiglio(){
    return figlio;
}

std::vector<bool> Fish::getDNA(){
    return dna;
}

int Fish::getNumber(std::vector <bool> adn){
    int aux = 1, number=0;
    for (int x= adn.size()-1; x>0; --x){
        if (adn[x]){
            number+=aux;
        }
        aux*=2;
    }
    return number;
}

std::vector<bool> Fish::reproducir(std::vector<bool> adn){
    int corte = rand()% dna.size();
    int first = rand()%2;
    std::vector<bool> hijo;
    if (first==0){
        for (int x=0;x<corte;++x){
            hijo.push_back(adn[x]);
        }
        for (int x=corte; x<dna.size();++x){
            hijo.push_back(dna[x]);
        }
    }else{
        for (int x=0;x<corte;++x){
            hijo.push_back(dna[x]);
        }
        for (int x=corte; x<dna.size();++x){
            hijo.push_back(adn[x]);
        }
    }
    for (int x=0; x<dna.size();++x){
        int aux = rand()%dna.size();
        if (aux<4){
            if (hijo[x]){
                hijo[x]=false;
            }else{
                hijo[x]=true;
            }
        }
    }
    libido=0;
    return hijo;
}
