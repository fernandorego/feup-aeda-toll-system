//
// Created by mim on 03/11/20.
//

#ifndef AEDA2020_PORTAGENS_UTILS_H
#define AEDA2020_PORTAGENS_UTILS_H

#include <string>
#include <vector>
using namespace std;
class Utils {
public:
    Utils() =default;
    int ShowMenu(vector<string> menu);
    int getNumber(int max);
    float getFloat();
    string getString();
};


#endif //AEDA2020_PORTAGENS_UTILS_H
