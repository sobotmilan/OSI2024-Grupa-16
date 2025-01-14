#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include "file.h"
#include "User.h"
#include "Operater.h"
using namespace std;

void showMenuUser(const User &currentUser)
{
    int choice;
    do
    {
        std::cout << "\n===== MENI ZA KORISNIKE =====" << std::endl;
        std::cout << "1. Kreiraj tiket" << std::endl;      
        std::cout << "2. Prikazi sve tikete" << std::endl; 
        std::cout << "3. Odjava" << std::endl;
        std::cout << "Unesite svoj izbor: ";
        std::cin >> choice;
        Ticket createdTicket;

        file ticketBase("Ticket.csv");

        if (choice == 1)
        {
            createdTicket = ticketBase.unosInformacijaOTiketu(currentUser);
        }

        else if (choice == 2)
        {
            ticketBase.showUserTickets(currentUser);
        }

    } while (choice != 3);
}
