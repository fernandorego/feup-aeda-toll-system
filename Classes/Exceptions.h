#ifndef AEDA2020_PORTAGENS_EXCEPTIONS_H
#define AEDA2020_PORTAGENS_EXCEPTIONS_H

#include <string>
using namespace std;

/*class DontExistAnyHighway {
public:
    DontExistAnyHighway() = default;;
};

class DontExistAnyToll {
public:
    DontExistAnyToll() = default;
};

class DontExistAnyLane {
public:
    DontExistAnyLane() = default;
};

 class DontExistAnyVehicle {
public:
    DontExistAnyVehicle() = default;
};*/

class DontExistAnyMovement {
public:
    DontExistAnyMovement() = default;
};

class DontExistAnyExitMovement {
public:
    DontExistAnyExitMovement() = default;
};

class VehicleDoesNotExist {
    string plate;
public:
    VehicleDoesNotExist(string plate) : plate(plate) {}
    string getPlate() const {return plate;}
};

class EmployeeDoesNotExist {
    string name;
public:
    EmployeeDoesNotExist(string name) : name(name) {}
    string getName() const {return name;}
};

#endif //AEDA2020_PORTAGENS_EXCEPTIONS_H