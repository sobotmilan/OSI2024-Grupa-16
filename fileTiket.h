#pragma once
#include "file.h"
#include <iostream>
#include <vector>

class file
{
    // 2.1 Prikaz tiketa operatera
public:
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

            if (data[3] == user.getUsername()) // Operater cetvrti podatak tiketa
            {
                std::cout << "ID: " << data[0] << std::endl
                          << "Info: " << data[1] << std::endl
                          << "Status: " << data[2] << std::endl
                          << "Operator: " << data[3] << std::endl
                          << "User: " << data[4] << std::endl;
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