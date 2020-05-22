//
// Created by Daniel Courvoisier on 14.05.20.
//

#ifndef PROJET_V1_CROSSROADS_HPP
#define PROJET_V1_CROSSROADS_HPP

#include <string>
#include "Global.hpp"

class Crossroads {
private:
    int compteurOccupation;
public:
    Crossroads();
    void occuper_carrefour(string nom);
    void sortir_carrefour(string nom);
    int get_occupation();
};


// Constructeur
Crossroads::Crossroads() {
    compteurOccupation = 0;
}


// Occuper carrefour
void Crossroads::occuper_carrefour(string nom){
    compteurOccupation++;
}


// Sortir carrefour
void Crossroads::sortir_carrefour(string nom){
    compteurOccupation--;
}


// Retourne occupation
int Crossroads::get_occupation(){
    return compteurOccupation;
}

#endif
