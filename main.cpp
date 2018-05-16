#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <string>

#include "Salle.h"

using namespace std;

vector<string> charger_fichiers(int salle_max, string emplacement, string extension);
vector<Salle> placer_salles(unsigned int nb_salles, vector<string> codes_salles);

int main()
{
    unsigned int genseed;
    genseed=time(NULL);
    genseed=1526457752;
    srand(genseed);
    cout<< "seed: " << genseed<<endl;
    vector<Salle> rand_map;

    rand_map = placer_salles(5, charger_fichiers(7, "Salles/salle", ".txt"));

    afficher_carte(&rand_map);

    return 0;
}

vector<string> charger_fichiers(int salle_max, string emplacement, string extension)
{
    vector<string> codes;
    string nom_fichier, line, code_cour("");
    ostringstream oss; //Aucune reference aux films

    for(int i=1 ; i<=salle_max ; i++)
    {
        oss << emplacement << i << extension;
        nom_fichier = oss.str();
        oss.str("");
        ifstream fic(nom_fichier.c_str(), ios::in);
        if(fic)
        {
            code_cour = "";
            while(getline(fic, line))
            {
                line = " "+line;
                code_cour+=line;
            }
            codes.push_back(code_cour);

            fic.close();
        }
        else
        {
            cerr << "Impossible d'ouvrir le fichier " << nom_fichier << endl;
        }
    }

    return codes;
}

vector<Salle> placer_salles(unsigned int nb_salles, vector<string> codes_salles)
{
    int num_sal, n=codes_salles.size();
    vector<Salle> carte;
    Salle sal;

    /* On place la premiere salle en (0;0) */
    cout << "Placement de la salle 1" << endl;
    num_sal = rand()%n;
    cout << num_sal << endl;
    sal.genererSalle(codes_salles[num_sal]);
    carte.push_back(sal);
    sal.supprimer_blocs();
    /* ----------------------------------- */
    for(unsigned int i=1 ; i<nb_salles ; i++)
    {
        cout << "Placement de la salle " << i+1 << endl;
        num_sal = rand()%n;
        cout << num_sal+1 << endl;
        sal.genererSalle(codes_salles[num_sal]);
        sal.placement(&carte);
        cout << "Salle placee !" << endl;
        carte.push_back(sal);
        sal.supprimer_blocs();
    }

    return carte;
}
