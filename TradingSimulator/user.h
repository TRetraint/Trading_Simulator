#ifndef UTILISATEUR_H
#define UTILISATEUR_H

#include"includes.h"

//Classe représentant une transaction, elle est représentée par un montant en devise de base et en devise de contrepartie (pouvant être négatifs ou positifs) et par une date
class Transaction{
    double montantDeviseBase;
    double montantDeviseContrepartie;
    QDate date;
public:
    Transaction(){} // pour constructeur []
    Transaction(double base, double contrepartie, QDate d): montantDeviseBase(base), montantDeviseContrepartie(contrepartie), date(d){}
    ~Transaction(){}
    void setBase(double base){montantDeviseBase=base;}
    void setContrepartie(double contrepartie){montantDeviseContrepartie=contrepartie;}
    void setDate(QDate d){date=d;}
    double getBase(){return montantDeviseBase;}
    double getContrepartie(){return montantDeviseContrepartie;}
    bool getType(){if(montantDeviseBase<0){return true;} return false;}
    QDate getDate(){return date;}

    //Méthode permettant de retourner une QString décrivant la transaction, utilisée pour afficher les transactions dans transactionsviewer
    QString toString(bool type){
        QString string;
        if(type){
        string.append("Achat de ");
        string.append(QString::number(montantDeviseContrepartie));
        string.append(" devise de contrepartie contre ");
        string.append(QString::number(-montantDeviseBase));
        string.append(" devise de base le ");
        string.append(date.toString());
        }
        else{
            string.append("Vente de ");
            string.append(QString::number(-montantDeviseContrepartie));
            string.append(" devise de contrepartie contre ");
            string.append(QString::number(montantDeviseBase));
            string.append(" devise de base le ");
            string.append(date.toString());
        }
    return string;
    }

};

//Classe représentant la série de transactions de la simulation en cours, elle est dotée d'un itérator sur la classe Transaction
class EvolutionTransactions{
protected:
    Transaction* transactions = nullptr;
    unsigned int nbTransactions = 0;
    unsigned int nbMaxTransactions = 0;
public:
    EvolutionTransactions(){}
    void addTransaction(double base,double contrepartie,QDate d);
    void deleteLastTransaction();
    ~EvolutionTransactions();
    unsigned int getNbTransactions() const { return nbTransactions; }
    using iterator = Transaction*;
    iterator begin() { return iterator(transactions); }
    iterator end() { return iterator(transactions + nbTransactions); }
    using const_iterator = const Transaction*;
    const_iterator begin() const { return transactions; }
    const_iterator end() const { return transactions + nbTransactions; }
    const_iterator cbegin() const { return transactions; }
    const_iterator cend() const { return transactions + nbTransactions; }

};





//Classe représenant l'Utilisateur, elle contient les différents montants de devises ainsi que la liste des transactions effectuées
class User{
private:
    double montantDeviseBase;
    double montantDeviseContrepartie;
    double montantDeviseBaseDebut; //contient le montant présent au départ de la simulation pour calculer ROI
    double montantDeviseContrepartieDebut;
    double retourSurInvestissement;
    EvolutionTransactions* evolutionTransactions;
public:
    //Constructeur de la classe User
    User(double base=0, double contrepartie=0){
        evolutionTransactions=new EvolutionTransactions;
        montantDeviseBase=base;
        montantDeviseContrepartie=contrepartie;
    }

    EvolutionTransactions* getEvolution(){return evolutionTransactions;}
    void setMontantBase(double base){
        montantDeviseBase=base;
        if(evolutionTransactions->getNbTransactions()>0){
        }
    }
    void setMontantContrepartie(double contrepartie){
        montantDeviseContrepartie=contrepartie;
        if(evolutionTransactions->getNbTransactions()>0){
        }
    }

    void setMontantBaseDebut(double base){
        montantDeviseBaseDebut=base;
    }
    void setMontantContrepartieDebut(double contrepartie){
        montantDeviseContrepartieDebut=contrepartie;

    }
    //Méthode permettant de calculer le ROI, elle est appelé quand on change les montants de devises de l'utilisateur
    void updateRetourSurInvestissement(){
        retourSurInvestissement=((montantDeviseContrepartie-montantDeviseContrepartieDebut)/montantDeviseContrepartieDebut)*100;
    }
    double getRetourSurInvestissement(){
        return retourSurInvestissement;
    }

    double getMontantBase(){
        return montantDeviseBase;
    }
    double getMontantContrepartie(){
        return montantDeviseContrepartie;
    }
    double getMontantBaseDebut(){
        return montantDeviseBaseDebut;
    }
    double getMontantContrepartieDebut(){
        return montantDeviseContrepartieDebut;
    }

    //Méthode permettant de modéliser l'action d'achat, elle change les montants de devises de l'utilisateur et créé une transaction, retourne true si l'achat a réussi, false sinon
    bool buy(double contrepartie, double taux,QDate date);


    //Méthode permettant de modéliser l'action de vente, elle change les montants de devises de l'utilisateur et créé une transaction, retourne true si l'achat a réussi, false sinon
    bool sell(double contrepartie, double taux,QDate date);

    //Méthode appelé lorsque l'on souhaite revenir en arrière, elle rétablit les transaction postérieures à la date donnée en argument, ré-actualisant les montants de devises de l'utilisateur
    void update(QDate d);
};

#endif // UTILISATEUR_H
