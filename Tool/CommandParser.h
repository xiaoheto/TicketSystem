// //
// // Created by 43741 on 2025/5/16.
// //
//
// #ifndef COMMANDPARSER_H
// #define COMMANDPARSER_H
// #include <string>
//
// #include "Error.h"
// #include "MyChar.h"
// using std::string;
//
// class Command {
//     string input;
//     char split_char = ' ';
//     int position = 0;
//
// public:
//     string timeStamp;
//     MyChar<24> cur_username;  //-c
//     MyChar<24> username;  //-u
//     MyChar<24> name;  //-n
//     MyChar<32> mailAddr;  //-m
//     int first_privilege;  //-p
//     int second_privilege;  //-g
//
//     MyChar<24> trainID; //-i
//     int stationNum; //-n;
//     vector<MyChar<24>> stations; //-s
//     vector<int>prices; //-p
//     string cmd;
//     int count = 0;
//     int current = 2; // 当前已经处理的字符
//
//     Command() = default;
//
//     static int str_to_int(const string &s) {
//         int res = 0;
//         for(auto i:s) {
//             res *= 10;
//             res += i - '0';
//         }
//         return res;
//     }
//
//     Command(const string &input, char split = ' '): input(input), split_char(split) {
//         while(position < input.length() && input[position] == split_char) {
//             ++position;//找到第一个非分隔符
//         }
//         countArguments();
//         timeStamp = getNext();
//         cmd = getNext();
//         while (current < count) {
//             string key = getNext();
//             switch (key) {
//                 case "-c": {
//                     cur_username = getNext();
//                     break;
//                 }
//                 case "-u": {
//                     username = getNext();
//                     break;
//                 }
//                 case "-n": {
//                     name = getNext();
//                     break;
//                 }
//                 case "-m": {
//                     mailAddr = getNext();
//                     break;
//                 }
//                 case "-p": {
//                     first_privilege = str_to_int(getNext());
//                     break;
//                 }
//                 case "-g": {
//                     second_privilege = str_to_int(getNext());
//                 }
//                 default: {
//                     throw Error("Invalid\n");
//                 }
//             }
//         }
//     }
//
//     string getNext() {
//         if (position >= input.length() || input[position] == '\r' || input[position] == '\n') {
//             return "";
//         }
//         string res;
//         while(position < input.length() && input[position] == split_char && input[position] != '\r' && input[position] != '\n') {
//             res += input[position];
//             ++position;
//         }
//
//         while(position < input.length() && input[position] != '\r' && input[position] != '\n') {
//             ++position;
//         }
//         return res;
//     }
//
//     void setSplit(char split) {
//         split_char = split;
//     }
//
//     void reset() {
//         position = 0;
//         split_char = ' ';
//         count = 0;
//     }
//
//     void countArguments() {
//         int i = 0;
//         count = 0;
//         while(i < input.size() && input[i] != '\r' && input[i] != '\n') {
//             while(input[i] == split_char) {
//                 ++i;
//             }
//             if (i < input.length() && input[i] != '\r' && input[i] != '\n') {
//                 ++count;
//             }
//             while(i < input.length() && input[i] != split_char) {
//                 ++i;
//             } // 找到下一个分隔符
//         }
//     }
//
// };
//
// #endif //COMMANDPARSER_H
