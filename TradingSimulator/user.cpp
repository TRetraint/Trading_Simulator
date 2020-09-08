#include "user.h"

//Méthode permettant d'ajouter une transaction dans un objet EvolutionTransactions
void EvolutionTransactions::addTransaction(double base, double contrepartie, QDate d) {
        if (nbMaxTransactions == nbTransactions) { // agrandissement du tableau
                Transaction* newtab = new Transaction[nbMaxTransactions + 100];
                for (unsigned int i = 0; i < nbTransactions; i++) newtab[i] = transactions[i];
                // mise � jour des attributs
                Transaction* old = transactions;
                transactions = newtab;
                nbMaxTransactions += 100;
                delete[] old; // destruction de l'ancien tableau
        }
        // ajout du Indicateurs
        transactions[nbTransactions].setBase(base);
        transactions[nbTransactions].setContrepartie(contrepartie);
    transactions[nbTransactions].setDate(d);
        nbTransactions++;}

//Permet de supprimer la dernière transaction effectuée par l'utilisateur, utilisée dans la méthode update de user pour revenir en arrière
void EvolutionTransactions::deleteLastTransaction(){
    //delete (end()-1);
    nbTransactions--;

}



EvolutionTransactions::~EvolutionTransactions() {
        delete[] transactions;
}


bool User::buy(double contrepartie, double taux,QDate date){
    if(contrepartie*taux > montantDeviseBase){
        return false;
    }
    else{
          montantDeviseBase = montantDeviseBase-contrepartie*taux;
          montantDeviseContrepartie += contrepartie;
          evolutionTransactions->addTransaction(-contrepartie*taux,contrepartie,date);
          updateRetourSurInvestissement();
          return true;
    }
}

bool User::sell(double contrepartie, double taux,QDate date){
    if(contrepartie > montantDeviseContrepartie){
        return false;
    }
    else{
          montantDeviseContrepartie  =montantDeviseContrepartie - contrepartie;
          montantDeviseBase += contrepartie*taux;
          evolutionTransactions->addTransaction(contrepartie*taux,-contrepartie,date);
          updateRetourSurInvestissement();
          return true;

    }
}

void User::update(QDate d){
    if(evolutionTransactions->getNbTransactions()>0){
    EvolutionTransactions::iterator it=evolutionTransactions->end()-1;
    while(it->getDate()>d && it!=evolutionTransactions->begin()-1){
        //On rétablit l'état avant la transaction
        montantDeviseContrepartie-=it->getContrepartie();
        montantDeviseBase-=it->getBase();
        updateRetourSurInvestissement();
        evolutionTransactions->deleteLastTransaction();
        it--;
    }
    }
}
