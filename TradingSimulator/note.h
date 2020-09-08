#ifndef NOTE_H
#define NOTE_H
#include"includes.h"

//Permet l'affichage d'une feuille de note. Chargement et Sauveguarde.
class Note : public QWidget{
    Q_OBJECT
    QString path;
    QPushButton* save;
    QVBoxLayout* coucheNote;
    QTextEdit* zone_text;//contient les notes prises
public:
    explicit Note(QWidget *parent = nullptr);
    QVBoxLayout* getCoucheNote(){return coucheNote;}
    QTextEdit* getZone_text(){return zone_text;}
signals:
    void sauv_note();   //emit quand on veut mettre a jour l'affichage de evolution viewer.
private slots:
     void afficher_feuille_note();
     void sauveguarder_note();
public slots:
};

#endif // NOTE_H
