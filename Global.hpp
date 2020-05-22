//
// Created by Daniel Courvoisier on 14.05.20.
//

#ifndef PROJET_V1_GLOBAL_HPP
#define PROJET_V1_GLOBAL_HPP

#include <mutex>

// Constantes
const int NB_BUCKETS        = 3;
const int BUCKET_CAPACITY   = 5;
const int QTY_TO_PRODUCE    = 20;


// Mutex globaux
std::recursive_mutex displayLock;
std::mutex myLocks[NB_BUCKETS];
std::condition_variable CV1[NB_BUCKETS];

std::mutex arret_carrefour;
std::mutex verrou_parcours;
std::condition_variable CV2[2];

// Variable globales
bool stopWork = 0;
int  precedent_owner[3];

#endif
