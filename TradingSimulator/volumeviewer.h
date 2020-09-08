#ifndef VOLUMEVIEWER_H
#define VOLUMEVIEWER_H
#include "includes.h"
#include"trading.h"

//Classe affichant le graphique en barre représenant les volumes
class VolumeViewer : public QWidget{
    Q_OBJECT
    //Permet d'accéder
    EvolutionCours& evolution;
    QBarSeries* series; // un ensemble de barres
    QChart *chart; // un graphique sur un ensemble de bougies
    QChartView *chartView; // un viewer de graphique
    QLineEdit* volume; // barres d’édition
    QLabel* volumel; // labels

    QHBoxLayout* cvolume; // couches de placement
    QVBoxLayout* coucheCours;
    QHBoxLayout* fenetre;
    QWidget* window;
    QBarSet* bar;
public:
    explicit VolumeViewer(EvolutionCours& e,QWidget *parent = nullptr);
    QHBoxLayout* GetFenetre(){return fenetre;}
    QWidget* GetWindow(){return window;}
    void Update(int i); //permet de mettre à jour le graphique
signals:
private slots:
public slots:
};


#endif // VOLUMEVIEWER_H
