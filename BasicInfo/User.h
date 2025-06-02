#ifndef USER_H
#define USER_H
#include <iostream>
#include "../Tool/MyChar.h"

using std::ostream;

class UserManagement;
class TrainManagement;
class TicketManagement;
class User {

    MyChar<24> username;
    MyChar<32> password;
    MyChar<24> name;
    MyChar<32> mailAddr;
    int privilege = -1;

    friend class UserManagement;
    friend class TrainManagement;
    friend class TicketManagement;

public:
    User() = default;

    User(const MyChar<24> &username, const MyChar<32> &password, const MyChar<24> &name, const MyChar<32> &mailAddr,
         int privilege): username(username), password(password), name(name), mailAddr(mailAddr), privilege(privilege) {
    }

    User(const User &other) = default;

    User &operator=(const User &other) {
        if (this != &other) {
            username = other.username;
            password = other.password;
            name = other.name;
            mailAddr = other.mailAddr;
            privilege = other.privilege;
        }
        return *this;
    }

    friend ostream &operator<<(ostream &out, const User &obj) {
        out << obj.username << ' ' << obj.name << ' ' << obj.mailAddr << ' ' << obj.privilege;
        return out;
    }
};
#endif //USER_H
