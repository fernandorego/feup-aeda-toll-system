#include "Highway.h"

using namespace std;

Highway::Highway(string name) : name(name) { tolls.clear(); }

int Highway::getNumTolls() const {return tolls.size();}

Toll * Highway::getToll(string name) {
    for (size_t i = 0; i < tolls.size(); i++) {
        if (name == tolls[i]->getName())
            return tolls[i];
    }
    return nullptr;
}

Toll * Highway::getTollIndex(int i) {
    if (i < getNumTolls())
        return tolls[i];
    return nullptr;
}

string Highway::getInfo() const { return name; }

string Highway::showHighway() const {
    return "Highway Name: " + name;
}

void Highway::setName(const string new_name) { name = new_name; }

bool Highway::addToll(string name, string geolocal, float highway_kilometer,bool type) {
    if (!type) {
        Toll *t1 = new TollEntrance(name, geolocal, highway_kilometer);
        for (size_t i = 0; i < tolls.size(); i++) {
            if (tolls[i]->getInfo() == t1->getInfo()) {
                return false;
            }
        }
        tolls.push_back(t1);
    }
    else {
        Toll *t1 = new TollOut(name, geolocal, highway_kilometer);
        for (size_t i = 0; i < tolls.size(); i++) {
            if (tolls[i]->getInfo() == t1->getInfo()) {
                return false;
            }
        }
        tolls.push_back(t1);
    }
    return true;
}

bool Highway::addToll(Toll *t){
    tolls.push_back(t);
}

bool Highway::removeToll(Toll *t1) {
    for (size_t i = 0; i < tolls.size(); i++) {
        if (*tolls[i] == *t1) {
            tolls.erase(tolls.begin()+i);
            return true;
        }
    }
    return false;
}

bool Highway::removeToll(int i) {
    if (i >= tolls.size())
        return false;
    tolls.erase(tolls.begin()+i);
    return true;
}

bool Highway::checkTollName(string name) {
    for (size_t i = 0; i < tolls.size(); i++) {
        if (name == tolls[i]->getName())
            return true;
    }
    return false;
}

bool Highway::operator==(const Highway &l2) { return name == l2.getInfo(); }