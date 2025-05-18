//
// Created by 43741 on 2025/5/16.
//

#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H
#include <string>

#include "Error.h"
#include "MyChar.h"
#include "../STLite/vector.h"
using std::string;

class Command {
    string input;
    char split_char = ' ';

public:
    int position = 0;

    string timeStamp;
    string cmd;
    int count = 0;
    int current = 2; //当前处理到的单词位置,初始时先跳过timeStamp和cmd

    Command() = default;

    static int str_to_int(const string &s) {
        int res = 0;
        for (auto i: s) {
            res *= 10;
            res += i - '0';
        }
        return res;
    }

    Command(const string &input, char split = ' '): input(input), split_char(split) {
        while (position < input.length() && input[position] == split_char) {
            ++position; //找到第一个非分隔符
        }
        countArguments();
        timeStamp = getNext();
        cmd = getNext();
    }

    string getNext() {
        if (position >= input.length() ||
            input[position] == '\r' || input[position] == '\n') {
            return "";
        }

        std::string result;
        // 读取直到下一个分隔符或结束
        while (position < input.length() &&
               input[position] != split_char &&
               input[position] != '\r' &&
               input[position] != '\n') {
            result += input[position];
            position++;
        }

        // 跳过分隔符
        while (position < input.length() && input[position] == split_char) {
            position++;
        }

        return result;
    }

    void setSplit(char split) {
        split_char = split;
    }

    void reset() {
        position = 0;
        split_char = ' ';
        count = 0;
    }

    void countArguments() {
        count = 0;
        size_t i = 0;
        while (i < input.length() && input[i] != '\r' && input[i] != '\n') {
            while (i < input.length() && input[i] == split_char) {
                i++;
            }

            if (i < input.length() && input[i] != '\r' && input[i] != '\n') {
                count++;
                // 找到下一个分隔符
                while (i < input.length() && input[i] != split_char && input[i] != '\r' && input[i] != '\n') {
                    i++;
                }
            }
        }
    }
};

#endif //COMMANDPARSER_H
