//
// Created by Daniel Courvoisier on 14.05.20.
//

#ifndef PROJET_V1_COURSE_HPP
#define PROJET_V1_COURSE_HPP

#include <iostream>
#include "Global.hpp"
#include "Crossroads.hpp"

// Variables globales
extern std::recursive_mutex displayLock;


// Classe Course (parcours)
class Course {
private:
    int _orientation;
    Crossroads * crossover;
    int compteurZoneDetection;

public:
    Course(int orientation, Crossroads* croisementX);
    int reactualiser(string name, int car_position);
    int getDetect();
};


// Constructeur
Course::Course(int orientation, Crossroads* croisementX) {
    _orientation = orientation;
    crossover = croisementX;
    compteurZoneDetection = 0;
}


// Actualiser position voiture
int Course::reactualiser(string name, int car_position) {        // remplacer par moins détaillé
    int carPosition = car_position;
    carPosition++;

    // Affiche le trajet de la voiture
    switch(carPosition){
        case 1:
            displayLock.lock();
            cout << name << " démarre sa course " << endl;
            displayLock.unlock();
            break;

        case 6:
            displayLock.lock();
            compteurZoneDetection++;       //il faut le protéger en incrémentation et décrémentation par le même mutex général
            cout << ".......... " << name
                 << " est arrivée en zone de détection  (position 6). NB voitures détectées : "
                 << compteurZoneDetection << endl;
            displayLock.unlock();
            break;

        case 7:
            displayLock.lock();
            compteurZoneDetection--;
            crossover->occuper_carrefour(name);
            cout << ".................... " << name << " est arrivée dans le carrefour  (position 7). " << endl;
            displayLock.unlock();

        case 8: displayLock.lock();
            crossover->sortir_carrefour(name);
            cout << ".............................. " << name << " est sortie du carrefour (position 8). NB voitures détectées : " << compteurZoneDetection << endl;
            displayLock.unlock();
            break;

        case 10:
            displayLock.lock();
            cout << name <<" est arrivé au point "  << _orientation<< endl;
            displayLock.unlock();
            return 1000;

        default:
            displayLock.lock();
            cout << name <<" atteint la position " << carPosition << endl;
            displayLock.unlock();
    }
    return carPosition;
}


// Retourne compteur zone
int Course::getDetect() {
    return compteurZoneDetection;
}

#endif
