#include"simulation.h"

//Constructeur de SImulation
Simulation::Simulation(QWidget *parent) : QWidget (parent){
    //On creer le widget pour la feuille de note
    note_affiche = new Note(this);

    //Creation des boutton de sauveguarde
    sauv_note = new QPushButton("save notes");
    connect(sauv_note, SIGNAL(clicked()), this, SLOT(handleSauve_note()));

    sauv_donnees = new QPushButton("save datas");

    sauv_tab = new QPushButton("save transactions");

    //Creation du boutton de fermeture
    fini = new QPushButton("done");
    connect(fini, SIGNAL(released()), this, SLOT(fermer_windows_sauv()));

    //Mise en page
    fenetre = new QVBoxLayout;
    fenetre->addWidget(sauv_note);
    fenetre->addWidget(sauv_donnees);
    fenetre->addWidget(sauv_tab);
    fenetre->addWidget(fini);

    windows_sauv = new QWidget;
    windows_sauv->setLayout(fenetre);

}

//Permet de sauveguarder une note lorsqu'on sauveguarde une simulation
void Simulation::handleSauve_note(){
    QString texte;  //Contient le texte qui sera envoyer au fichier
    QString path_txt;   //contient le chemin vers le fichier

    //Onr ecupere le chemin vers le fichier
    path_txt = QFileDialog::getSaveFileName(this, tr("Save File"),"/",tr("Text files (*.txt)"));

    QFile fichier(path_txt);

    if( fichier.open(QIODevice::ReadWrite | QIODevice::Text) ){//Si le fichier s'ouvre
        //On supprime l'ancien fichier et on en creer un nouveau
        fichier.remove();
        fichier.open(QIODevice::ReadWrite | QIODevice::Text);
        //On rempli le nouveau fichier creer
        texte = note_affiche->getZone_text()->toPlainText();//Si aucun fichier note n'avait ete charger. Le fichier sauveguarder sera vide.
        QTextStream flux (&fichier);
        flux << texte;
        fichier.close();
    }else{
        QMessageBox::critical(nullptr, "Erreur"," Impossible d'ouvrir le fichier");
    }
}

//permet de sauveguarder la lsite des transactions effectuees.
void Simulation::Sauveguarde_tab(unsigned int nb_transaction, QLineEdit** transaction){
    QString texte;  //Contient la ligne qu'on ecrira dans le fichier
    QString path_txt;   //Contient le chemion vers le fichier

    //On laisse l'utilisateur selectionner l'emplacement du fichier et son nom.
    path_txt = QFileDialog::getSaveFileName(this, tr("Save File"),"/",tr("Text files (*.txt)"));

    QFile fichier(path_txt);


    if( fichier.open(QIODevice::ReadWrite | QIODevice::Text) ){//Si le fichier s'ouvre
        //On supprime l'ancien fichier et on en creer un nouveau
        fichier.remove();
        fichier.open(QIODevice::ReadWrite | QIODevice::Text);
        //On rempli le nouveau fichier creer
        for (unsigned int i=0; i<nb_transaction; i++ ){ //On recopie transaction apr transaction
            texte = transaction[i]->displayText();  //On recupere la transaction i
            QTextStream flux (&fichier);
            flux << texte;
            flux<< "\n";
        }
        fichier.close();
    }else{
        QMessageBox::critical(nullptr, "Erreur"," Impossible d'ouvrir le fichier");
    }
}
//Effectue la sauveguarde des donnees
void Simulation::Sauveguarde_donnees(EvolutionCours* cours){
    QString texte;
    QString path_txt;

    //On laisse l'utilisateur selectionner l'emplacement du fichier et son nom.
    path_txt = QFileDialog::getSaveFileName(this, tr("Save File"),"/",tr("Text files (*.csv)"));

    QFile fichier(path_txt);

    EvolutionCours::iterator it;     //Pointe sur la premiere donnee.
    QTextStream flux (&fichier);

    if( fichier.open(QIODevice::ReadWrite | QIODevice::Text) ){//Si le fichier s'ouvre
        //On supprime l'ancien fichier et on en creer un nouveau
        fichier.remove();
        fichier.open(QIODevice::ReadWrite | QIODevice::Text);

        //On rempli le nouveau fichier creer
        flux << "Date,Open,High,Low,Close,Adj Close,Volume\n";      //On mets en haut du fichier le nom des colonnes.
        for (it =cours->begin(); it<cours->end(); it++ ){ //On recopie donnees par donnees

            flux<< it->getDate().year() <<"-"<<it->getDate().month()<<"-"<<it->getDate().day()<<",";    //On sauveguarde la date sous le format normaliser
            flux<< it->getOpen()<<",";
            flux<< it->getHigh()<<",";
            flux<< it->getLow()<<",";
            flux<< it->getClose()<<",";
            flux<< it->getClose()<<",";
            flux<< it->getVolume();
            flux<<"\n";
        }
        fichier.close();
    }else{
        QMessageBox::critical(nullptr, "Erreur"," Impossible d'ouvrir le fichier");
    }
}
