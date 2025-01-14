#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

class Organizacija
{
private:
    std::string kljuc;
    std::string nazivOrganizacije;

public:
    void setKljuc(std::string kljuc) { this->kljuc = kljuc; }
    void setNazivOrganizacije(std::string nazivOrganizacije) { this->nazivOrganizacije = nazivOrganizacije; }

    std::string getKljuc() const { return kljuc; }
    std::string getNazivOrganizacije() const { return nazivOrganizacije; }

    // Metoda za unos ključa
    bool unosKljuča()
    {
        std::string unos;
        std::cout << "Unesite ključ za aktivaciju komercijalne verzije: ";
        std::cin >> unos;
        setKljuc(unos);
        std::cout << "Ključ je unesen: " << unos << std::endl;
        return true;
    }


    // Provjerava verziju organizacije
    std::string provjeriVerziju()
    {
        // Ako je ključ prazan, organizacija je besplatna
        if (kljuc.empty())
        {
            return "Besplatna verzija";
        }

        // Ako ključ nije prazan, provjeravamo CSV fajl
        std::ifstream file("keys.csv");
        std::string line;
        std::string status = "Besplatna verzija"; // Podrazumijevano je besplatna

        // Čitanje CSV fajla
        while (std::getline(file, line))
        {
            std::stringstream ss(line);
            std::string key, keyStatus;
            std::getline(ss, key, ',');  // Učitaj ključ
            std::getline(ss, keyStatus); // Učitaj status

            // Ako nađemo odgovarajući ključ
            if (key == kljuc && keyStatus == "aktivan")
            {
                status = "Komercijalna verzija";
                break;
            }
        }
        file.close();
        return status;
    }
};