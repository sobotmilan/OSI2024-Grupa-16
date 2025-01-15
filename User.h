#pragma once
#include <iostream>
#include <string>
#include <cctype>
using namespace std;

bool passIsStrong(string password);

class User
{
private:
    string username;
    string password;
    string role;

public:
    User(string user = "", string pass = "", string role = "") : username(user), password(pass), role(role)
    {
    }

    ~User() {}

    void setUsername(string user) { this->username = user; }

    void setPassword(string pass)
    {
        while (!passIsStrong(pass))
        {
            cout << "Nedovoljno jaka sifra! Unesite neku jacu (sadrzi bar jedan numericki karakter, bar jedno malo slovo, bar jedno veliko slovo, bar jedan specijalni karakter, kao i duzinu od barem 7 znakova)..." << endl;
            cout << "Novi pokusaj:";
            cin.ignore();
            cin >> pass;
        }
        this->password = pass;
    }
    void setPasswordNoCheck(string pass)
    {
        this->password = pass;
    }
    void setRole(string role) { this->role = role; }

    string getUsername() const { return username; }

    string getPassword() const { return password; }

    string getRole() const { return role; }

    bool isEmpty()
    {
        if (this->username.empty() && this->password.empty() && this->role.empty())
        {
            return true;
        }
        return false;
    }
};

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
    bool hasWhitespace = false;
    // flagovi za 4 tipa karaktera koje trazim u stringu, false status znaci da taj tip karaktera za sada nije nadjen u string, true znaci da je bar jednom nadjen u stringu do sada.

    for (char ch : password) // string ima ugradjen iterator sto mi je zgodno jer drukcije je nemoguce analizirati string (po mom misljenju)
    {

        if (isupper(ch)) // provjera da li je trenutni znak veliko slovo (ABCDEFGHIJKLMNOPQRSTUVWXYZ)
            hasUppercase = true;

        else if (isdigit(ch)) // provjera da li je trenutni znak broj (A1234567890)
            hasDigit = true;

        else if (!isalnum(ch)) // provjera da li je trenutni znak specijalni karakter(/@#....)
            hasSpecialChar = true;

        else if (islower(ch)) // provjera da li je trenutni znak malo slovo (abcdefghijklmnopqrstuvwxyz)
            hasLowercase = true;

        else if (isspace(ch))
            hasWhitespace = true;

        if (hasDigit && hasUppercase && hasLowercase && hasSpecialChar && !hasWhitespace) // znaci, sifra je jaka SAMO AKO sadrzi sve cetvoro (ispunjava sva 4 uslova : malo/veliko slovo, broj i poseban karakter)
            return true;
    }

    return false; // Return false if one or more conditions are not met
}
