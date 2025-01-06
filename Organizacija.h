#pragma once
#include <iostream>
  
class Organizacija {
private:
    std::string kljuc;
    std::string nazivOrganizacije;
public:
    void setKljuc(std::string kljuc){this->kljuc=kljuc;}
    void setNazivOrganizacije(std::string nazivOrganizacije){this->nazivOrganizacije=nazivOrganizacije;}

    std::string getKljuc() const{return kljuc;}
    std::string getNazivOrganizacije() const{return nazivOrganizacije;}

    // Metoda za unos ključa
    bool unosKljuča() {
        std::string unos;
        std::cout << "Unesite ključ za aktivaciju komercijalne verzije: ";
        std::cin >> unos;
        setKljuc(unos);
        std::cout << "Ključ je unesen: " << unos << std::endl;
        return true;
    }

};