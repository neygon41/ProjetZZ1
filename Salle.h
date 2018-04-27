#ifndef SALLE_H
#define SALLE_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

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

        friend void afficher_carte(vector<Salle>* s);
    protected:
    private:
        vector<Bloc> m_blocs;
        int m_x;
        int m_y;
};

#endif // SALLE_H
