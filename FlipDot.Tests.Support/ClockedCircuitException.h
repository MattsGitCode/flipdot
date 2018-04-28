#pragma once

#include <exception>
#include <string>


class ClockedCircuitComponentException : public std::exception {
    const char *what() const noexcept override {
        return message.c_str();
    }

private:
    std::string message;
public:
    explicit ClockedCircuitComponentException(std::string message) : message(message) {}
};