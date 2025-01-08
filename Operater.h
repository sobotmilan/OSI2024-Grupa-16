#pragma once
#include <string>
#include <iostream>
#include <queue>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

class Operater
{
protected:
    std::string user; // korisnicko ime
    int numTickets;   // br dodijeljenih tiketa

public:
    Operater(const std::string &name, int tickets)
        : user(name), numTickets(tickets) {}
    std::string getUser() const
    {
        return this->user;
    }
    int getNumTickets() const
    {
        return this->numTickets;
    }
    bool operator<(const Operater &other) const
    {
        if (this->numTickets == other.numTickets) // ako se desi ne daj Boze isti broj tiketa u dva operatera, redace se leksikografski u red.
        {
            return this->user > other.user;
        }
        return this->numTickets < other.numTickets;
    }
    bool operator>(const Operater &other) const
    {
        if (this->numTickets == other.numTickets) // ako se desi ne daj Boze isti broj tiketa u dva operatera, redace se leksikografski u red.
        {
            return this->user > other.user;
        }
        return this->numTickets > other.numTickets;
    }
    bool operator==(const Operater &other) const
    {
        return ((this->numTickets == other.numTickets) & (this->user == other.user));
    }
    bool operator!=(const Operater &other) const
    {
        return ((this->numTickets != other.numTickets) || (this->user != other.user));
    }
};

std::ostream &operator<<(std::ostream &os, const Operater &op)
{
    os << op.getUser() << "," << op.getNumTickets();
    return os;
}

Operater findSuitableOperater()
{
    ifstream file("Ops.csv");
    if (!file)
    {
        throw runtime_error("Nemoguce otvaranje datoteke sa operaterima.");
    }

    priority_queue<Operater, vector<Operater>, greater<Operater>> pq;
    string currLine;

    if (!getline(file, currLine))
    {
        throw std::runtime_error("Datoteka prazna ili fale headeri.");
    }

    while (getline(file, currLine))
    {
        istringstream ss(currLine);
        string user;
        int numT;

        if (getline(ss, user, ',') && ss >> numT)
        {
            pq.emplace(user, numT);
        }
    }

    file.close();

    if (pq.empty())
    {
        throw runtime_error("Nema operatera u datoteci!");
    }

    return pq.top();
}