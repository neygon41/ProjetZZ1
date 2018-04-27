#include "Bloc.h"

Bloc::Bloc(int x,int y,Type_Bloc t)
{
    m_x=x;
    m_y=y;
    type=t;
    switch (type)
    {
    case VIDE:
        rep='.';
        break;
    case AIR:
        rep=' ';
        break;
    case MUR:
        rep='X';
        break;
    case PORTE:
        rep='/';
        break;
    default:
        rep='0';
    }
}

Bloc::~Bloc()
{
    //dtor
}
