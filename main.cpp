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

void quick_sort(vector<int> &arr, int left, int right) {
    if (left >= right) return;
    int pivot = arr[left + (right - left) / 2];
    int i = left, j = right;
    while (i <= j) {
        while (arr[i] < pivot) ++i;
        while (arr[j] > pivot) --j;
        if (i <= j) {
            std::swap(arr[i], arr[j]);
            ++i;
            --j;
        }
    }
    if (left < j) quick_sort(arr, left, j);
    if (i < right) quick_sort(arr, i, right);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    BPTree<MyChar<65>,int> db;

    int n;
    std::cin >> n;
    std::string line;
    std::getline(std::cin, line); // 读掉换行符

    for (int i = 0; i < n; ++i) {
        std::getline(std::cin, line);
        std::stringstream ss(line);
        std::string command;
        MyChar<65>index;
        int value;

        ss >> command;

        if (command == "insert") {
            ss >> index >> value;
            db.insert(index, value);
        } else if (command == "delete") {
            ss >> index >> value;
            db.erase(index, value);
        } else if (command == "find") {
            ss >> index;
            vector<int> result = db.query(index);
            if (result.empty()) {
                std::cout << "null\n";
            } else {
                quick_sort(result, 0, result.size() - 1);
                for (int j = 0; j < result.size(); ++j) {
                    if (j) std::cout << ' ';
                    std::cout << result[j];
                }
                std::cout << '\n';
            }
        }
    }

    return 0;
}
