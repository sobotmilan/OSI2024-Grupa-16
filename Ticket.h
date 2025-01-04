#pragma once
#include <iostream>

class Ticket {
    private:
    int ID;
    std::string status;
    std::string informacije;
    std::string operater;
    std::string korisnik;

    public:
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