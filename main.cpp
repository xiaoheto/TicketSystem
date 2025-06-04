#include <iostream>

#include "BPT/STLite/utility.h"
#include "../Management/TicketManagement.h"
#include "../Management/TrainManagement.h"
#include "../Management/UserManagement.h"
#include "Tool/CommandParser.h"
#include "Tool/Time.h"

using std::cin;
using std::cout;
using sjtu::pair;

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
    Command temp(input, '|');
    temp.position = 0;
    while (temp.count--) {
        res.push_back(Command::str_to_int(temp.getNext()));
    }
    return res;
}

SalesDate getSalesDate(const string &input) {
    Command temp(input, '|');
    temp.position = 0;
    string ans1 = temp.getNext();
    string ans2 = temp.getNext();
    SalesDate ans(ans1, ans2);
}

Clock getStartTime(const string &s) {
    Clock clock(s);
    return clock;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    UserManagement user_management;
    TrainManagement train_management;
    TicketManagement ticket_management;

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
                    privilege = Command::str_to_int(command.getNext());
                }
                command.current += 2;
            }
            cout << command.timeStamp << ' ' << user_management.add_user(
                        cur_username, username, password, name, mailAddr, privilege)
                    << '\n';
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
            SalesDate saleDate;
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
                    saleDate = getSalesDate(d);
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
        } else if (command.cmd == "clean") {
            user_management.clean_user_file();
            user_management.LogInStack.clear();
            cout << command.timeStamp << ' ' << 0 << '\n';
        } else if (command.cmd == "exit") {
            cout << command.timeStamp << ' ' << "bye" << '\n';
            break;
        }
    }
    return 0;
}
