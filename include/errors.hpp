#pragma once

#include <exception>
#include <string>

using std::exception;
using std::string;
using std::to_string;

#ifdef COLOR
#define BUILD_ERROR_STRING(err_type, msg, line, column) \
    "\033[91m" + err_type + " error\033[0m in \033[92m" + to_string(line) + ":" + to_string(column) + "\033[0m: " + msg
#else
#define BUILD_ERROR_STRING(err_type, msg, line, column) \
    err_type + " error in " + to_string(line) + ":" + to_string(column) + ": " + msg
#endif

class AssemblerError : public exception {
public:
    const string msg;

    AssemblerError(
        const string& _err_type, const string& _msg,
        const int _line, const int _column
    ) : msg(BUILD_ERROR_STRING(_err_type, _msg, _line, _column))
    {}

    AssemblerError(const string& _msg) : msg(_msg) {}

    virtual const char* what() const noexcept {
        return msg.c_str();
    }
};

