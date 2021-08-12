#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<fstream>
#include<sstream>
using namespace std ;
struct occurrence
{
    string mot;
    int nbrOccur ;
};
struct stats
{
     string mot ;
     string chemin;
     int nombreOccurence;
};
bool Exist(string mot ,vector<string> requete)
{
    for(int i=0;i<requete.size();i++)
    {
        if(requete[i]==mot)
            return true;
    }
    return false;
}
bool notexiste(vector<occurrence> l,string ch)
{
    for(int i=0;i<l.size();i++)
    {

       if(l[i].mot==ch)
          return false;
    }
    return true;
}
vector<stats>  convMapVect(map<string,vector<occurrence> > m)
{    stats tmp;
     vector <stats> v(0);
int i=0;

    for( map<string,vector<occurrence> >::iterator it = m.begin() ; it != m.end(); ++it) {
            for(int i=0;i<m[it->first].size();i++)
            {
                 tmp.mot=it->first ;
                tmp.chemin=it->second[i].mot ;
                tmp.nombreOccurence= it->second[i].nbrOccur ;

            }


    }
    return v;
}
 map<string,vector<occurrence> > convVectMap( vector<stats> vect)
 {
     int i , n;
     map<string,vector<occurrence> > mapOccur;
     n=vect.size();
     for(i=0;i<n;i++)
     {
         occurrence occ;
         occ.mot=vect[i].chemin;
         occ.nbrOccur=vect[i].nombreOccurence;
         mapOccur[vect[i].mot].push_back(occ);
     }
     return mapOccur;
 }


//********classe lecture*********
class lecture
{

public:

    vector<string> lire(fstream &f)
    {
        vector<string> vectMot(0);
        if(f.is_open())
        {
            while(!f.eof())
            {
                string mot;
                f>>mot;
                if(mot!="")
                {
                    vectMot.push_back(mot);
                }
            }
            f.close();
        }
        else
            cout<<"erreur de fichier";
    return vectMot;
    }
};

//********class analyseur***********
bool notExist(string chaine,vector<occurrence> &vect)
{
    int i;
    for(i=0;i<vect.size();i++)
    {
        if(vect[i].mot==chaine)
            return false ;
    }
    return true ;

};



int strlen(string s)
{
    int i;
    i=0;
    while(s[i]!='\0')
        ++i;
    return i;
    }
string RendreToutLesMotsMin(string mot) /*fonction optionnelle pour diminuer le risque de trouver les mêmes mots dont la différence se trouve au niveau du majusc ou  min des lettres */
{
    int i,j;
   vector<char> vect(strlen(mot));
    for(j=0;j<strlen(mot);j++)
    {
        vect.at(j)=mot[j];
    }
    for(i=0;i<strlen(mot);i++)
    {
      if( vect.at(i) >= 'A' && vect.at(i) <= 'Z' )
            mot[i]=mot[i]+32;
    }
  return mot;
}





class Analyseur
{
public:
 virtual vector<occurrence> analyser(vector<string> text)=0;
};
class AnalyseurVector: public Analyseur
{

public:

    vector<occurrence> analyser(vector<string> text)
         {
        int n;
        n=text.size();
        vector<occurrence> vectOccur(1);
        int i,j;
        int k=-1;

         for(i=0;i<n;i++)
        {  string ch1=RendreToutLesMotsMin(text[i]);
            if(notExist(ch1,vectOccur))
            {
                k++;
                vectOccur.resize(k+1);



                vectOccur[k].mot=ch1;
                vectOccur[k].nbrOccur=0;

               for(j=0;j<text.size();j++)
                {  string ch2=RendreToutLesMotsMin(text[j]);
                    if(ch1==ch2)
                    {
                        vectOccur[k].nbrOccur++;
                    }
                }
            }

        }
        if(k==-1)
        {
            cout<<"le texte est vide!!!!";
        }
        return vectOccur;

    }
};
class AnalyseurMap:public Analyseur
{
public:

