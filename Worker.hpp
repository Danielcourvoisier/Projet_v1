//
// Created by Daniel Courvoisier on 26.03.20.
//

#ifndef PROJET_V1_WORKER_HPP
#define PROJET_V1_WORKER_HPP

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
extern int precedent_owner[3];
extern std::recursive_mutex displayLock;
extern std::mutex myLocks[NB_BUCKETS];
extern std::condition_variable CV1[NB_BUCKETS];


class Worker {
private:
    thread thd;
    vector<Bucket> *buckets;
    int quantityReceived;
    int bucketNb;
    void doHisJob();
    void goBackGetWood();
    void emptyBucket();
    void bringWoodToSawmill();
    void cutWood();


public:
    Worker(vector<Bucket> *bucketv);
    ~Worker();
};


// Constructeur
Worker::Worker(vector<Bucket> *bucketv) {
    quantityReceived = 0;
    bucketNb = 2;
    buckets = bucketv;
    precedent_owner[2] = 3;
    thd = thread(&Worker::doHisJob, this);
}


// Destructeur
Worker::~Worker() {
    thd.join();
}


// Travail à faire
void Worker::doHisJob() {
    myLocks[bucketNb].lock();
    while (quantityReceived < QTY_TO_PRODUCE) {
        goBackGetWood();
        emptyBucket();
        bringWoodToSawmill();
        cutWood();
    }
    displayLock.lock();
    cout << "L'ouvrier termine son activité" << endl;
    displayLock.unlock();
    myLocks[bucketNb].unlock();
    stopWork = 1;   // averti les autres de stopper leurs activités
}


// Revenir chercher bois
void Worker::goBackGetWood() {
    int attente = rand() % 300;
    this_thread::sleep_for(std::chrono::milliseconds(attente));
    displayLock.lock();
    cout << "L'ouvrier revient chercher du bois" << endl;
    displayLock.unlock();
}


// Vider la benne
void Worker::emptyBucket() {
    displayLock.lock();
    cout << "L'ouvrier veut vider la benne " << bucketNb << endl;
    displayLock.unlock();

    if ((*buckets)[bucketNb].isEmpty(bucketNb)) {
        cout << "L'ouvrier va libérer la benne " << bucketNb << endl;               // lock?
        myLocks[bucketNb].unlock();
        bucketNb = (bucketNb + 1) % 3;
        CV1[bucketNb].notify_one();
        displayLock.lock();
        cout << "L'ouvrier va essayer de récupérer la benne "<< bucketNb << endl;  // new mutex
        displayLock.unlock();
        {
            mutex m;
            unique_lock<mutex> lck(m);
            while (precedent_owner[bucketNb] != 2) {
                CV1[bucketNb].wait(lck);
            }
        }
        myLocks[bucketNb].lock();
        precedent_owner[bucketNb] = 3;
    }

    (*buckets)[bucketNb].empty(1, bucketNb);
    displayLock.lock();
    cout << "L'ouvrier a retiré le bois de la benne " << bucketNb << endl;      //new
    displayLock.unlock();
    quantityReceived++;
}


// Apporter bois à la scierie
void Worker::bringWoodToSawmill() {
    int attente = rand() % 300;
    this_thread::sleep_for(std::chrono::milliseconds(attente));
    displayLock.lock();
    cout << "L'ouvrier apporte le bois à la scierie" << endl;
    displayLock.unlock();
}


// Découper le bois
void Worker::cutWood() {
    int attente = rand() % 3000;
    this_thread::sleep_for(std::chrono::milliseconds(attente));
    displayLock.lock();
    cout << "L'ouvrier découpe le bois" << endl;
    displayLock.unlock();
}

#endif
