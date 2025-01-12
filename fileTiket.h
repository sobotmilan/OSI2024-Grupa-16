#pragma once
#include "file.h"
#include <iostream>
#include <vector>


    // 2.1 Prikaz tiketa operatera

    // void showOperatorTickets(const User &user)
    // {
    //     std::ifstream inputFile("Ticket.csv");
    //     std::string line;
    //     bool found = false;

    //     if (!inputFile)
    //     {
    //         std::cout << "Error opening file." << std::endl;
    //         return;
    //     }

    //     while (std::getline(inputFile, line))
    //     {
    //         std::stringstream ss(line);
    //         std::string part;
    //         std::vector<std::string> data;

    //         while (std::getline(ss, part, ','))
    //         {
    //             data.push_back(part);
    //         }

    //         if (data[3] == user.getUsername()) // Operater cetvrti podatak tiketa
    //         {
    //             std::cout << "ID: " << data[0] << std::endl
    //                       << "Info: " << data[1] << std::endl
    //                       << "Status: " << data[2] << std::endl
    //                       << "Operator: " << data[3] << std::endl
    //                       << "User: " << data[4] << std::endl;
    //             found = true;
    //         }
    //     }

    //     if (!found)
    //     {
    //         std::cout << "No tickets found." << std::endl;
    //     }

    //     inputFile.close();
    // }

    // // 2.2 Promjena statusa tiketa

    // void changeTicketStatus(Ticket &ticket)
    // {

    //     statusUpdateMenu(ticket);

    //     int choice;
    //     std::cin >> choice;

    //     if (choice == 0)
    //     {
    //         std::cout << "Status change canceled." << std::endl;
    //         return;
    //     }
    //     else if (ticket.getStatus() == "dodijeljen operateru")
    //     {
    //         if (choice == 1)
    //         {
    //             ticket.setStatus("u izradi");
    //             std::cout << "Ticket status changed to 'u izradi'." << std::endl;
    //             return;
    //         }
    //         else if (choice == 2)
    //         {
    //             ticket.setStatus("vracen korisniku");
    //             std::cout << "Ticket status changed to 'vracen korisniku'." << std::endl;
    //             return;
    //         }
    //     }
    //     else if (ticket.getStatus() == "u izradi")
    //     {
    //         if (choice == 1)
    //         {
    //             ticket.setStatus("vracen korisniku");
    //             std::cout << "Ticket status changed to 'vracen korisniku'." << std::endl;
    //             return;
    //         }
    //         else if (choice == 2)
    //         {
    //             ticket.setStatus("zatvoren");
    //             std::cout << "Ticket status changed to 'zatvoren'." << std::endl;
    //             return;
    //         }
    //     }
    //     else if (ticket.getStatus() == "vracen korisniku")
    //     {
    //         if (choice == 1)
    //         {
    //             ticket.setStatus("u izradi");
    //             std::cout << "Ticket status changed to 'u izradi'." << std::endl;
    //             return;
    //         }
    //     }

    //     std::cout << "Invalid selection." << std::endl;
    //     return;
    // }

    // void updateTicketStatusInFile(const Ticket &ticket)
    // {
    //     std::ifstream inputFile("Ticket.csv");
    //     std::ofstream outputFile("TicketTemp.csv");

    //     if (!inputFile || !outputFile)
    //     {
    //         std::cout << "Error opening file." << std::endl;
    //         return;
    //     }

    //     std::string line;

    //     while (std::getline(inputFile, line))
    //     {
    //         std::stringstream ss(line);
    //         std::string part;
    //         std::vector<std::string> data;

    //         while (std::getline(ss, part, ','))
    //         {
    //             data.push_back(part);
    //         }

    //         if (std::stoi(data[0]) == ticket.getID())
    //         {
    //             data[1] = ticket.getStatus();
    //         }

    //         for (int i = 0; i < data.size(); ++i)
    //         {
    //             outputFile << data[i];
    //             if (i < data.size() - 1)
    //             {
    //                 outputFile << ",";
    //             }
    //         }
    //         outputFile << "\n";
    //     }

    //     inputFile.close();
    //     outputFile.close();

    //     std::remove("Ticket.csv");
    //     std::rename("TicketTemp.csv", "Ticket.csv");
    // }


    // void statusUpdateMenu(const Ticket &ticket)
    // {
    //     std::cout << "Current Status: " << ticket.getStatus() << std::endl;
    //     std::cout << "Pick a number for the new status, or type 0 to exit:" << std::endl;

    //     if (ticket.getStatus() == "dodijeljen operateru")
    //     {
    //         std::cout << "1. u izradi" << std::endl;
    //         std::cout << "2. vracen korisniku" << std::endl;
    //     }
    //     else if (ticket.getStatus() == "u izradi")
    //     {
    //         std::cout << "1. vracen korisniku" << std::endl;
    //         std::cout << "2. zatvoren" << std::endl;
    //     }
    //     else if (ticket.getStatus() == "vracen korisniku")
    //     {
    //         std::cout << "1. u izradi" << std::endl;
    //     }
    //     else
    //     {
    //         std::cout << "No status changes are allowed for the current state." << std::endl;
    //     }
    // }


    // 1.4 dopuna informacija tiketa

    void addNewInfoToTicket(Ticket &ticket)
    {
        std::cout << "Current information: " << ticket.getInfo() << std::endl;
        std::cout << "Add new information: ";

        std::string newInfo;
        std::getline(std::cin, newInfo);
        ticket.setInfo(ticket.getInfo() + " " + newInfo);

        std::cout << "Information added succesfully." << std::endl;
    }

    void addTicketNewInfoToFile(const Ticket &ticket)
    {
        std::ifstream inputFile("Ticket.csv");
        std::ofstream outputFile("TicketTemp.csv");

        if (!inputFile.is_open() || !outputFile.is_open())
        {
            std::cout << "Error opening file." << std::endl;
            return;
        }

        std::string line;

        while (std::getline(inputFile, line))
        {
            std::stringstream ss(line);
            std::string part;
            std::vector<std::string> data;

            while (std::getline(ss, part, ','))
            {
                data.push_back(part);
            }

           if (data.empty()) 
        {
            continue; // Preskoči prazan red
        }

        try
        {
            int ticketID = std::stoi(data[0]);
            if (ticketID == ticket.getID()) 
            {
                data[2] = ticket.getInfo(); 
            }
        }
        catch (const std::invalid_argument& e)
        {
            continue; 
        }
        catch (const std::out_of_range& e)
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

        std::remove("Ticket.csv");
        std::rename("TicketTemp.csv", "Ticket.csv");
    }
