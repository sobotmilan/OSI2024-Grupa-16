#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Ticket.h"
#include <vector>
#include "User.h"
#include <chrono>

class file
{
private:
    std::string namefile;

public:
    file() : namefile("") {}
    file(const std::string &namefile) : namefile(namefile) {}
    void setNamefile(const std::string &namefile) { this->namefile = namefile; }
    std::string getNamefile() const { return namefile; }

    // Metode za Usere

    bool isEmpty()
    {
        std::ifstream file(namefile);
        if (!file)
        {
            std::cout << "File failed opening" << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(file, line))
        {
            if (!line.empty() && line.find_first_not_of(" \t\r\n") != std::string::npos)
            {
                file.close();
                return false;
            }
        }
        file.seekg(0, std::ios::beg);
        file.close();
        return true;
    }

    bool login(std::string username, std::string password)
    {
        std::ifstream file(namefile);
        if (!file)
        {
            std::cout << "File failed opening" << std::endl;
            return false;
        }
        std::string line;
        while (std::getline(file, line))
        {
            std::stringstream ss(line);
            std::string user, pass, role;

            if (std::getline(ss, user, ',') && std::getline(ss, pass, ',') && std::getline(ss, role, ','))
            {
                if (user == username && pass == password)
                {
                    file.close();
                    return true;
                }
            }
            else
            {
                std::cout << "Invalid line format: " << line << std::endl;
                file.close();
                return false;
            }
        }
        file.close();
        return false;
    }

