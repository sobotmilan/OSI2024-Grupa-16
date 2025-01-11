#pragma once
#include <iostream>

class User{
    private:
        std::string username;
        std::string password;
        std::string role;
    public:
        void setUsername(std::string user){this->username=user;}
        void setPassword(std::string pass){this->password=pass;}
        void setRole(std::string role){this->role=role;}
        std::string getUsername() const{return username;}
        std::string getPassword() const{return password;}
        std::string getRole() const{return role;}
        bool isEmpty()
        {
            if (this->username == "")
            {
                return true;
            }
            return false;
        }
        void prikaziMeniZaKorisnike() {
    int izbor = 0;

    do {
        // Prikaz menija
        std::cout << "\n===== MENI ZA KORISNIKE =====" << std::endl;
        std::cout << "1. Kreiraj tiket" << std::endl;
        std::cout << "2. Prikazi sve tikete" << std::endl;
        std::cout << "3. Upisi novu organizaciju" << std::endl;
        std::cout << "4. Izlaz" << std::endl;
        std::cout << "Unesite svoj izbor: ";
        std::cin >> izbor;

        // Obrada izbora korisnika
        switch (izbor) {
            case 1: {
                try {
                    std::cin.ignore(); // Očisti ulazni tok pre unosa
                    std::string operater = "Marko"; // Primer: Operater koji kreira tiket
                    Ticket noviTiket = kreirajTiket(operater);
                    std::cout << "Tiket uspjesno kreiran!" << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "Greska: " << e.what() << std::endl;
                }
                break;
            }
            case 2: {
                // Prikaz svih tiketa (pretpostavljamo da je vektor tiketa dostupan)
                std::vector<Ticket> tiketi = {
                    Ticket(1, "Marko", "Korisnik1", "Otvoren", "Problem sa mrežom"),
                    Ticket(2, "Ana", "Korisnik2", "Zatvoren", "Problem sa računarom")
                };
                std::cout << "\nLista svih tiketa:\n";
                for (const auto& tiket : tiketi) {
                    std::cout << "ID: " << tiket.getID() << ", Status: " << tiket.getStatus()
                              << ", Operater: " << tiket.getOperater()
                              << ", Korisnik: " << tiket.getKorisnik()
                              << ", Info: " << tiket.getInfo() << std::endl;
                }
                break;
            }
            case 3: {
                try {
                    std::cin.ignore(); // Očisti ulazni tok pre unosa
                    upisiOrganizacijuUFajl();
                } catch (const std::exception& e) {
                    std::cerr << "Greska: " << e.what() << std::endl;
                }
                break;
            }
            case 4:
                std::cout << "Izlaz iz menija. Dovidjenja!" << std::endl;
                break;
            default:
                std::cout << "Nepostojeca opcija. Pokusajte ponovo." << std::endl;
                break;
        }
    } while (izbor != 4);
}

};