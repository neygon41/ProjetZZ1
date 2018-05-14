#ifndef SALLE_H
#define SALLE_H

#include <iostream>
#include <vector>
#include <string>
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
        Salle(int x, int y);
        virtual ~Salle();

        void genererSalle(string code);
        void afficherSalle();

        void changer_coord(int x, int y);

        friend void afficher_carte(vector<Salle>* s);

        vector<Bloc> m_blocs;
        int m_x;
        int m_y;

    protected:
    private:
};

#endif // SALLE_H
