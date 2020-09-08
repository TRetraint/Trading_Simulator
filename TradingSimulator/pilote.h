#ifndef PILOTE_H
#define PILOTE_H
#include"includes.h"
#include "evolutionviewer.h"
#include "user.h"

//Classe représentant le menu affiché à l'éxecution de l'application, permet de choisir la date, les montants, le fichier, et le mode de simulation
class Pilote : public QWidget{
    Q_OBJECT
    EvolutionViewer* viewer; //Graphique en bougies

    User* user; // Utilisateur
    QLabel* message; // labels
    QPushButton* buttonDisplay; // bouton servant à afficher les graphiques
    QPushButton* buttonFile; // bouton servant à charger les données d'un fichier csv
    QPushButton* buttonDate;
    QPushButton* buttonMontant;
    QVBoxLayout* fenetre;
    QWidget* window;
    QVBoxLayout* total;
    QWidget* window2;

    QString path; //permet de stocker le chemin d'acces
    QLabel* yearBoxMessage;
    QLabel* monthBoxMessage;
    QLabel* dayBoxMessage;

    QComboBox* yearBox;
    QComboBox* monthBox;
    QComboBox* dayBox;

    QVBoxLayout* yearLayout;
    QVBoxLayout* monthLayout;
    QVBoxLayout* dayLayout;

    QHBoxLayout* dateForm;//Contient les différentes listes déroulantes
    QWidget* dateTotal; //Contient dateForm car on ne peut pas cacher un layout

    QLabel* montantBaseMessage;
    QLabel* montantContrepartieMessage;

    QLineEdit* montantBase;
    QLineEdit* montantContrepartie;

    QVBoxLayout* montantBaseLayout;
    QVBoxLayout* montantContrepartieLayout;

    QHBoxLayout* montantForm;
    QWidget* montantTotal;

    QLabel* mode_titre;
    QLabel* mode_choisit_label;

    QLineEdit* mode_choisit_edit;   //Affiche le mode selectionner

    QPushButton* choisir_mode;  //Permet d epasser a la suite
    QPushButton* Button_auto;
    QPushButton* Button_manuel;

    QHBoxLayout* liste_button_mode;
    QHBoxLayout* choix_mode;
    QVBoxLayout* affiche_mode;
    QWidget* mode;  //Widget de la fenetre de selection du mode

public:
    explicit Pilote(EvolutionViewer *v,QWidget *parent = nullptr);
    QVBoxLayout* GetFenetre(){return fenetre;}
    QWidget* GetWindow(){return window;}
    void LoadFile(QString path,QChar delim, QChar delimd, QDate dateCondition=QDate(1950,1,1));
signals:
    void loadFile(QString path);
private slots:
    //Slot appelé lorsque l'on clique sur Display Graphique, elle affiche la fenetre principale du programme, contenant tous les graphiques
    void handleButtonDisplay(){
        viewer->getTransactionsViewer()->update_mode();     //On mets a jour la fenetre des transaction selon le mode choisit
        //On affiche les boutton de deplacement seulement en mode manuel.
        if(viewer->getTransactionsViewer()->Getmode_choisit()){//false : mode maneul / true : mode auto
            viewer->HideButon_day();
        }else{
            viewer->ShowButton_day();
        }
        window2->show();    //On affiche la fenetre des graphique
        viewer->getTransactionsViewer()->GetWindow()->show();   //on affiche la fenetre des transaciton
    }

    //Slot appelé lorsque l'on clique sur le boutton Choose File, elle ouvre une boite de dialogue permettant de choisir le fichier à charger
    void handleButtonFile(){
        path = QFileDialog::getOpenFileName(this, tr("Open File"),"/",tr("Data (*.csv)"));
       // this->LoadFile(path,',','-');
        dateTotal->setVisible(true);
        buttonDate->setVisible(true);
    }

    //Slot appelé lorsque que l'on clique sur le boutton Choose Date, il charge le fichier en donnant en paramètre le chemin de celui-ci et la date à partir de laquelle afficher les données
    void handleButtonDate(){

        this->LoadFile(path,',','-',QDate(yearBox->currentText().toInt(),monthBox->currentText().toInt(),dayBox->currentText().toInt()));
qWarning("jalon");
        montantTotal->setVisible(true);
        buttonMontant->setVisible(true);
    }
    //Slot appelé lorsque l'on clique sur le boutton Choose Montant, il initialise l'objet User et TransactionsViewer avec les montants correspondants
    void handleButtonMontant(){
        viewer->getUser()->setMontantBaseDebut(montantBase->text().toDouble());
        viewer->getUser()->setMontantContrepartieDebut(montantContrepartie->text().toDouble());
        viewer->getUser()->setMontantBase(montantBase->text().toDouble());
        viewer->getUser()->setMontantContrepartie(montantContrepartie->text().toDouble());
        viewer->getTransactionsViewer()->updateMontantBaseOwned(0);
        viewer->getTransactionsViewer()->updateMontantContrepartieOwned(0);
        mode->setVisible(true);
    }
    //Permet d'acceder a la suite une fois le mode choisit
    void handleChoix_mode(){
        buttonDisplay->setVisible(true);
    }
    //Met le mode choisit comme automatique
    void handle_Button_auto(){
        mode_choisit_edit->setText("Automatique");
        viewer->getTransactionsViewer()->setmode_choisit(true);
    }
    //Met le mode choisit comme manuel
    void handle_Button_manuel(){
        mode_choisit_edit->setText("Manuel");
        viewer->getTransactionsViewer()->setmode_choisit(false);
    }
    void ajuster_window(){window2->adjustSize();}
public slots:
};






#endif // PILOTE_H
