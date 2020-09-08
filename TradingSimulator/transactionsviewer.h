#ifndef TRANSACTIONSVIEWER_H
#define TRANSACTIONSVIEWER_H
#include "includes.h"
#include "user.h"
#include"trading.h"

/*Remarque les achat se font toujours par 1000 en mode automatique.*/

//Classe représentant la seconde fenetre du programme, permettant de gérer les transactions, aussi bien en mode automatique qu'en mode manuel
class TransactionsViewer : public QWidget{
    Q_OBJECT
    //Tableau de pointeurs sur QLineEdit, qui permet d'afficher les transactions effectuées, sera utilisé pour l'itérateur
    QLineEdit** transactions=nullptr;
    unsigned int nbTransactions = 0;
    unsigned int nbMaxTransactions = 0;

    //Contient les différents QLineEdit
    QVBoxLayout* listeTransactions;

    //Permettent d'accéder aux données des cours afin de récupérer le taux de change du jour courant lors de l'achat ou de la vente de devises
    EvolutionCours* cours;
    int* last;

    //Permet d'accéder aux montants de l'utilisateur pour savoir s'il peut acheter ou vendre, et pour mettre à jour ces données
    User* user;

    //bouttons pour acheter et vendre
    QPushButton* buttonBuy;
    QPushButton* buttonSell;

    QPushButton* auto_begin;    //Permet de lancer une simulation automatique

    bool mode_choisit = false;  //true :Automatique / false :manuel

    QComboBox* selection;   //Permet de selectionner la fontion automatique a utiliser
    QComboBox* nombre_jour; //En mode auto : permet de choisir le nombre de transaction a faire ( 1 apr jour).

    //Permettent d'afficher les montants de devises possédés par l'utilisateur
    QLabel* montantBaseOwnedMessage;
    QLineEdit* montantBaseOwned;
    QLabel* montantContrepartieOwnedMessage;
    QLineEdit* montantContrepartieOwned;
    QLabel* montantTotalOwnedMessage; //ROI
    QLineEdit* montantTotalOwned;

    QLabel* montantBaseMessage;
    QLineEdit* montantBase;
    QVBoxLayout* baseTotal;
    QLabel* montantContrepartieMessage;
    QLineEdit* montantContrepartie;
    QVBoxLayout* contrepartieTotal;
    QHBoxLayout* form;
    QVBoxLayout* fenetre;
    QWidget* window;
    public:
    TransactionsViewer(EvolutionCours* c, User* u,int* l);
    unsigned int getNbTransactions() const { return nbTransactions; }
    using iterator = QLineEdit**;
    iterator begin() { return iterator(transactions); }
    iterator end() { return iterator(transactions + nbTransactions); }
    QWidget* GetWindow(){return window;}
    QLineEdit** GetTransactions(){return transactions;}
    void addTransaction(Transaction* transaction,bool type);
    void deleteTransactions();
    void updateMontantBaseOwned(double oldBase);
    void updateMontantContrepartieOwned(double oldContrepartie);
    void updateMontantTotalOwned(){ //Calcul du ROI
        montantTotalOwned->setText(QString::number(user->getRetourSurInvestissement())+"%");
    }
    void updateTransactions();
    void setmode_choisit(bool choix){mode_choisit = choix;}
    bool Getmode_choisit(){return mode_choisit;}
    void update_mode();
    //Les differentes fonction d'achat/vente automatique
    int Variation_1Bougie();
    int Variation_3Bougie();
    int Variation_5Bougie();
    int tendance_5Bougie();
    int tendance_10Bougie();
    int Comparaison_mtn_10bougie();
signals:
    void nextDay(); //Emit en mode auto lorsque l'on a fini un transaction. Equivalent a appuyer sur le boutton "next day" en mode manuel.
    private slots:
    void handle_automatique();  //Permet de gerer une simulation automatique lorsqu'on clique sur le bouton begin
    void handleButtonSell();
    void handleButtonBuy();
};

#endif // TRANSACTIONSVIEWER_H
