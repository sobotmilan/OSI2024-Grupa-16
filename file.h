#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Ticket.h"
#include <vector>
#include "User.h"
#include "Operater.h"
#include <chrono>
#include "Organizacija.h"
#include <algorithm>

using namespace std;

class file
{
private:
    string namefile;
    std::vector<User> users;

public:
    file() : namefile("") {}
    file(const string &namefile) : namefile(namefile) {}
    void setNamefile(const string &namefile) { this->namefile = namefile; }
    string getNamefile() const { return namefile; }
    int numOpsInOrg() const
    {
        ifstream userBase("User.csv", ios::in);
        if (userBase.is_open() == false)
            throw("Nemoguce otvaranje baze korisnika.");

        string currLine;
        int numOp = 0;

        while (getline(userBase, currLine))
        {
            istringstream ss(currLine);
            string username, pass, role;

            if (getline(ss, username, ',') && getline(ss, pass, ',') && getline(ss, role, ','))
            {
                if (role == "Operater")
                {
                    numOp++;
                }
            }
        }

        return numOp;
    }

    // Metode za Usere

    bool isEmpty()
    {
        ifstream file(namefile);
        if (!file)
        {
            cout << "File failed opening" << endl;
            return false;
        }

        string line;
        while (getline(file, line))
        {
            if (!line.empty() && line.find_first_not_of(" \t\r\n") != string::npos)
            {
                file.close();
                return false;
            }
        }
        file.seekg(0, ios::beg);
        file.close();
        return true;
    }

    bool login(string username, string password)
    {
        ifstream file(namefile);
        if (!file)
        {
            cout << "File failed opening" << endl;
            return false;
        }
        file.clear(); // Resetuje error state ako je došlo do EOF
        file.seekg(0, std::ios::beg);
        std::string line;
        while (std::getline(file, line))
        {
            stringstream ss(line);
            string user, pass, role;

            if (getline(ss, user, ',') && getline(ss, pass, ',') && getline(ss, role, ','))
            {
                if (user == username && pass == password)
                {
                    file.close();
                    return true;
                }
            }
            else
            {
                cout << "Invalid line format: " << line << endl;
                file.close();
                return false;
            }
        }
        file.close();
        return false;
    }

    bool addUser(const string &username, const string &password, const string &role)
    {
        if (!userExist(username))
        {
            ofstream fileAppend(namefile, ios::app);
            if (!fileAppend)
            {
                cout << "File failed opening, close running file" << endl;
                return false;
            }

            fileAppend << username << "," << password << "," << role << "\n"; // Pravilno formatiranje
            fileAppend.close();
            return true;
        }
        else
        {
            return true;
        }
    }

    bool deleteUser(string username)
    {
        ifstream file(namefile);
        if (!file)
        {
            cerr << "File failed opening, close running file" << endl;
            return false;
        }

        vector<string> users;
        string line, user, pass, role;
        bool userDeleted = false;

        while (getline(file, line))
        {
            istringstream iss(line);
            if (getline(iss, user, ',') && getline(iss, pass, ',') && getline(iss, role, ','))
            {
                if (user == username)
                {
                    userDeleted = true;
                }
                else
                {

                    users.push_back(user + "," + pass + "," + role);
                }
            }
        }
        file.close();

        if (!userDeleted)
        {
            cerr << "User not found or already inactive." << endl;
            return false;
        }

        ofstream outFile(namefile, ios::trunc);
        if (!outFile)
        {
            cerr << "Error: Cannot write to the file!" << endl;
            return false;
        }

        for (const auto &entry : users)
        {
            outFile << entry << "\n";
        }
        outFile.close();

        return true;
    }

    string findRole(const string &username)
    {
        ifstream file(namefile);
        if (!file)
        {
            cout << "File failed opening" << endl;
            return "";
        }

        string line;
        while (getline(file, line))
        {
            stringstream ss(line);
            string user, pass, role;

            if (getline(ss, user, ',') && getline(ss, pass, ',') && getline(ss, role, ','))
            {
                if (user == username)
                {
                    file.close();
                    return role;
                }
            }
        }
        file.close();
        return "";
    }

