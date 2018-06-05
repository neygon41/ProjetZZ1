#ifndef SALLE_H
#define SALLE_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

#define H_M_S 5
#define L_M_S 5

using namespace std;

typedef enum
{
    VIDE,
    AIR,
    MUR,
    PORTE,
}Type_Bloc;

typedef struct
{
    Type_Bloc type;
    int x;
    int y;
} Bloc;

class Salle
{
    public:
        Salle();
        Salle(int x, int y);
        virtual ~Salle();

        void genererSalle(string code);
        void afficherSalle();
        void placement(vector<Salle>* s);

        void changer_coord(int x, int y);
        void supprimer_blocs();

        friend void afficher_carte(vector<Salle>* s);

        Salle fusionner_salle( Salle s2);
        int posx();
        int posy();
        vector<Bloc> m_portes;
        vector<Bloc> m_blocs;

    private:
        int m_x;
        int m_y;
};

#endif // SALLE_H
