#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <iostream>
#include <string>
#include <math.h>
#include <stack>
#include <vector>
#include <time.h>
#include"Fish.h"
#include"Predator.h"



using namespace std;

#define PI 3.14159265
#define SWidth 1500
#define SHeight 780
#define FPS 20

int comida[SWidth][SHeight];
int boids=100, predators =4, sourceFood =2, foodRate=200, season =800;
int dnaSize =89;
//0-23 Color 1, 24-47 Color 2, 48-55 Turing Morph Rule, 56-57 Transformacion, 58-63 Vida Maxima, 64-69 Vision, 70-77 Libido
//78-79 Velocidad, 80-86 Resistencia, 87-88 Metabolismo
//Fish boid;
vector<Fish> flock;
vector<Predator> dragons;
vector<pair<int,int>> arboles;


struct rule {
    char letter;
    string production;
};

struct tree {
    float length;
    float angle;
    ALLEGRO_COLOR color;
    string code;
};

struct tree_state {
    float posx;
    float posy;
    float angle;
};

string create_tree(int numr, rule rules[], string code, int deep){
    for (int x=0;x<deep;++x){
        string aux="";
        for (int y=0; y<code.size();++y){
            bool exist = false;
            for (int z=0;z<numr;++z){
                if (code[y]==rules[z].letter){
                    aux+=rules[z].production;
                    exist= true;
                }
            }
            if (!exist){
                aux+=code[y];
            }
        }
        code = aux;
    }

    return code;
}

void draw_tree(tree arbolito, float xinit, float yinit){
    float newx,newy;
    tree_state current ={xinit,yinit,270};
    stack<tree_state> states;

    for (int x=0;x < arbolito.code.size();++x){
        if (arbolito.code[x]=='F'){
            newx=current.posx+arbolito.length*cos(current.angle*PI/180);
            newy=current.posy+arbolito.length*sin(current.angle*PI/180);

            al_draw_line(current.posx,current.posy,newx,newy,arbolito.color,1.0);

            current.posx=newx;
            current.posy=newy;
        }
        if (arbolito.code[x]=='+'){
            current.angle-=arbolito.angle;
        }
        if (arbolito.code[x]=='-'){
            current.angle+=arbolito.angle;
        }
        if (arbolito.code[x]=='['){
            states.push(current);
        }
        if (arbolito.code[x]==']'){
            current=states.top();
            states.pop();
        }
    }
}


int getVecindario(bool x, bool y, bool z){
    if (x){
        if (y){
            if (z){
                return 7;
            }else{
                return 6;
            }
        }else if(z){
            return 5;
        }else{
            return 4;
        }
    }else if(y){
        if (z){
            return 3;
        }else{
            return 2;
        }
    }else if (z){
        return 1;
    }else{
        return 0;
    }
}

void sandPile(int x, int y){
    comida[x][y]++;
    comida[x+2][y]++;
    comida[x+4][y]++;
    comida[x+6][y]++;
    comida[x+8][y]++;
    comida[x-2][y]++;
    comida[x-4][y]++;
    comida[x-6][y]++;
    comida[x-8][y]++;
    comida[x][y+2]++;
    comida[x][y+4]++;
    comida[x][y+6]++;
    comida[x][y+8]++;
    comida[x][y-2]++;
    comida[x][y-4]++;
    comida[x][y-6]++;
    comida[x][y-8]++;
    if (comida[x][y]>=4){
        comida[x][y]-=4;
        comida[x+2][y]-=4;
        comida[x+4][y]-=4;
        comida[x+6][y]-=4;
        comida[x+8][y]-=4;
        comida[x-2][y]-=4;
        comida[x-4][y]-=4;
        comida[x-6][y]-=4;
        comida[x-8][y]-=4;
        comida[x][y+2]-=4;
        comida[x][y+4]-=4;
        comida[x][y+6]-=4;
        comida[x][y+8]-=4;
        comida[x][y-2]-=4;
        comida[x][y-4]-=4;
        comida[x][y-6]-=4;
        comida[x][y-8]-=4;
        sandPile((x+SWidth+10)%SWidth,y);
        sandPile((x+SWidth-10)%SWidth,y);
        sandPile((x)%SWidth,(y+SHeight+10)%SHeight);
        sandPile((x)%SWidth,(y+SHeight-10)%SHeight);
    }

}

