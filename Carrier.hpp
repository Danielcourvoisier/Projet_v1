//
// Created by Daniel Courvoisier on 26.03.20.
//

#ifndef PROJET_V1_CARRIER_HPP
#define PROJET_V1_CARRIER_HPP

#include <iostream>
#include <cstdio>
#include <vector>
#include <thread>
#include <mutex>
#include "Bucket.hpp"
#include "Global.hpp"


// Namespace
using namespace std;


// Variables globales
extern bool stopWork;
extern int  precedent_owner[3];

extern std::recursive_mutex displayLock;
extern std::mutex myLocks[NB_BUCKETS];
extern std::condition_variable CV1[NB_BUCKETS];


class Carrier {
private:
    thread thd;
    vector<Bucket> *buckets;
    int bucketNb;
    void doHisJob();
    void dropEmptyBucket();
    void dropFullBucket();
    void pickEmptyBucket();
    void pickFullBucket();
    void goToFactory();
    void goToForest();
public:
    Carrier(vector<Bucket> *bucket);
    ~Carrier();
};



// Constructeur
Carrier::Carrier(vector<Bucket> *bucket) {
    buckets     = bucket;
    bucketNb    = 1;
    precedent_owner[1] = 2;
    thd = thread(&Carrier::doHisJob, this);
}


// Destructeur
Carrier::~Carrier() {
    thd.join();
}


// Travail à faire
void Carrier::doHisJob() {
    myLocks[bucketNb].lock();
    while (!stopWork) {
        goToForest();
        dropEmptyBucket();
        pickFullBucket();
        goToFactory();
        dropFullBucket();
        pickEmptyBucket();
    }
    displayLock.lock();
    cout << "Le transporteur termine son activité" << endl;
    displayLock.unlock();
    myLocks[bucketNb].unlock();
}


// Déposer benne vide à la forêt
void Carrier::dropEmptyBucket() {
    int attente = rand() % 1000;
    this_thread::sleep_for(std::chrono::milliseconds(attente));
    displayLock.lock();
    cout << "Le transporteur dépose la benne " << bucketNb << " en forêt" << endl;
    displayLock.unlock();
    myLocks[bucketNb].unlock();
    bucketNb = (3 + bucketNb-1)%3;
    CV1[bucketNb].notify_one();
}


// Déposer benne pleine à l'usine
void Carrier::dropFullBucket() {
    int attente = rand() % 1000;
    this_thread::sleep_for(std::chrono::milliseconds(attente));
    displayLock.lock();
    cout << "Le transporteur dépose la benne " << bucketNb << " à l'usine" << endl;
    displayLock.unlock();
    myLocks[bucketNb].unlock();
    bucketNb = (3 + bucketNb-1)%3;
    CV1[bucketNb].notify_one();
}


// Prendre benne vide à l'usine
void Carrier::pickEmptyBucket() {
    int attente = rand() % 1000;
    this_thread::sleep_for(std::chrono::milliseconds(attente));
    displayLock.lock();
    cout << "Le transporteur veut amarrer la benne " << bucketNb << " à l'usine" << endl;
    displayLock.unlock();

    {
        mutex m;
        unique_lock<mutex> lck(m);
        while (precedent_owner[bucketNb] != 3) {
            CV1[bucketNb].wait(lck);
        }
    }

    myLocks[bucketNb].lock();
    precedent_owner[bucketNb] = 2;

    displayLock.lock();
    cout << "Le transporteur a réussi à amarrer la benne " << bucketNb << " à l'usine" << endl;
    displayLock.unlock();
}


// Prendre benne pleine à en forêt
void Carrier::pickFullBucket() {
    int attente = rand() % 1000;
    this_thread::sleep_for(std::chrono::milliseconds(attente));
    displayLock.lock();
    cout << "Le transporteur veut prendre la benne " << bucketNb << " en forêt" << endl;
    displayLock.unlock();

    {
        mutex m;
        unique_lock<mutex> lck(m);
        while (precedent_owner[bucketNb] != 1) {
            CV1[bucketNb].wait(lck);
        }
    }
    myLocks[bucketNb].lock();
    precedent_owner[bucketNb] = 2;

    displayLock.lock();
    cout << "Le transporteur a réussi à amarrer la benne " << bucketNb << " en forêt" << endl;
    displayLock.unlock();
}


// Aller à l'usine
void Carrier::goToFactory() {
    int attente = rand() % 1000;
    this_thread::sleep_for(std::chrono::milliseconds(attente));
    displayLock.lock();
    cout << "Le transporteur va à l'usine" << endl;
    displayLock.unlock();


    // Ajouter passage carrefour



}


// Aller à la forêt
void Carrier::goToForest() {
    int attente = rand() % 1000;
    this_thread::sleep_for(std::chrono::milliseconds(attente));
    displayLock.lock();
    cout << "Le transporteur va en forêt" << endl;
    displayLock.unlock();

    attente = rand() % 1000;  // Temps pour aller arriver jusqu'au carrefour
    this_thread::sleep_for(std::chrono::milliseconds(attente));

    // Ajouter passage carrefour
    displayLock.lock();
    cout << "Le transporteur arrive début du carrefour" << endl;
    displayLock.unlock();

}

#endif
