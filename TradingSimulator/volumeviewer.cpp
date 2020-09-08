#include "volumeviewer.h"

//Constructeur de volumeViewer
VolumeViewer::VolumeViewer(EvolutionCours& e,QWidget *parent) :
    QWidget (parent),evolution(e) {
    //Creation des barres du graphique.
    series = new QBarSeries();
    series->setName("Evolution des Volumes de paires de devises");

    //On creer une barre par bougie, representant son volume
    bar = new QBarSet(evolution.getPaireDevises().toString());
    for (EvolutionCours::iterator it = evolution.begin(); it!=evolution.end(); it++)
    {
        CoursOHLC& cours = *it;
        unsigned int v = cours.getVolume();
        bar->append(v);
    }
    series->append(bar);

    chart = new QChart;
    chart->addSeries(series);
    chart->setTitle("CoursOHCL");
    chart->createDefaultAxes();
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    chartView = new QChartView(chart);
    chartView->setFixedWidth(1200);
    chartView->setFixedHeight(300);

    //Creation de la zone de texte affichant le volume de la barre selecitonner
    volumel = new QLabel("volume",this);
    volume = new QLineEdit(this);

    //Mise en apge
    cvolume = new QHBoxLayout;
    coucheCours = new QVBoxLayout;
    fenetre = new QHBoxLayout;

    cvolume->addWidget(volumel);
    cvolume->addWidget(volume);

    coucheCours->addLayout(cvolume);

    fenetre->addWidget(chartView);
    fenetre->addLayout(coucheCours);
    window = new QWidget;
    window->setLayout(fenetre);
}

void VolumeViewer::Update(int nombre){
    chart->removeSeries(series);
    series->clear();
    bar = new QBarSet(evolution.getPaireDevises().toString());
    //idem que pour evolutionviewer
    if(nombre>=10){
    EvolutionCours::iterator it= evolution.begin()+nombre-10;
    unsigned int volume;
    for(int i=0;i<10;i++){
         volume=it->getVolume();
         bar->append(volume);

         it++;
    }}
    else{
        EvolutionCours::iterator it= evolution.begin();
        unsigned int volume;
        for(int i=0;i<nombre;i++){
             volume=it->getVolume();
             bar->append(volume);

             it++;

        }
    }
    series->append(bar);
    chart->addSeries(series);
    chart->createDefaultAxes();

}

