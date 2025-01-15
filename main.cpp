#include <iostream>
#include <string>
#include <cstdlib>
#include "file.h"
#include "User.h"
#include "menuOperater.h"
#include "menu_admin.h"
#include "menuUser.h"

int main()
{
    User currentUser;
    Organizacija Organizacija;
    bool stopProgram = false;
    file Organization("Organization.csv");
    file User("User.csv");
    file Keys("Keys.csv");

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
            while (!passIsStrong(adminPassword))
            {
                cout << "Nedovoljno jaka lozinka, pokusajte ponovo: ";
                cin.ignore();
                std::cin >> adminPassword;
            }
        } while (User.isExisting(adminUsername));

        User.addUser(adminUsername, adminPassword, "Admin");
        currentUser.setUsername(adminUsername);
        currentUser.setPassword(adminPassword);
        currentUser.setRole("Admin");

        Organization.addOrganization(organizationName, "");
    }
    do
    {
        if (currentUser.getUsername() == "")
        {
            std::cout << "Prijava na nalog: " << std::endl;
            std::string username, password;
            std::cout << "Unesite korisnicko ime: " << std::endl;
            std::cin >> username;
            std::cout << "Unesite lozinku: " << std::endl;
            std::cin >> password;
            if (User.login(username, password))
            {
                currentUser.setUsername(username);
                currentUser.setPassword(password);
                currentUser.setRole(User.findRole(username));
            }
            else
            {
                std::cout << "Pogresna sifra/ime" << std::endl;
            }
        }
        int choice;
        if (currentUser.getRole() == "User")
        {
            do
            {
                showMenuUser(currentUser);
            } while (choice != 3);
        }
        else if (currentUser.getRole() == "Operater")
        {
            showMenuOperater(currentUser);
        }
        else if (currentUser.getRole() == "Admin")
        {
            menuForAdmin(User, Keys, Organization, Organizacija, currentUser.getUsername());
        }
        currentUser.setUsername("");
        currentUser.setPasswordNoCheck("");
        currentUser.setRole("");
    } while (!stopProgram);

    return 0;
}