    // Metoda koja provjerava da li organizacija već postoji u fajlu
    bool organizationExists(const string &organizationName)
    {
        ifstream file(namefile);
        if (!file)
        {
            throw "Error: Could not open file for reading.\n";
            return false;
        }

        string line;
        while (getline(file, line))
        {
            stringstream ss(line);
            string currentOrganization, key;

            if (getline(ss, currentOrganization, ',') && getline(ss, key))
            {
                if (currentOrganization == organizationName)
                {
                    file.close();
                    return true; // Organizacija postoji
                }
            }
        }

        file.close();
        return false; // Organizacija ne postoji
    }

    // Metoda koja dodaje organizaciju u fajl, ako već ne postoji
    void addOrganization(const string &organizationName, const string &key)
    {
        if (organizationExists(organizationName))
        {
            cout << "Organizacija vec postoji.\n";
        }

        ofstream fileAppend(namefile, ios::app);
        if (!fileAppend.is_open())
        {
            throw "Greska: nemoguce otvaranje datoteke za izmjene.\n";
        }

        fileAppend << organizationName << "," << key << "\n"; // Dodaj organizaciju
        fileAppend.close();
        cout << "Organizacija uspjesno dodata.\n";
    }

    void allTickets()
    {
        ifstream inputFile("Ticket.csv");
        string line;
        if (!inputFile)
        {
            cout << "Error opening file." << endl;
            return;
        }

        while (getline(inputFile, line))
        {
            stringstream ss(line);
            string part;
            vector<string> data;

            while (getline(ss, part, ','))
            {
                data.push_back(part);
            }
            cout << endl;
            if (data.size() >= 7 && data[2] != "zatvoren")
            {
                cout << "ID: " << data[0] << endl
                     << "Info: " << data[1] << endl
                     << "Status: " << data[2] << endl
                     << "Operator: " << data[3] << endl
                     << "User: " << data[4] << endl
                     << "Datum open: " << data[5] << endl
                     << "Datum end: " << data[6] << endl;
            }
            cout << endl;
        }

        inputFile.close();
    }

    bool changePassword(string oldPass, string newPass, string username)
    {
        if (login(username, oldPass))
        {
            ifstream file(namefile);
            if (!file)
            {
                cout << "Error: Failed to open file. Make sure it exists and is not being used by another process." << endl;
                return false;
            }

            vector<string> lines;
            string line;
            bool userFound = false;

            while (getline(file, line))
            {
                stringstream ss(line);
                string user, pass, role;

                getline(ss, user, ',');
                getline(ss, pass, ',');
                getline(ss, role, ',');

                if (user == username)
                {
                    lines.push_back(user + "," + newPass + "," + role);
                    userFound = true;
                }
                else
                {
                    lines.push_back(line);
                }
            }
            file.close();

            if (!userFound)
            {
                cout << "Error: User not found in the file." << endl;
                return false;
            }

            ofstream outFile(namefile, ios::trunc);
            if (!outFile)
            {
                cout << "Error: Cannot write to the file!" << endl;
                return false;
            }

            for (const auto &entry : lines)
            {
                outFile << entry << "\n";
            }
            outFile.close();

            cout << "Password successfully changed." << endl;
            return true;
        }
        else
        {
            cout << "Error: You have entered the wrong current password or user." << endl;
            return false;
        }
    }

    // Metode za Key.csv
    bool validateKey(const string &key)
    {
        ifstream inputFile("Keys.csv", ios::in); // Otvori fajl sa ključevima

        // Otvaranje fajla za čitanje
        if (!inputFile)
        {
            cerr << "Greška. Nije moguće otvoriti datoteku za čitanje.\n";
            return false;
        }
        string line;
        while (getline(inputFile, line))
        {
            stringstream ss(line);
            string status, storedKey;

            // Parsiraj organizaciju i ključ
            if (getline(ss, storedKey, ',') && getline(ss, status))
            {
                if (storedKey == key)
                {
                    inputFile.close();
                    return true; // Ključ je pronađen
                }
            }
        }

        inputFile.close();
        return false; // Ključ nije validan
    }

