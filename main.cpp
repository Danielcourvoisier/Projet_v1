#include <iostream>
#include "Bucket.hpp"
#include "Woodcutter.hpp"
#include "Carrier.hpp"
#include "Worker.hpp"
#include "Global.hpp"
#include "Timer.hpp"
#include "Course.hpp"
#include "TrafficLight.hpp"
#include "CrossingLightManagement.hpp"
#include "CarFlow.hpp"

int main() {

    // Instanciation et démarrage timer
    MyTimer timer1("Timer Simulation");
    timer1.startTimer(10000);

    // Instanciation des bennes
    vector<Bucket> buckets(NB_BUCKETS);

    // Instanciation bucheron, transporteur et ouvrier
    Woodcutter woodcutter(&buckets);
    Carrier    carrier   (&buckets);
    Worker     worker    (&buckets);

    // Instanciation carrefour
    Crossroads crossroads;

    // Instanciation parcours
    Course course_1(0, &crossroads); // orientation Nord-Sud (Flux A) // rajouter sens inverse
    Course course_2(1, &crossroads); // orientation Sud-Nord (Flux B) // rajouter sens inverse
    // Transporteur va à l'usine West-Est
    // Transporteur va en fôret

    // Instanciation de feu
    TrafficLight trafficLights[2]; // 1 pour NS-SN, 1 pour WE-EW

    // Instanciation gestion du feux
    CrossingLightManagement crossingLightManagement(&course_1, &course_2, trafficLights, &crossroads);

    // Instanciation des générateurs de flux de voitures
    CarFlow carFlow_NS(&course_1, 0, &timer1, trafficLights);
    CarFlow carFlow_WE(&course_2, 1, &timer1, trafficLights);


    // Fin programme
    return EXIT_SUCCESS;
}
