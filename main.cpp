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
void creer_couloir(vector<Salle> * carte);

int main()
{
    unsigned int genseed;
    genseed=time(NULL);
    genseed=1527059125;
    srand(genseed);
    cout<< "seed: " << genseed<<endl;
    vector<Salle> rand_map;

    rand_map = placer_salles(2, charger_fichiers(7, "Salles/salle", ".txt"));

    Salle s1 = rand_map[rand_map.size()-1];
    Salle s2 = rand_map[rand_map.size()-2];
    rand_map.pop_back();
    rand_map.pop_back();

    rand_map.push_back(s1.fusionner_salle(s2));
    cout<< "NB SALLE VECTOR: " << rand_map.size()<<endl;
    afficher_carte(&rand_map);

    cout << "Tentative de creation couloir" <<endl;
    creer_couloir(&rand_map);
    cout << "couloir" << endl;
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

void creer_couloir(vector<Salle> * carte)
{
    /* choix de la porte de depart! */
    int sp1i=rand()%carte->size(), sp2i=0;
    Salle sp1 = (*carte)[sp1i];
    int nb_porte= sp1.m_portes.size();
    int num_porte=rand()%nb_porte;
    Bloc ported= sp1.m_portes[num_porte];
    bool continuer=true;
    int i=0, x=ported.x, y=ported.y;
    int maximum, minimum;

    /* initialisation matrice */
    int minx=0,miny=0,maxx=0,maxy=0;
    vector<Bloc> e;

    /* "suppression" de la porte de travail */
    (*carte)[sp1i].m_portes.erase((*carte)[sp1i].m_portes.begin() + num_porte);

    for(unsigned int i=0; i<(carte->size()); i++)
    {
        Salle sal=(*carte)[i];
        e=sal.m_blocs;
        for(unsigned int j=0; j<(e.size()); j++)
        {
            minx=min(minx,sal.posx() + e[j].x);
            maxx=max(maxx,sal.posx() + e[j].x);
            miny=min(miny,sal.posy() + e[j].y);
            maxy=max(maxy,sal.posy() + e[j].y);
        }
    }

    int t[maxx-minx+3][maxy-miny+3];
    maximum=(maxy-miny+3)*(maxx-minx+3);
    for(int i=0; i<maxx-minx+3; i++)
    {
        for(int j=0; j<maxy-miny+3; j++)
            t[i][j]=maximum;
    }
    for(unsigned int i=0; i<carte->size(); i++)
    {
        Salle sal=(*carte)[i];
        e=sal.m_blocs;
        for(unsigned int j=0; j<(e.size()); j++)
        {
            if(e[j].type != VIDE)
            {
                t[sal.posx()+e[j].x-minx+1][sal.posy()+e[j].y-miny+1]=-1;
            }
        }
    }
    /* case portes */
    for(unsigned int i=0; i<(carte->size()); i++)
    {
        Salle sal=(*carte)[i];
        e=sal.m_portes;
        for(unsigned int j=0; j<(e.size()); j++)
        {
            if (i!=sp1i)
                t[e[j].x-minx+1][e[j].y-miny+1]=-2;
            else
                t[e[j].x-minx+1][e[j].y-miny+1]=-1;
        }
    }
    /* case de départ */
    t[x+1][y+1]=0;
    e=sp1.m_portes;

    /* boucle principale */
    while (continuer)
    {
        cout<< "boucle"<<endl;
        x=-1;
        while(x<maxx-minx+3 && continuer)
        {
            x++;
            y=-1;
            while(y<maxy-miny+3 && continuer)
            {
                y++;
                if (t[x][y]==i)
                {
                    minimum = maximum;
                    if (x-1>0 )
                    {
                        if(t[x-1][y]>=0)
                        {
                            minimum= min(minimum,t[x-1][y]);
                        }
                        else if (t[x-1][y]==-2)
                        {
                            continuer = false;
                            for(unsigned int j=0; j<(e.size()); j++)
                            {
                                if(e[j].x+1 == x-1-minx && e[j].y+1 == y-miny)
                                {
                                    continuer = true;
                                }
                            }
                            if (not(continuer))
                                x-=1;
                        }
                    }
                    if (y-1>0 && continuer)
                    {
                        if(t[x][y-1]>=0)
                        {
                            minimum= min(minimum,t[x][y-1]);
                        }
                        else if (t[x][y-1]==-2)
                        {
                            for(unsigned int j=0; j<(e.size()); j++)
                            {
                                continuer =false;
                                if(e[j].x+1 == x-minx && e[j].y+1 == y-1-miny)
                                {
                                    continuer = true;
                                }
                                if (not(continuer))
                                    y-=1;
                            }
                        }
                    }
                    if (x+1<maxx-minx+3 && continuer)
                    {
                        if(t[x+1][y]>=0)
                        {
                            minimum= min(minimum,t[x+1][y]);
                        }
                        else if (t[x+1][y]==-2)
                        {
                            for(unsigned int j=0; j<(e.size()); j++)
                            {
                                continuer =false;
                                if(e[j].x+1 == x+1-minx && e[j].y+1 == y-miny)
                                {
                                    continuer = true;
                                }
                                if (not(continuer))
                                    x+=1;
                            }
                        }
                    }
                    if (y+1<maxy-miny+3 && continuer)
                    {
                        if(t[x][y+1]>=0)
                        {
                            minimum= min(minimum,t[x][y+1]);
                        }
                        else if (t[x][y+1]==-2)
                        {
                            for(unsigned int j=0; j<(e.size()); j++)
                            {
                                continuer =false;
                                if(e[j].x+1 == x-minx && e[j].y+1 == y+1-miny)
                                {
                                    continuer = true;
                                }
                                if (not(continuer))
                                    y+=1;
                            }
                        }
                    }

                    /* avancement */
                    if (x-1>=0)
                    {
                        if (x+1<maxx-minx+3 && t[x+1][y]==minimum)
                            t[x-1][y]=min(t[x-1][y], i+1);
                        else
                            t[x-1][y]=min(t[x-1][y], i+2);
                    }
                    if (x+1<=maxx-minx+3)
                    {
                        if (x>0 && t[x-1][y]==minimum)
                            t[x+1][y]=min(t[x+1][y], i+1);
                        else
                            t[x+1][y]=min(t[x+1][y], i+2);
                    }
                    if (y-1>=0)
                    {
                        if (y+1<maxy-miny+3 && t[x][y+1]==minimum)
                            t[x][y-1]=min(t[x][y-1], i+1);
                        else
                            t[x][y-1]=min(t[x][y-1], i+2);
                    }
                    if (y+1<maxy-miny+3)
                    {
                        if (y>0 && t[x][y-1]==minimum)
                            t[x][y+1]=min(t[x][y+1], i+1);
                        else
                            t[x][y+1]=min(t[x][y+1], i+2);
                    }
                }
            }

        }
        i++;
    }

    /* recherche de la salle correspondant a la porte trouvee */
    int k=0;
    continuer = true;
    while (k<carte->size() && continuer )
    {
        for( unsigned int j =0; j<(*carte)[k].m_portes.size();j++)
        {
            if ((*carte)[k].m_portes[j].x ==x-1-minx && (*carte)[k].m_portes[j].y == y-1-minx)
            {
                sp2i=k;
                continuer = false;
                /* "suppression" de la porte trouvee */
                (*carte)[k].m_portes.erase((*carte)[k].m_portes.begin() + j);
            }
        }
        k++;
    }

    /* creation du couloir */
    Salle couloir;
    Bloc c;
    c.type=AIR;
    couloir.changer_coord(min(x,ported.x)-minx-1,min(y,ported.y)-minx-1);

    while (t[x][y]!=0)
    {
        cout << "x,y :"<<x << " "<<y<<endl;
        cout << "minimum :" << minimum<< endl;
        cout << "t[x][y]:"<<t[x][y]<<endl;
        cout << "t[x-1][y]:"<<t[x-1][y]<<endl;
        cout << "t[x+1][y]:"<<t[x+1][y]<<endl;
        cout << "t[x][y-1]:"<<t[x][y-1]<<endl;
        cout << "t[x][y+1]:"<<t[x][y+1]<<endl;
        for(int i=0; i<maxx-minx+3; i++)
        {
            for(int j=0; j<maxy-miny+3; j++)
                cout<< t[i][j]<< " ";
            cout<<endl;
        }

        /* recherche du minimum dans les cases adjacente */
        minimum = maximum;
        if (x-1>=0 && t[x-1][y]>=0)
        {
            minimum= min(minimum,t[x-1][y]);
        }
        if (y-1>=0 && t[x][y-1]>=0)
        {
            minimum= min(minimum,t[x][y-1]);
        }
        if (x+1<maxx-minx+3 && t[x+1][y]>=0)
        {
            minimum= min(minimum,t[x+1][y]);
        }
        if (y+1<maxy-miny+3 && t[x][y+1]>=0)
        {
            minimum= min(minimum,t[x][y+1]);
        }

        /* application */
        if (x-1>0 && t[x-1][y]==minimum)
        {
            x-=1;
        }
        else
        {
            if (y-1>0 && t[x][y-1]==minimum)
            {
                y-=1;
            }
            else
            {
                if (x+1<maxx-minx+3 && t[x+1][y]==minimum)
                {
                    x+=1;
                }
                else
                {
                    if (y+1<maxy-miny+3 && t[x][y+1]==minimum)
                    {
                        y+=1;
                    }
                }
            }
        }
        c.x=x-1;
        c.y=y-1;
        couloir.m_blocs.push_back(c);
    }


    for (unsigned int j =0;j<couloir.m_blocs.size(); j++ )
        t[couloir.m_blocs[j].x+1][couloir.m_blocs[j].y+1]=-1;

    /*création des murs */
    c.type=MUR;
    for (unsigned int j=0; j<couloir.m_blocs.size();j++)
    {
        x = couloir.m_blocs[j].x+1;
        y = couloir.m_blocs[j].y+1;
        if ((x-1>=0 && t[x-1][y]>0) || x-1<0)
        {
            c.x=x-2; c.y=y-1;
            couloir.m_blocs.push_back(c);
        }
        if ((y-1>=0 && t[x][y-1]>0) || y-1<0)
        {
            c.x=x-1; c.y=y-2;
            couloir.m_blocs.push_back(c);
        }
        if ((x+1<maxx-minx+3 && t[x+1][y]>0) || x+1>maxx-minx+3)
        {
            c.x=x; c.y=y-1;
            couloir.m_blocs.push_back(c);
        }
        if ((y+1<maxy-miny+3 && t[x][y+1]>0) || y+1>maxy-miny+3)
        {
            c.x=x-1; c.y=y;
            couloir.m_blocs.push_back(c);
        }

        if ( y+1>=maxy-miny+3 || x+1>=maxx-minx+3 || (y+1<maxy-miny+3 && x+1<maxx-minx+3 && t[x+1][y+1]>0))
        {
            c.x=x+2; c.y=y+2;
            couloir.m_blocs.push_back(c);
        }
        if ( y+1>=maxy-miny+3 || x-1<0 || (y+1<maxy-miny+3 && x-1>=0 && t[x-1][y+1]>0))
        {
            c.x=x; c.y=y+2;
            couloir.m_blocs.push_back(c);
        }
        if ( y-1<0 || x+1>=maxx-minx+3 || (y-1>=0 && x+1<maxx-minx+3 && t[x+1][y-1]>0))
        {
            c.x=x+2; c.y=y;
            couloir.m_blocs.push_back(c);
        }
        if ( y-1<0 || x-1<0 || (y-1>=0 && x-1>=0 && t[x-1][y-1]>0))
        {
            c.x=x; c.y=y;
            couloir.m_blocs.push_back(c);
        }
    }

    /* fusion des salles */
    Salle s3=(*carte)[min(sp2i,sp1i)].fusionner_salle((*carte)[max(sp2i,sp1i)]);
    s3=s3.fusionner_salle(couloir);
    carte->push_back(s3);
    carte->erase(carte->begin() + max(sp1i,sp2i));
    carte->erase(carte->begin() + min(sp1i,sp2i));
}
