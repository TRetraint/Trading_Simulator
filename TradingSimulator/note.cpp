#include "note.h"

/*!
 * Constructeur de Note.
*/
Note::Note(QWidget *parent) : QWidget (parent){

    path = QString();//On initialise la variable comme vide.

    save = new QPushButton("sauveguarder note");
    connect(save, SIGNAL(clicked()), this, SLOT(sauveguarder_note()) );
    save->setVisible(false);    //Le boutton n'est pas visible tant que on ne prend pas de note
    coucheNote = new QVBoxLayout;

    zone_text = new QTextEdit;
    zone_text->setMinimumWidth(400);
    zone_text->setMaximumWidth(600);
    zone_text->setVisible(false);

    coucheNote->addWidget(zone_text);
    coucheNote->addWidget(save);
}

/*!
 * Permet d'afficher la feuille pour prendre des notes.
*/
void Note::afficher_feuille_note(){
    //on recupere le chemin
    path = QFileDialog::getOpenFileName(this, tr("Open File"),"/",tr("Data (*.txt)"));

    QString texte;
    QFile fichier(path);
    //Attention il cherche le fichier dans le dossier contenant l'executalne. Ce n'est pas le meme que le dossier contenant le projet.

    if( fichier.open(QIODevice::ReadWrite | QIODevice::Text) ){
        texte = fichier.readAll();
        fichier.close();
    }else{
        texte = "Impossible d'ouvrir le fichier";
    }

    zone_text->setText(texte);
    zone_text->setVisible(true);
    save->setVisible(true);
}

/*!
 * Permet de sauveguarde nos note dans un fichier .txt sur le disque.
 */
void Note::sauveguarder_note(){
    QString texte;
    QFile fichier(path);    //Le chemin du fichier est celui que l'on a decider lorsqu'on a ouvert notre feille de notes.

    if( fichier.open(QIODevice::ReadWrite | QIODevice::Text) ){//Si le fichier s'ouvre
        //On supprime l'ancien fichier et on en creer un nouveau
        fichier.remove();
        fichier.open(QIODevice::ReadWrite | QIODevice::Text);
        //On rempli le nouveau fichier creer
        texte = zone_text->toPlainText();
        QTextStream flux (&fichier);
        flux << texte;
        fichier.close();

        //On referme l'editeur de texte
        zone_text->setVisible(false);
        save->setVisible(false);

        path = QString();   //On vide le chemin d'acces.

        emit sauv_note();

    }else{//Si le fichier ne c'est pas ouvert
        QMessageBox::critical(nullptr, "Erreur"," Impossible d'ouvrir le fichier");
    }
}
