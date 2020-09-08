#ifndef INDICATEURSVIEWER_H
#define INDICATEURSVIEWER_H

#include "includes.h"
#include "indicateur.h"

//Classe représenant le graphique montrant les indicateurs
class IndicateursViewer : public QWidget{
    Q_OBJECT
    QLineSeries *series;
    QChart *chart; // un graphique sur un ensemble de bougies
    QChartView *chartView; // un viewer de graphique
    QHBoxLayout* fenetre;
    QWidget* window;
    public:
    IndicateursViewer();
    void Update(RSI *evolution, unsigned int nombre);
    void UpdateEMA(EMA* evolution,unsigned int nombre);
    void UpdateMACD(MACD* evolution,unsigned int nombre);
    QWidget *GetWindow(){return window;}
};

#endif // INDICATEURSVIEWER_H
