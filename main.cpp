#include <chrono>
#include <string>
#include <sstream>
#include <string>
#include <fstream>
#include <random>
#include <iostream>
using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::pair;
using std::string;
using std::ios;
using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::cin;
using std::cout;
using std::getline;
#include "BPT/BPlusTree.h"
#include "Tool/CommandParser.h"
#include "Tool/MyChar.h"
#include "Tool/Time.h"
#include "Management/UserManagement.h"

vector<string> getStr(const string &input) {
    vector<string> res;
    Command temp(input,'|');
    temp.position = 0;
    while (temp.count--) {
        res.push_back(temp.getNext());
    }
    return res;
}

vector<int> getInt(const string &input) {
    vector<int>res;
    Command temp(input,'|');
    temp.position = 0;
    while(temp.count--) {
        res.push_back(temp.str_to_int(temp.getNext()));
    }
    return res;
}

// int main() {
//     string input;
//     cin >> input;
//     vector<string> res = getStr(input);
//     for(auto i : res) {
//         cout << i << '\n';
//     }
//     return 0;
// }

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    UserManagement usermanagement;
    string input;
    while (getline(cin, input)) {
        Command command(input);
        if (command.cmd == "add_user") {
            MyChar<24> cur_username;
            MyChar<24> username;
            MyChar<32> password;
            MyChar<24> name;
            MyChar<32> mailAddr;
            int privilege = 10;
            if (!usermanagement.flag) {
                while (command.current < command.count) {
                    string key = command.getNext();
                    if (key == "-u") {
                        username = command.getNext();
                    } else if (key == "-p") {
                        password = command.getNext();
                    } else if (key == "-n") {
                        name = command.getNext();
                    } else if (key == "-m") {
                        mailAddr = command.getNext();
                    }
                    command.current += 2;
                }
                usermanagement.flag = true;
                usermanagement.add_first_user(username, password, name, mailAddr);
            } else {
                while (command.current < command.count) {
                    string key = command.getNext();
                    if (key == "-c") {
                        cur_username = command.getNext();
                    } else if (key == "-u") {
                        username = command.getNext();
                    } else if (key == "-p") {
                        password = command.getNext();
                    } else if (key == "-n") {
                        name = command.getNext();
                    } else if (key == "-m") {
                        mailAddr = command.getNext();
                    } else if (key == "-g") {
                        privilege = command.str_to_int(command.getNext());
                    }
                    command.current += 2;
                }
                usermanagement.add_user(cur_username, username, password, name, mailAddr, privilege);
            }
        } else if (command.cmd == "login") {
            MyChar<24> username;
            MyChar<32> password;
            while (command.current < command.count) {
                string key = command.getNext();
                if (key == "-u") {
                    username = command.getNext();
                } else if (key == "-p") {
                    password = command.getNext();
                }
                command.current += 2;
            }
            usermanagement.login(username, password);
        } else if (command.cmd == "logout") {
            string key = command.getNext();
            MyChar<24> username;
            username = command.getNext();
            usermanagement.logout(username);
        } else if (command.cmd == "query_profile") {
            MyChar<24> cur_username;
            MyChar<24> username;
            while (command.current < command.count) {
                string key = command.getNext();
                if (key == "-c") {
                    cur_username = command.getNext();
                } else if (key == "-u") {
                    username = command.getNext();
                }
                command.current += 2;
            }
            cout << usermanagement.query_profile(cur_username, username) << '\n';
        } else if (command.cmd == "modify_profile") {
            MyChar<24> cur_username;
            MyChar<24> username;
            MyChar<32> password;
            MyChar<24> name;
            MyChar<32> mailAddr;
            int privilege;
            while (command.current < command.count) {
                string key = command.getNext();
                if (key == "-c") {
                    cur_username = command.getNext();
                } else if (key == "-u") {
                    username = command.getNext();
                } else if (key == "-p") {
                    password = command.getNext();
                } else if (key == "-n") {
                    name = command.getNext();
                } else if (key == "-m") {
                    mailAddr = command.getNext();
                } else if (key == "-g") {
                    privilege = command.str_to_int(command.getNext());
                }
                command.current += 2;
            }
            cout << usermanagement.modify_profile(cur_username, username, password, name, mailAddr, privilege) << '\n';
        } else if (command.cmd == "add_train") {
            MyChar<24> trainID;
            int stationNum;
            int seatNum;
            vector<string> stations;
            vector<int>prices;
            Clock startTime;
            vector<int>travelTimes;
            vector<int>stopoverTimes;
            Date saleDate;
            MyChar<2> type;

            while(command.current < command.count) {
                string key = command.getNext();
                if (key == "-i") {
                    trainID = command.getNext();
                }
                else if (key == "-n") {
                    stationNum = command.str_to_int(command.getNext());
                }
                else if (key == "-m") {
                    seatNum = command.str_to_int(command.getNext());
                }
                else if (key == "-s") {
                    string s = command.getNext();
                    stations = getStr(s);
                }
                else if (key == "-p") {
                    string p = command.getNext();
                }
            }
        }
        return 0;
    }
}
