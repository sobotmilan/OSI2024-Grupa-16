#pragma once
#include "file.h"
#include <iostream>
#include <vector>
class file
{
    public:
    //Metoda za prikaz svih aktivnih tiketa
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
};