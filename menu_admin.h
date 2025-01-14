#include <iostream>
#include <string>
#include "file.h"
#include "Organizacija.h"
#include "User.h"

void menuForAdmin(file User, Organizacija org, std::string currentUser)
{
    int choice;
    do
    {
        std::cout << "\n===== MENI ZA ADMINE =====" << std::endl;
        std::cout << "1. Prikazi sve tikete" << std::endl;                         // Marija
        std::cout << "2. Upravljanje nalozima korisnika i operatera" << std::endl; // Marija
        std::cout << "3. Pregled statistike" << std::endl;                         // Milan
        std::cout << "4. Provjera verzije sistema" << std::endl;                   // Sanja
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
                std::string role;

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
                        if (User.numOpsInOrg() < 2)
                        {
                            dozvola = true;
                        }
                        else
                        {
                            dozvola = false;
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
                    std::string username, password;
                    int count;
                    do
                    {
                        std::cout << "Unesite korisnicko ime: " << std::endl;
                        std::cin >> username;
                        std::cout << "Unesite lozinku: " << std::endl;
                        std::cin >> password;
                        while (!passIsStrong(password))
                        {
                            cout << "Nedovoljno jaka lozinka, pokusajte ponovo: ";
                            cin.ignore();
                            std::cin >> password;
                        }
                    } while (User.isExisting(username) && count != 1 && count != 2 && count != 3);
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
                } while (!User.deleteUser(delete_user));
            }
        }
        else if (choice == 3)
        {
            cin.ignore();

            cout << "Unesite pocetni datum filtriranja (u formatu: YYYY-MM-DD) ili ostavite prazno: ";
            string startDate;
            getline(cin, startDate);

            cout << "Unesite krajnji datum filtriranja (u formatu: YYYY-MM-DD) ili ostavite prazno: ";
            string endDate;
            getline(cin, endDate);

            if (!validateDates(startDate, endDate))
            {
                cerr << "Nepravilan unos datuma." << endl;
                // povratak u proslu funkciju
            }
            else
            {
                int status;

                cout << "Odaberite koje tikete zelite vidjeti (trenutno otvorene, trenutno zatvorene ili sve): " << endl;
                cout << "1. Otvoreni" << endl;
                cout << "2. Zatvoreni" << endl;
                cout << "3. Svi" << endl;
                cout << "4. Povratak" << endl;

                cin >> status;
                if (status == 1 || status == 2 || status == 3)
                {
                    try
                    {
                        auto filteredTickets = filterTickets(startDate, endDate, status);

                        cout << "Broj filtriranih tiketa: " << filteredTickets.size() << endl;
                        generateStatistics(filteredTickets);
                    }
                    catch (const exception &e)
                    {
                        cerr << "Greska: " << e.what() << endl;
                    }
                }
            }
        }
        else if (choice == 4)
        {
            std::cout << "Provjera verzije sistema" << std::endl;
            std::string verzija = org.provjeriVerziju();
            std::cout << "Trenutna verzija sistema: " << verzija << std::endl;

            if (verzija == "Besplatna verzija")
            {
                char aktivacija;
                std::cout << "Želite li aktivirati komercijalnu verziju? (Y/N): ";
                std::cin >> aktivacija;

                if (aktivacija == 'Y' || aktivacija == 'y')
                {
                    std::string key;
                    if (org.unosKljuča()) // Unos ključa za aktivaciju
                    {
                        std::string key = org.getKljuc();

                        if (User.validateKey(key)) // Provjera validnosti ključa
                        {
                            std::cout << "Ključ je validan!" << std::endl;

                            // Dodaj ključ organizaciji
                            std::string freeKey = User.getFirstFreeKey();
                            if (!freeKey.empty())
                            {
                                User.addKeyToOrganization("organization_name", freeKey);
                                std::cout << "Komercijalna verzija je aktivirana!" << std::endl;
                            }
                            else
                            {
                                std::cout << "Nema slobodnih ključeva!" << std::endl;
                            }
                        }
                        else
                        {
                            std::cout << "Nevažeći ključ!" << std::endl;
                        }
                    }
                }
                else if (aktivacija == 'N' || aktivacija == 'n')
                {
                    std::cout << "Odlučili ste da ne aktivirate komercijalnu verziju." << std::endl;
                }
                else
                {
                    std::cout << "Nevažeći unos. Molimo vas da unesete 'Y' ili 'N'." << std::endl;
                }
            }
        }
    } while (choice != 5);
}