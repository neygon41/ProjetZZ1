#ifndef SALLE_H
#define SALLE_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

#define H_M_S 10
#define L_M_S 10

using namespace std;

typedef enum
{
    VIDE,
    AIR,
    MUR,
    PORTE
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

        friend void afficher_carte(vector<Salle>* s);

    private:
        vector<Bloc> m_blocs;
        int m_x;
        int m_y;
};

Salle random_load(int salle_max, string emplacement, string extension);
vector<Salle> placer_salles(unsigned int nb_salles);

#endif // SALLE_H