    bool addKeyToOrganization(const string &key)
    {
        ifstream inputFile("Keys.csv", ios::in);
        if (!inputFile)
        {
            cerr << "Greška: Nije moguće otvoriti datoteku za čitanje.\n";
            return false;
        }

        ofstream tempFile("temp.csv", ios::out); // Privremeni fajl za upis
        if (!tempFile)
        {
            cerr << "Greška. Nije moguće kreirati privremenu datoteku.\n";
            inputFile.close();
            return false;
        }

        string line;
        bool keyFound = false;
        while (getline(inputFile, line))
        {
            stringstream ss(line);
            string currentKey, currentStatus;
            if (getline(ss, currentKey, ',') && getline(ss, currentStatus))
            {
                if (currentKey == key)
                {
                    // Ako je ključ pronađen i status je "slobodan"
                    if (currentStatus == "slobodan")
                    {
                        currentStatus = "aktivan"; // Mijenjamo status ključa u 'aktivan'
                        keyFound = true;
                    }
                }
                tempFile << currentKey << "," << currentStatus << "\n"; // Zapisujemo ključ bez obzira na status
            }
        }

        inputFile.close();
        tempFile.close();

        // Ako ključ nije pronađen ili nije slobodan, vraćamo grešku
        if (!keyFound)
        {
            cerr << "Ključ nije pronađen ili nije slobodan.\n";
            return false;
        }

        // Zamjena originalnog fajla sa ažuriranim
        if (remove("Keys.csv") != 0 || rename("temp.csv", "Keys.csv") != 0)
        {
            cerr << "Greška. Nije moguće zamijeniti privremenu datoteku.\n";
            return false;
        }

        return true;
    }

    // Ostale metode...
    int getNumAdmin() const
    {
        ifstream userBase("User.csv", ios::in);
        if (userBase.is_open() == false)
            throw("Nemoguce otvaranje baze korisnika.");

        string currLine;
        int numAd = 0;

        while (getline(userBase, currLine))
        {
            istringstream ss(currLine);
            string username, pass, role;

            if (getline(ss, username, ',') && getline(ss, pass, ',') && getline(ss, role, ','))
            {
                if (role == "Admin")
                {
                    numAd++;
                }
            }
        }
        return numAd;
    }

    bool canAddUser(const User &user)
    {
        if (getNumAdmin() >= 1)
        { // Koristi getNumAdmin()
            std::cerr << "Limit za administratore je dostignut!\n";
            return false;
        }
        if (numOpsInOrg() >= 2)
        { // Koristi numOpsInOrg()
            std::cerr << "Limit za operatere je dostignut!\n";
            return false;
        }
        users.push_back(user);
        return true;
    }
    bool isExisting(std::string username)
    {
        return userExist(username);
    }

private:
    bool userExist(string username)
    {
        ifstream file(namefile);
        if (!file)
        {
            cout << "File failed opening, close running file" << endl;
            return false;
        }

        string line;
        while (getline(file, line))
        {
            stringstream ss(line);
            string user, pass, role;

            if (getline(ss, user, ',') && getline(ss, pass, ',') && getline(ss, role, ',') && (user == username))
            {
                cout << "User with the username '" << username << "' already exists." << endl;
                file.close();
                return true;
            }
        }
        file.close();
        return false;
    }

public:
    void updateOperatorInFile(const Ticket &ticket)
    {
        ifstream inputFile("Ticket.csv");
        ofstream outputFile("Ticket_temp.csv");
        string line;
        bool found = false;

        if (!inputFile.is_open() || !outputFile.is_open())
        {
            cout << "Error opening file." << endl;
            return;
        }

        while (getline(inputFile, line))
        {
            stringstream ss(line);
            string token;
            vector<string> data;

            while (getline(ss, token, ','))
            {
                data.push_back(token);
            }

            try
            {
                if (!data.empty() && stoi(data[0]) == ticket.getID())
                {
                    data[1] = "dodijeljen operateru";
                    data[3] = ticket.getOperater();
                    found = true;
                }
            }
            catch (const invalid_argument &e)
            {
                cout << "Invalid ID found in the CSV: " << data[0] << endl;
                continue;
            }

            for (size_t i = 0; i < data.size(); ++i)
            {
                outputFile << data[i];
                if (i < data.size() - 1)
                {
                    outputFile << ",";
                }
            }
            outputFile << "\n";
        }

        inputFile.close();
        outputFile.close();

        remove("Ticket.csv");
        rename("Ticket_temp.csv", "Ticket.csv");

        if (found)
        {
            cout << "Ticket with ID: " << ticket.getID() << " successfully updated." << endl;
        }
        else
        {
            cout << "Ticket with ID: " << ticket.getID() << " not found." << endl;
        }
    }

