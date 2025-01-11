#pragma once
#include <iostream>
  
class Organizacija
{
private:
std::string kljuc;
std::string nazivOrganizacije;
public:
void setKljuc(std::string kljuc){this->kljuc=kljuc;}
 void setNazivOrganizacije(std::string nazivOrganizacije){this->nazivOrganizacije=nazivOrganizacije;}

 std::string getKljuc() const{return kljuc;}
  std::string getNazivOrganizacije() const{return nazivOrganizacije;}
  void upisiOrganizacijuUFajl(const std::string& filename = "Organizacije.csv") {
    std::string ime, adresa, brojTelefona, email;

    // Unos podataka o organizaciji
    std::cout << "Unesite ime organizacije: ";
    std::getline(std::cin, ime);

    std::cout << "Unesite adresu organizacije: ";
    std::getline(std::cin, adresa);

    std::cout << "Unesite broj telefona organizacije: ";
    std::getline(std::cin, brojTelefona);

    std::cout << "Unesite email organizacije: ";
    std::getline(std::cin, email);

    // Otvaranje fajla za upisivanje (append mode)
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        throw std::runtime_error("Greska prilikom otvaranja fajla " + filename);
    }

    // Upisivanje podataka u fajl
    file << ime << "," << adresa << "," << brojTelefona << "," << email << "\n";

    std::cout << "Organizacija uspjesno upisana u fajl!" << std::endl;

    file.close();
}
};