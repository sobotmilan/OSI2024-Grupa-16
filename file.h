#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Ticket.h"
#include <vector>
#include "User.h"


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
    std::ofstream outputFile("TicketTemp.csv");
    std::string line;
    
     if (!inputFile || !outputFile) {
        std::cout << "Error opening file." << std::endl;
        return;
    }

    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        std::string part;
        std::vector<std::string> data; //Vektor koji cuva podatke jednog po jednog tiketa

        while (std::getline(ss, part, ',')) {
            data.push_back(part);
        }

        if (std::stoi(data[0]) == ticket.getID()) { //Promjena operatera i statusa ako je pronadjen odgovarajuci tiket
            data[1] = ticket.getStatus();  
            data[3] = ticket.getOperater();   
        }

        for (int i = 0; i < data.size(); ++i) { //Dodavanje svakog tiketa (neizmjenjenih i izmijenjenog) u temp fajl
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
    std::rename("TicketTemp.csv", "Ticket.csv"); //Pocetni fajl zamjenjujemo temp fajlom
}

//     void inputTicketInformation() {
//     int id = generateID(); 

//     std::string status = "otvoren";  
//     std::string operater="";
//     std::string informacije, korisnik;

//     std::cout << "ID tiketa: " << id << std::endl;

//     std::cout << "Unesite vase ime: ";
//     std::getline(std::cin, korisnik);

//     std::cout << "Unesite informacije o tiketu: ";
//     std::getline(std::cin, informacije);
    
//     Ticket ticket(id, status, informacije, operater, korisnik);
//     saveTicketToFile(ticket); 
//     std::cout << "Tiket je uspjesno kreiran." << std::endl;
// }




private:
    void saveTicketToFile(const Ticket &ticket) 
    {
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
                << ticket.getKorisnik() << "\n";

        fileOut.close();
    }

    int generateID() //Metoda koja cita i dodjeljuje jedinstveni ID tiketu
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
    }

    public:
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