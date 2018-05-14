#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "Salle.h"

using namespace std;

int main()
{
    srand(time(NULL));
    vector<Salle> ok;

    Salle op(0,0), op2(0,0);
    op.genererSalle("2 0 0 2 1 0 3 2 0 2 3 0 3 0 1 2 0 2 2 0 3 2 1 3 2 2 3 2 3 3 2 3 2 2 3 1 1 1 1 1 1 2 1 2 1 1 2 2");
    op2.genererSalle("2 0 0 2 1 0 3 2 0 2 3 0 3 0 1 2 0 2 2 0 3 2 1 3 2 2 3 2 3 3 2 3 2 2 3 1 1 1 1 1 1 2 1 2 1 1 2 2 2 4 4");
    ok.push_back(op);
    op2.placement(&ok);
    ok.push_back(op2);
    afficher_carte(&ok);

    return 0;
}
