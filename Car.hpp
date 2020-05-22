//
// Created by Daniel Courvoisier on 14.05.20.
//

#ifndef PROJET_V1_CAR_HPP
#define PROJET_V1_CAR_HPP

#include <string>
#include <thread>
#include "Global.hpp"
#include "Course.hpp"
#include "TrafficLight.hpp"

// Variables globales
extern std::recursive_mutex displayLock;


// Classe Car
class Car {
private:
    string name;
    int orientation;
    Course* course;
    int position;
    TrafficLight *crossingLight;

public:
    Car(string nom, int carOrientation, Course * CX, TrafficLight *CL);
    void drive();
};


// Constructeur
Car::Car(string nom, int carOrientation, Course * CX, TrafficLight *CL) {
    name = nom;
    orientation = carOrientation;
    course = CX;
    crossingLight = CL;
    position = 0;

    displayLock.lock();
    cout <<"============================================================================> " << name <<" est créée"  << endl;
    displayLock.unlock();
};


// Rouler
void Car::drive() {
    verrou_parcours.lock();
    position = course->reactualiser(name, position);    // passer la pos actuelle voiture, retourne position après actu
    verrou_parcours.unlock();

    while (position < 100){
        if (position == 6){
            std::unique_lock<std::mutex> monlock(arret_carrefour);
            while ((crossingLight->getCouleur() == "R")) {
                displayLock.lock();
                cout << "---------- " << name << " est bloquée devant le carrefour car le feu est rouge" << endl;
                displayLock.unlock();
                CV2[orientation].wait(monlock);
                displayLock.lock();
                cout << "-------------------- " << name << " est libérée et va rentrer dans le carrefour" << endl;
                displayLock.unlock();
            }
            int attente = rand()%30; // Temps rentrer carrefour
            std::this_thread::sleep_for(std::chrono::milliseconds(attente));
        }

        int attente = rand()%200;
        std::this_thread::sleep_for(std::chrono::milliseconds(attente));
        verrou_parcours.lock();
        position = course->reactualiser(name, position);
        verrou_parcours.unlock();
    }
}

#endif
