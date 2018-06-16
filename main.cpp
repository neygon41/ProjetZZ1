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
    /*genseed=1528709861;*/
    srand(genseed);
    cout<< "seed: " << genseed<<endl;
    vector<Salle> rand_map;

    rand_map = placer_salles(5, charger_fichiers(7, "Salles/salle", ".txt"));

    cout<< "NB SALLE VECTOR: " << rand_map.size()<<endl;
    afficher_carte(&rand_map);

    cout << "Tentative de creation couloir" <<endl;
    while (rand_map.size()>1)
    {
        creer_couloir(&rand_map);
        cout << "couloir" << endl;
    }
    for(int i=0 ; i < rand_map.size() ; i++)
        rand_map[i].boucher_portes();
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
    int sp1i=rand()%(carte->size()), sp2i;
    Salle sp1 = (*carte)[sp1i], sp2;
    int nb_porte= sp1.m_portes.size();
    int num_porte=rand()%nb_porte;
    Bloc ported= sp1.m_portes[num_porte];
    bool continuer=true;
    int i=0;
    int maximum, minimum;

    /* initialisation matrice */
    int minx=0,miny=0,maxx=0,maxy=0;
    vector<Bloc> e;

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

    int x=ported.x+sp1.posx()-minx +1, y=ported.y+sp1.posy()-miny +1;

    /* initialisation tableau */
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

                if ( e[j].type != PORTE)
                {
                    t[sal.posx()+e[j].x-minx+1][sal.posy()+e[j].y-miny+1]=-1;
                }
            }
        }
    }

    for(unsigned int i=0; i<carte->size(); i++)
    {
        Salle sal=(*carte)[i];
        e=sal.m_portes;
        for(unsigned int j=0; j<(e.size()); j++)
        {
            if (i!=sp1i)
            {
                t[sal.posx()+e[j].x-minx+1][sal.posy()+e[j].y-miny+1]=-2;
            }
            else
            {
                t[sal.posx()+e[j].x-minx+1][sal.posy()+e[j].y-miny+1]=-3;
            }
        }
    }

    /* case portes */
    /*
    for(int i=0; i<maxy-miny+3; i++)
    {
        for(int j=0; j<maxx-minx+3; j++)
            if (t[j][i]==maximum)
            {
                cout<< "  "<<"";
            }
            else
            {
                    if (t[j][i]>=0 && t[j][i]<10)
                    {cout<< t[j][i]<<" ";}
                    else
                    {
                        cout<< t[j][i]<<"";
                    }
            }
        cout << endl;
    }*/

    /* case de départ */
    t[x][y]=0;
    e=sp1.m_portes;
    cout << "nb porte salle depart: " << e.size()<<endl;
    i=0;

    /* boucle principale */
    while (continuer)
    {
        /*cout<< "i: "<< i<<endl;*/
        x=-1;
        while(x<maxx-minx+3-1 && continuer)
        {
            x++;
            y=-1;
            while(y<maxy-miny+3-1 && continuer)
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
                        else
                        {
                            if (t[x-1][y]==-2)
                            {
                                continuer = false;
                                for(unsigned int j=0; j<=(e.size()); j++)
                                {
                                    cout<<"porte x: "<<sp1.posx()+e[j].x<<" porte y: "<<sp1.posy()+e[j].y<<" x "<<x-1+minx-1<<"y"<<y+miny-1<<endl;
                                    if(sp1.posx()+e[j].x == x-1+minx-1 && sp1.posy()+e[j].y == y+miny-1)
                                    {
                                        continuer = true;
                                    }
                                }
                                if (not(continuer))
                                    x-=1;
                            }
                        }
                    }
                    if (y-1>0 && continuer)
                    {
                        if(t[x][y-1]>=0)
                        {
                            minimum= min(minimum,t[x][y-1]);
                        }
                        else
                        {
                            if (t[x][y-1]==-2)
                            {
                                for(unsigned int j=0; j<=(e.size()); j++)
                                {
                                    continuer =false;
                                    cout<<"porte x: "<< sp1.posx()+e[j].x << " porte y: " << sp1.posy()+e[j].y<< " x "<< x+minx-1 << "y" << y-1+miny-1 <<endl;
                                    if(sp1.posx()+e[j].x == x+minx-1 && sp1.posy()+e[j].y == y-1+miny-1)
                                    {continuer = true;
                                    }
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
                        else
                        {
                            if (t[x+1][y]==-2)
                            {
                                for(unsigned int j=0; j<=(e.size()); j++)
                                {
                                    continuer =false;
                                    cout<<"porte x: "<<sp1.posx()+ e[j].x << " porte y: " << sp1.posy()+e[j].y<< " x "<< x+1+minx-1 << "y" << y+miny-1 <<endl;
                                    if(sp1.posx()+e[j].x == x+1+minx-1 && sp1.posy()+e[j].y == y+miny-1)
                                    {
                                        continuer = true;
                                    }
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
                        else
                        {
                            if (t[x][y+1]==-2)
                            {
                                for(unsigned int j=0; j<=(e.size()); j++)
                                {
                                    continuer =false;
                                    cout<<"porte x: "<< sp1.posx()+e[j].x << " porte y: " << sp1.posy()+e[j].y<< " x "<< x+minx-1 << "y" << y+1+miny-1 <<endl;
                                    if(sp1.posx()+e[j].x == x+minx-1 && sp1.posy()+e[j].y == y+1+miny-1)
                                    {
                                        continuer = true;
                                    }
                                }
                                if (not(continuer))
                                    y+=1;
                            }
                        }
                    }

                    /* avancement */
                    if (continuer)
                    {
                        if (x-1>=0)
                        {
                            if (x+1<maxx-minx+3 && t[x+1][y]==minimum)
                                t[x-1][y]=min(t[x-1][y], i+1);
                            else
                                t[x-1][y]=min(t[x-1][y], i+2);
                        }
                        if (x+1<maxx-minx+3)
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
        }
        i++;
    }

    /* affichage de l'etape */
    /*
    for(int i=0; i<maxy-miny+3; i++)
    {
        for(int j=0; j<maxx-minx+3; j++)
        {
            if (t[j][i]==maximum)
            {
                cout<< "  "<<"";
            }
            else
            {
                    if (t[j][i]>=0 && t[j][i]<10)
                    {cout<< t[j][i]<<" ";}
                    else
                    {
                        cout<< t[j][i]<<"";
                    }
            }
        }
        cout << endl;
    }*/

    /* recherche de la salle correspondant a la porte trouvee */
    int k=0;
    continuer = true;
    /*cout<<"t[x][y] porte:"<<t[x][y] <<endl;*/
    while (k<carte->size() && continuer )
    {
        sp2 = (*carte)[k];
        e=sp2.m_portes;
        for( unsigned int j =0; j<sp2.m_portes.size();j++)
        {
            if (sp2.posx()+e[j].x == x+minx-1 && sp2.posy()+e[j].y == y+miny-1)
            {
                continuer = false;
                sp2i=k;
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
    couloir.changer_coord(min(x,ported.x)-minx-2,min(y,ported.y)-minx-2);
    cout<<"parcours"<<endl;
    while (t[x][y]!=0 && t[x][y]!=-3)
    {
        /*cout << "t[x][y]: " << t[x][y] <<" x: " <<x <<" y: "<<y <<endl;*/
        /* recherche du minimum dans les cases adjacentes */
        minimum = maximum;
        if (x-1>=0 && t[x-1][y]!=-1 && t[x-1][y]!=-2)
        {
            minimum= min(minimum,t[x-1][y]);
        }
        if (y-1>=0 && t[x][y-1]!=-1 && t[x][y-1]!=-2)
        {
            minimum= min(minimum,t[x][y-1]);
        }
        if (x+1<maxx-minx+3 && t[x+1][y]!=-1 && t[x+1][y]!=-2)
        {
            minimum= min(minimum,t[x+1][y]);
        }
        if (y+1<maxy-miny+3 && t[x][y+1]!=-1 && t[x][y+1]!=-2)
        {
            minimum= min(minimum,t[x][y+1]);
        }

        /* on s'y dirige */
        if (x-1>=0 && t[x-1][y]==minimum)
        {
            x-=1;
        }
        else
        {
            if (y-1>=0 && t[x][y-1]==minimum)
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
        c.x=x-1-couloir.posx()+minx;
        c.y=y-1-couloir.posy()+miny;
        couloir.m_blocs.push_back(c);
    }
    cout<<"question"<<endl;
    /* si on passe a cote d'une autre porte de la premiere salle, cette porte deviens celle d'origine*/
    if (t[x][y]==-3)
    {
        for( unsigned int j =0; j<=(*carte)[sp1i].m_portes.size();j++)
        {
            if ((*carte)[sp1i].posx()+(*carte)[sp1i].m_portes[j].x ==x-1-minx && (*carte)[sp1i].posy()+(*carte)[sp1i].m_portes[j].y == y-1-miny)
            {
                num_porte=j;
            }
        }
    }
    cout<<"bon"<<endl;
    /* "suppression" de porte de la premiere salle trouvee */
    (*carte)[sp1i].m_portes.erase((*carte)[sp1i].m_portes.begin() + num_porte);
    cout<<"pif"<<endl;
    /* pour eviter la redefinition d'une porte */
    couloir.m_blocs.pop_back();

    for (unsigned int j =0;j<couloir.m_blocs.size(); j++ )
        t[couloir.m_blocs[j].x+1-minx+couloir.posx()][couloir.m_blocs[j].y+1-miny+couloir.posy()]=-1;
    cout<<"express"<<endl;

    /*création des murs */
    c.type=MUR;
    int taille_couloir = couloir.m_blocs.size();
    for (unsigned int j=0; j<taille_couloir;j++)
    {
        x = couloir.m_blocs[j].x+1-minx+couloir.posx();
        y = couloir.m_blocs[j].y+1-miny+couloir.posy();
        if ((x-1>=0 && t[x-1][y]>0) || x-1<0)
        {
            c.x=x-1-couloir.posx()+minx-1; c.y=y-couloir.posy()+miny-1;
            couloir.m_blocs.push_back(c);
        }
        if ((y-1>=0 && t[x][y-1]>0) || y-1<0)
        {
            c.x=x-couloir.posx()+minx-1; c.y=y-1-couloir.posy()+miny-1;
            couloir.m_blocs.push_back(c);
        }
        if ((x+1<maxx-minx+3 && t[x+1][y]>0) || x+1>maxx-minx+3)
        {
            c.x=x+1-couloir.posx()+minx-1; c.y=y-couloir.posy()+miny-1;
            couloir.m_blocs.push_back(c);
        }
        if ((y+1<maxy-miny+3 && t[x][y+1]>0) || y+1>maxy-miny+3)
        {
            c.x=x-couloir.posx()+minx-1; c.y=y+1-couloir.posy()+miny-1;
            couloir.m_blocs.push_back(c);
        }

        if ( y+1>=maxy-miny+3 || x+1>=maxx-minx+3 || (y+1<maxy-miny+3 && x+1<maxx-minx+3 && t[x+1][y+1]>0))
        {
            c.x=x+1-couloir.posx()+minx-1; c.y=y+1-couloir.posy()+miny-1;
            couloir.m_blocs.push_back(c);
        }
        if ( y+1>=maxy-miny+3 || x-1<0 || (y+1<maxy-miny+3 && x-1>=0 && t[x-1][y+1]>0))
        {
            c.x=x-1-couloir.posx()+minx-1; c.y=y+1-couloir.posy()+miny-1;
            couloir.m_blocs.push_back(c);
        }
        if ( y-1<0 || x+1>=maxx-minx+3 || (y-1>=0 && x+1<maxx-minx+3 && t[x+1][y-1]>0))
        {
            c.x=x+1-couloir.posx()+minx-1; c.y=y-1-couloir.posy()+miny-1;
            couloir.m_blocs.push_back(c);
        }
        if ( y-1<0 || x-1<0 || (y-1>=0 && x-1>=0 && t[x-1][y-1]>0))
        {
            c.x=x-1-couloir.posx()+minx-1; c.y=y-1-couloir.posy()+miny-1;
            couloir.m_blocs.push_back(c);
        }
    }

    cout << "fin boucle 3  :"<< sp1i << " "<< sp2i <<endl;
    /* fusion des salles */
    Salle s3=((*carte)[sp1i]).fusionner_salle((*carte)[sp2i]);
    cout << "fin fusion 1" <<endl;
    s3=s3.fusionner_salle(couloir);

    cout << "fin fusion" <<endl;
    carte->erase(carte->begin() + max(sp1i,sp2i));
    carte->erase(carte->begin() + min(sp1i,sp2i));
    carte->push_back(s3);

    cout << "fin fct" <<endl;

}
