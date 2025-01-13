#pragma once
#include <iostream>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Ticket
{
private:
    int ID;
    std::string status;
    std::string informacije;
    std::string operater;
    std::string korisnik;

public:
    explicit Ticket(int id, const std::string &op, const std::string &kor, std::string stat, std::string info)
        : ID(id), status(stat), informacije(info), operater(op), korisnik(kor) {}

    int getID() const
    {
        return this->ID;
    }

    std::string getStatus() const
    {
        return this->status;
    }

    std::string getInfo() const
    {
        return this->informacije;
    }

    std::string getOperater() const
    {
        return this->operater;
    }

    std::string getKorisnik() const
    {
        return this->korisnik;
    }
};
// posto nemamo funkciju koja ce citati tikete i smjestiti ih u neku STL strukturu, citacemo direktno iz baze podataka koristenjem klasa i funkcija u okviru <fstream>,<iomanip>,<string> itd, i tako stvoriti rezultat. Ovo je samo prekopirana i blago modifikovana implementacija mog nacina citanja broja tiketa iz Ticket.csv koju sam koristio u Operater.h prilikom trazenja operatera sa najmanjim brojem tiketa.

int brojTiketaOperatera(const std::string &operaterIme)
{
    ifstream ticketBase("Ticket.csv", ios::in);
    if (!ticketBase.is_open())
        throw runtime_error("Nemoguce otvaranje ulazne baze.");

    string currLine;

    if (!getline(ticketBase, currLine))
        throw runtime_error("Ulazne baza je prazna ili nedostaje zaglavlje."); // NE BRISATI OVO, SLUŽI ZA ČITANJE I PRESKAKANJE PRVE VRSTE U CSV DATOTECI, KOJA SADRŽI ZAGLAVLJA TABELE (ID STATUS OPERATOR INFORMACIJA ITD.)

    int res = 0;

    while (getline(ticketBase, currLine))
    {
        istringstream ss(currLine);
        string id, stat, info, ope, kor, datumo, datumz;
        if (getline(ss, id, ',') && getline(ss, stat, ',') && getline(ss, info, ',') && getline(ss, ope, ',') && getline(ss, kor, ',') && getline(ss, datumo, ',') && getline(ss, datumz, ','))
        {
            if (ope == operaterIme)
                res++;
        }
    }

    ticketBase.close();

    return res;
}
