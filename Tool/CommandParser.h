#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H
#include <string>
#include <bits/stl_algo.h>
using std::string;
class Command {
    string input = {'\0'}; // original input
    char splitChar = ' ';

public:
    int count = 0;
    int position = 0;

    string timeStamp;
    string cmd;
    int current = 2; //当前处理到的单词位置,初始时先跳过timeStamp和cmd

    Command() = delete;

    Command(const string &input, char split = ' '): input(input), splitChar(split) {
        while (position < input.length() && input[position] == splitChar) {
            ++position; //找到第一个非分隔符
        }

        countArguments();
        timeStamp = getNext();
        cmd = getNext();
    }

    static int str_to_int(const string &s) {
        int res = 0;
        for(auto i : s) {
            res *= 10;
            res += i - '0';
        }
        return res;
    }

    string getNext() {
        if (position >= input.length() || input[position] == '\n' || input[position] == '\r') {
            return "";
        }
        string result;
        while(position < input.length() && input[position] != splitChar && input[position] != '\r' && input[position] != '\n') {
            result += input[position];
            ++position;
        }

        while(position < input.length() && input[position] == splitChar) {
            ++position;
        }

        return result;
    }

    void countArguments() {
        count = 0;
        int i = 0;
        while(i < input.length() && input[i] == splitChar && input[i] != '\r' && input[i] != '\n') {
            ++i;
        }

        if (i < input.length() && input[i] != '\r' && input[i] != '\n') {
            ++count;
            while(i < input.length() &&input[i] != splitChar && input[i] != '\r' &&input[i] != '\n') {
                ++i;
            }
        }
    }
};
#endif