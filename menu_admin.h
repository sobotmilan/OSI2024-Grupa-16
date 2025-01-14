#include <iostream>
#include <string>
#include "file.h"
#include "Organizacija.h"
void menuForAdmin(file User, Organizacija org, std::string currentUser)
{
    int choice;
    do
    {
        std::cout << "\n===== MENI ZA ADMINE =====" << std::endl;
        std::cout << "1. Prikazi sve tikete" << std::endl;                         // Marija
        std::cout << "2. Upravljanje nalozima korisnika i operatera" << std::endl; // Marija
        std::cout << "3. Pregled statistike (dnevna i mjesečna)" << std::endl;     // Milan
        std::cout << "4. Provjera verzije" << std::endl;                           // Sanja
        std::cout << "5. Odjava" << std::endl;
        std::cout << "Unesite svoj izbor: ";
        std::cin >> choice;

        if (choice == 1)
        {
            file Tickets("Tickets.csv");
            Tickets.allTickets();
        }
        else if (choice == 2)
        {
            int izbor;
            do
            {
                std::cout << "Administracija naloga:" << std::endl;
                std::cout << "1. Kreirajte naloge" << std::endl;
                std::cout << "2. Brisite naloge" << std::endl;
                std::cin >> izbor;

            } while (izbor != 1 && izbor != 2);
            if (izbor == 1)
            {
                int count;
                std::cout << std::endl;
                std::cout << "Izaberite tip naloga: " << std::endl;
                std::cout << "1. Korisnicki nalog " << std::endl;
                std::cout << "2. Operatorski nalog " << std::endl;
                std::cout << "3. Administratorski nalog" << std::endl;
                std::cin >> count;

                std::string verzija = org.provjeriVerziju();
                bool dozvola;
                if (verzija == "Besplatna verzija")
                {
                    if (count == 1)
                    {
                        dozvola = true;
                    }
                    else if (count == 2)
                    {
                        if (User.getNumAdmin() <= 2)// treba izmijeniti na numoperatora
                        { 
                            dozvola = false;
                        }
                        else
                        {
                            dozvola == true;
                        }
                    }
                    else if (count == 3)
                    {
                        dozvola = false;
                    }
                }
                if (dozvola == false && verzija == "Besplatna verzija")
                {
                    std::cout << "Dosegli ste maksimalan broj naloga" << std::endl;
                }
                if ((verzija == "Komercijalna verzija") || dozvola == true)
                {
                    std::string username, password, role;
                    int count;
                    do
                    {
                        std::cout << "Unesite korisnicko ime: " << std::endl;
                        std::cin >> username;
                        std::cout << "Unesite lozinku: " << std::endl;
                        std::cin >> password;
                    } while (User.isExisting(username) && count != 1 && count != 2 && count != 3);
                    if (count == 1)
                    {
                        role = "User";
                    }
                    else if (count == 2)
                    {
                        role = "Operater";
                    }
                    else if (count == 3)
                    {
                        role = "Admin";
                    }
                    User.addUser(username, password, role);
                }
            }
            else if (izbor == 2)
            {
                // User.showExcept(currentUser);
                std::string delete_user;
                do
                {
                    std::cout << "Unesite ime korisnika koji zelite brisati: " << std::endl;
                    std::cin >> delete_user;
                }while(!User.deleteUser(delete_user));
            }
        }
    } while (choice != 5);
}