    Ticket unosInformacijaOTiketu(const User &user)
{
    int id = generateID();

    if (id == -1)
    {
        throw runtime_error("Neuspješno generisanje ID-a tiketa nakon više pokušaja.");
    }

    string status = "Otvoren";
    string operater = "";
    string informacije, korisnik;
    string datumZatvaranja = "";

    korisnik = user.getUsername();

    auto now = chrono::system_clock::now();
    auto time_t_now = chrono::system_clock::to_time_t(now);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&time_t_now));
    string datumOtvaranja = buffer;

    cout << "ID tiketa: " << id << endl;
    std::cin.ignore();
    do
    {
        std::cout << "Unesite zahtjeve tiketa: ";
        std::getline(std::cin, informacije);
        if (informacije.empty())
        {
            std::cout << "Ne možete otvoriti tiket bez zahtjeva. Pokušajte ponovo." << std::endl;
        }
    } while (informacije.empty());

    Ticket ticket(id, status, informacije, operater, korisnik, datumOtvaranja, datumZatvaranja);
    ticket.setOperater(findSuitableOperater().getUsername());
    ticket.setStatus("Dodijeljen operateru");
    saveTicketToFile(ticket);
    cout << "Tiket uspjesno kreiran." << endl;

    return ticket;
}


    void updateTicketClosureDateInFile(const Ticket &ticket)
    {
        ifstream inputFile("Ticket.csv");
        ofstream outputFile("Ticket_temp.csv");
        string line;
        bool found = false;

        if (!inputFile.is_open() || !outputFile.is_open())
        {
            cout << "Error opening file." << endl;
            return;
        }

        while (getline(inputFile, line))
        {
            stringstream ss(line);
            string part;
            vector<string> data;

            // Parse the current line of the CSV file
            while (getline(ss, part, ','))
            {
                data.push_back(part);
            }

            try
            {
                if (!data.empty() && stoi(data[0]) == ticket.getID())
                {
                    // Check if the data vector has enough elements to access index 6 (datumZatvaranja)
                    if (data.size() >= 7)
                    {
                        data[6] = ticket.getDatumZ(); // Update the closing date (index 6)
                    }
                    else
                    {
                        cerr << "Invalid data format, not enough fields in CSV line." << endl;
                    }
                    found = true;
                }
            }
            catch (const invalid_argument &e)
            {
                cout << "Invalid ID found in the CSV: " << data[0] << endl;
                continue; // Skip to the next line
            }

            // Write the row to the temporary file
            for (size_t i = 0; i < data.size(); ++i)
            {
                outputFile << data[i];
                if (i < data.size() - 1)
                {
                    outputFile << ",";
                }
            }
            outputFile << "\n";
        }

        inputFile.close();
        outputFile.close();

        // Replace the old file with the new one
        remove("Ticket.csv");
        rename("Ticket_temp.csv", "Ticket.csv");

        if (found)
        {
            cout << "Ticket with ID: " << ticket.getID() << " successfully updated." << endl;
        }
        else
        {
            cout << "Ticket with ID: " << ticket.getID() << " not found." << endl;
        }
    }

    // 1.3 Prikaz tiketa korisnika
    void showUserTickets(const User &user)
        {
            std::ifstream inputFile("Ticket.csv");
            std::string line;
            std::vector<Ticket> tickets;
            bool found = false;
            if (!inputFile.is_open())
            {
                std::cout << "Freska prilikom otvaranja fajla." << std::endl;
                return;
            }
            std::cout << std::endl;
            while (getline(inputFile, line))
            {
                std::stringstream ss(line);
                std::string id, status, info, ope, kor, datumo, datumz; // id,status,zahtjevi,operater,korisnik,datum otvaranja,datum zatvaranja
                if (getline(ss, id, ',') && getline(ss, status, ',') && getline(ss, info, ',') && getline(ss, ope, ',') && getline(ss, kor, ',') && getline(ss, datumo, ','))
                {
                    getline(ss, datumz, ',');
                    if (kor == user.getUsername())
                    {
                        std::cout << "ID: " << id << std::endl
                                  << "Status: " << status << std::endl
                                  << "Zahtjev: " << info << std::endl
                                  << "Operater: " << ope << std::endl
                                  << "Korisnik: " << kor << std::endl
                                  << "Datum otvaranja: " << datumo << std::endl;
                        if (!datumz.empty())
                            std::cout << "Datum zatvaranja: " << datumz << std::endl;
                        found = true;
                        std::cout << std::endl;
                        if (status == "vracen korisniku")
                        {
                            Ticket ticket(std::stoi(id), ope, kor, status, info, datumo, datumz);
                            tickets.push_back(ticket);
                        }
                    }
                }
            }

            inputFile.close();

            if (!found)
            {
                std::cout << "Nemate dodijeljenih tiketa." << std::endl;
                return;
            }

            if (!tickets.empty())
            {
                std::cout << "Zelite li dopuniti informacije za tikete sa statusom 'vracen korisniku'? (da/ne): ";
                std::string odgovor;
                std::cin >> odgovor;
                std::cin.ignore();

                if (odgovor == "da")
                {
                    std::cout << "Tiketi sa statusom 'Vracen korisniku':" << std::endl;

                    for (const auto &ticket : tickets)
                    {
                        std::cout << "ID: " << ticket.getID() << std::endl
                                  << "Status: " << ticket.getStatus() << std::endl
                                  << "Zahtjev: " << ticket.getInfo() << std::endl
                                  << "Operater: " << ticket.getOperater() << std::endl
                                  << "Korisnik: " << ticket.getKorisnik() << std::endl
                                  << "Datum otvaranja: " << ticket.getDatumO() << std::endl;
                        if (!ticket.getDatumZ().empty())
                            std::cout << "Datum zatvaranja: " << ticket.getDatumZ() << std::endl;
                        std::cout << std::endl;
                    }

                    std::cout << "Unesite ID tiketa za dopunu: ";
                    int ticketID;
                    std::cin >> ticketID;
                    std::cin.ignore();

                    auto it = std::find_if(tickets.begin(), tickets.end(), [ticketID](const Ticket &ticket)
                                           { return ticket.getID() == ticketID; });

                    if (it != tickets.end())
                    {
                        Ticket &selectedTicket = *it;
                        addNewInfoToTicket(selectedTicket);     // Dodavanje novih informacija
                        addTicketNewInfoToFile(selectedTicket); // Ažuriranje u fajlu
                        std::cout << "Tiket je uspješno ažuriran." << std::endl;
                    }
                    else
                    {
                        std::cout << "Tiket sa unesenim ID-om ne postoji u listi." << std::endl;
                    }
                }
            }
        }

