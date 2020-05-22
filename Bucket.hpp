//
// Created by Daniel Courvoisier on 26.03.20.
//
#ifndef PROJET_V1_BUCKET_HPP
#define PROJET_V1_BUCKET_HPP

#include <iostream>
#include <cstdio>
#include <mutex>
#include <cstdbool>
#include "Global.hpp"

using namespace std;


class Bucket {
private:
    int woodQuantity = 0;
    int capacity = BUCKET_CAPACITY;
public:
    void fill   (int quantity, int bucketNb);
    void empty  (int quantity, int bucketNb);
    bool isFull (int bucketNb);
    bool isEmpty(int bucketNb);

};


// Remplir benne
void Bucket::fill(int quantity, int bucketNb) {
    if (woodQuantity <= capacity) {
        cout << "Le bûcheron rempli la benne " << bucketNb << endl;
        woodQuantity += quantity;
    } else {
        cout << "La benne " << bucketNb << " est pleine, impossible de remplir" << endl;
    }
}


// Vider benne
void Bucket::empty(int quantity, int bucketNb) {
    if (woodQuantity > 0) {
        cout << "L'ouvrier retire du bois de la benne " << bucketNb << endl;
        woodQuantity -= quantity;
    } else {
        cout << "La benne " << bucketNb << " est vide, impossible de vider" << endl;
    }
}


// Benne remplie
bool Bucket::isFull(int bucketNb) {
    if (woodQuantity >= capacity) {
        cout << "La benne " << bucketNb << " est remplie" << endl;
        return true;
    }
    return false;
}


// Benne vide
bool Bucket::isEmpty(int bucketNb) {
    if (woodQuantity == 0) {
        cout << "La benne " << bucketNb << " est vide" << endl;
        return true;
    }
    return false;
}

#endif
