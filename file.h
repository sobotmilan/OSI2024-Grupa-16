#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


class file {
private:
    std::string namefile;
public:
    file() : namefile("") {}
    file(const std::string& namefile) : namefile(namefile) {}
    void setNamefile(const std::string& namefile) { this->namefile = namefile; }
    std::string getNamefile() const { return namefile; }

    //Metode za Usere
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

    bool isEmpty(){
        std::ifstream file(namefile);
        if (!file){
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

    bool login(std::string username, std::string password){
        std::ifstream file(namefile);
        if (!file){
            std::cout << "File failed opening" << std::endl;
            return false;
        }
        std::string line;
        while (std::getline(file, line)){
            std::stringstream ss(line);
            std::string user, pass, role;

            if (std::getline(ss, user, ',') && std::getline(ss, pass, ',') && std::getline(ss, role, ',')){
                if (user == username && pass == password){
                    file.close();
                    return true;
                }
            }
            else{
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
            return false;
            file.close();
        };

};
