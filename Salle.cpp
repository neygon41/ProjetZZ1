#include "Salle.h"

Salle::Salle()
{
    m_x = 0;
    m_y = 0;
}

Salle::Salle(int x, int y)
{
    m_x = x;
    m_y = y;
}

Salle::~Salle()
{
    //dtor
}


void Salle::genererSalle(string code)
{
    istringstream iss(code); //Flux sur le code
    int code_bloc, pos_x, pos_y; //Les 3 infos a collecter pour chaque bloc
    Bloc cour; //Le bloc courant (qui va etre ajoute a la salle)

    while(!iss.eof()) //Tant qu'on a pas lu tout le flux
    {
        iss >> code_bloc >> pos_x >> pos_y; //On recupere les infos
        cour.x = pos_x;
        cour.y = pos_y;
        switch(code_bloc) //Un meilleur moyen de faire ? L'assignation directe ne marchait pas...
        {
        case AIR:
            cour.type = AIR;
            break;
        case MUR:
            cour.type = MUR;
            break;
        case PORTE:
            cour.type = PORTE;
            break;
        default:
            cour.type = AIR;
            break;
        }
        m_blocs.push_back(cour);
    }
}

void Salle::placement(vector<Salle>* s)
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
    e=this->m_blocs;

    while (continuer)
    {
        cout<<" essai"<<endl;
        px=rand()%(-minx+L_M_S+maxx+L_M_S+1)-minx-L_M_S;
        py=rand()%(-miny+H_M_S+maxy+H_M_S+1)-miny-H_M_S;
        this->changer_coord(px,py);
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

void Salle::changer_coord(int x, int y)
{
    m_x=x;
    m_y=y;
}

void Salle::supprimer_blocs()
{
    m_blocs.clear();
}

void Salle::afficherSalle()
{
    for(unsigned int i = 0 ; i < m_blocs.size() ; i++)
        cout << m_blocs[i].type << " " << m_blocs[i].x << " " << m_blocs[i].y << endl;
}

//Fonction amie de Salle
void afficher_carte(vector<Salle>* s)
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
    char t[maxx-minx+1][maxy-miny+1];
    for(int i=0; i<=maxx-minx; i++)
    {
        for(int j=0; j<=maxy-miny; j++)
            t[i][j]=' ';
    }
    for(unsigned int i=0; i<s->size(); i++)
    {
        Salle sal=(*s)[i];
        cout<<"pos sal:"<<sal.m_x<<" "<<sal.m_y<<endl;
        e=sal.m_blocs;
        for(unsigned int j=0; j<(e.size()); j++)
        {
            switch(e[j].type)
            {
            case VIDE:
                t[sal.m_x+e[j].x-minx][sal.m_y+e[j].y-miny]=' ';
                break;
            case AIR:
                t[sal.m_x+e[j].x-minx][sal.m_y+e[j].y-miny]='.';
                break;
            case MUR:
                t[sal.m_x+e[j].x-minx][sal.m_y+e[j].y-miny]='#';
                break;
            case PORTE:
                t[sal.m_x+e[j].x-minx][sal.m_y+e[j].y-miny]='/';
                break;
            default:
                t[sal.m_x+e[j].x-minx][sal.m_y+e[j].y-miny]='0';
                break;
            }
        }
    }
    for(int i=0; i<maxy-miny+1; i++)
    {
        for(int j=0; j<maxx-minx+1; j++)
        {
            cout << t[j][i];
        }
        cout << endl;
    }
}
