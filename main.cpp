#include <string>
#include <sstream>
#include <string>
#include <fstream>
#include <random>
#include <iostream>
#include "BPT/BPlusTree.h"
#include "Management/TrainManagement.h"
#include "Tool/CommandParser.h"
#include "Tool/MyChar.h"
#include "Tool/Time.h"
#include "Management/UserManagement.h"

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;
using sjtu::pair;
using std::string;
using std::ios;
using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;
using std::cin;
using std::cout;
using std::getline;

vector<string> getStr(const string &input) {
    vector<string> res;
    Command temp(input, '|');
    temp.position = 0;
    while (temp.count--) {
        res.push_back(temp.getNext());
    }
    return res;
}

vector<int> getInt(const string &input) {
    vector<int> res;
    res.push_back(0);
    Command temp(input, '|');
    temp.position = 0;
    while (temp.count--) {
        res.push_back(temp.str_to_int(temp.getNext()));
    }
    return res;
}

Clock getStartTime(const string &input) {
    Clock clock(input);
    return clock;
}

SaleDate getSaleDate(const string &input) {
    Command temp(input, '|');
    temp.position = 0;
    string ans1 = temp.getNext();
    string ans2 = temp.getNext();
    SaleDate ans(ans1, ans2);
    return ans;
}

// int main() {
//     string input;
//     cin >> input;
//     Clock res = getStartTime(input);
//     cout << res << '\n';
//     return 0;
// }

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    UserManagement user_management;
    TrainManagement train_management;
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
            if (!user_management.flag) {
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
                user_management.flag = true;
                cout << command.timeStamp << ' ' << user_management.add_first_user(username, password, name, mailAddr)
                        << '\n';
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
                cout << command.timeStamp << ' ' << user_management.add_user(
                    cur_username, username, password, name, mailAddr, privilege) << '\n';
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
            cout << command.timeStamp << ' ' << user_management.login(username, password) << '\n';
        } else if (command.cmd == "logout") {
            string key = command.getNext();
            MyChar<24> username;
            username = command.getNext();
            cout << command.timeStamp << ' ' << user_management.logout(username) << '\n';
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
            if (!user_management.query_profile(cur_username, username).second) {
                cout << command.timeStamp << ' ' << -1 << '\n';
            } else {
                cout << command.timeStamp << ' ' << user_management.query_profile(cur_username, username).first << '\n';
            }
        } else if (command.cmd == "modify_profile") {
            MyChar<24> cur_username;
            MyChar<24> username;
            MyChar<32> password;
            MyChar<24> name;
            MyChar<32> mailAddr;
            int privilege = -1;
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
            if (!user_management.modify_profile(cur_username, username, password, name, mailAddr, privilege).second) {
                cout << command.timeStamp << ' ' << -1 << '\n';
            } else {
                cout << command.timeStamp << ' ' << user_management.modify_profile(
                            cur_username, username, password, name, mailAddr, privilege).first
                        << '\n';
            }
        } else if (command.cmd == "add_train") {
            MyChar<24> trainID;
            int stationNum;
            int seatNum;
            vector<string> stations;
            vector<int> prices;
            Clock startTime;
            vector<int> travelTimes;
            vector<int> stopoverTimes;
            SaleDate saleDate;
            MyChar<2> type;

            while (command.current < command.count) {
                string key = command.getNext();
                if (key == "-i") {
                    trainID = command.getNext();
                } else if (key == "-n") {
                    stationNum = command.str_to_int(command.getNext());
                } else if (key == "-m") {
                    seatNum = command.str_to_int(command.getNext());
                } else if (key == "-s") {
                    string s = command.getNext();
                    stations = getStr(s);
                } else if (key == "-p") {
                    string p = command.getNext();
                    prices = getInt(p);
                } else if (key == "-x") {
                    string x = command.getNext();
                    startTime = getStartTime(x);
                } else if (key == "-t") {
                    string t = command.getNext();
                    travelTimes = getInt(t);
                } else if (key == "-o") {
                    string o = command.getNext();
                    if (o == "_") {
                        stopoverTimes.clear();
                    } else {
                        stopoverTimes = getInt(o);
                    }
                } else if (key == "-d") {
                    string d = command.getNext();
                    saleDate = getSaleDate(d);
                } else if (key == "-y") {
                    type = command.getNext();
                }
                command.current += 2;
            }
            cout << command.timeStamp << ' ' << train_management.add_train(
                trainID, stationNum, seatNum, stations, prices, startTime, travelTimes, stopoverTimes, saleDate,
                type) << '\n';
        } else if (command.cmd == "delete_train") {
            MyChar<24> trainID;
            string i = command.getNext();
            trainID = command.getNext();
            cout << command.timeStamp << ' ' << train_management.delete_train(trainID) << '\n';
        } else if (command.cmd == "release_train") {
            MyChar<24> trainID;
            string i = command.getNext();
            trainID = command.getNext();
            cout << command.timeStamp << ' ' << train_management.release_train(trainID) << '\n';
        } else if (command.cmd == "query_train") {
            MyChar<24> trainID;
            Date d;
            while (command.current < command.count) {
                string key = command.getNext();
                if (key == "-i") {
                    trainID = command.getNext();
                } else if (key == "-d") {
                    string temp = command.getNext();
                    Date day(temp);
                    d = day;
                }
                command.current += 2;
            }
            cout << command.timeStamp << ' ';
            train_management.query_train(trainID, d);
        } else if (command.cmd == "query_ticket") {
            Date d;
            MyChar<24> s;
            MyChar<24> t;
            string p;
            while (command.current < command.count) {
                string key = command.getNext();
                if (key == "-s") {
                    s = command.getNext();
                } else if (key == "-t") {
                    t = command.getNext();
                } else if (key == "-d") {
                    string temp = command.getNext();
                    Date tt(temp);
                    d = tt;
                } else if (key == "-p") {
                    p = command.getNext();
                }
                command.current += 2;
            }
            if (p == "time") {
                cout << command.timeStamp << ' ';
                train_management.query_ticket_time(d, s, t);
            } else if (p == "cost") {
                cout << command.timeStamp << ' ';
                train_management.query_ticket_cost(d, s, t);
            }
        } else if (command.cmd == "query_transfer") {
            Date d;
            MyChar<24> s;
            MyChar<24> t;
            string p;
            while (command.current < command.count) {
                string key = command.getNext();
                if (key == "-s") {
                    s = command.getNext();
                } else if (key == "-t") {
                    t = command.getNext();
                } else if (key == "-d") {
                    string temp = command.getNext();
                    Date tt(temp);
                    d = tt;
                } else if (key == "-p") {
                    p = command.getNext();
                }
                command.current += 2;
            }
            if (p == "time") {
                train_management.query_transfer_time(d, s, t);
            } else if (p == "cost") {
                train_management.query_transfer_cost(d, s, t);
            }
        } else if (command.cmd == "buy_ticket") {
            MyChar<24> username;
            MyChar<24> trainID;
            Date d;
            MyChar<24> st;
            MyChar<24> en;
            int n;
            string q;
            while (command.current < command.count) {
                string key = command.getNext();
                if (key == "-u") {
                    username = command.getNext();
                } else if (key == "-i") {
                    trainID = command.getNext();
                } else if (key == "-d") {
                    string temp = command.getNext();
                    Date dd(temp);
                    d = dd;
                } else if (key == "-f") {
                    st = command.getNext();
                } else if (key == "-t") {
                    en = command.getNext();
                } else if (key == "-n") {
                    n = command.str_to_int(command.getNext());
                } else if (key == "-q") {
                    q = command.getNext();
                }
                command.current += 2;
            }
            if (q == "true") {
                //TODO
            } else if (q == "false") {
                //TODO
            }
        } else if (command.cmd == "query_order") {
            string u = command.getNext();
            MyChar<24> username = command.getNext();
            //TODO
        } else if (command.cmd == "refund_ticket") {
            MyChar<24> username;
            int n;
            while (command.current < command.count) {
                string key = command.getNext();
                if (key == "-u") {
                    username = command.getNext();
                } else if (key == "-n") {
                    n = command.str_to_int(command.getNext());
                }
                command.current += 2;
            }
            //TODO
        } else if (command.cmd == "clean") {
            user_management.clean_user_file();
            cout << 0 << '\n';
            //TODO:清除所有数据
        } else if (command.cmd == "exit") {
            user_management.end();
            cout << "bye" << '\n';
            //TODO:下线所有用户
            break;
        } else {
            throw Error("Invalid\n");
        }
    }
    return 0;
}
