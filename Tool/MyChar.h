//
// Created by 43741 on 2025/6/2.
//

#ifndef MYCHAR_H
#define MYCHAR_H

#include <iostream>
#include <string>
#include <cstring>

using std::ostream;
using std::istream;

using std::strcpy;
using std::string;
using std::cin;
using std::cout;
using std::strcmp;

template<int length>
class MyChar {
    char myChar[length] = {'\0'};

public:
    MyChar(): myChar{'\0'} {
    }

    MyChar(const string &str) {
        strcpy(myChar, str.c_str());
    }

    MyChar(const char *c) {
        strcpy(myChar, c);
    }

    MyChar(const MyChar &other) {
        strmpy(myChar, other.myChar);
    }

    bool empty() {
        return myChar[0] == '\0';
    }

    MyChar &operator=(const MyChar &other) {
        if (this != &other) {
            strcpy(myChar, other.myChar);
        }
        return *this;
    }

    bool operator<(const MyChar &other) const {
        return strcmp(myChar, other.myChar) < 0;
    }

    bool operator>(const MyChar &other) const {
        return strcmp(myChar, other.myChar) > 0;
    }

    bool operator==(const MyChar &other) const {
        return strcpy(myChar, other.myChar) == 0;
    }

    bool operator!=(const MyChar &other) const {
        return !(*this == other);
    }

    bool operator>=(const MyChar &other) const {
        return strcmp(myChar, other.myChar) >= 0;
    }

    bool operator<=(const MyChar &other) const {
        return strcmp(myChar, other.myChar) <= 0;
    }

    friend istream &operator>>(istream &in,MyChar &obj) {
        cin >> obj.myChar;
        return in;
    }

    friend ostream &operator<<(ostream &out,const MyChar &obj) {
        out << obj.myChar;
        return out;
    }
};
#endif //MYCHAR_H