private:
    void saveTicketToFile(const Ticket &ticket)
    {
        const string namefile = "Ticket.csv";
        ofstream fileOut(namefile, ios::app); // Otvaranje fajla u režimu dodavanja
        if (!fileOut)
        {
            cout << "Greska prilikom otvaranja fajla." << endl;
            return;
        }

        fileOut << ticket.getID() << ","
                << ticket.getStatus() << ","
                << ticket.getInfo() << ","
                << ticket.getOperater() << ","
                << ticket.getKorisnik() << ","
                << ticket.getDatumO() << ","
                << ticket.getDatumZ() << "\n";

        fileOut.close();
    }

    int generateID() // Metoda koja cita i dodjeljuje jeinstveni ID tiketu
    {
        ifstream inputFile("id.txt");
        int currentID;

        if (inputFile)
        {
            inputFile >> currentID;

            inputFile.close();

            currentID++;

            ofstream outputFile("id.txt");
            outputFile << currentID;
            outputFile.close();

            return currentID;
        }

        return -1;
    }

    // 1.4 dopuna informacija tiketa

     void addNewInfoToTicket(Ticket &ticket)
    {
        std::cout << "Trenutne informacije: " << ticket.getInfo() << std::endl;
        std::cout << "Dodajte nove informacije: ";

        std::string newInfo;
        std::getline(std::cin, newInfo);
        ticket.setInfo(ticket.getInfo() + " " + newInfo);

    }

    void addTicketNewInfoToFile(const Ticket &ticket)
    {
        ifstream inputFile("Ticket.csv");
        ofstream outputFile("TicketTemp.csv");

        if (!inputFile.is_open() || !outputFile.is_open())
        {
            cout << "Greska prilikom otvaranja fajla." << endl;
            return;
        }

        string line;

        while (getline(inputFile, line))
        {
            stringstream ss(line);
            string part;
            vector<string> data;

            while (getline(ss, part, ','))
            {
                data.push_back(part);
            }

            if (data.empty())
            {
                continue; // Preskoči prazan red
            }

            try
            {
                int ticketID = stoi(data[0]);
                if (ticketID == ticket.getID())
                {
                    data[2] = ticket.getInfo();
                }
            }
            catch (const invalid_argument &e)
            {
                continue;
            }
            catch (const out_of_range &e)
            {
                continue;
            }

            for (int i = 0; i < data.size(); i++)
            {
                outputFile << data[i];
                if (i < data.size() - 1)
                {
                    outputFile << ",";
                }
            }
            outputFile << "\n";
        }

        inputFile.close();
        outputFile.close();

        remove("Ticket.csv");
        rename("TicketTemp.csv", "Ticket.csv");
    }

    // 2.2 Promjena statusa tiketa