     vector<occurrence> analyser(vector<string> text)
    {


        int i,n;
        n=text.size();
        map<string,int>mapOccur;
        for(i=0;i<n;i++)
        {  string ch1=RendreToutLesMotsMin(text[i]);
            if(mapOccur.find(ch1)!=mapOccur.end())
            {
                mapOccur[ch1]++;
            }
            else
            {
                mapOccur.insert(std::pair<string,int>(ch1,1));
            }
        }
//copier les elements de la map dans vector d'occurrence
    vector<occurrence> vectOccur(0);
    for(map<string,int>::iterator it=mapOccur.begin();it!=mapOccur.end();it++)
    {
    occurrence occ ;
    occ.mot=it->first;
    occ.nbrOccur=it->second;
    vectOccur.push_back(occ);
    }




return vectOccur;



}



};

//*************class index**************
class index
{

public:
    index(){};
    vector<stats> ind;
      vector<stats> getInd()const
       {
           return ind;
       }
       void setindex(vector<stats> v)
       {
           ind=v;
       }

   virtual void indexer(string,int,string)=0;
   virtual ~index(){};

};

class indexVector :public index
{
     public:


      indexVector():index(){};


      void indexer(string chemin,int nombreOccurence,string mot)
      { int i=0;
         while(i<ind.size())
          {
              if (ind[i].chemin==chemin && ind[i].mot==mot)
              {
                 ind[i].nombreOccurence+=nombreOccurence;
                 break;

              }
              i++;

          }
          if(i==ind.size())
               {
                  stats stat;
                  stat.chemin=chemin;
                  stat.mot=mot;
                  stat.nombreOccurence=nombreOccurence;
                  int n =ind.size();
                  ind.resize(n+1);
                  ind[n]=stat;
               }
      }

};
class indexMap: public index
{
public:
    indexMap():index(){};






    void indexer(string chemin ,int nbOccur,string chaine)
    {
        map<string,vector<occurrence> > indMap=convVectMap( getInd());
         int k=0;
        if(indMap.find(chaine)!=indMap.end())
        {

            while(k<indMap[chaine].size())
            {
               if(indMap[chaine][k].mot==chemin)
               {
                 indMap[chaine][k].nbrOccur+=nbOccur;
                   break;

               }
               k++;

            }
            if(k>=indMap[chaine].size())
            {
                indMap[chaine].resize(k+1);
                indMap[chaine][k].mot=chemin;
                indMap[chaine][k].nbrOccur=nbOccur;
            }
        }
        else
        {
            occurrence occ;
            occ.mot=chemin;
            occ.nbrOccur=nbOccur;
            vector<occurrence>vect(1);
            vect[0]=occ;
          indMap.insert(std::pair<string,vector<occurrence> >(chaine,vect));
        }
        ind=convMapVect(indMap);

    }

    ~indexMap(){};

};


//*******class persistance******

class persistance
{



public:


    void enregistrer(vector<stats> vect,fstream &f)
    {
        if(!f.is_open())
            cout<<"impossible d ouvrir le fichier en ecriture";
        else
        {
          for(int i=0;i<vect.size();i++)
          {
              f<<vect[i].chemin<<"\t"<<vect[i].mot<<"\t"<<vect[i].nombreOccurence<<endl;
          }
        }
    }

    void recuperer(vector<stats> &vect,fstream &f)
    {
       if(!f.is_open())
          cout<<"impossible d ouvrir le fichier en lecture"<<endl;
       else
       {
            while(!f.eof())
            {
                string mot;
                stats sta;


                f>>mot;
               sta.chemin=mot;



                f>>mot;

                sta.mot=mot;



                 f>>mot;

                 istringstream (mot)>>sta.nombreOccurence;
                   if(f.eof())
                 {
                     break;
                 }


                 vect.push_back(sta);

            }
            f.close();

       }
    }
};

//********class ordonnanceur*******
class ordonnanceur
{
public:

  virtual vector<occurrence> ordonnancer(vector<stats> & ,vector<string>)=0;
};


class ordonnanceurBinaire : public ordonnanceur
{


  public:

vector<occurrence> ordonnancer(vector<stats> &vect,vector<string> v)
{   vector<occurrence> l(0);
    occurrence tmp;
    string ch;
    for(int i=0;i<vect.size();i++ )
    {for(int j=0;j<v.size();j++)
    {
        if(vect[i].mot==v[j])
        { if(notexiste(l,vect[i].chemin)==true)
           {
               tmp.mot= vect[i].chemin;
               tmp.nbrOccur=vect[i].nombreOccurence;
              l.push_back(tmp);
           }


          else
          {
              for(int k=0;k<l.size();k++)
              {
                  if(l[k].mot==vect[i].chemin)
                       l[k].nbrOccur+=vect[i].nombreOccurence;

              }
          }
        }
     }
    }



    return l;

}




};



