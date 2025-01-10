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
    //Metoda za promjenu lozinke
        bool changePassword(std::string oldPass, std::string newPass, std::string username)
        {
            if (newPass.empty())
            {
                std::cout << "New password cannot be empty." << std::endl;
                return false;
            }

            if (login(username, oldPass))
            {
                std::ifstream file(namefile);
                if (!file)
                {
                    std::cout << "Error: Failed to open file. Make sure it exists and is not being used by another process." << std::endl;
                    return false;
                }

                std::ofstream tempfile("temp.csv");
                if (!tempfile)
                {
                    std::cout << "Error: Failed to create temporary file." << std::endl;
                    file.close();
                    return false;
                }

                std::string line;
                bool userFound = false;

                while (std::getline(file, line))
                {
                    std::stringstream ss(line);
                    std::string user, pass, role;

                    std::getline(ss, user, ',');
                    std::getline(ss, pass, ',');
                    std::getline(ss, role, ',');

                    if (user == username)
                    {
                        // Update password for the user
                        tempfile << user << "," << newPass << "," << role << "\n";
                        userFound = true;
                    }
                    else
                    {
                        // Write the original line for other users
                        tempfile << line << "\n";
                    }
                }

                file.close();
                tempfile.close();

                if (!userFound)
                {
                    std::cout << "Error: User not found in the file." << std::endl;
                    std::remove("temp.csv");
                    return false;
                }

                if (std::remove(namefile.c_str()) != 0)
                {
                    std::cout << "Error: Failed to delete the original file." << std::endl;
                    return false;
                }

                if (std::rename("temp.csv", namefile.c_str()) != 0)
                {
                    std::cout << "Error: Failed to rename the temporary file." << std::endl;
                    return false;
                }

                std::cout << "Password successfully changed." << std::endl;
                return true;
            }
            else
            {
                std::cout << "Error: You have entered the wrong current password." << std::endl;
                return false;
            }
        }
};