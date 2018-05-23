#include "Predator.h"

Predator::Predator(float x, float y, float speedx,float speedy,std::vector <bool> dna)
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
   this->sleeping =false;


    for (int x=0; x<6;++x){
        std::vector <bool> aux;
        for (int y=0; y<8;++y){
            aux.push_back(dna[y+x]);
            //cout<<aux[y]<<endl;
        }
            //cout<<getNumber(aux)<<endl;
        color.push_back(Predator::getNumber(aux));
    }

    std::vector <bool> aux;

    for (int x=48;x<56;++x){
        aux.push_back(dna[x]);
    }

    rule = Predator::getNumber(aux);

    aux.clear();
    aux.push_back(dna[56]);
    aux.push_back(dna[57]);
    transformacion = Predator::getNumber(aux);

    aux.clear();
    for (int x=58;x<64;++x){
        aux.push_back(dna[x]);
    }
    vida= Predator::getNumber(aux);

    aux.clear();
    for (int x=64;x<70;++x){
        aux.push_back(dna[x]);
    }
    vision= 70 + Predator::getNumber(aux);

    aux.clear();
    for (int x=70;x<78;++x){
        aux.push_back(dna[x]);
    }
    celo= 500 + Predator::getNumber(aux);

    aux.clear();
    for (int x=78;x<80;++x){
        aux.push_back(dna[x]);
    }
    velocidad= 1+Predator::getNumber(aux);

    aux.clear();
    for (int x=80;x<87;++x){
        aux.push_back(dna[x]);
    }
    resistencia= 300 + Predator::getNumber(aux);

    aux.clear();
    for (int x=87;x<89;++x){
        aux.push_back(dna[x]);
    }

    metabolismo=1+Predator::getNumber(aux);

}

Predator::~Predator()
{
    //dtor
}


void Predator::Draw(ALLEGRO_DISPLAY* display)
{
    Predator::Move();
    al_draw_bitmap(image,position.first,position.second,0);
    if (reserva<=0){
        hambre+=metabolismo;
    }else{
        reserva--;
        hambre-=metabolismo;
    }
    if (hambre>resistencia){
        vida=0;
    }
    if (libido>=celo){
        figlio.clear();
        figlio = Predator::reproducir();
        marry = true;
        libido=0;
    }
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
    edad++;
    libido++;
    if (edad== 100){
        edad=0;
        vida--;
    }
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

    if (reserva>=40){
        speed.first=0;
        speed.second=0;
        sleeping=true;
    }else if (sleeping){
        speed.first=(rand()%2)-1;
        speed.second=(rand()%2)-1;
        sleeping=false;
    }
}

int Predator::Eat(std::vector <std::pair<float,float>> flock)
{
    if(sleeping){
        return -1;
    }
    for (int x=0; x<flock.size();++x){
        float dist;

        float dx = flock[x].first - position.first;
        float dy = flock[x].second - position.second;

        dist = std::sqrt(dx*dx + dy*dy);

        if (dist>0 && dist<=20){

            reserva+=100;
            return x;

        }
    }
    return -1;
}

std::pair<float,float> Predator::getPosition()
{
    return position;
}


std::vector<int> Predator::getColor()
{
    return color;
}

int Predator::getTransformacion(){
    return transformacion;
}

int Predator::getRule(){
    return rule;
}

int Predator::getVida(){
    return vida;
}

void Predator::setImage(ALLEGRO_BITMAP* image){
    this->image = image;
}

void Predator::divorce(){
    figlio.clear();
    marry=false;
}


bool Predator::getMarry(){
    return marry;
}

std::vector<bool> Predator::getFiglio(){
    return figlio;
}

std::vector<bool> Predator::getDNA(){
    return dna;
}

int Predator::getNumber(std::vector <bool> adn){
    int aux = 1, number=0;
    for (int x= adn.size()-1; x>0; --x){
        if (adn[x]){
            number+=aux;
        }
        aux*=2;
    }
    return number;
}

std::vector<bool> Predator::reproducir(){

    std::vector<bool> hijo;


    for (int x=0; x<dna.size();++x){
        int aux = rand()%dna.size();
        hijo.push_back(dna[x]);
        if (aux<7){
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
