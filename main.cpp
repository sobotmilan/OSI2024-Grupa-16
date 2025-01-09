#include <iostream>
#include "file.h"

int main() {
    file f("Tickets.csv");
    f.allTickets("zatvoren");
    return 0;
}
