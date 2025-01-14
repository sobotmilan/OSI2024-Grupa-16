#pragma once
#include <iostream>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <chrono>
using namespace std;

class Ticket
{
public:
    int id;
    string status;
    string info;
    string ope;
    string kor;
    string datumO; // prosiriti ovime
    string datumZ; // prosiriti ovime

    Ticket() {}
    Ticket(int id, const string &op, const string &kor, string stat, string info, string datumO, string datumZ)
        : id(id), status(stat), info(info), ope(op), kor(kor), datumO(datumO), datumZ(datumZ) {}

    int getID() const
    {
        return this->id;
    }

    string getStatus() const
    {
        return this->status;
    }

    string getInfo() const
    {
        return this->info;
    }

    string getOperater() const
    {
        return this->ope;
    }

    string getKorisnik() const
    {
        return this->kor;
    }
    string getDatumO() const
    {
        return this->datumO;
    }
    string getDatumZ() const
    {
        return this->datumZ;
    }

    void setID(int value)
    {
        this->id = value;
    }

    void setStatus(string value)
    {
        this->status = value;
    }

    void setInfo(string value)
    {
        this->info = value;
    }

    void setOperater(string value)
    {
        this->ope = value;
    }

    void setKorisnik(string value)
    {
        this->kor = value;
    }
    void setDatumO(string value)
    {
        this->datumO = value;
    }
    void setDatumZ(string value)
    {
        this->datumZ = value;
    }
};

int brojTiketaOperatera(const string &operaterIme)
{
    ifstream ticketBase("Ticket.csv", ios::in);
    if (!ticketBase.is_open())
        throw runtime_error("Nemoguce otvaranje baze tiketa.");

    string currLine;

    if (!getline(ticketBase, currLine))
        throw runtime_error("Baza tiketa je prazna ili nedostaje zaglavlje."); // NE BRISATI OVO, SLUŽI ZA ČITANJE I PRESKAKANJE PRVE VRSTE U CSV DATOTECI, KOJA SADRŽI ZAGLAVLJA TABELE (ID STATUS OPERATOR INFORMACIJA ITD.)

    int res = 0;

    while (getline(ticketBase, currLine))
    {
        istringstream ss(currLine);
        string id, stat, info, ope, kor, datumo, datumz;
        if (getline(ss, id, ',') && getline(ss, stat, ',') && getline(ss, info, ',') && getline(ss, ope, ',') && getline(ss, kor, ',') && getline(ss, datumo, ','))
        {
            getline(ss, datumz, ',');
            if (ope == operaterIme)
                res++;
        }
    }

    ticketBase.close();

    return res;
}

chrono::system_clock::time_point convertToDate(const string &str)
{
    tm res = {};
    istringstream ss(str);
    ss >> get_time(&res, "%Y-%m-%d");
    if (ss.fail())
    {
        throw runtime_error("Neuspjesno pretvaranje u datum.");
    }
    return chrono::system_clock::from_time_t(mktime(&res));
}

bool isDateInRange(const string &filterDate, const string &startDate, const string &endDate)
{
    auto filter = convertToDate(filterDate);
    auto startRes = startDate.empty() ? chrono::system_clock::time_point::min() : convertToDate(startDate);
    auto endRes = endDate.empty() ? chrono::system_clock::time_point::max() : convertToDate(endDate);

    return filter >= startRes && filter <= endRes;
}

vector<Ticket> readTickets()
{
    vector<Ticket> tickets;
    ifstream file("Ticket.csv", ios::in);

    if (!file.is_open())
        throw runtime_error("Neuspjesno otvaranje baze tiketa. ");

    string currLine;
    getline(file, currLine); // Preskakanje zaglavlja

    while (getline(file, currLine))
    {
        istringstream ss(currLine);

        Ticket temp;

        string idAsStr;

        getline(ss, idAsStr, ',');

        temp.id = stoi(idAsStr);

        getline(ss, temp.status, ',');
        getline(ss, temp.info, ',');
        getline(ss, temp.ope, ',');
        getline(ss, temp.kor, ',');
        getline(ss, temp.datumO, ',');
        getline(ss, temp.datumZ, ',');

        tickets.push_back(temp);
    }

    file.close();
    return tickets;
}

vector<Ticket> filterTickets(const string &startDate, const string &endDate, int status)
{

    vector<Ticket> tickets = readTickets();

    vector<Ticket> filteredTickets;

    for (const auto &ticket : tickets)
    {
        bool statusMatch = false;
        switch (status)
        {
        case 1: // Samo otvoreni
            statusMatch = (ticket.status == "Otvoren" || ticket.status == "Dodijeljen operateru" ||
                           ticket.status == "U izradi" || ticket.status == "Vracen korisniku");
            break;
        case 2: // Samo zatvoreni
            statusMatch = (ticket.status == "Zatvoren");
            break;
        case 3: // Svi
            statusMatch = true;
            break;
        default:
            return {}; // Vraca praznu listu, prema SRS-u
        }
        if (statusMatch && (startDate.empty() || isDateInRange(ticket.datumO, startDate, endDate)))
        {
            filteredTickets.push_back(ticket);
        }
    }

    return filteredTickets;
}

void generateStatistics(const vector<Ticket> &tickets)
{
    if (tickets.empty())
    {
        cout << "Prazna lista." << endl;
        return;
    }

    map<string, int> statusCount;

    for (const auto &ticket : tickets)
    {
        statusCount[ticket.status]++;
    }

    cout << "Broj tiketa sa statusima:" << endl;
    for (const auto &entry : statusCount)
    {
        cout << entry.first << ": " << entry.second << endl;
    }

    cout << endl;
    cout << endl;
    cout << endl;

    for (auto &iter : tickets)
    {
        cout << "ID: " << iter.id << "\n Status: " << iter.status << "\n Zahtjev: " << iter.info << "\n Korisnik: " << iter.kor << "\n Operater: " << iter.ope << "\n Datum otvaranja: " << iter.datumO << "\n";
        if (!iter.datumZ.empty())
            cout << "Datum zatvaranja : " << iter.datumZ << endl;
    }
}

bool validateDates(string start, string end)
{
    auto startRes = start.empty() ? chrono::system_clock::time_point::min() : convertToDate(start);
    auto endRes = end.empty() ? chrono::system_clock::time_point::max() : convertToDate(end);

    return startRes <= endRes;
}