#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "Salle.h"

using namespace std;

#define H_M_S 10
#define L_M_S 10

void Placement(Salle* p, vector<Salle>* s)
{
    int minx=0,miny=0,maxx=0,maxy=0;
    vector<Bloc> e;
    for(unsigned int i=0; i<(s->size()); i++)
    {
        Salle sal=(*s)[i];
        e=sal.m_blocs;

        for(unsigned int j=0; j<(e.size()); j++)
        {
            minx=min(minx,sal.m_x + e[j].x);
            maxx=max(maxx,sal.m_x + e[j].x);
            miny=min(miny,sal.m_y + e[j].y);
            maxy=max(maxy,sal.m_y + e[j].y);
        }
    }
    //cout << "min x-y "<<minx<<"-"<<miny<<endl<< "max x-y "<<maxx<<"-"<<maxy<<endl;
    int t[maxx-minx+1][maxy-miny+1];
    for(int i=0; i<=maxx-minx; i++)
    {
        for(int j=0; j<=maxy-miny; j++)
            t[i][j]=0;
    }
    for(unsigned int i=0; i<s->size(); i++)
    {
        Salle sal=(*s)[i];
        cout<<"pos sal:"<<sal.m_x<<" "<<sal.m_y<<endl;
        e=sal.m_blocs;
        for(unsigned int j=0; j<(e.size()); j++)
        {
            if (e[j].type!=VIDE)
            {
                t[sal.m_x+e[j].x-minx][sal.m_y+e[j].y-miny]=1;
            }
        }
    }
    bool continuer=true;
    int px,py;
    e=p->m_blocs;

    while (continuer)
    {
        cout<<" essai"<<endl;
        px=rand()%(-minx+L_M_S+maxx+L_M_S+1)-minx-L_M_S;
        py=rand()%(-miny+H_M_S+maxy+H_M_S+1)-miny-H_M_S;
        p->changer_coord(px,py);
        int i=0;
        continuer=false;
        while (i<e.size() && not(continuer))
        {
            if (px+e[i].x<maxx && py+e[i].y<maxy)
            {
                if (t[px+e[i].x-minx][py+e[i].y-miny]==1)
                {
                    continuer=true;
                }
            }
            if ((px+e[i].x-1>=minx && py+e[i].y<maxy && t[px+e[i].x-minx-1][py+e[i].y-miny]==1)
                || (px+e[i].x+1<maxx && py+e[i].y<maxy && t[px+e[i].x-minx+1][py+e[i].y-miny]==1)
                || (py+e[i].y-1>=miny && px+e[i].x<maxx && t[px+e[i].x-minx][py+e[i].y-miny-1]==1)
                || (py+e[i].y+1<maxy && px+e[i].x<maxx && t[px+e[i].x-minx][py+e[i].y-miny+1]==1))
                    continuer=true;
            i++;
        }
    }
}

int main()
{
    srand(time(NULL));
    vector<Salle> ok;

    Salle op(0,0), op2(0,0);
    op.genererSalle("2 0 0 2 1 0 3 2 0 2 3 0 3 0 1 2 0 2 2 0 3 2 1 3 2 2 3 2 3 3 2 3 2 2 3 1 1 1 1 1 1 2 1 2 1 1 2 2");
    op2.genererSalle("2 0 0 2 1 0 3 2 0 2 3 0 3 0 1 2 0 2 2 0 3 2 1 3 2 2 3 2 3 3 2 3 2 2 3 1 1 1 1 1 1 2 1 2 1 1 2 2 2 4 4");
    ok.push_back(op);
    Placement(&op2,&ok);
    ok.push_back(op2);
    afficher_carte(&ok);

    return 0;
}
