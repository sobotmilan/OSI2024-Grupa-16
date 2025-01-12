#pragma once
#include "file.h"
#include <iostream>
#include <vector>


    // 2.1 Prikaz tiketa operatera

    void showOperatorTickets(const User &user)
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

           if (data.size() >= 7 && data[3] == user.getUsername()) // Korisnik peti podatak tiketa
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
