#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <unordered_map>
#include "file.h"

using namespace std;

class Organizacija
{
private:
    string kljuc;
    string nazivOrganizacije;

public:
    void setKljuc(string kljuc) { this->kljuc = kljuc; }
    void setNazivOrganizacije(string nazivOrganizacije) { this->nazivOrganizacije = nazivOrganizacije; }

    string getKljuc() const { return kljuc; }
    string getNazivOrganizacije() const { return nazivOrganizacije; }

    string checkVersion(string kljuc)
    {
        // Ako je ključ prazan, organizacija je besplatna
        if (kljuc.empty())
        {
            return "Besplatna verzija";
        }

        // Ako ključ nije prazan, provjeravamo CSV fajl
        ifstream file("Keys.csv");

        // Provjera da li je fajl uspešno otvoren
        if (!file.is_open())
        {
            cerr << "Greška pri otvaranju datoteke." << endl;
            return "Greška. Nije moguće otvoriti datoteku za čitanje.";
        }

        string line;
        string status = "Besplatna verzija"; // Podrazumijevano je besplatna

        // Čitanje CSV fajla
        while (getline(file, line))
        {
            stringstream ss(line);
            string key, keyStatus;
            getline(ss, key, ',');  // Učitaj ključ
            getline(ss, keyStatus); // Učitaj status

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

    int numOpsInOrg() const
    {
        ifstream userBase("User.csv", ios::in);
        if (userBase.is_open() == false)
            throw("Nemoguce otvaranje baze korisnika.");

        string currLine;
        int numOp = 0;

        while (getline(userBase, currLine))
        {
            istringstream ss(currLine);
            string username, pass, role;

            if (getline(ss, username, ',') && getline(ss, pass, ',') && getline(ss, role, ','))
            {
                if (role == "Operater")
                {
                    numOp++;
                }
            }
        }

        return numOp;
    }
};