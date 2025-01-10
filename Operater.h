#pragma once
#include <string>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include "User.h"
using namespace std;

class Operater : public User
{
protected:
    int numTickets; // br dodijeljenih tiketa

public:
    explicit Operater(std::string username, std::string password, std::string role, int tickets)
        : User(username, password, role), numTickets(tickets) {}
    Operater(const Operater &other) : User(other.getUsername(), other.getPassword(), other.getRole()), numTickets(other.numTickets) {}
    Operater(Operater &&other) : User(other.getUsername(), other.getPassword(), other.getRole()), numTickets(other.numTickets)
    {
        other.setUsername("");
        other.setPassword("");
        other.setRole("");
        other.setNumTickets(0);
    }
    ~Operater() {}

    Operater &operator=(const Operater &other)
    {
        if (this != &other)
        {
            this->setUsername(other.getUsername());
            this->setPassword(other.getPassword());
            this->setNumTickets(other.getNumTickets());
        }
        return *this;
    }
    Operater &operator=(Operater &&other)
    {
        if (this != &other)
        {
            this->setUsername(other.getUsername());
            this->setPassword(other.getPassword());
            this->setNumTickets(other.getNumTickets());

            other.setUsername("");
            other.setPassword("");
            other.setRole("");
            other.setNumTickets(0);
        }
        return *this;
    }

    void setNumTickets(int value)
    {
        this->numTickets = value;
    }
    int getNumTickets() const
    {
        return this->numTickets;
    }
    bool operator<(const Operater &other) const
    {
        if (this->numTickets == other.numTickets) // ako se desi ne daj Boze isti broj tiketa u dva operatera, redace se leksikografski u red.
        {
            return this->getUsername() > other.getUsername();
        }
        return this->numTickets < other.numTickets;
    }
    bool operator>(const Operater &other) const
    {
        if (this->numTickets == other.numTickets) // ako se desi ne daj Boze isti broj tiketa u dva operatera, redace se leksikografski u red.
        {
            return this->getUsername() > other.getUsername();
        }
        return this->numTickets > other.numTickets;
    }
    bool operator==(const Operater &other) const
    {
        return ((this->numTickets == other.numTickets) & (this->getUsername() == other.getUsername()));
    }
    bool operator!=(const Operater &other) const
    {
        return ((this->numTickets != other.numTickets) || (this->getUsername() != other.getUsername()));
    }
};

std::ostream &operator<<(std::ostream &os, const Operater &op)
{
    os << op.getUsername() << "," << op.getNumTickets();
    return os;
}

Operater findSuitableOperater()
{
    ifstream userBase("User.csv", ios::in);
    if (!userBase.is_open())
    {
        throw runtime_error("Nemoguce otvaranje datoteke sa operaterima.");
    }

    userBase.seekg(0, ios::beg);

    ifstream ticketBase("Ticket.csv", ios::in);
    if (!ticketBase.is_open())
    {
        throw runtime_error("Nemoguce otvaranje datoteke sa tiketima.");
    }

    ticketBase.seekg(0, ios::beg);

    priority_queue<Operater, vector<Operater>, greater<Operater>> pq;
    string currLine;

    unordered_map<string, int> operaterTicketCount;

    if (!getline(userBase, currLine)) // NE UKLANJATI OVU LINIJU, OVO PRESKACE PRVU VRSTU KOJA SADRZI ZAGLAVALJA "USER,PASS,ROLE"
    {
        throw runtime_error("Datoteka prazna ili nemoguće čitanje.");
    }

    while (getline(userBase, currLine))
    {
        istringstream ss(currLine);
        string username, pass, role;

        if (getline(ss, username, ',') && getline(ss, pass, ',') && getline(ss, role, ','))
        {
            if (role == "Operater")
            {
                Operater op(username, pass, role, 0);
                pq.emplace(op);
                operaterTicketCount[username] = 0;
            }
        }
    }

    if (!getline(ticketBase, currLine)) // NE UKLANJATI OVU LINIJU, OVO PRESKACE PRVU VRSTU KOJA SADRZI ZAGLAVALJA "ID,OPERATER" ITD.
    {
        throw runtime_error("Datoteka prazna ili fale headeri.");
    }

    while (getline(ticketBase, currLine))
    {
        istringstream ss(currLine);
        string id, status, info, ope, kor, datumo, datumz;
        if (getline(ss, id, ',') && getline(ss, status, ',') && getline(ss, info, ',') && getline(ss, ope, ',') && getline(ss, kor, ',') && getline(ss, datumo, ',') && getline(ss, datumz, ','))
        {
            if (operaterTicketCount.find(ope) != operaterTicketCount.end())
            {
                operaterTicketCount[ope]++;
            }
        }
    }

    userBase.close();
    ticketBase.close();

    if (pq.empty())
    {
        throw runtime_error("Nema operatera u datoteci!");
    }

    priority_queue<Operater, vector<Operater>, greater<Operater>> updatedPQ;

    while (!pq.empty())
    {
        Operater op = pq.top();
        pq.pop();

        op.setNumTickets(operaterTicketCount[op.getUsername()]);
        updatedPQ.emplace(op);

        // pravim novi, rezultantni prioritetni red iz kog ce se vratiti prvi element
    }

    return updatedPQ.top();
}

void writeOpsToCSV(const std::vector<Operater> &operators)
{
    ofstream userBase("User.csv", ios::app);
    if (!userBase.is_open())
    {
        throw runtime_error("Nemoguce otvaranje datoteke za pisanje operatera.");
    }

    userBase.seekp(0, ios::end);
    if (userBase.tellp() == 0)
    {
        userBase << "username,password,role\n";
    }

    for (const auto &op : operators)
    {
        userBase << op.getUsername() << ","
                 << op.getPassword() << ","
                 << op.getRole() << "\n";
    }

    userBase.close();
    std::cout << "Operateri su uspesno upisani u datoteku.\n";
}
