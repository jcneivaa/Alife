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



using namespace std;

#define PI 3.14159265
#define SWidth 1500
#define SHeight 780
#define FPS 20
int boids=100;
//Fish boid;
vector<Fish> flock;


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
            if (option==0){
                al_put_pixel(x,y,aux);
                //cout<<"Shalalalala"<<endl;
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

    ALLEGRO_BITMAP* cute = al_load_bitmap("Small_Boid.png");
    ALLEGRO_BITMAP* xfish = al_create_bitmap(al_get_bitmap_width(cute),al_get_bitmap_height(cute));




    //Fin Cosas feas

    ALLEGRO_COLOR eblue = al_map_rgb(44,117,255);
    ALLEGRO_COLOR blue = al_map_rgb(0,255,0);

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
        int option=rand()%4;
        xfish=transformation(cute,option);
        al_set_target_backbuffer(display);
        Fish boid(rand()%SWidth,rand()%SHeight,xfish,(rand()%2) - 1,(rand()%2) - 1);
        flock.push_back(boid);
        //flock.push_back(new Fish::Fish(rand()%100,rand()%100));
    }


//Fin de Boids

    bool done=false, arbolito=true;
    int tree_move = 0;
    int option=0;
    al_start_timer(timer);

    while(!done){
        ALLEGRO_EVENT events;
            al_wait_for_event(event_queue,&events);


        if (events.type==ALLEGRO_EVENT_KEY_DOWN){
            switch(events.keyboard.keycode){
                case ALLEGRO_KEY_SPACE:
                    xfish=transformation(cute,option);
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

//    al_draw_bitmap(xfish,0,0,0);

    for (int x=0;x<flock.size();++x){
        flock[x].Draw(display);
        flock[x].Behavior(flock);
    }

/*
    for (int x=0;x<5;++x){
        draw_tree(arbolitos[x],(x+1)*250,SHeight);
    }
*/
    //draw_tree(treee,200,SHeight);
    //draw_tree(treec,500,SHeight);
    //cout<<flock.size()<<endl;
    al_flip_display();
    al_clear_to_color(al_map_rgb(0,0,0));

    }


    return 0;
}
