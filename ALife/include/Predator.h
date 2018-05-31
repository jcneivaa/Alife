#pragma once

#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <cmath>
#include <vector>
#include"Fish.h"

class Predator
{
    private:
        std::pair <float, float> position, speed;
        ALLEGRO_BITMAP* image;
        int vision;
        void Move();
        void SpeedLimit();
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
        bool sleeping;


    public:
        Predator(float x, float y, float speedx,float speedy, std::vector <bool> dna);
        ~Predator();

        void Draw(ALLEGRO_DISPLAY* display);
        void Behavior(std::vector <std::pair<float,float>> flock);
        int Eat(std::vector <std::pair<float,float>> flock);
        std::pair <float,float> getPosition();
        std::vector <int> getColor();
        int getTransformacion();
        int getRule();
        int getVida();
        int getVision();
        bool getMarry();
        void divorce();
        void setImage(ALLEGRO_BITMAP* image);
        std::vector <bool> getDNA();
        std::vector <bool> getFiglio();
        std::vector <bool> reproducir();
};
