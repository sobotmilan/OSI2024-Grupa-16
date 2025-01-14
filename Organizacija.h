#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <unordered_map>
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

    // Metoda za unos ključa
    bool unosKljuča()
    {
        string unos;
        cout << "Unesite ključ za aktivaciju komercijalne verzije: ";
        cin >> unos;
        setKljuc(unos);
        cout << "Ključ je unesen: " << unos << endl;
        return true;
    }

    // Provjerava verziju organizacije
    string provjeriVerziju()
    {
        // Ako je ključ prazan, organizacija je besplatna
        if (kljuc.empty())
        {
            return "Besplatna verzija";
        }

        // Ako ključ nije prazan, provjeravamo CSV fajl
        ifstream file("keys.csv");
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

    void upisiOrganizacijuUFajl(const string &filename = "Organizacije.csv")
    {
        string ime, adresa, brojTelefona, email;

        // Unos podataka o organizaciji
        cout << "Unesite ime organizacije: ";
        getline(cin, ime);

        cout << "Unesite adresu organizacije: ";
        getline(cin, adresa);

        cout << "Unesite broj telefona organizacije: ";
        getline(cin, brojTelefona);

        cout << "Unesite email organizacije: ";
        getline(cin, email);

        // Otvaranje fajla za upisivanje (append mode)
        ofstream file(filename, ios::app);
        if (!file.is_open())
        {
            throw runtime_error("Greska prilikom otvaranja fajla " + filename);
        }

        // Upisivanje podataka u fajl
        file << ime << "," << adresa << "," << brojTelefona << "," << email << "\n";

        cout << "Organizacija uspjesno upisana u fajl!" << endl;

        file.close();
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