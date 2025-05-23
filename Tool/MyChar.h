#ifndef MYCHAR_H
#define MYCHAR_H
#include <cstring>
#include <iomanip>

using std::string;
using std::strcpy;
using std::istream;
using std::ostream;

class UserManagement; //防止循环包含
template<int length>
class MyChar {
    char myChar[length];
    friend class UserManagement;

public:
    MyChar() : myChar("") {
    }

    MyChar(const string &str) {
        std::strncpy(myChar, str.c_str(), length - 1);
        myChar[length - 1] = '\0';
    }

    MyChar(const MyChar &other) {
        strcpy(myChar, other.myChar);
    }

    explicit MyChar(const char *ch) {
        strcpy(myChar, ch);
    }

    const char *c_str() const {
        return myChar;
    }

    MyChar &operator=(const MyChar &other) {
        if (this != &other) {
            strcpy(myChar, other.myChar);
        }
        return *this;
    }

    bool empty() const {
        return myChar[0] == '\0';
    }

    bool operator==(const MyChar &other) const {
        return strcmp(myChar, other.myChar) == 0;
    }

    bool operator!=(const MyChar &other) const {
        return !(*this == other);
    }

    bool operator<(const MyChar &other) const {
        return strcmp(myChar, other.myChar) < 0;
    }

    bool operator>(const MyChar &other) const {
        return strcmp(myChar, other.myChar) > 0;
    }

    bool operator<=(const MyChar &other) const {
        return *this == other || *this < other;
    }

    bool operator>=(const MyChar &other) const {
        return *this == other || *this > other;
    }

    friend istream &operator>>(istream &in, MyChar &obj) {
        in >> obj.myChar;
        return in;
    }

    friend ostream &operator<<(ostream &out, const MyChar &obj) {
        out << obj.myChar;
        return out;
    }
};
#endif //MYCHAR_H
