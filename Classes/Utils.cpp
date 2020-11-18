//
// Created by mim on 03/11/20.
//

#include "Utils.h"

#include <iostream>

using namespace std;

int Utils::ShowMenu(vector<string> menu) {
    cout << "Please choose one of the options below: " << endl << endl;
    for (size_t i = 0; i < menu.size(); i++) {
        cout << (i+1) << " - " << menu[i] << endl;
    }
    cout << "0 - Exit" << endl << endl;
    return getNumber(menu.size());
}

int Utils::getNumber(int max) {
    string s_index;
    int index;
    while (true) {
        bool check = false;
        do {
        getline(cin, s_index);
        } while(s_index.empty());
        for (size_t i = 0; i < s_index.size(); i++) {
            if (s_index[i] < 48 || s_index[i] > 57 || s_index == "") {
                cout << "Error: Invalid input." << endl;
                check = true;
                break;
            }
        }
        if (check)
            continue;
        index = stoi(s_index);
        if (index > max) {
            cout << "Error: Invalid input, please choose one of above." << endl;
            continue;
        }
        break;
    }
    return index;
}

float Utils::getFloat() {
    string s_index;
    float index;
    while (s_index != "EXIT") {
        bool check = false;
        getline(cin, s_index);
        if (s_index == "EXIT")
            return -1;
        for (size_t i = 0; i < s_index.size(); i++) {
            if ((s_index[i] < 48 || s_index[i] > 57) && s_index[i] != 46) {
                cout << "Error: Invalid input. Example: 2 or 2.5" << endl;
                check = true;
                break;
            }
        }
        if (check)
            continue;
        index = stof(s_index);
        if (index < 0) {
            cout << "Error: Invalid input. Input a positive number" << endl;
            continue;
        }
        break;
    }
    return index;
}

void Utils::waitForInput() {
    string a;
    cout << '\n' << "Press enter to continue...";
    getline(cin, a);
    cout << "\x1b[A";
    //cout << "\033[2J\033[H";
}

void Utils::clrScreen() {
    cout << "\033[2J\033[H";
}