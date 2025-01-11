#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <stdexcept>


class Ticket {
    private:
    int ID;
    std::string status;
    std::string informacije;
    std::string operater;
    std::string korisnik;

    public:

    Ticket(int id, const std::string& op, const std::string& kor, std::string stat, std::string info)
    : ID(id), status(stat), informacije(info), operater(op), korisnik(kor) {}
    
    int getID () const
    {
        return this->ID;
    }

    std::string getStatus () const
    {
        return this->status;
    }

     std::string getInfo () const
    {
        return this->informacije;
    }

     std::string getOperater () const
    {
        return this->operater;
    }

    std::string getKorisnik () const
    {
        return this->korisnik;
    }

};
// Funkcija za generisanje ID-a
int generateID() {
    static int id = 0;
    return ++id;
}

// Funkcija za provjeru validnosti korisničkog imena
bool isValidUser(const std::string& username, const std::string& filename = "User.csv") {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Greska prilikom otvaranja fajla " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string user, pass, role;
        std::getline(ss, user, ',');
        std::getline(ss, pass, ',');
        std::getline(ss, role, ',');

        if (user == username) {
            return true;
        }
    }
    return false;
}

// Funkcija za kreiranje tiketa
Ticket kreirajTiket(const std::string& operater) {
    // Generisanje ID-a
    int id = generateID();

    // Unos korisničkog imena
    std::string korisnik;
    std::cout << "Unesite korisnicko ime: ";
    std::cin >> korisnik;

    // Provjera validnosti korisničkog imena
    if (!isValidUser(korisnik)) {
        throw std::invalid_argument("Korisnicko ime nije validno!");
    }

    // Unos informacija
    std::cin.ignore(); // Čišćenje ulaznog toka
    std::string informacije;
    std::cout << "Unesite informacije o tiketu: ";
    std::getline(std::cin, informacije);

    // Dodjela statusa
    std::string status = "Otvoren";

    // Kreiranje i vraćanje tiketa
    return Ticket(id, operater, korisnik, status, informacije);
}