public:
    // 2.1 Prikaz tiketa operatera

    int showOperatorTickets(const User &user)
    {
        ifstream inputFile("Ticket.csv");
        string line;
        bool found = false;
        int result = 0;

        if (!inputFile.is_open())
        {
            cout << "Error opening file." << endl;
            return -1;
        }
        cout << endl;
        while (getline(inputFile, line))
        {
            stringstream ss(line);
            string id, status, info, ope, kor, datumo, datumz; // id,status,zahtjevi,operater,korisnik,datum otvaranja,datum zatvaranja
            if (getline(ss, id, ',') && getline(ss, status, ',') && getline(ss, info, ',') && getline(ss, ope, ',') && getline(ss, kor, ',') && getline(ss, datumo, ','))
            {
                getline(ss, datumz, ',');
                if (ope == user.getUsername())
                {
                    cout << "ID: " << id << endl
                         << "Status: " << status << endl
                         << "Zahtjev: " << info << endl
                         << "Operater: " << ope << endl
                         << "Korisnik: " << kor << endl
                         << "Datum otvaranja: " << datumo << endl;
                    if (!datumz.empty())
                        cout << "Datum zatvaranja: " << datumz << endl;
                    found = true;
                    result = 1;
                    cout << endl;
                }
            }
        }

        if (!found)
        {
            cout << "Nemate dodijeljenih tiketa." << endl;
            result = 0;
        }

        inputFile.close();
        return result;
    }

    Ticket lookupTicket(int ID, string username)
    {
        ifstream ticketBase("Ticket.csv", ios::in);
        if (!ticketBase.is_open())
            throw runtime_error("Neuspjesno otvaranje baze tiketa.");

        string currLine;

        while (getline(ticketBase, currLine))
        {
            istringstream ss(currLine);
            string id, status, info, ope, kor, datumo, datumz; // id,status,zahtjevi,operater,korisnik,datum otvaranja,datum zatvaranja
            if (getline(ss, id, ',') && getline(ss, status, ',') && getline(ss, info, ',') && getline(ss, ope, ',') && getline(ss, kor, ',') && getline(ss, datumo, ','))
            {
                getline(ss, datumz, ',');
                if (id == to_string(ID))
                {
                    if (username == ope)
                        return Ticket(stoi(id), ope, kor, status, info, datumo, datumz);
                    else
                    {
                        cout << "Nemate ovlastenja za promjenu tiketa sa ovim ID-om!";
                        return Ticket(-1, "", "", "", "", "", "");
                    }
                }
            }
        }

        return Ticket(-1, "", "", "", "", "", "");
    }

    void changeTicketStatus(Ticket &ticket)
    {

        statusUpdateMenu(ticket);

        int choice;
        cin >> choice;

        if (choice == 0)
        {
            cout << "Status change canceled." << endl;
            return;
        }
        else if (ticket.getStatus() == "Dodijeljen operateru")
        {
            if (choice == 1)
            {
                ticket.setStatus("U izradi");
                updateTicketStatusInFile(ticket);
                cout << "Ticket status changed to 'u izradi'." << endl;
                return;
            }
            else if (choice == 2)
            {
                ticket.setStatus("Vracen korisniku");
                updateTicketStatusInFile(ticket);
                cout << "Ticket status changed to 'vracen korisniku'." << endl;
                return;
            }
        }
        else if (ticket.getStatus() == "U izradi")
        {
            if (choice == 1)
            {
                ticket.setStatus("Vracen korisniku");
                updateTicketStatusInFile(ticket);
                cout << "Ticket status changed to 'vracen korisniku'." << endl;
                return;
            }
            else if (choice == 2)
            {
                ticket.setStatus("Zatvoren");
                updateTicketStatusInFile(ticket);

                cout << "Ticket status changed to 'Zatvoren'." << endl;

                auto now = chrono::system_clock::now();
                auto time_t_now = chrono::system_clock::to_time_t(now);
                char buffer[20];
                strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&time_t_now));
                ticket.setDatumZ(buffer);
                updateTicketClosureDateInFile(ticket);

                return;
            }
        }
        else if (ticket.getStatus() == "Vracen korisniku")
        {
            if (choice == 1)
            {
                ticket.setStatus("U izradi");
                cout << "Ticket status changed to 'u izradi'." << endl;
                return;
            }
        }

        cout << "Invalid selection." << endl;
        return;
    }

