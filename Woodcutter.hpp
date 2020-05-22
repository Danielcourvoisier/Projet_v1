//
// Created by Daniel Courvoisier on 26.03.20.
//

#ifndef BENNES_WOODCUTTER_HPP
#define BENNES_WOODCUTTER_HPP

#include <iostream>
#include <cstdio>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <chrono>
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


class Woodcutter {
private:
    thread thd;
    vector<Bucket> *buckets;
    int quantityCutted;
    int bucketNb;
    void doHisJob();
    void cutWood();
    void bringWood();
    void fillBucket();
    void goBack();
public:
    Woodcutter(vector<Bucket> *bucketv);
    ~Woodcutter();

};


// Constructeur
Woodcutter::Woodcutter(vector<Bucket> *bucketv) {
    buckets = bucketv;
    quantityCutted = 0;
    bucketNb = 0;
    precedent_owner[0] = 1;
    thd = thread(&Woodcutter::doHisJob, this);
}


// Destructeur
Woodcutter::~Woodcutter() {
    thd.join();
}


// Travail à faire
void Woodcutter::doHisJob() {
    myLocks[bucketNb].lock();
    while (!stopWork) {
        cutWood();
        bringWood();
        fillBucket();
        goBack();
    }
    displayLock.lock();
    cout << "Le bûcheron termine son activité" << endl;
    displayLock.unlock();
    myLocks[bucketNb].unlock();
}


// Coupe le bois
void Woodcutter::cutWood() {
    int attente = rand() % 1000;
    this_thread::sleep_for(std::chrono::milliseconds(attente));
    displayLock.lock();
    cout << "Le bûcheron coupe du bois" << endl;
    displayLock.unlock();
    quantityCutted++;
}


// Rapporte le bois
void Woodcutter::bringWood() {
    int attente = rand() % 1000;
    this_thread::sleep_for(std::chrono::milliseconds(attente));
    displayLock.lock();
    cout << "Le bûcheron rapporte du bois" << endl;
    displayLock.unlock();
}


// Remplissage benne
void Woodcutter::fillBucket() {
    displayLock.lock();
    cout << "Le bûcheron veut remplir la benne " << bucketNb << endl;
    displayLock.unlock();

    if ((*buckets)[bucketNb].isFull(bucketNb)) {
        cout << "Le bûcheron libére la benne " << bucketNb << endl;
        myLocks[bucketNb].unlock();
        bucketNb = (bucketNb + 1) % 3;
        CV1[bucketNb].notify_one();
        displayLock.lock();
        cout << "Le bûcheron va essayer de récupérer la benne " << bucketNb << endl;    // new mutex
        displayLock.unlock();
        {
            mutex m;
            unique_lock<mutex> lck(m);
            while (precedent_owner[bucketNb] != 2) {
                CV1[bucketNb].wait(lck);
            }
        }
        myLocks[bucketNb].lock();
        precedent_owner[bucketNb] = 1;
    }
    (*buckets)[bucketNb].fill(quantityCutted, bucketNb);
    quantityCutted = 0;
}


// Retourne en forêt
void Woodcutter::goBack() {
    int attente = rand() % 1000;
    this_thread::sleep_for(std::chrono::milliseconds(attente));
    displayLock.lock();
    cout << "Le bûcheron retourne en forêt" << endl;
    displayLock.unlock();
}

#endif
