#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Ticket.h"
#include <vector>
#include "User.h"

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
  
    // Metoda za promjenu lozinke
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
};
