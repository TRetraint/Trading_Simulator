#ifndef SIMULATION_H
#define SIMULATION_H
#include"note.h"
#include"includes.h"
#include"trading.h"

//Classe Permettant la sauveguarde d'une simulation.
class Simulation : public QWidget{
    Q_OBJECT
    Note* note_affiche; //Contient le widget pour la feuille de note
    QVBoxLayout* fenetre;
    QWidget* windows_sauv;
    QPushButton* sauv_note;     //Boutton pour sauveguarder les 3 elemnts d'ue simulation.
    QPushButton* sauv_donnees;
    QPushButton* sauv_tab;
    QPushButton* fini;      //Boutton pour fermer l'interface de sauveguarde.
public:
    explicit Simulation(QWidget *parent = nullptr);
    Note* getNote_affiche(){return note_affiche;}
    QWidget* getWindows_sauv(){return windows_sauv;}
    QPushButton* GetSauv_donnees(){return sauv_donnees;}
    QPushButton* GetSauv_tab(){return sauv_tab;}
signals:
private slots:
    void afficher_windows_sauv(){windows_sauv->show();} //Affiche la fenetre de sauveguarde de la simulation
    void fermer_windows_sauv(){windows_sauv->hide();}   //Cache la fenetre de sauveguarde de la simulation
    void handleSauve_note();
    void Sauveguarde_donnees(EvolutionCours* cours);    //effectue la sauveguarde des donnees
    void Sauveguarde_tab(unsigned int nb_transaction, QLineEdit** transaction);     //effectue la sauveguarde des transactions
public slots:
};


#endif // SIMULATION_H
