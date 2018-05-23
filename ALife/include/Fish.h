#pragma once

#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <cmath>
#include <vector>
#include"Predator.h"

class Fish
{
    private:
        std::pair <float, float> position, speed;
        ALLEGRO_BITMAP* image;
        int vision;
        void SpeedLimit();
        std::pair<float,float> SteerForceLimit(std::pair <float,float> valor);
        void Move();
        std::vector <bool> dna;
        std::vector <bool> figlio;
        std::vector <int> color;
        int transformacion;
        int rule;
        int vida;
        int edad;
        int libido;
        int celo;
        int velocidad;
        int reserva;
        int hambre;
        int resistencia;
        int metabolismo;
        int getNumber(std::vector <bool> dna);
        bool marry;


    public:
        Fish(float x, float y, float speedx,float speedy, std::vector <bool> dna);
        ~Fish();

        void Draw(ALLEGRO_DISPLAY* display, int comida[1500][780]);
        void Behavior(std::vector <Fish> flock, int comida[1500][780], std::vector<std::pair <float,float>> predators);
        std::pair <float,float> getPosition();
        std::vector <int> getColor();
        int getTransformacion();
        int getRule();
        int getVida();
        bool getMarry();
        void divorce();
        void setImage(ALLEGRO_BITMAP* image);
        std::vector <bool> getDNA();
        std::vector <bool> getFiglio();
        std::vector <bool> reproducir(std::vector<bool> adn);

};