void createArboles(){
    arboles.clear();
    for (int x=0; x<sourceFood*2;++x){
        //int aux=rand()%SWidth, aux2=rand()%s
        pair <int,int> aux;
        aux.first=rand()%SWidth/4;
        aux.second=rand()%SHeight;
        if (x>=sourceFood){
            aux.first+=(SWidth/4)*3;
        }
        arboles.push_back(aux);
    }
}

void createFood(bool winter){

    for (int x=0; x<SWidth; ++x){
        for (int y = 0; y< SHeight; ++y){
            comida[x][y]= 0;
        }
    }

    for (int i=0; i<sourceFood;++i){
        for (int x=0; x<400; ++x){
            if (winter){
                sandPile(arboles[i].first,arboles[i].second);
            }else{
                sandPile(arboles[i+sourceFood].first,arboles[i+sourceFood].second);
            }
        }
    }
}

vector<bool> create_dna(){
        vector<bool> dna;
        for (int x=0; x<dnaSize;++x){
            int aux= rand();
            if (aux%2 ==0){
                dna.push_back(false);
            }else{
                dna.push_back(true);
            }
        }
        return dna;
}

ALLEGRO_COLOR getColor (int r, int g, int b){
    ALLEGRO_COLOR color = al_map_rgb(r,g,b);
    return color;
}

ALLEGRO_BITMAP* transformation(ALLEGRO_BITMAP* cute, int option){

    ALLEGRO_BITMAP* test = al_create_bitmap(al_get_bitmap_width(cute),al_get_bitmap_height(cute));


    ALLEGRO_LOCKED_REGION* lc = al_lock_bitmap(cute,ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_READONLY);
    ALLEGRO_LOCKED_REGION* lt = al_lock_bitmap(test,ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_WRITEONLY);

    al_set_target_bitmap(test);

    ALLEGRO_COLOR aux;
    //cout<<option;

    for(int x=0;x<al_get_bitmap_width(cute);++x){
        for (int y=0;y<al_get_bitmap_height(cute);++y){
            aux=al_get_pixel(cute,x,y);
            //cout<<aux.a<<endl;

            if (option==0){
                al_put_pixel(x,y,aux);
            }
            if (option==1){
                al_put_pixel(x,((al_get_bitmap_height(cute)/2)-y)*sin(int((int(x/(al_get_bitmap_width(cute)/-180.0)))%180)*PI/180)+al_get_bitmap_height(cute)/2,aux);
            }
            if (option==2){
                al_put_pixel(x,((al_get_bitmap_height(cute)/2)-y)*cos(int((int(x/(al_get_bitmap_width(cute)/90.0))+90)%180)*PI/180)+al_get_bitmap_height(cute)/2,aux);
            }
            if (option==3){
                al_put_pixel(x,((al_get_bitmap_height(cute)/2)-y)*cos(int((int(x/(al_get_bitmap_width(cute)/90.0))+160)%360)*PI/180)+al_get_bitmap_height(cute)/2,aux);
            }
        }
    }

    al_unlock_bitmap(test);
    al_unlock_bitmap(cute);

    return test;

    //al_save_bitmap("shalalala.png",test);

    //al_set_target_backbuffer(display);
}

