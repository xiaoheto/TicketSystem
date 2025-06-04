#ifndef USERMANAGEMENT_H
#define USERMANAGEMENT_H

#include <fstream>
#include <filesystem>

#include "../BPT/BPlusTree.h"
#include "../BPT/MemoryRiver.h"
#include "../BasicInfo/User.h"
#include "../BPT/STLite/map.h"
#include "../BPT/STLite/utility.h"

using std::ofstream;
using std::ios;
using std::ostream;
using sjtu::map;
using sjtu::pair;
class TicketManagement;
class TrainManagement;

class UserManagement {
    BPTree<MyChar<24>, int, 50> user_data; // username -> index
    MemoryRiver<User, 2> user_index;
    int size = 0;

    friend class TicketManagement;
    friend class TrainManagement;

public:
    map<MyChar<24>, int> LogInStack; // <username,privilege>

    UserManagement(): user_data("user.data") {
        user_index.initialise("user.index");
        user_index.get_info(size, 1);
    }

    ~UserManagement() {
        user_index.write_info(size, 1);
    }

    int add_user(const MyChar<24> &cur_username, const MyChar<24> &username, const MyChar<32> &password,
                 const MyChar<24> &name, const MyChar<32> &mailAddr, int privilege) {
        if (size == 0) {
            // the first user
            User new_user(username, password, name, mailAddr, 10);
            int offset = user_index.write(new_user);
            user_data.insert(username, offset);
            ++size;
            return 0;
        }

        auto it = LogInStack.find(cur_username);
        if (it == LogInStack.end()) {
            return -1;
        }
        if (!user_data.query(username).empty()) {
            return -1;
        }
        if (privilege >= it->second) {
            return -1;
        }

        User new_user(username, password, name, mailAddr, privilege);
        int offset = user_index.write(new_user);
        user_data.insert(username, offset);
        ++size;
        return 0;
    }

    int login(const MyChar<24> &username, const MyChar<32> &password) {
        vector<int> user_res = user_data.query(username);
        if (user_res.empty()) {
            return -1;
        }
        auto it = LogInStack.find(username);
        if (it != LogInStack.end()) {
            return -1;
        }

        User user;
        user_index.read(user, user_res[0]);
        if (user.password != password) {
            return -1;
        }

        LogInStack.insert({username, user.privilege});
        return 0;
    }

    int logout(const MyChar<24> &username) {
        auto it = LogInStack.find(username);
        if (it == LogInStack.end()) {
            return -1;
        }
        LogInStack.erase(it);
        return 0;
    }

    pair<User, bool> query_profile(const MyChar<24> &cur_username, const MyChar<24> &username) {
        auto it = LogInStack.find(cur_username);
        User user;
        if (it == LogInStack.end()) {
            return {user, false};
        }

        vector<int> res_cur = user_data.query(cur_username);
        vector<int> res_user = user_data.query(username);
        User cur_user;
        user_index.read(cur_user, res_cur[0]);
        user_index.read(user, res_user[0]);

        if (!(cur_username == username) && cur_user.privilege <= user.privilege) {
            return {user, false};
        }

        return {user, true};
    }

    pair<User, bool> modify_profile(const MyChar<24> &cur_username, const MyChar<24> &username,
                                    const MyChar<32> &password, const MyChar<24> &name,
                                    const MyChar<32> &mailAddr, int privilege) {
        auto it = LogInStack.find(cur_username);
        User user;
        if (it == LogInStack.end()) {
            return {user, false};
        }
        auto index = user_data.query(username);
        if (index.empty()) {
            return {user, false};
        }
        user_index.read(user, index[0]);
        if (it->second < user.privilege || (it->second == user.privilege && cur_username != username) || privilege > it
            ->
            second) {
            return {user, false};
        }
        if (privilege != -1) {
            user.privilege = privilege;
        }
        if (!password.empty()) {
            user.password = password;
        }
        if (!name.empty()) {
            user.name = name;
        }
        if (!mailAddr.empty()) {
            user.mailAddr = mailAddr;
        }
        user_index.update(user, index[0]);
        return {user, true};
    }

    static void clean_user_file() {
        std::filesystem::path path1("user.data");
        std::filesystem::path path2("user.index");
        if (exists(path1)) std::filesystem::remove(path1);
        if (exists(path2)) std::filesystem::remove(path2);
    }
};

#endif