class ordonnanceurOccur:public ordonnanceur
{

  public:
   vector<occurrence> ordonnancer(vector<stats> &vect,string requete)
      {
          cout<<"appelle";
          map<string,vector<occurrence> > mapDeStat=convVectMap(vect);
              vector<occurrence> vectOccur= mapDeStat[requete];
             if(mapDeStat.count(requete)>0)
             {

                 int k=vectOccur.size();
                 cout<<"taille"<<k;
               occurrence tmp ;

                 for(int i=0;i<k;i++) //on va trier le vecteur par nbre d occurrence
                 {
                     for(int j=i+1;j<k;j++)
                     {
                         if(vectOccur[j].nbrOccur>vectOccur[i].nbrOccur)
                         {
                             tmp.mot=vectOccur[i].mot;
                             tmp.nbrOccur=vectOccur[i].nbrOccur;
                             vectOccur[i]=vectOccur[j];
                            vectOccur[j]=tmp;

                         }
                     }
                 }



           }
      return vectOccur;
    }
    vector<occurrence> ordonnancer(vector<stats>  &vectDeStat,vector<string> requete)
{
    vector<occurrence> fichiers(0);
    for(int i=0;i<vectDeStat.size();i++) //le remplissage de vecteur avec tous les chemins des fichiers existant dans l index
    {
        if(notExist(vectDeStat[i].chemin,fichiers))
        {
            occurrence occ;
            occ.mot=vectDeStat[i].chemin;
            occ.nbrOccur=0;
            fichiers.push_back(occ);
        }
    }
    for(int i=0;i<fichiers.size();i++)
    {
        for(int j=0;j<vectDeStat.size();j++)
        {
            if(vectDeStat[j].chemin==fichiers[i].mot)
                {


                if((Exist(vectDeStat[j].mot,requete)))
                  {
                        fichiers[i].nbrOccur+=vectDeStat[j].nombreOccurence;
                  }
                }
        }
    }
      int k=fichiers.size();
      occurrence tmp ;

     for(int i=0;i<k;i++) //on va trier le vecteur par nbre d occurrence
            {
              for(int j=i+1;j<k;j++)
                  {
                         if(fichiers[j].nbrOccur>fichiers[i].nbrOccur)
                         {
                             tmp.mot=fichiers[i].mot;
                             tmp.nbrOccur=fichiers[i].nbrOccur;
                             fichiers[i]=fichiers[j];
                            fichiers[j]=tmp;

                         }
                     }
            }




      return fichiers;

}


};
//********class moteur********
class moteur
{
public:
    Analyseur* monAnalyseur;
     index* monIndex;
     ordonnanceur* monOrdonanceur;
     moteur( Analyseur* a,index*i,ordonnanceur* o)
     {
         monAnalyseur=a;
         monIndex=i;
         monOrdonanceur=o;
     }

     void indexerMoteur()
     {
                     string nom;
            cout<<"donner le chemin de fichier à indexer"<<endl;
            cin>>nom;
             const char* nomFichier;
            nomFichier=new char;
            nomFichier=nom.c_str();
            fstream fichier(nomFichier);

            lecture L ;
            vector<string> vect(0);
            vect=L.lire(fichier);

            vector<occurrence> vecteuroccur(0);

            vecteuroccur= this->monAnalyseur->analyser(vect);

            cout<<vecteuroccur.size()<<endl;
            for(int i=0;i<vecteuroccur.size();i++)
             {
                  cout<<"le mot "<<vecteuroccur[i].mot<<" existe "<<vecteuroccur[i].nbrOccur<<" fois"<<endl;

             }


            for(int i=0;i<vecteuroccur.size();i++)
            {

                this->monIndex->indexer(nom,vecteuroccur[i].nbrOccur,vecteuroccur[i].mot);



            }




     }