ALLEGRO_BITMAP* turingMorph(ALLEGRO_BITMAP* cute, int morphingRule, ALLEGRO_COLOR azul, ALLEGRO_COLOR morado){
    ALLEGRO_BITMAP* test = al_create_bitmap(al_get_bitmap_width(cute),al_get_bitmap_height(cute));


    ALLEGRO_LOCKED_REGION* lc = al_lock_bitmap(cute,ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_READONLY);
    ALLEGRO_LOCKED_REGION* lt = al_lock_bitmap(test,ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_WRITEONLY);

    al_set_target_bitmap(test);

    ALLEGRO_COLOR aux;
    //ALLEGRO_COLOR azul= al_map_rgb(0,128,175);
    //ALLEGRO_COLOR morado = al_map_rgb(129,84,202);

    //Inicio TuringMorph
    vector<bool> state,nextState;
    bool rules[8];
    for (int x=0; x<8;++x){
        rules[x]=false;
    }

    int lumi=0;
    while (morphingRule>0){
        if(morphingRule & 1){
          rules[lumi]= true;
        }
    lumi++;
    morphingRule = morphingRule>>1;
    }

    for (int x=0;x<al_get_bitmap_height(cute);++x){
        if (rand()%2 ==0){
            state.push_back(false);
        }else{
            state.push_back(true);
        }
    }

    for (int x=0; x<al_get_bitmap_width(cute);++x){
        for (int y=0;y<state.size();++y){
            aux=al_get_pixel(cute,x,y);
            int vecindario = getVecindario(state[(y+state.size()-1)%state.size()],state[(y+state.size())%state.size()],state[(y+state.size()+1)%state.size()]);
            if (rules[vecindario]){
                nextState.push_back(true);
                if (aux.a!=0){
                    al_put_pixel(x,y,azul);
                }else{
                    al_put_pixel(x,y,aux);
                }
            }else{
                nextState.push_back(false);
                if (aux.a!=0){
                    al_put_pixel(x,y,morado);
                }else{
                    al_put_pixel(x,y,aux);
                }
            }
        }
        state = nextState;
        nextState.clear();
    }

    //Fin TuringMorph




    al_unlock_bitmap(test);
    al_unlock_bitmap(cute);

    return test;

}

void createBoid(ALLEGRO_BITMAP* cute, ALLEGRO_BITMAP* xfish, ALLEGRO_DISPLAY* display,vector<bool> dna, pair<int,int> pos){
    Fish boid(pos.first,pos.second,(rand()%2)-1,(rand()%2)-1,dna);
    vector <int> colores = boid.getColor();
    cute = turingMorph(cute, boid.getRule(),getColor(colores[0],colores[1],colores[2]),getColor(colores[3],colores[4],colores[5]));
    al_set_target_backbuffer(display);

    xfish=transformation(cute,boid.getTransformacion());
    al_set_target_backbuffer(display);
    boid.setImage(xfish);
    flock.push_back(boid);
}

void createPredator(ALLEGRO_BITMAP* cute, ALLEGRO_BITMAP* xfish, ALLEGRO_DISPLAY* display,vector<bool> dna, pair<int,int> pos){
    Predator boid(pos.first,pos.second,(rand()%2)-1,(rand()%2)-1,dna);
    vector <int> colores = boid.getColor();
    cute = turingMorph(cute, boid.getRule(),getColor(colores[0],colores[1],colores[2]),getColor(colores[3],colores[4],colores[5]));
    al_set_target_backbuffer(display);

    xfish=transformation(cute,boid.getTransformacion());
    al_set_target_backbuffer(display);
    boid.setImage(xfish);
    dragons.push_back(boid);
}

