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
    int numOpsInOrg() const;  
    
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
        string line;
        while (getline(file, line))
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
            cout << "Organization already exists.\n";
        }

        ofstream fileAppend(namefile, ios::app);
        if (!fileAppend.is_open()) 
        {
            throw "Error: Could not open file for appending.\n";                                       
        }

        fileAppend << organizationName << "," << key << "\n"; // Dodaj organizaciju
        fileAppend.close();
        cout << "Organization added successfully.\n";
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

            if (data.size() >= 5 && data[2] != "zatvoren")
            {
                cout << "ID: " << data[0] << endl
                     << "Info: " << data[1] << endl
                     << "Status: " << data[2] << endl
                     << "Operator: " << data[3] << endl
                     << "User: " << data[4] << endl;
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
        ifstream inputFile(namefile); // Otvori fajl sa ključevima

        // Otvaranje fajla za čitanje
        if (!inputFile)
        {
            cerr << "Error: Could not open file for reading.\n";
            return false;
        }
        string line;
        while (getline(inputFile, line))
        {
            stringstream ss(line);
            string organization, storedKey;

            // Parsiraj organizaciju i ključ
            if (getline(ss, organization, ',') && getline(ss, storedKey, ','))
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

    string getFirstFreeKey()
    {
        ifstream file(namefile);
        if (!file.is_open())
        {
            cerr << "File failed opening." << endl;
            return "";
        }

        vector<pair<string, string>> keys;
        string line, key, status;
        bool found = false;
        string firstFreeKey = "";

        // Čitanje fajla
        while (getline(file, line))
        {
            istringstream iss(line);
            if (getline(iss, key, ',') && getline(iss, status, ','))
            {
                if (!found && status == "slobodan")
                {
                    firstFreeKey = key;
                    status = "aktivan";
                    found = true;
                }
                keys.emplace_back(key, status);
            }
        }
        file.close();

        // Ako nije pronađen slobodan ključ
        if (firstFreeKey.empty())
        {
            cerr << "No free keys available." << endl;
            return "";
        }

        // Ažuriranje fajla
        ofstream outFile(namefile, ios::trunc);
        if (!outFile.is_open())
        {
            cerr << "Error: Cannot write to the file!" << endl;
            return "";
        }

        for (const auto &pair : keys)
        {
            outFile << pair.first << "," << pair.second << "\n";
        }
        outFile.close();

        return firstFreeKey;
    }

    bool addKeyToOrganization(const string &organizationName, const string &key)
    {
        ifstream inputFile(namefile);
        if (!inputFile)
        {
            cerr << "Error: Could not open file for reading.\n";
            return false;
        }
        ofstream tempFile("temp.csv"); // Privremeni fajl za upis
        if (!tempFile)
        {
            cerr << "Error: Could not create temporary file.\n";
            inputFile.close();
            return false;
        }

        bool found = false; // Da li je organizacija pronađena
        string line;
        while (getline(inputFile, line))
        {
            stringstream ss(line);
            string currentOrganization, currentKey;

            // Parsiraj liniju u organizaciju i ključ
            if (getline(ss, currentOrganization, ',') && getline(ss, currentKey, ','))
            {
                if (currentOrganization == organizationName)
                {
                    found = true;
                    tempFile << currentOrganization << "," << key << "\n"; // Ažuriraj ključ
                }
                else
                {
                    tempFile << line << "\n"; // Zadrži postojeću liniju
                }
            }
            else
            {
                tempFile << line << "\n"; // Zadrži nevažeće linije
            }
        }

        if (!found)
        {
            // Ako organizacija nije pronađena, dodaj novu
            tempFile << organizationName << "," << key << "\n";
        }

        inputFile.close();
        tempFile.close();

        // Zamijeni originalni fajl privremenim fajlom
        if (remove(namefile.c_str()) != 0 || rename("temp.csv", namefile.c_str()) != 0)
        {
            cerr << "Error: Could not replace the original file.\n";
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

    bool canAddUser(const User& user) {
    if (getNumAdmin() >= 1) {  // Koristi getNumAdmin()
        std::cerr << "Limit za administratore je dostignut!\n";
        return false;
    }
    if (numOpsInOrg() >= 2) {  // Koristi numOpsInOrg()
        std::cerr << "Limit za operatere je dostignut!\n";
        return false;
    }
    users.push_back(user);
    return true;
}

    bool isExisting(string username)
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

        string status = "Otvoren";
        string operater = "";
        string informacije, korisnik;
        string datumOtvaranja, datumZatvaranja = "";

        korisnik = user.getUsername();

        auto now = chrono::system_clock::now();
        auto time_t_now = chrono::system_clock::to_time_t(now);
        char buffer[20];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&time_t_now));
        datumOtvaranja = buffer;

        cout << "ID tiketa: " << id << endl;

        do
        {
            cout << "Unesite zahtjeve tiketa: ";
            getline(cin, informacije);
            if (informacije.empty())
            {
                cout << "Ne mozete otvoriti tiket bez zahtjeva. Pokusajte ponovo." << endl;
            }
        } while (informacije.empty());

        Ticket ticket(id, status, informacije, operater, korisnik, datumOtvaranja, datumZatvaranja);
        ticket.setOperater(findSuitableOperater().getUsername());
        upisTiketaUFajl(ticket);
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
        ifstream inputFile("Ticket.csv");
        string line;
        bool found = false;

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

            if (data.size() >= 7 && data[4] == user.getUsername()) // Korisnik peti podatak tiketa
            {
                cout << "ID: " << data[0] << endl
                     << "Zahtjevi: " << data[1] << endl
                     << "Status: " << data[2] << endl
                     << "Operater: " << data[3] << endl
                     << "Korisnik: " << data[4] << endl
                     << "Datum otvaranja: " << data[5] << endl
                     << "Datum zatvaranja: " << data[6] << endl;
                found = true;
            }
        }

        if (!found)
        {
            cout << "No tickets found." << endl;
        }

        inputFile.close();
    }

private:
    void upisTiketaUFajl(const Ticket &ticket)
    {
        const string namefile = "Ticket.csv";
        ofstream fileOut(namefile, ios::app); // Otvaranje fajla u režimu dodavanja
        if (!fileOut)
        {
            cout << "Error opening file." << endl;
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
        int currentID = 0;

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
        cout << "Current information: " << ticket.getInfo() << endl;
        cout << "Add new information: ";

        string newInfo;
        getline(cin, newInfo);
        ticket.setInfo(ticket.getInfo() + " " + newInfo);

        cout << "Information added succesfully." << endl;
    }

    void addTicketNewInfoToFile(const Ticket &ticket)
    {
        ifstream inputFile("Ticket.csv");
        ofstream outputFile("TicketTemp.csv");

        if (!inputFile.is_open() || !outputFile.is_open())
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

    Ticket
    lookupTicket(int ID, string username)
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
