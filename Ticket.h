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
    std::string datumOtvaranja;
    std::string datumZatvaranja;

public:
    explicit  Ticket(int id, const std::string &status, const std::string &info, const std::string &operater,
           const std::string &korisnik, const std::string &datumOt, const std::string &datumZat)
        : ID(id), status(status), informacije(info), operater(operater), korisnik(korisnik),
          datumOtvaranja(datumOt), datumZatvaranja(datumZat) {}
    
    Ticket &operator=(const Ticket &other)
    {
        if (this == &other) 
            return *this;

        ID = other.ID;
        status = other.status;
        informacije = other.informacije;
        operater = other.operater;
        korisnik = other.korisnik;
        datumOtvaranja = other.datumOtvaranja;
        datumZatvaranja = other.datumZatvaranja;

        return *this;
    }

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

    std::string getDatumOtvaranja() const
    {
        return this->datumOtvaranja;
    }

    std::string getDatumZatvaranja() const
    {
        return this->datumZatvaranja;
    }

      void setOperater(const std::string &noviOperater)
    {
        operater = noviOperater;
    }

    void setDatumZatvaranja(const std::string &datum)
    {
        datumZatvaranja = datum;
    }

    void setInfo(const std::string &noveInfo)
    {
        informacije = noveInfo;
    }

    void setStatus(const std::string &noviStatus)
    {
        status = noviStatus;
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
