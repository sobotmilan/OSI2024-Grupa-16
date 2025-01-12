#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

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
    bool openFile()
    {
        std::ifstream file(namefile);
        if (!file)
        {
            std::ofstream createFile(namefile);
            if (!createFile)
            {
                std::cerr << "Error: Could not create file.\n";
                return false;
            }
            createFile.close();
            std::cout << "File created: " << namefile << "\n";
            return true;
        }
        std::cout << "File opened: " << namefile << "\n";
        file.close();
        return true;
    }

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
            std::cout << "File failed opening, close running file" << std::endl;
            return false;
        }
        std::ofstream tempfile("temp.csv");
        if (!tempfile)
        {
            std::cout << "File failed opening, close running file" << std::endl;
            file.close();
            return false;
        }

        bool succes = false;
        std::string line;
        while (std::getline(file, line))
        {
            std::stringstream ss(line);
            std::string user, pass, role;

            std::getline(ss, user, ',');
            std::getline(ss, pass, ',');
            std::getline(ss, role, ',');

            if (user != username)
            {
                tempfile << line << "\n";
            }
            else
            {
                succes = true;
            }
        }
        file.close();
        tempfile.close();

        if (!succes)
        {
            std::cout << "User is not found";
            std::remove("temp.csv");
            return succes;
        }
        if (std::remove(namefile.c_str()) != 0)
        {
            std::cout << "Error deleting original file";
            return false;
        }
        if (std::rename("temp.csv", namefile.c_str()) != 0)
        {
            std::cout << "Error rename temp file";
            return false;
        }
        return true;
    };

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
    // Metoda za pronalaženje prvog slobodnog ključa i njegovo aktiviranje
    static std::string getFirstFreeKey(const std::string &fileName)
    {
        std::ifstream file(fileName);
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
        std::ofstream outFile(fileName, std::ios::trunc);
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
    };
};