    bool addUser(const std::string &username, const std::string &password, const std::string &role)
    {
        if (!userExist(username))
        {
            std::ofstream fileAppend(namefile, std::ios::app);
            if (!fileAppend)
            {
                std::cout << "File failed opening, close running file" << std::endl;
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

    bool deleteUser(std::string username)
    {
        std::ifstream file(namefile);
        if (!file)
        {
            std::cerr << "File failed opening, close running file" << std::endl;
            return false;
        }

        std::vector<std::string> users;
        std::string line, user, pass, role;
        bool userDeleted = false;

        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            if (std::getline(iss, user, ',') && std::getline(iss, pass, ',') && std::getline(iss, role, ','))
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
            std::cerr << "User not found or already inactive." << std::endl;
            return false;
        }

        std::ofstream outFile(namefile, std::ios::trunc);
        if (!outFile)
        {
            std::cerr << "Error: Cannot write to the file!" << std::endl;
            return false;
        }

        for (const auto &entry : users)
        {
            outFile << entry << "\n";
        }
        outFile.close();

        return true;
    }

    std::string findRole(const std::string &username)
    {
        std::ifstream file(namefile);
        if (!file)
        {
            std::cout << "File failed opening" << std::endl;
            return "";
        }

        std::string line;
        while (std::getline(file, line))
        {
            std::stringstream ss(line);
            std::string user, pass, role;

            if (std::getline(ss, user, ',') && std::getline(ss, pass, ',') && std::getline(ss, role, ','))
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
bool file::organizationExists(const std::string &organizationName)
{
    std::ifstream file(namefile);
    if (!file)
    {
        std::cerr << "Error: Could not open file for reading.\n"; // zamijeniti sa throw
        return false;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string currentOrganization, key;

        if (std::getline(ss, currentOrganization, ',') && std::getline(ss, key, ',')) // drugi zarez suvisan
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
bool file::addOrganization(const std::string &organizationName, const std::string &key) // promjeniti povratni tip u "void"
{
    if (organizationExists(organizationName))
    {
        std::cout << "Organization already exists.\n";
        return false; // Organizacija već postoji , ne vraća ništa
    }

    std::ofstream fileAppend(namefile, std::ios::app);
    if (!fileAppend) // dio biblioteke <fstream> , zamjeniti sa fileAppend.is_open()
    {
        std::cerr << "Error: Could not open file for appending.\n"; // zamijeniti sa throw
        return false;                                               // ne vraća ništa
    }

    fileAppend << organizationName << "," << key << "\n"; // Dodaj organizaciju
    fileAppend.close();
    std::cout << "Organization added successfully.\n";
    return true; // ne vraća ništa
}

    void allTickets()
    {
        std::ifstream inputFile("Ticket.csv");
        std::string line;
        if (!inputFile)
        {
            std::cout << "Error opening file." << std::endl;
            return;
        }

        while (std::getline(inputFile, line))
        {
            std::stringstream ss(line);
            std::string part;
            std::vector<std::string> data;

            while (std::getline(ss, part, ','))
            {
                data.push_back(part);
            }

            if (data.size() >= 5 && data[2] != "zatvoren")
            {
                std::cout << "ID: " << data[0] << std::endl
                          << "Info: " << data[1] << std::endl
                          << "Status: " << data[2] << std::endl
                          << "Operator: " << data[3] << std::endl
                          << "User: " << data[4] << std::endl;
            }
            std::cout << std::endl;
        }

        inputFile.close();
    }

    bool changePassword(std::string oldPass, std::string newPass, std::string username)
    {
        if (login(username, oldPass))
        {
            std::ifstream file(namefile);
            if (!file)
            {
                std::cout << "Error: Failed to open file. Make sure it exists and is not being used by another process." << std::endl;
                return false;
            }

            std::vector<std::string> lines;
            std::string line;
            bool userFound = false;

            while (std::getline(file, line))
            {
                std::stringstream ss(line);
                std::string user, pass, role;

                std::getline(ss, user, ',');
                std::getline(ss, pass, ',');
                std::getline(ss, role, ',');

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
                std::cout << "Error: User not found in the file." << std::endl;
                return false;
            }

            std::ofstream outFile(namefile, std::ios::trunc);
            if (!outFile)
            {
                std::cout << "Error: Cannot write to the file!" << std::endl;
                return false;
            }

            for (const auto &entry : lines)
            {
                outFile << entry << "\n";
            }
            outFile.close();

            std::cout << "Password successfully changed." << std::endl;
            return true;
        }
        else
        {
            std::cout << "Error: You have entered the wrong current password or user." << std::endl;
            return false;
        }
    }

    // Metode za Key.csv
    bool validateKey(const std::string &key)
    {
        std::ifstream inputFile(namefile); // Otvori fajl sa ključevima

        // Otvaranje fajla za čitanje
        if (!inputFile)
        {
            std::cerr << "Error: Could not open file for reading.\n";
            return false;
        }
        std::string line;
        while (std::getline(inputFile, line))
        {
            std::stringstream ss(line);
            std::string organization, storedKey;

            // Parsiraj organizaciju i ključ
            if (std::getline(ss, organization, ',') && std::getline(ss, storedKey, ','))
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

    std::string getFirstFreeKey()
    {
        std::ifstream file(namefile);
        if (!file.is_open())
        {
            std::cerr << "File failed opening." << std::endl;
            return "";
        }

        std::vector<std::pair<std::string, std::string>> keys;
        std::string line, key, status;
        bool found = false;
        std::string firstFreeKey = "";

        // Čitanje fajla
        while (std::getline(file, line))
        {
            std::istringstream iss(line);
            if (std::getline(iss, key, ',') && std::getline(iss, status, ','))
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
            std::cerr << "No free keys available." << std::endl;
            return "";
        }

        // Ažuriranje fajla
        std::ofstream outFile(namefile, std::ios::trunc);
        if (!outFile.is_open())
        {
            std::cerr << "Error: Cannot write to the file!" << std::endl;
            return "";
        }

        for (const auto &pair : keys)
        {
            outFile << pair.first << "," << pair.second << "\n";
        }
        outFile.close();

        return firstFreeKey;
    }

    bool addKeyToOrganization(const std::string &organizationName, const std::string &key)
    {
        std::ifstream inputFile(namefile);
        if (!inputFile)
        {
            std::cerr << "Error: Could not open file for reading.\n";
            return false;
        }
        std::ofstream tempFile("temp.csv"); // Privremeni fajl za upis
        if (!tempFile)
        {
            std::cerr << "Error: Could not create temporary file.\n";
            inputFile.close();
            return false;
        }

        bool found = false; // Da li je organizacija pronađena
        std::string line;
        while (std::getline(inputFile, line))
        {
            std::stringstream ss(line);
            std::string currentOrganization, currentKey;

            // Parsiraj liniju u organizaciju i ključ
            if (std::getline(ss, currentOrganization, ',') && std::getline(ss, currentKey, ','))
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

        // Zameni originalni fajl privremenim fajlom
        if (std::remove(namefile.c_str()) != 0 || std::rename("temp.csv", namefile.c_str()) != 0)
        {
            std::cerr << "Error: Could not replace the original file.\n";
            return false;
        }

        return true;
    }
    // Ostale metode...
    int getNumAdmin() const
    {
        std::ifstream userBase("User.csv", std::ios::in);
        if (userBase.is_open() == false)
            throw("Nemoguce otvaranje baze korisnika.");

        std::string currLine;
        int numAd = 0;

        while (getline(userBase, currLine))
        {
            std::istringstream ss(currLine);
            std::string username, pass, role;

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

    bool isExisting(std::string username){
        return userExist(username);
    }

private:
    bool userExist(std::string username)
    {
        std::ifstream file(namefile);
        if (!file)
        {
            std::cout << "File failed opening, close running file" << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(file, line))
        {
            std::stringstream ss(line);
            std::string user, pass, role;

            if (std::getline(ss, user, ',') && std::getline(ss, pass, ',') && std::getline(ss, role, ',') && (user == username))
            {
                std::cout << "User with the username '" << username << "' already exists." << std::endl;
                file.close();
                return true;
            }
        }
        file.close();
        return false;
    }

       public:

 void updateOperatorInFile(const Ticket& ticket) {
    std::ifstream inputFile("Ticket.csv");
    std::ofstream outputFile("Ticket_temp.csv");
    std::string line;
    bool found = false;

    if (!inputFile.is_open() || !outputFile.is_open()) {
        std::cout << "Error opening file." << std::endl;
        return;
    }

    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> data;

        while (std::getline(ss, token, ',')) {
            data.push_back(token);
        }

        try
        {
            if (!data.empty() && std::stoi(data[0]) == ticket.getID())
            {
                data[1] = "dodijeljen operateru"; 
                data[3] = ticket.getOperater();  
                found = true;
            }
        }
        catch (const std::invalid_argument &e)
        {
            std::cout << "Invalid ID found in the CSV: " << data[0] << std::endl;
            continue; 
        }

        for (size_t i = 0; i < data.size(); ++i) {
            outputFile << data[i];
            if (i < data.size() - 1) {
                outputFile << ",";
            }
        }
        outputFile << "\n";
    }

    inputFile.close();
    outputFile.close();
    
    std::remove("Ticket.csv");
    std::rename("Ticket_temp.csv", "Ticket.csv");

    if (found) {
        std::cout << "Ticket with ID: " << ticket.getID() << " successfully updated." << std::endl;
    } else {
        std::cout << "Ticket with ID: " << ticket.getID() << " not found." << std::endl;
    }
}

    Ticket unosInformacijaOTiketu(const User& user) {
    int id = generateID(); 

    std::string status = "otvoren";  
    std::string operater="";
    std::string informacije, korisnik;
    std::string datumOtvaranja, datumZatvaranja = "u toku";

    korisnik = user.getUsername();

    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&time_t_now));
    datumOtvaranja = buffer;
    
    std::cout << "ID tiketa: " << id << std::endl;

    do {
        std::cout << "Enter ticket information: ";
        std::getline(std::cin, informacije);
        if (informacije.empty()) {
            std::cout << "Informations can't be empty. Please try again." << std::endl;
        }
    } while (informacije.empty());
    
    
    Ticket ticket(id, status, informacije, operater, korisnik, datumOtvaranja, datumZatvaranja);
    upisTiketaUFajl(ticket); 
    std::cout << "Ticket successfully created." << std::endl;

    return ticket;
}

void updateTicketClosureDateInFile(const Ticket& ticket) {
    std::ifstream inputFile("Ticket.csv");
    std::ofstream outputFile("Ticket_temp.csv");
    std::string line;
    bool found = false;

    if (!inputFile.is_open() || !outputFile.is_open()) {
        std::cout << "Error opening file." << std::endl;
        return;
    }

    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        std::string part;
        std::vector<std::string> data;

        // Parse the current line of the CSV file
        while (std::getline(ss, part, ',')) {
            data.push_back(part);
        }

       try {
        if (!data.empty() && std::stoi(data[0]) == ticket.getID()) {
            // Check if the data vector has enough elements to access index 6 (datumZatvaranja)
            if (data.size() >= 7) {
                data[6] = ticket.getDatumZatvaranja();  // Update the closing date (index 6)
            } else {
                std::cerr << "Invalid data format, not enough fields in CSV line." << std::endl;
            }
            found = true;
        }
    } catch (const std::invalid_argument &e) {
        std::cout << "Invalid ID found in the CSV: " << data[0] << std::endl;
        continue; // Skip to the next line
    }

        // Write the row to the temporary file
        for (size_t i = 0; i < data.size(); ++i) {
            outputFile << data[i];
            if (i < data.size() - 1) {
                outputFile << ",";
            }
        }
        outputFile << "\n";
    }

    inputFile.close();
    outputFile.close();

    // Replace the old file with the new one
    std::remove("Ticket.csv");
    std::rename("Ticket_temp.csv", "Ticket.csv");

    if (found) {
        std::cout << "Ticket with ID: " << ticket.getID() << " successfully updated." << std::endl;
    } else {
        std::cout << "Ticket with ID: " << ticket.getID() << " not found." << std::endl;
    }
}




private:
    void upisTiketaUFajl(const Ticket &ticket) 
    {
        const std::string namefile = "Ticket.csv";
        std::ofstream fileOut(namefile, std::ios::app); // Otvaranje fajla u režimu dodavanja
        if (!fileOut)
        {
            std::cout << "Error opening file." << std::endl;
            return;
        }

        fileOut << ticket.getID() << ","
                << ticket.getStatus() << ","
                << ticket.getInfo() << ","
                << ticket.getOperater() << ","
                << ticket.getKorisnik() << ","
                << ticket.getDatumOtvaranja() << ","
                << ticket.getDatumZatvaranja() << "\n";

        fileOut.close();
    }

    int generateID() //Metoda koja cita i dodjeljuje jeinstveni ID tiketu
    {
        std::ifstream inputFile("id.txt");
        int currentID = 0;

        if (inputFile)
        {
            inputFile >> currentID;

            inputFile.close();

            currentID++;

            std::ofstream outputFile("id.txt");
            outputFile << currentID;
            outputFile.close();

            return currentID;
        }

        return -1;
    }

    //1.3 Prikaz tiketa korisnika
        void showUserTickets(const User &user)
        {
            std::ifstream inputFile("Ticket.csv");
            std::string line;
            bool found = false;

            if (!inputFile)
            {
                std::cout << "Error opening file." << std::endl;
                return;
            }

            while (std::getline(inputFile, line))
            {
                std::stringstream ss(line);
                std::string part;
                std::vector<std::string> data;

                while (std::getline(ss, part, ','))
                {
                    data.push_back(part);
                }

                if (data.size() >= 7 && data[4] == user.getUsername()) // Korisnik peti podatak tiketa
                {
                    std::cout << "ID: " << data[0] << std::endl
                              << "Info: " << data[1] << std::endl
                              << "Status: " << data[2] << std::endl
                              << "Operator: " << data[3] << std::endl
                              << "User: " << data[4] << std::endl
                              << "Opening Date: " << data[5] << std::endl   
                              << "Closing Date: " << data[6] << std::endl; 
                    found = true;
                }
            }

            if (!found)
            {
                std::cout << "No tickets found." << std::endl;
            }

            inputFile.close();
        }
};