int main()
{
    //This command create a screen
    ALLEGRO_DISPLAY* display;

    //Allegro Setup Error Message
    if(!al_init())
        {
            al_show_native_message_box(NULL,NULL,NULL,"Allegro couldnt initialize",NULL,NULL);
            return -1;
        }

    //Screen Resolution
    //display = al_create_display(1000,720);
    srand(time(NULL));
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    display = al_create_display(SWidth,SHeight);
    al_set_window_position(display,0,0);
    al_set_window_title(display,"ALife");


    //Allegro Screen Creating Error
    if(!display)
        {
            al_show_native_message_box(NULL,NULL,NULL,"Couldnt create Screen",NULL,NULL);
            return -1;
        }

    //Inicializar cosas de Allegro

    al_init_primitives_addon();
    al_init_image_addon();
    al_install_keyboard();

    //Configuracion del teclado y timer
    ALLEGRO_TIMER* timer = al_create_timer(1.0/FPS);
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    al_register_event_source(event_queue,al_get_keyboard_event_source());
    al_register_event_source(event_queue,al_get_timer_event_source(timer));

    //Inicio cosas feas

    ALLEGRO_BITMAP* cute = al_load_bitmap("Boid.png");
    ALLEGRO_BITMAP* depredador = al_load_bitmap("Predator.png");
    ALLEGRO_BITMAP* bigFish = al_load_bitmap("Pez.png");
    ALLEGRO_BITMAP* xfish = al_create_bitmap(al_get_bitmap_width(cute),al_get_bitmap_height(cute));
    ALLEGRO_BITMAP* xpredator = al_create_bitmap(al_get_bitmap_width(depredador),al_get_bitmap_height(depredador));
    ALLEGRO_BITMAP* testFish = al_create_bitmap(al_get_bitmap_width(bigFish),al_get_bitmap_height(bigFish));


    //bigFish = turingMorph(bigFish,122);
    al_set_target_backbuffer(display);

    //Fin Cosas feas

    ALLEGRO_COLOR eblue = al_map_rgb(44,117,255);
    ALLEGRO_COLOR white = al_map_rgb(255,255,255);

    //  Inicio L-Systems

    rule rulea = {'F',"F[+F]F[-F]F"};
    rule ruleb = {'F',"F[+F]F[-F][F]"};
    rule rulec = {'F',"FF-[-F+F+F]+[+F-F-F]"};
    rule ruled1 = {'X',"F[+X]F[-X]+X"};
    rule ruled2 = {'F', "FF"};
    rule rulee1 = {'X', "F[+X][-X]FX"};
    rule rulef1 = {'X', "F-[[X]+X]+F[F[+FX]-X"};

    rule rules[1];
    rule btree[1];
    rule ctree[1];
    rule dtree[2];
    rule etree[2];
    rule ftree[2];

    rules[0] = rulea;
    btree[0] = ruleb;
    ctree[0] = rulec;
    dtree[0]=ruled1;
    dtree[1]=ruled2;
    etree[0]=rulee1;
    etree[1]=ruled2;
    ftree[0]=rulef1;
    ftree[1]=ruled2;

    tree treea = {1,25.7,eblue,create_tree(1,rules,"F",5)};
    tree treeb = {4,20,eblue,create_tree(1,btree,"F",5)};
    tree treec = {6,22.5,eblue,create_tree(1,ctree,"F",4)};
    tree treed = {1,20,eblue,create_tree(2,dtree,"X",7)};
    tree treee = {1,25.7,eblue,create_tree(2,etree,"X",7)};
    //tree treef = {1,22.5,eblue,create_tree(2,ftree,"X",5)};

    tree arbolitos [5] = {treea,treeb,treec,treed,treee};

    //draw_tree(tree1);
    //draw_tree(treec);
    //draw_tree(treed);

//    al_draw_bitmap(cute,0,0,ALLEGRO_FLIP_HORIZONTAL);
//    al_draw_bitmap(test,0,0,0);

//Inicio de boids

    for (int x =0; x<boids;++x){
        vector <bool> dna = create_dna();
        pair <int, int> pos;
        pos.first = rand()%SWidth - 100;
        pos.second = rand()%SHeight - 100;
        createBoid(cute,xfish,display,dna, pos);
        //flock.push_back(new Fish::Fish(rand()%100,rand()%100));
    }


//Fin de Boids

//Inicio de Predator
    for (int x =0; x<predators;++x){
        vector <bool> dna = create_dna();
        pair <int, int> pos;
        pos.first = rand()%SWidth - 100;
        pos.second = rand()%SHeight - 100;
        createPredator(depredador,xpredator,display,dna, pos);

        //flock.push_back(new Fish::Fish(rand()%100,rand()%100));
    }


//Fin de Predator

//Inicio de comida
    createArboles();

//Fin de comida

    bool done=false, arbolito=true, theBig=false;;
    int tree_move = 0;
    int option=0;
    int destroyFish =-1;
    int sandCount=0;
    int seasonTime=0;
    bool winter = true;
    vector <pair <float,float>> flockPosition, predatorPosition ;
    //std::pair <float,float> posAux;
    createFood(winter); //Creacion de la comida inicial
    testFish=transformation(bigFish,option);
    al_set_target_backbuffer(display);
    al_start_timer(timer);


    while(!done){
        ALLEGRO_EVENT events;
            al_wait_for_event(event_queue,&events);


        if (events.type==ALLEGRO_EVENT_KEY_DOWN){
            switch(events.keyboard.keycode){
                case ALLEGRO_KEY_ENTER:
                    if(theBig){
                        theBig=false;
                    }else{
                        theBig=true;
                    }
                    break;
                case ALLEGRO_KEY_SPACE:
                    testFish=transformation(bigFish,option);
                    ++option;
                    if (option==4){
                            option=0;
                            }
                    al_set_target_backbuffer(display);
                    break;
                case ALLEGRO_KEY_ESCAPE:
                    done=true;
                    break;
            }
        }
/*
    if (arbolito){
        ++tree_move;
        //treed.angle+=0.01;
        treec.angle+=0.5;
        if (tree_move==20){
            arbolito=false;
        }
    }else{
        --tree_move;
        //treed.angle-=0.01;
        treec.angle-=0.5;
        if (tree_move==0){
            arbolito=true;
        }
    }
*/
//Manejo de estaciones
    ++seasonTime;
    if (seasonTime==season){
        if (winter){
            winter = false;
        }else{
            winter = true;
        }
        seasonTime=0;
        createFood(winter);
    }


//Dibujar Comida
    for (int x=0; x<SWidth;++x){
        for (int y=0; y<SHeight;++y){
            if (comida[x][y]>0){
                al_draw_pixel(x,y,white);
            }
        }
    }

//Dibujar Arboles
    for (int x=0; x<sourceFood*2;++x){
        draw_tree(arbolitos[(x%5)],arboles[x].first,arboles[x].second);
    }



//Dibujar Peces
    if (theBig){
        al_draw_bitmap(testFish,0,0,0);
    }

    for (int x=0;x<flock.size();++x){
        flock[x].Behavior(flock, comida, predatorPosition);
        flock[x].Draw(display, comida);
        flockPosition.push_back(flock[x].getPosition());
        //if (comida[int(flock[x].getPosition().first)][int(flock[x].getPosition().second)]>0){
        //    comida[int(flock[x].getPosition().first)][int(flock[x].getPosition().second)]--;
        //}
    }

    predatorPosition.clear();
//Dibujar Presas
    for (int x=0;x<dragons.size();++x){
        dragons[x].Draw(display);
        dragons[x].Behavior(flockPosition);
        destroyFish = dragons[x].Eat(flockPosition);
        predatorPosition.push_back(dragons[x].getPosition());
        if (destroyFish>=0){
            flock.erase(flock.begin()+destroyFish);
        }
    }

    flockPosition.clear();

//Muerte "natural" y nacimiento
    for (int x=0; x<flock.size();++x){
        if (flock[x].getMarry()){
            createBoid(cute,xfish,display,flock[x].getFiglio(),flock[x].getPosition());
            flock[x].divorce();
        }
        if (flock[x].getVida()<=0){
            flock.erase(flock.begin()+x);
            x--;
        }
    }

    for (int x=0; x<dragons.size();++x){
        if (dragons[x].getMarry()){
            createPredator(depredador,xpredator,display,dragons[x].getFiglio(),dragons[x].getPosition());
            dragons[x].divorce();
        }
        if (dragons[x].getVida()<=0){
            dragons.erase(dragons.begin()+x);
            x--;
        }
    }

/*
    for (int x=0;x<5;++x){
        draw_tree(arbolitos[x],(x+1)*250,SHeight);
    }
*/
    //draw_tree(treee,200,SHeight);
    //draw_tree(treec,500,SHeight);
    cout<<flock.size()<<"   "<<dragons.size()<<"    "<<seasonTime<<endl;

    al_flip_display();
    al_clear_to_color(al_map_rgb(0,0,0));

    }


    return 0;
}