private:
    void updateTicketStatusInFile(const Ticket &ticket)
    {
        ifstream inputFile("Ticket.csv");
        ofstream outputFile("TicketTemp.csv");

        if (!inputFile || !outputFile)
        {
            cout << "Error opening file." << endl;
            return;
        }

        string line;

        while (getline(inputFile, line))
        {
            stringstream ss(line);
            string part;
            vector<string> data;

            while (getline(ss, part, ','))
            {
                data.push_back(part);
            }

            try
            {
                if (data.size() > 0 && !data[0].empty())
                {
                    int ticketID = stoi(data[0]);   // Pokušaj konverzije u int
                    if (ticketID == ticket.getID()) // Upoređivanje ID-a tiketa
                    {
                        data[1] = ticket.getStatus(); // Ažuriranje statusa
                    }
                }
            }
            catch (const invalid_argument &e)
            {
                cout << "Invalid ticket ID: " << data[0] << endl;
                continue; // Nastavi dalje ako je ID neispravan
            }
            catch (const out_of_range &e)
            {
                cout << "Ticket ID out of range: " << data[0] << endl;
                continue;
            }

            for (int i = 0; i < data.size(); ++i)
            {
                outputFile << data[i];
                if (i < data.size() - 1)
                {
                    outputFile << ",";
                }
            }
            outputFile << "\n";
        }

        inputFile.close();
        outputFile.close();

        remove("Ticket.csv");
        rename("TicketTemp.csv", "Ticket.csv");
    }

    void statusUpdateMenu(const Ticket &ticket)
    {
        cout << "Current Status: " << ticket.getStatus() << endl;
        cout << "Pick a number for the new status, or type 0 to exit:" << endl;

        if (ticket.getStatus() == "Dodijeljen operateru")
        {
            cout << "1. U izradi" << endl;
            cout << "2. Vracen korisniku" << endl;
        }
        else if (ticket.getStatus() == "U izradi")
        {
            cout << "1. Vracen korisniku" << endl;
            cout << "2. Zatvoren" << endl;
        }
        else if (ticket.getStatus() == "Vracen korisniku")
        {
            cout << "1. U izradi" << endl;
        }
        else
        {
            cout << "No status changes are allowed for the current state." << endl;
        }
    }
};
