#ifndef BLOC_H
#define BLOC_H

typedef enum
{
    VIDE,
    AIR,
    MUR,
    PORTE
}Type_Bloc;

class Bloc
{
    public:
        Bloc(int x,int y,Type_Bloc t);
        virtual ~Bloc();
        char rep;
        int m_x;
        int m_y;
        Type_Bloc type;
        int salle;
    protected:
    private:

};

#endif // BLOC_H
