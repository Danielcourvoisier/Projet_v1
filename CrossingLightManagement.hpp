//
// Created by Daniel Courvoisier on 14.05.20.
//

#ifndef PROJET_V1_CROSSINGLIGHTMANAGEMENT_HPP
#define PROJET_V1_CROSSINGLIGHTMANAGEMENT_HPP

#include <mutex>
#include <thread>
#include <string>
#include <iostream>
#include "Global.hpp"
#include "Course.hpp"
#include "TrafficLight.hpp"
#include "Crossroads.hpp"

// Variables globales
extern std::recursive_mutex displayLock;


// Classe CrossingLightManagement
class CrossingLightManagement {
private:
    Course *course1;
    Course *course2;
    TrafficLight *crossingLight;
    Crossroads *crossroads;
    int state;
    thread thd;

public:
    CrossingLightManagement(Course* C1, Course* C2, TrafficLight* CL, Crossroads* CR);
    ~CrossingLightManagement();

    [[noreturn]] void detecter();
};


// Constructeur
CrossingLightManagement::CrossingLightManagement(Course *C1, Course *C2, TrafficLight *CL, Crossroads *CR) {
    course1 = C1;
    course2 = C2;
    crossingLight = CL,
    crossroads = CR;
    state = 0;
    thd = thread(&CrossingLightManagement::detecter, this);
}


// Destructeur
CrossingLightManagement::~CrossingLightManagement() {
    thd.join();
}


[[noreturn]] // Detecter
void CrossingLightManagement::detecter() {
    bool aFirst = true;
    bool bFirst = false;

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        if ((state == 0) && (aFirst) && (course1->getDetect() > 0)) {
            displayLock.lock();
            cout <<"\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> état 0 : FLUX A détecté -> FEU B imposé à ROUGE \n"  << endl;
            crossingLight[1].update("R");
            state = 1;
            displayLock.unlock();
        } else

        if ((state == 0) && (aFirst) && ((!course1->getDetect()) > 0)) {
            displayLock.lock();
            cout <<"\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> état 0 : FLUX A pas détecté -> On bascule \n"  << endl;
            crossingLight[1].update("R");
            aFirst = false;
            bFirst = true;
            displayLock.unlock();
        } else

        if ((state == 0) && (bFirst) && ((!course2->getDetect()) > 0)) {
            displayLock.lock();
            cout <<"\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> état 0 : FLUX B pas détecté -> On bascule \n"  << endl;
            crossingLight[1].update("R");
            aFirst = true;
            bFirst = false;
            displayLock.unlock();
        } else

        // Après que le flux A soit détecté et le feu B imposé à rouge, et si le carrefour est vide, alors on passe le feu à vert pour le flux A
        if ((state == 1) && (!crossroads->get_occupation())) {
            displayLock.lock();
            cout <<"\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> état 1:  FLUX B bloqué et on libère FLUX A ->  FEU A imposé à VERT \n"  << endl;
            crossingLight[0].update("V");
            state = 2;
            CV2[0].notify_all();                     // Notification voiture (selon orientation)
            displayLock.unlock();
        } else

        // Si le flux A est passé au vert, et que le carrefour n'est plus occuper, et que le parcours 1 (Flux A) est détecté -> état attente
        if ((state == 2) && (!crossroads->get_occupation()) && (!course1->getDetect())) {
            displayLock.lock();
            cout <<"\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> état 2: FLUX A tari -> on revient à l'état d'attente\n" << endl;
            state = 0;
            aFirst = false;
            bFirst = true;
            displayLock.unlock();
        } else

        // Si flux B détecté, le feu A est imposé à rouge
        if ((state == 0) && (bFirst) && (course2->getDetect() > 0)) {
            displayLock.lock();
            cout <<"\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> état 0: FLUX B détecté -> FEU A imposé à ROUGE \n"  << endl;
            crossingLight[0].update("R");
            state = 3;
            displayLock.unlock();
        } else

        // Si flux B détecté, Feu A est au rouge, et que le carrefour est libre, alors feu du flux B passe au vert
        if ((state == 3) && (!crossroads->get_occupation())) {
            displayLock.lock();
            cout <<"\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> état 3: FLUX A bloqué et on libère FLUX B -> FEU B imposé à VERT \n"  << endl;
            crossingLight[1].update("V");
            state = 4;
            CV2[1].notify_all();
            displayLock.unlock();
        } else

        // Si le flux B a pu passer dans le carrefour, et qu'il n'est plus occuper, alors en revient en état d'attente
        if ((state == 4) && (!crossroads->get_occupation()) && (!course2->getDetect())) {
            displayLock.lock();
            cout <<"\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> état 4: FLUX B tari -> on revient à l'état d'attente\n" << endl;
            state = 0;
            aFirst = true;
            bFirst = false;
            displayLock.unlock();
        }
    }
}


#endif
