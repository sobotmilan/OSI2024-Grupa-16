#pragma once
#include <iostream>
#include <string>
#include <cctype>
using namespace std;

class User
{
private:
    std::string username;
    std::string password;
    std::string role;

public:
    User(std::string user = "", std::string pass = "", std::string role = "") : username(user), role(role)
    {
        this->password = pass;
        while (!passIsStrong(pass))
        {
            cout << "Nedovoljno jaka šifra..." << endl;
            cout << "Unesite novu:";
            cin.ignore();
            string newPass;
            cin >> newPass;
            this->password = newPass;
        }
    }

    ~User() {}

    void setUsername(std::string user) { this->username = user; }

    void setPassword(std::string pass)
    {
        if (!passIsStrong(pass))
            cout << "Nedovoljno jaka sifra! Unesite neku jacu (sadrzi bar jedan numericki karakter, bar jedno malo slovo, bar jedno veliko slovo, bar jedan specijalni karakter, kao i duzinu od barem 7 znakova)..." << endl;
        else
            this->password = pass;
    }

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

    bool passIsStrong(string password)
    {
        if (password.length() < 7) // trazim da duzina bude >6, odnosno barem 7 znakova
        {
            return false;
        }

        bool hasDigit = false;
        bool hasUppercase = false;
        bool hasLowercase = false;
        bool hasSpecialChar = false;
        // flagovi za 4 tipa karaktera koje trazim u stringu, false status znaci da taj tip karaktera za sada nije nadjen u string, true znaci da je bar jednom nadjen u stringu do sada.

        for (char ch : password) // std::string ima ugradjen iterator sto mi je zgodno jer drukcije je nemoguce analizirati string (po mom misljenju)
        {

            if (std::isupper(ch)) // provjera da li je trenutni znak veliko slovo (ABCDEFGHIJKLMNOPQRSTUVWXYZ)
            {
                hasUppercase = true;
            }
            else if (std::isdigit(ch)) // provjera da li je trenutni znak broj (A1234567890)
            {
                hasDigit = true;
            }
            else if (!std::isalnum(ch)) // provjera da li je trenutni znak specijalni karakter(/@#....)
            {
                hasSpecialChar = true;
            }
            else if (std::islower(ch)) // provjera da li je trenutni znak malo slovo (abcdefghijklmnopqrstuvwxyz)
            {
                hasLowercase = true;
            }

            if (hasDigit && hasUppercase && hasLowercase && hasSpecialChar) // znaci, sifra je jaka SAMO AKO sadrzi sve cetvoro (ispunjava sva 4 uslova : malo/veliko slovo, broj i poseban karakter)
            {
                return true;
            }
        }

        return false; // Return false if one or more conditions are not met
    }
};