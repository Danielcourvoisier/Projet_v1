//
// Created by Daniel Courvoisier on 14.05.20.
//

#ifndef PROJET_V1_CARFLOW_HPP
#define PROJET_V1_CARFLOW_HPP

#include <thread>
#include "Course.hpp"
#include "TrafficLight.hpp"
#include "Crossroads.hpp"
#include "Car.hpp"
#include "Timer.hpp"



// Classe CarFlow
class CarFlow {        //modification avec timer
private:
    Course * course;
    int orientation;
    MyTimer* timer;
    TrafficLight* crossingLights;
    int co_voitures = 0;
    std::vector<std::thread> FLUX;  //typedef std::thread X;
    thread thd;

public:
    CarFlow(Course *C, int carOrientation, MyTimer* aTimer, TrafficLight* CL);
    ~CarFlow();
    void createCars();
};


// Constructeur
CarFlow::CarFlow(Course *C, int carOrientation, MyTimer* aTimer, TrafficLight* CL) {
    course = C;
    orientation = carOrientation;
    timer = aTimer;
    crossingLights = CL;
    thd = thread(&CarFlow::createCars, this);
}


// Destructeur
CarFlow::~CarFlow() {
    thd.join();
}


// Créer voiture
void CarFlow::createCars() {
    while (timer->getTimerState() != 4) {
        Car VX("V_" + std::to_string(orientation) + "_" + std::to_string(co_voitures++), orientation, course, &crossingLights[orientation]);
        FLUX.push_back(std::thread(&Car::drive,VX)); //A ne pas mettre la référence mais l'objet lui-même
        int attente = rand()%200;
        std::this_thread::sleep_for(std::chrono::milliseconds(attente));// @suppress("Function cannot be resolved")
    }

    // Join flux voiture
    for(auto it = std::begin(FLUX); it != std::end(FLUX); ++it) {
        (*it).join();
    }
    cout << "toutes les voitures de l'orientation " << orientation << " ont fini leur exécution" << endl;
}

#endif
