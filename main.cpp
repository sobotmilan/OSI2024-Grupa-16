#include <iostream>
#include "Operater.h"
using namespace std;

int main()
{
    vector<Operater> arr = {
        Operater("Milan", 1),
        Operater("Sanja", 2),
        Operater("Ana", 3),
        Operater("MarijaS", 4),
        Operater("MarijaG", 5),
    };

    writeOpsToCSV(arr);

    cout << "Ovaj tiket ce da obradjuje " << findSuitableOperater().getUser() << "." << endl;
    return 0;
}
