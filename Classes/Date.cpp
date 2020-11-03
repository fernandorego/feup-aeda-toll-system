//
// Created by mim on 31/10/20.
//

#include "Date.h"

Date::Date(string& date) : s_date(date) { //"year/month/day hour:minute:second"
    int count = 0;
    while (count != 6) {
        int index;
        string s;
        for (int i = 0; i < date.length(); i++) {
            if (date[i] == '/' || date[i] == ':') {
                s = date.substr(0,i);
                date.erase(0,i+1);
                break;
            }
            else if (date[i] == ' ') {
                s = date.substr(0,i);
                date.erase(0,i+2);
                break;
            }
            if (i == date.length()-1)
                s = date;
        }
        switch (count) {
            case 0:
                day = stoi(s, nullptr);
                count++;
                break;
            case 1:
                month = stoi(s, nullptr);
                count++;
                break;
            case 2:
                year = stoi(s, nullptr);
                count++;
                break;
            case 3:
                hour = stoi(s, nullptr);
                count++;
                break;
            case 4:
                minute = stoi(s, nullptr);
                count++;
                break;
            case 5:
                second = stoi(s, nullptr);
                count++;
                break;
        }
    }
}

int Date::getYear() const {return year;}

int Date::getMonth() const {return month;}

int Date::getDay() const {return day;}

int Date::getHour() const {return hour;}

int Date::getMinute() const {return minute;}

int Date::getSecond() const {return second;}

string Date::getInfo() const {return s_date;}

void Date::setYear(const int new_year) { year = new_year; }

void Date::setMonth(const int new_month) { month = new_month; }

void Date::setDay(const int new_day) { day = new_day; }

void Date::setHour(const int new_hour) { hour = new_hour; }

void Date::setMinute(const int new_minute) { minute = new_minute; }

void Date::setSecond(const int new_second) { second = new_second; }