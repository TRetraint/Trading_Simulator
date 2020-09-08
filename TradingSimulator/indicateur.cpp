#include "indicateur.h"

//Méthode permettant l'ajout d'indicateurs dans un objet EvolutionIndicateurs
void EvolutionIndicateurs::addIndicateur(double v,const QDate& d) {
        if (nbMaxIndicateurs == nbIndicateurs) { // agrandissement du tableau
                Indicateur* newtab = new Indicateur[nbMaxIndicateurs + 100];
                for (unsigned int i = 0; i < nbIndicateurs; i++) newtab[i] = indicateurs[i];
                // mise � jour des attributs
                Indicateur* old = indicateurs;
                indicateurs = newtab;
                nbMaxIndicateurs += 100;
                delete[] old; // destruction de l'ancien tableau
        }
        // ajout du Indicateurs
        indicateurs[nbIndicateurs].setValue(v);
    indicateurs[nbIndicateurs].setDate(d);
        nbIndicateurs++;}

//Destructeur de EvolutionIndicateurs
EvolutionIndicateurs::~EvolutionIndicateurs() {
        delete[] indicateurs;
}

//Méthode calculant le RSI pour tous les cours de l'argument cours
void RSI::Calcul(EvolutionCours *cours){
    EvolutionCours::iterator itCours;
    EvolutionIndicateurs::iterator itIndicateurs;
    unsigned int k=0; //compte à quel indicateur nous sommes
    double increase=0;
    double decrease=0;
    double color;
    itIndicateurs=this->begin();
    for(unsigned int j=0;j<nbIndicateurs;j++){  //pour chaque bougie on associe un indicateur
            itCours=cours->begin()+lookback+j;
            for(unsigned int i=0;i<lookback;i++){   //on boucle sur la periode a etudie (14 par defaut)

               color=itCours->getClose()-itCours->getOpen();   //on prends l'ouverture et la fermuture du cours
               if(color>0){
                   increase+=color;
               }
               else{
                   decrease+=-color;
               }
               itCours--;
               color=0;
            }
            increase=increase/lookback;      //moyenne des hausse sur la période
            decrease=decrease/lookback;      //moyenne des baisses sur la période
            itIndicateurs->setValue(100-(100/(1+(increase/decrease))));     //formule du RSI, on l'ajoute a l'iterator
           itIndicateurs++;
           increase=0;
           decrease=0;
        }

    }

//Méthode calculant l'EMA pour tous les cours de l'argument cours
double* EMA::Calcul(EvolutionCours *cours,unsigned int period){
    EvolutionCours::iterator itCours;
    EvolutionIndicateurs::iterator itIndicateurs;
    itIndicateurs=this->begin();
    double alpha=(2.0/(period+1));    //calcul de alpha, coefficient de l'EMA associé
    double moypred;
    double moy;
    double *Tableau=new double[nbIndicateurs];      //tableau pour ensuite calculé le MACD
    for(unsigned int j=0;j<nbIndicateurs;j++){    //pour chaque bougie on associe un indicateur
        itCours=cours->begin()+j;
        if(j==0){
            moypred=0;
        }
           moy=(alpha*itCours->getClose())+((1-alpha)*moypred);         //calcul de l'EMA
           itIndicateurs->setValue(moy);                    //ajoute la valeur a l'iterator
           Tableau[j]=moy;
           moypred=moy;                         //on passe a la bougie suivante...
           itIndicateurs++;
    }
    return Tableau;
}

//Méthode calculant le MCAD pour tous les cours de l'argument cours
void MACD::Calcul(EvolutionCours* cours,EMA* ema1,EMA* ema2){
    EvolutionCours::iterator itCours;
    EvolutionIndicateurs::iterator itIndicateurs;
    double* TabEma1=new double[ema1->getNbIndicateurs()];
    double* TabEma2=new double[ema2->getNbIndicateurs()];
    TabEma1 = ema1->Calcul(cours,shortPeriod);   //on recupère les valeurs de calculs d'EMA 12 et 26
    TabEma2 = ema2->Calcul(cours,longPeriod);
    itIndicateurs = this->begin();
    for(unsigned int j=0;j<ema1->getNbIndicateurs();j++){
        itCours=cours->begin()+j;
        itIndicateurs->setValue(TabEma1[j]-TabEma2[j]);         //on les soustraits pour avoir le MACD
        itIndicateurs++;
    }
    delete [] TabEma1;   //on libère la mémoire
    delete [] TabEma2;
}
