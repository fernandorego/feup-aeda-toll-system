//
// Created by mim on 22/10/20.
//

#include "Vehicle.h"

using namespace std;

Vehicle::Vehicle(string plate, int v_class): plate(plate), v_class(v_class) {}

void Vehicle::defineLaneType(bool greenlane){this->greenlane=greenlane;}


