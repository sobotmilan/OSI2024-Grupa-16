#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Ticket.h"
#include <vector>
#include "User.h"
#include <chrono>
#include <sstream>


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

    

    // METODE ZA TIKETE

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

        // Parse the current line of the CSV file
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
};