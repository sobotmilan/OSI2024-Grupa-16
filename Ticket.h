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

    Ticket(int id, std::string& op, const std::string& kor, std::string stat, std::string info)
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

    void setStatus(const std::string& newStatus)
    {
        status=newStatus;
    }

    void setInfo(const std::string& updatedInfo)
    {
        informacije = updatedInfo;
    }

};