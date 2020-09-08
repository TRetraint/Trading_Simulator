#ifndef EVOLUTIONVIEWER_H
#define EVOLUTIONVIEWER_H
#include "includes.h"
#include "user.h"
#include "indicateur.h"
#include "simulation.h"
#include "indicateursviewer.h"
#include "transactionsviewer.h"
#include "volumeviewer.h"

//Classe représentant les bougies sur le graphiques : d'ou le fait qu'elle hérite de QCandlesitckSet
class Bougie : public QCandlestickSet {
    Q_OBJECT
    //Informations inhérentes à la bougie : permettent entre autre d'afficher celles-ci sur le graphique
    CoursOHLC* cours;
    double rsi;
    double ema;
    double macd;
public:
    //Constructeur de la classe, on initialise bien tous les paramètre et on connecte la bougie au signaux clicked pour afficher les informations liées au cours
    //et pressed() pour les indicateurs et les formes de bougie
    Bougie(qreal open, qreal high, qreal low, qreal close,CoursOHLC* c,double r, double e,double m,qreal
        timestamp = 0.0,QObject *parent = nullptr):
        QCandlestickSet(open, high, low, close, timestamp, parent), cours(c),rsi(r),ema(e),macd(m){
        connect(this,SIGNAL(clicked()),this, SLOT( viewCoursOHLC()));
        connect(this, SIGNAL(pressed()), this, SLOT(handlepressed()));
    }
    CoursOHLC& getCoursOHLC() { return *cours; }
    double getRSI(){return rsi;}
    double getEMA(){return ema;}
    double getMACD(){return macd;}
    double getOpen(){return open();}
    double getClose(){return close();}
    double getHigh(){return high();}
    double getLow(){return low();}
    const CoursOHLC& getCoursOHLC() const { return *cours; }

    QString forme() const;

    bool isToupie() const;
    bool isDoji() const;

    bool isDragonflyDoji() const;

    bool isGraveStoneDoji() const;

    bool isHanggingMan() const;

    bool isHammer() const;

    bool isInvertedHammer() const;

    bool isInvertedBlackHammer() const;

    bool isLongLowerShadow() const;

    bool isLongUpperShadow() const;

    bool isMarubozu() const;

    bool isShootingStar() const;

    bool isShavenBottom() const;

    bool isShavenHead() const;
    void analyseForm();
signals:
    void clickBougie(Bougie* b);
private slots:
    //émet un signal qui sera transmis à la classe evolutionviewer pour l'affichage dans les QLineEdit
    void viewCoursOHLC(){ emit clickBougie(this); }
    void handlepressed(){analyseForm();}
};

class EvolutionViewer : public QWidget{
    Q_OBJECT
    //Pointeurs sur les différentes informations liées au cours (OHCLV, indicateurs) car c'est evolutionviewer qui update tout en fonction du jour qu'on affiche
    EvolutionCours& evolution;
    RSI* rsi;
    EMA* ema;
    EMA* ema1;
    EMA* ema2;
    MACD* macd;

    //Utilisateur
    User* user;

    //Pointeurs sur les différents "viewer" car c'est evolutionviewer qui update tout en fonction du jour qu'on affiche
    TransactionsViewer* transactionsviewer;
    VolumeViewer* volumeviewer; //Le graphique en barre
    IndicateursViewer* rsiviewer;
    IndicateursViewer* emaviewer;
    IndicateursViewer* macdviewer;

    QCandlestickSeries* series; // un ensemble de bougies
    QChart *chart; // un graphique sur un ensemble de bougies
    QChartView *chartView; // un viewer de graphique
    QBarCategoryAxis* axisX;

    //Interface graphique
    QLineEdit* open; // barres d’édition
    QLineEdit* high;
    QLineEdit* low;
    QLineEdit* close;
    QLabel* openl; // labels
    QLabel* highl;
    QLabel* lowl;
    QLabel* closel;
    QStringList* dates; //contient la liste de dates de series
    QPushButton* buttonNextDay; // bouton
    QPushButton* buttonPreviousDay; // bouton
    QPushButton* note; // bouton pour ouvrir une note
    QHBoxLayout* copen; // couches de placement
    QHBoxLayout* chigh;
    QHBoxLayout* clow;
    QHBoxLayout* cclose;
    QVBoxLayout* coucheCours;
    QHBoxLayout* fenetre;
    QWidget* window;

    //Pointeur sur l'interface de sauveguarde d'un simulation
    Simulation* sim;
    QPushButton* save_simulation;

    int last; //numéro du dernier cours OHCL affiché dans le graphique
public:
    explicit EvolutionViewer(EvolutionCours& e,QWidget *parent = nullptr);
    QHBoxLayout* GetFenetre(){return fenetre;}
    EvolutionCours* GetEvolution(){return &evolution;}
    QWidget* GetWindow(){return window;}
    void Update(int i); //permet de mettre à jour le graphique une fois les cours ajoutés à évolution
    void nextDay();
    void previousDay();
    void setLast(int num){last=num;}
    EMA* getEMA(){return ema;}
    EMA* getEMA1(){return ema1;}
    EMA* getEMA2(){return ema2;}
    VolumeViewer* getVolumeViewer(){return volumeviewer;}
    int getLast(){return last;}
    RSI* getRSI(){return rsi;}
    MACD* getMACD(){return macd;}
    User* getUser(){return user;}
    TransactionsViewer* getTransactionsViewer(){return transactionsviewer;}
    void setRsiViewer(IndicateursViewer* rsi){rsiviewer=rsi;}
    void setEmaViewer(IndicateursViewer* ema){emaviewer=ema;}
    void setMacdViewer(IndicateursViewer* macd){macdviewer=macd;}
    IndicateursViewer* getRsiViewer(){return rsiviewer;}
    IndicateursViewer* getEmaViewer(){return emaviewer;}
    IndicateursViewer* getMacdViewer(){return macdviewer;}
    void HideButon_day();
    void ShowButton_day();
    Simulation* GetSim(){return sim;}
signals:
    void Sauv_tab(unsigned int nb_transaction, QLineEdit** transaction);    //Signal contenant les information des transactions a sauveguarder.
    void Sauv_donnees(EvolutionCours* cours);    //signal contenant les donnees a sauveguarder.
private slots:
     void viewCoursOHLC(Bougie* b); //Permet d'afficher les données liées à une bougie

     //handlers des bouttons
     void handleButtonNextDay(){
         nextDay(); }
     void handleButtonPreviousDay(){
         previousDay();
     }
     void ajuster_window(){window->adjustSize();}
     void handle_signal_Sauv_tab(); //recupere la pression sur le boutton "sauv_tab" et renvoie a simulation le signal "Sauv_tab"
     void handle_signal_Sauv_donnees();     //meme chose pour les donnees

public slots:
};

#endif // EVOLUTIONVIEWER_H
