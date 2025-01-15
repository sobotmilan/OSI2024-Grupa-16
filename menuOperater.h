#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include "file.h"
#include "User.h"
using namespace std;

void showMenuOperater(User currentUser)
{
    int choice;
    do
    {
        cout << "\n===== MENI ZA OPERATERE =====" << endl;
        cout << "1. Prikazi sve tikete" << endl;
        cout << "2. Odjava" << endl;
        cout << "Unesite svoj izbor: ";
        cin.ignore();
        cin >> choice;
        if (choice == 1)
        {
            file ticketBase("Ticket.csv");
            int result = ticketBase.showOperatorTickets(currentUser);
            if (result == 1)
            {

                cout << "1. Promijeni status tiketa" << endl;
                cout << "2. Povratak" << endl;
                cout << "Unesite svoj izbor: ";
                int tikChoice;
                cin.ignore();
                cin >> tikChoice;
                if (tikChoice == 1)
                {
                    int tickID;
                    cout << "Unesite ID tiketa koji mijenjate: " << endl;
                    cin.ignore();
                    cin >> tickID;
                    Ticket temp = ticketBase.lookupTicket(tickID, currentUser.getUsername());
                    if (temp.getID() == -1)
                        cout << "Niste odabrali validan tiket za promjenu." << endl;
                    else
                        ticketBase.changeTicketStatus(temp);
                }
            }
            else if (result == 0)
            {
            }
            else
                cout << "Neuspjesno otvaranje baze tiketa." << endl;
        }
        else if (choice == 2)
        {
            break;
        }
        else
        {
            cout << "Neispravan izbor." << endl;
        }
    } while (choice != 2);
}
