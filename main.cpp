#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>

#include "Salle.h"

using namespace std;

int main()
{
    srand(time(NULL));
    vector<Salle> ok;

    ok = placer_salles(5);

    afficher_carte(&ok);

    return 0;
}
