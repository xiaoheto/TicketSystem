//
// Created by 43741 on 2025/5/17.
//

#ifndef ERROR_H
#define ERROR_H
#include <string>
#include <exception>

using std::exception;
using std::string;

class Error : public exception {
    string err;

public:
    Error() noexcept : err("Unknown Error") {
    }

    explicit Error(const string &message) noexcept : err(message) {
    }

    const char *what() const noexcept override {
        return err.c_str();
    }
};
#endif //ERROR_H
