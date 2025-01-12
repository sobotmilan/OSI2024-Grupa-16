#pragma once
#include <iostream>
#include <string>

class User
{
private:
    std::string username;
    std::string password;
    std::string role;

public:
    User(std::string user="", std::string pass="", std::string role="") : username(user), password(pass), role(role) {}
    ~User() {}
    void setUsername(std::string user) { this->username = user; }
    void setPassword(std::string pass) { this->password = pass; }
    void setRole(std::string role) { this->role = role; }
    std::string getUsername() const { return username; }
    std::string getPassword() const { return password; }
    std::string getRole() const { return role; }
    bool isEmpty()
    {
        if (this->username.empty() && this->password.empty() && this->role.empty())
        {
            return true;
        }
        return false;
    }
};