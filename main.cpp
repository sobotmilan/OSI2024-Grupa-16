#include <iostream>
#include <string>
#include <cstdlib>
#include "file.h"
#include "User.h"
#include "menuOperater.h"
int main()
{
    User currentUser;
    bool stopProgram = false;
    file Organization("Organization.csv");
    file User("User.csv");

    if (Organization.isEmpty())
    {
        std::cout << "Kreiranje organizacije:" << std::endl;
        std::string organizationName;
        std::cout << "Unesite naziv organizacije: ";
        std::cin >> organizationName;
        // Organization.addOrganization(organizationName);
        std::cout << std::endl;

        std::cout << "Kreiranje administratorskog naloga: " << std::endl;
        std::string adminUsername, adminPassword;
        do
        {
            std::cout << "Unesite korisnicko ime admina: " << std::endl;
            std::cin >> adminUsername;
            std::cout << "Unesite lozinku za admina: " << std::endl;
            std::cin >> adminPassword;
        } while (User.isExisting(adminUsername));

        User.addUser(adminUsername, adminPassword, "Admin");
        currentUser.setUsername(adminUsername);
        currentUser.setPassword(adminPassword);
        currentUser.setRole("Admin");

        Organization.addOrganization(organizationName, "");

        std::cout << "Uspjesno kreirana organizacija" << std::endl;
    }
    do
    {
        if (currentUser.getUsername() == "")
        {
            std::cout << "Prijava na nalog: " << std::endl;
            std::string username, password;
            do
            {
                std::cout << "Unesite korisnicko ime: " << std::endl;
                std::cin >> username;
                std::cout << "Unesite lozinku: " << std::endl;
                std::cin >> password;
            } while (!User.login(username, password));
            currentUser.setUsername(username);
            currentUser.setPassword(password);
            currentUser.setRole(User.findRole(username));
        }
        int choice;
        if (currentUser.getRole() == "User")
        {
            do
            {
                std::cout << "\n===== MENI ZA KORISNIKE =====" << std::endl;
                std::cout << "1. Kreiraj tiket" << std::endl;
                std::cout << "2. Prikazi sve tikete" << std::endl;
                std::cout << "3. Ažuriraj informacije na tiketu" << std::endl;
                std::cout << "4. Odjava" << std::endl;
                std::cout << "Unesite svoj izbor: ";
                std::cin >> choice;

            } while (choice != 4);
        }
        else if (currentUser.getRole() == "Operater")
        {
            showMenuOperater(currentUser);
        }
        else if (currentUser.getRole() == "Admin")
        {
            do
            {
                std::cout << "\n===== MENI ZA ADMINE =====" << std::endl;
                std::cout << "1. Kreiraj tiket" << std::endl;
                std::cout << "2. Prikazi sve tikete" << std::endl;
                std::cout << "3. Upravljanje nalozima korisnika i operatera" << std::endl;
                std::cout << "4. Pregled statistike (dnevna i mjesečna)" << std::endl;
                std::cout << "5. Odjava" << std::endl;
                std::cout << "Unesite svoj izbor: ";
                std::cin >> choice;
            } while (choice != 5);
        }
        currentUser.setUsername("");
        currentUser.setPasswordNoCheck("");
        currentUser.setRole("");
    } while (!stopProgram);

    return 0;
}
