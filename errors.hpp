#pragma once

#include <exception>
#include <string>

using std::exception;
using std::string;

class GenericError : public exception {
public:
    const string m_msg;

    GenericError(const string& msg) : m_msg(msg) { }

    virtual const char* what() const throw () {
        return m_msg.c_str();
    }
};
