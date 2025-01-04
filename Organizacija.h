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
};