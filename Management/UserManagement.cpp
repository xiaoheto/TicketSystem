//
// Created by 43741 on 2025/5/17.
//

#include "UserManagement.h"

UserManagement::UserManagement() {
    user_index.initialise("user_info");
    user_index.get_info(size,1);
}

int UserManagement::add_first_user(const MyChar<24> &username, const MyChar<32> &password, const MyChar<24> &name, const MyChar<32> &mailAddr) {
}

int UserManagement::add_user(const MyChar<24> &cur_username, const MyChar<24> &username, const MyChar<32> &password, const MyChar<24> &name, const MyChar<32> &mailAddr, int privilege) {
}

int UserManagement::login(const MyChar<24> &username, const MyChar<32> &password) {
}

int UserManagement::logout(const MyChar<24> &username) {
}

User UserManagement::query_profile(const MyChar<24> &cur_username, const MyChar<24> &username) {
}

User UserManagement::modify_profile(const MyChar<24> &cur_username, const MyChar<24> &username, const MyChar<32> &password, const MyChar<24> &name, const MyChar<32> &mailAddr, int privilege) {
}


