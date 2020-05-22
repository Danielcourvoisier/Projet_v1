//
// Created by Daniel Courvoisier on 14.05.20.
//

#ifndef PROJET_V1_TIMER_HPP
#define PROJET_V1_TIMER_HPP

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <string>
#include "Global.hpp"

// Namespace
using namespace std;

// Variables globales
extern std::recursive_mutex displayLock;

// Classe MyTimer
class MyTimer {
private:
    int timerState;
    int remainingTime;
    string timerName;
    thread thd;

    void counter(int duration);

public:
    MyTimer(string name);
    ~MyTimer();
    void startTimer(int duration);
    void stopTimer();
    void initTimer(int duration);
    void restartTimer();
    int getTimerState();
};


// Constructeur
MyTimer::MyTimer(string name) {
    timerName = name;
    timerState = 0;
    remainingTime = 0;
}


// Destructeur
MyTimer::~MyTimer() {
    if (thd.joinable()) {
        thd.join();
    }
}



// Comptage timer (a être dans thread)
void MyTimer::counter(int duration) {
    long time_elapsed;
    timerState = 1;

    auto start = std::chrono::high_resolution_clock::now();
    do {
        auto end = std::chrono::high_resolution_clock::now();
        time_elapsed =  std::chrono::duration_cast<std::chrono::milliseconds> (end-start).count();
    } while ((time_elapsed < duration ) && (timerState == 1));

    displayLock.lock();
    if (timerState == 1) {
        cout << "\n" << timerName << " : Temps écoulé: timer s'arrête après : " << time_elapsed << " millisecondes." << endl;
        timerState = 4; // timer au repos

    } else {
        cout << "\n" << timerName << " : Timer stoppé après : " << time_elapsed << " millisecondes." << endl;
        remainingTime = duration - time_elapsed;
    }
    displayLock.unlock();
}

// Démarrer le timer
void MyTimer::startTimer(int duration) {
    thd = thread(&MyTimer::counter, this, duration);
}


// Arrêter le timer
void MyTimer::stopTimer() {                 // n'arrête pas le timer!!!!!!!!
    timerState = 2;
    displayLock.lock();
    cout << "\n" << timerName << " est stoppé avant la fin de son exécution" << endl;
    displayLock.unlock();
}


// Remsie à zéro du timer
void MyTimer::initTimer(int duration){
    remainingTime = duration;
    timerState = 0;
    displayLock.lock();
    cout << "\n" << timerName << " est réinitialisé" << endl;
    displayLock.unlock();
}


// Redémarrer le timer
void MyTimer::restartTimer(){
    startTimer(remainingTime);
}


// Retourner état timer
int MyTimer::getTimerState() {
    return timerState;
}


#endif //CARREFOUR_TIMER_HPP