   void rechercher()
   {
                  string mot;
            vector<string> requete(0);
            cout<<"donner votre requete de recherche mot par mot et taper f pour marquer la fin \n";
            do{
                cin>>mot;
                requete.push_back(mot);
            }while(mot!="f");
          vector<occurrence> vectfichiers(0);
          vector<stats> v =this->monIndex->getInd();

          vectfichiers=this->monOrdonanceur->ordonnancer(v, requete);

          for(int i=0;i<vectfichiers.size();i++)
           {
              cout<<vectfichiers[i].mot<<"\n";


           }
   }

};
int main()
{




    cout<<"******************************************BIENVENUE--MOTEUR--DE--RECHERCHE**********************************************"<<endl;
    //choix de l analyseur du moteur
    int choixUser;
    Analyseur* a;
    cout<<"******************taper 1 pour utiliser l'analyseur vector******************"<<endl;
    cout<<"******************taper 2 pour utiliser l'analyseur map*********************"<<endl;
    cout<<"***************VOTRE CHOIX D'Analyseur : ";
    cin>>choixUser;
    switch (choixUser)
    {
    case 1:
        {
           AnalyseurVector a1;
           a=&a1;
           break;
        }
    case 2:
        {
             AnalyseurMap a2;
             a=&a2;
            break;
        }
      default:
        cout<<"******************VOTRE CHOIX EST INCORRECT , TAPEZ 1 ou 2 S'IL VOUS PLAIT****************************"<<endl;
        cout<<"***************VOTRE CHOIX D'Analyseur : ";
        cin>>choixUser;
        break;
    }





 //choix de l indexeur du moteur
    int choix;
    index* i;
    cout<<"\n******************tapez 1 pour utiliser l'indexeur vector******************"<<endl;
    cout<<"******************tapez 2 pour utiliser l'indexeur map*********************"<<endl;
    cout<<"***************VOTRE CHOIX D'INDEXEUR : ";
    cin>>choix;
    switch (choix)
    {
    case 1:
        {
          i=new  indexVector ;


           break;
        }
    case 2:
        {
          i=new indexMap;
            break;
        }
       default:
        cout<<"***********************VOTRE CHOIX EST INCORRECT , TAPEZ 1 ou 2 S'IL VOUS PLAIT ****************************"<<endl;
         cout<<"***************VOTRE CHOIX D'INDEXEUR : ";
        cin>>choix;
        break;
    }




//choix de l ordonnanceur du moteur
    int Lechoix;
    ordonnanceur* o;
    cout<<"\n******************tapez 1 pour utiliser l'ordonnanceur binaire *********************"<<endl;
    cout<<"******************tapez 2 pour utiliser l'ordonnaceur occurence*********************"<<endl;
    cout<<"***************VOTRE CHOIX D'ODONNANCEUR : ";
    cin>>Lechoix;
    switch (Lechoix)
    {
    case 1:
        {
           o=new ordonnanceurBinaire;
           break;
        }
    case 2:
        {
            o=new ordonnanceurOccur;
            break;
        }
       default:
        cout<<"***********************VOTRE CHOIX EST INCORRECT , TAPEZ 1 ou 2 S'IL VOUS PLAIT ****************************"<<endl;

         cout<<"***************VOTRE CHOIX D'ODONNANCEUR : ";
        cin>>Lechoix;
        break;
    }


moteur M (a,i,o);
    //restaurer l’index à l’ouverture de l’application
    fstream f("enregistrement.txt");
   persistance p;

    p.recuperer(M.monIndex->ind,f);
    f.close();




   int c , ch;
   int Choix;
   do{ cout<<"******************************************************MENU********************************************************"<<endl;
       cout<<"-/--/---/----/-----/------/------/-------/le choix 1 indexation -/--/---/----/-----/------/------/-------/\n";
       cout<<"-/--/---/----/-----/------/------/-------/le choix 2  recherche -/--/---/----/-----/------/------/-------/\n";
       cout<<"-/--/---/----/-----/------/------/-------/le choix 3 configuration -/--/---/----/-----/------/------/-------/\n";
       cout<<"-/--/---/----/-----/------/------/-------/le choix 0 pour quitter -/--/---/----/-----/------/------/-------/\n";
       cout<<"-/--/---/----/-----/------/------/-------/donner un entier entre 0 et 3 -/--/---/----/-----/------/------/-------/\n";
       cout<<"-/--/---/----/-----/------/------/-------/VOTRE CHOIX EST: ";
       cin>>Choix;
       switch(Choix)
       {
       case 1:
        {
        do{

            cout<<"|=|==|===|====|=====|======| taper 1 pour indexer un fichier |=|==|===|====|=====|======|"<<endl;
            cout<<"|=|==|===|====|=====|======|taper 2 pour indexer plusieurs fichiers |=|==|===|====|=====|======|"<<endl;
            cout<<"|=|==|===|====|=====|======|taper 3 pour supprimer un fichier de l'index |=|==|===|====|=====|======|"<<endl;
            cout<<"|=|==|===|====|=====|======|taper 4 pou supprimer l'index|=|==|===|====|=====|======|"<<endl;
            cout<<"|=|==|===|====|=====|======|taper 5 pour afficher l index |=|==|===|====|=====|======|"<<endl;
            cout<<"|=|==|===|====|=====|======|taper 6 pour afficher des statistiques sur l index |=|==|===|====|=====|======|"<<endl;
            cout<<"|=|==|===|====|=====|======|taper 7 pour retourner au menu principal |=|==|===|====|=====|======|"<<endl;
            cout<<"|=|==|===|====|=====|======|donner un entier entre 1 et 7"<<endl;
            cin>>c;
            switch(c)
            {
            case 1:
                {
                   M.indexerMoteur();
                   break;
                }
            case 2:
                {
                    int j;
                    cout<<"************************donner le nombre des fichiers à indexer**************************"<<endl;
                    cout<<"************************VOTRE REPONSE EST: ";
                    cin>>j;
                    for(int k=0;k<j;k++)
                    {
                        M.indexerMoteur();
                    }
                    break;
                }
            case 3:
                {


                stats tmp;
                string N;
                cout<<"/*/*/*/*/*/*/*/*/*/*/*/*/*/*/ donner le nom de fichier /*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/*/"<<endl;
                cout<<"/*/*/*/*/*/*/*/*/*/*/*/*/*/*/ VOTRE REPONSE EST: ";
                cin>>N;

                vector<stats> nv(0);
                vector<stats> v(0);
                v=M.monIndex->getInd();
                for(int i=0;i<v.size();i++)
                {
                    if(v[i].chemin!=N)
                    {
                        tmp.chemin=v[i].chemin;
                        tmp.mot=v[i].mot;
                        tmp.nombreOccurence=v[i].nombreOccurence;
                        nv.push_back(tmp);
                    }

                }
                M.monIndex->setindex(nv);
                break;


            }
            case 4:
                {
                    string rep;
                    cout<<"vous voulez vraiment supprimer l index O/N !!!!!"<<endl;
                    cin>>rep;
                    if(rep=="O")
                    {
                           vector<stats> v(0);
                           M.monIndex->setindex(v);
                           cout<<"l'index est supprimé"<<endl;

                    }

                    break;
                }
            case 5:

               {
                          vector<stats> vectInd(0);
                         vectInd=M.monIndex->getInd();
                         cout<<vectInd.size();

                        for(int i=0;i<vectInd.size();i++)

                            {


                                    cout<<"mot:  "<<vectInd[i].mot;
                                    cout<<"     chemin:  ";
                                    cout<<vectInd[i].chemin;
                                    cout<<"     NombreDeOccurence:  ";
                                    cout<<vectInd[i].nombreOccurence<<endl;
                            }

                      break;
               }
            case 6:
                int  nbMotsTotal=0;
                vector<string> vecteurFichier(0);
                vector<string> vecteurMot(0);
                vector<stats> vect(0);
                vect=M.monIndex->getInd();
                cout<<"voici les statistiques sur l'index"<<endl;
                for(int i=0;i<vect.size();i++)
                {
                   if(!Exist(vect[i].mot,vecteurMot))
                   {
                       vecteurMot.push_back(vect[i].mot);
                   }
                }

                for(int i=0;i<vect.size();i++)
                {

                   if(!Exist(vect[i].chemin,vecteurFichier))
                   {
                       vecteurFichier.push_back(vect[i].chemin);
                   }

                }
                for(int i=0;i<vect.size();i++)
                {
                    nbMotsTotal+=vect[i].nombreOccurence;
                }

                cout<<vecteurFichier.size()<<"documents    "<<"  ,   "<<vecteurMot.size()<<"mots distincts    "<<"  ,  "<<nbMotsTotal<<"mots en total"<<endl;
            }











            } while(c!=7);
             break;
        }
       case 2 :
        {

            M.rechercher();
           break;
           //NZIDOU FAZET TEMPS DE REPONSE LENNA
        }
       case 3:
           {
            do{
               cout<<"/*/**/***/****/*****/******/ taper 1 pour changer l'analyseur /*/**/***/****/*****/******/"<<endl;
               cout<<"/*/**/***/****/*****/******/ taper 2 pour changer l'index /*/**/***/****/*****/******/"<<endl;
               cout<<" /*/**/***/****/*****/******/ taper 3 pour changer l'ordonnanceur /*/**/***/****/*****/******/"<<endl;
               cout<<"/*/**/***/****/*****/******/ taper 4 pour retouner au menu principal /*/**/***/****/*****/******/"<<endl;
               cout<<"/*/**/***/****/*****/******/VOTRE CHOIX EST : ";
               cin>>ch;
               switch(ch)
               {
               case 1:
                {
                        //choix de l analyseur du moteur

                        cout<<"******************taper 1 pour utiliser l'analyseur vector******************"<<endl;
                        cout<<"******************taper 2 pour utiliser l'analyseur map*********************"<<endl;
                        cout<<"***************VOTRE CHOIX D'Analyseur : ";
                        cin>>choixUser;
                        switch (choixUser)
                            {
                                case 1:
                                    {
                                        AnalyseurVector a1;
                                        a=&a1;
                                           break;
                                    }
                                case 2:
                                    {
                                         AnalyseurMap a2;
                                         a=&a2;
                                        break;
                                    }
                                  default:
                                    {




                                        cout<<"***********************VOTRE CHOIX EST INCORRECT , TAPEZ 1 ou 2 S'IL VOUS PLAIT ****************************"<<endl;

                                        cout<<"***************VOTRE CHOIX D'ANALYSEUR : ";
                                        cin>>choixUser;
                                        break;
                                    }
                            }





                            break;
                    }
                    case 2:

                                    {
                                         cout<<"\n******************tapez 1 pour utiliser l'indexeur vector******************"<<endl;
                                            cout<<"******************tapez 2 pour utiliser l'indexeur map*********************"<<endl;
                                            cout<<"***************VOTRE CHOIX D'INDEXEUR : ";
                                        cin>>choix;
                                        switch (choix)
                                        {
                                        case 1:
                                            {
                                              i=new  indexVector ;


                                               break;
                                            }
                                        case 2:
                                            {
                                              i=new indexMap;
                                                break;
                                            }
                                           default:
                                        {


                                            cout<<"***********************VOTRE CHOIX EST INCORRECT , TAPEZ 1 ou 2 S'IL VOUS PLAIT ****************************"<<endl;

                                            cout<<"***************VOTRE CHOIX D'INDEXEUR : ";
                                            cin>>choix;
                                            break;
                                        }
                                        }
                                    break;
                                }
                    case 3 :
                        {
                                        cout<<"\n******************tapez 1 pour utiliser l'ordonnanceur binaire *********************"<<endl;
                                        cout<<"******************tapez 2 pour utiliser l'ordonnaceur occurence*********************"<<endl;
                                        cout<<"***************VOTRE CHOIX D'ODONNANCEUR : ";
                                        cin>>Lechoix;
                                        switch (Lechoix)
                                        {
                                        case 1:
                                            {
                                               o=new ordonnanceurBinaire;
                                               break;
                                            }
                                        case 2:
                                            {
                                                o=new ordonnanceurOccur;
                                                break;
                                            }
                                           default:
                                            {


                                            cout<<"***********************VOTRE CHOIX EST INCORRECT , TAPEZ 1 ou 2 S'IL VOUS PLAIT ****************************"<<endl;

                                            cout<<"***************VOTRE CHOIX D'ODONNANCEUR : ";
                                            cin>>Lechoix;
                                            break;
                                            }
                                    }

                        break;
                  }

               }
            }while(ch!=4);
            break;
           }







    case 0 :
            {
              fstream f("enregistrement.txt");
           p.enregistrer(M.monIndex->ind,f);
            break;

           }
       default:
        {


        cout<<"***********************VOTRE CHOIX EST INCORRECT , TAPEZ 0,1,2 ou 3 S'IL VOUS PLAIT ****************************"<<endl;
        cout<<"************************VOTRE CHOIX : ";
        cin>>Choix;
        break;
       }
    }





}while(Choix!=0);

}

