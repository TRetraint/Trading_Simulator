#include "indicateursviewer.h"

//Constructeur de la classe IndicateursViewer
IndicateursViewer::IndicateursViewer(){
    series = new QLineSeries;
    chart = new QChart;
    chartView = new  QChartView(chart);
    chartView->setFixedWidth(1200);
    chartView->setFixedHeight(200);
    fenetre = new QHBoxLayout;
    fenetre->addWidget(chartView);
    window = new QWidget;
    window->setLayout(fenetre);
}

//Méthode mettant à jour le graphique RSI suivant la dernière bougie affiché ayant pour indice l'argument "nombre"
void IndicateursViewer::Update(RSI *evolution, unsigned int nombre){
    chart->removeSeries(series);
    series->clear();
    RSI::iterator it=evolution->begin()+nombre-10;

    for(int i=0;i<10;i++){
        series->append(i,it->getValue());

        it++;
    }
    QPen pen = series->pen();
    pen.setWidth(3);
    pen.setBrush(QBrush("red"));
    series->setPen(pen);
    chart->legend()->hide();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("RSI (14)");
    chart->axisY()->setMax(100);
    chart->axisY()->setMin(0);
    chart->axisX()->hide();
    chartView->setRenderHint(QPainter::Antialiasing);
}


//Méthode mettant à jour le graphique EMA suivant la dernière bougie affiché ayant pour indice l'argument "nombre"
void IndicateursViewer::UpdateEMA(EMA *evolution,unsigned int nombre){
     chart->removeSeries(series);
     series->clear();
     EMA::iterator it=evolution->begin()+nombre-10;

     for(int i=0;i<10;i++){
         series->append(i,it->getValue());
         it++;
     }
     QPen pen = series->pen();
     pen.setWidth(2);
     pen.setBrush(QBrush("green"));
     series->setPen(pen);
     chart->legend()->hide();
     chart->addSeries(series);
     chart->createDefaultAxes();
     chart->setTitle("EMA (9)");
     chart->axisY()->alignment();
     chartView->setRenderHints(QPainter::Antialiasing);
 }

//Méthode mettant à jour le graphique MCAD suivant la dernière bougie affiché ayant pour indice l'argument "nombre"
void IndicateursViewer::UpdateMACD(MACD *evolution,unsigned int nombre){
     chart->removeSeries(series);
     series->clear();
     MACD::iterator it=evolution->begin()+nombre-10;

     for(int i=0;i<10;i++){
         series->append(i,it->getValue());
         it++;
     }
     QPen pen = series->pen();
     pen.setWidth(2);
     pen.setBrush(QBrush("purple"));
     series->setPen(pen);
     chart->legend()->hide();
     chart->addSeries(series);
     chart->createDefaultAxes();
     chart->setTitle("MACD (12,26)");
     chartView->setRenderHints(QPainter::Antialiasing);
 }
