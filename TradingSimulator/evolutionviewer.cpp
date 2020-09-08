#include "evolutionviewer.h"

//Constructeur de la classe EvolutionViewer
EvolutionViewer::EvolutionViewer(EvolutionCours& e,QWidget *parent) :
    QWidget(parent),evolution(e){
    //Initialisation des indicateurs
    rsi = new RSI(14);
    ema = new EMA(9);
    macd = new MACD(12,26);
    ema1=new EMA(12);
    ema2=new EMA(26);

    //Initialisation de l'utilisateur
    user = new User;

    //Initialisation des viewer
    volumeviewer = new VolumeViewer(evolution);
    transactionsviewer = new TransactionsViewer(&evolution,user,&last);
    connect(transactionsviewer, SIGNAL(nextDay()), this, SLOT(handleButtonNextDay()));  //On connect le signal emis en mode auto pour passer au jour suivant avec la methode associé.
    rsiviewer = new IndicateursViewer;
    emaviewer = new IndicateursViewer;
    macdviewer = new IndicateursViewer;

    //Creation des donnees du graphique bougie
    series = new QCandlestickSeries();
    series->setName(evolution.getPaireDevises().toString());
    series->setIncreasingColor(QColor(Qt::green));
    series->setDecreasingColor(QColor(Qt::red));

    //bouttons permettant de passer les jours
    buttonNextDay = new QPushButton("next day");
    connect(buttonNextDay, SIGNAL (released()),this, SLOT (handleButtonNextDay()));

    buttonPreviousDay = new QPushButton("previous day");
    connect(buttonPreviousDay, SIGNAL (released()),this, SLOT (handleButtonPreviousDay()));

    dates = new QStringList;

    //Creation du graphique en bougie ( affichage)
    chart = new QChart;
    chart->addSeries(series);
    chart->setTitle("CoursOHCL");
    chart->createDefaultAxes();
    axisX = new QBarCategoryAxis;
    axisX->append(*dates);
    chart->setAxisX(axisX,series);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    chartView = new QChartView(chart);
    chartView->setFixedWidth(1200);
    chartView->setFixedHeight(300);

    //Creation des label et des editeur de text pour les parametre d'un bougie.
    openl = new QLabel("open",this);
    highl = new QLabel("high",this);
    lowl = new QLabel("low",this);
    closel = new QLabel("close",this);

    open = new QLineEdit(this);
    high = new QLineEdit(this);
    low = new QLineEdit(this);
    close = new QLineEdit(this);

    //Creation des Layout de EvolutionViewer
    copen = new QHBoxLayout;
    chigh = new QHBoxLayout;
    clow = new QHBoxLayout;
    cclose = new QHBoxLayout;
    coucheCours = new QVBoxLayout;
    fenetre = new QHBoxLayout;

    window = new QWidget;

    //Mise en page
    copen->addWidget(openl);
    copen->addWidget(open);

    chigh->addWidget(highl);
    chigh->addWidget(high);

    clow->addWidget(lowl);
    clow->addWidget(low);

    cclose->addWidget(closel);
    cclose->addWidget(close);

    coucheCours->addLayout(copen);
    coucheCours->addLayout(chigh);
    coucheCours->addLayout(clow);
    coucheCours->addLayout(cclose);
    coucheCours->addWidget(buttonNextDay);
    coucheCours->addWidget(buttonPreviousDay);

    //Creation de Simulation est des widgets associes
    sim = new Simulation(this);

    save_simulation = new QPushButton("save simulation");
    connect(save_simulation, SIGNAL(clicked()), sim, SLOT(afficher_windows_sauv()));
    coucheCours->addWidget(save_simulation);

    //On recupere le signal lorsque l'utilisateur presse sur "save transactions" et on en renvoit un autre à Simulation contenant les informations a sauveguarder.
    connect(sim->GetSauv_tab(), SIGNAL(clicked()), this , SLOT(handle_signal_Sauv_tab()));
    connect(this, SIGNAL(Sauv_tab(unsigned int, QLineEdit**)), sim, SLOT(Sauveguarde_tab(unsigned int, QLineEdit**)));

    //Meme chose pour les donnees.
    connect(sim->GetSauv_donnees(), SIGNAL(clicked()), this , SLOT(handle_signal_Sauv_donnees()));
    connect(this, SIGNAL(Sauv_donnees(EvolutionCours*)), sim, SLOT(Sauveguarde_donnees(EvolutionCours*)));

    //Creer un boutton pour pouvoir acceder à la feuille de note sans passer par le chargement de la Simulation
    connect(sim->getNote_affiche(), SIGNAL(sauv_note()), this, SLOT(ajuster_window()) );
    note = new QPushButton("feuille de note");
    connect(note, SIGNAL(clicked()), sim->getNote_affiche(), SLOT(afficher_feuille_note()) );

    coucheCours->addWidget(note);

    //Mise en page
    fenetre->addWidget(chartView);
    fenetre->addLayout(coucheCours);
    fenetre->addLayout(sim->getNote_affiche()->getCoucheNote());

    window->setLayout(fenetre);

}

//Slot appelé quand on clique sur une bougie, affiche les informations OHCL dans les QLineEdit
void EvolutionViewer::viewCoursOHLC(Bougie* b){
    open->setText(QString::number( b->getOpen()));
    high->setText(QString::number( b->getHigh()));
    low->setText(QString::number( b->getLow()));
    close->setText(QString::number( b->getClose()));
}

//Fonction appelée quand on passe les jours, utilise l'attribut last, qu'on passe en argument via "nombre" et qui permet de savoir quelle bougie afficher en dernier

void EvolutionViewer::Update(int nombre){
    chart->removeSeries(series); //On enlève la série du graphique pour la remettre une fois mise à jour
    series->clear(); //On enleve les bougies de la série pour remettre les nouvelles
    dates->clear(); // de même pour les dates

    //Si nombre est supérieur à 10, alors on peut afficher les 9 bougies antérieures au jour courant
    if(nombre>=10){
    EvolutionCours::iterator it = evolution.begin()+nombre-10; // On accède au premier cours que l'on a ajouté

    //Itérateurs sur les indicateurs pour initaliser les bougies avec les bons indicateurs
    EvolutionIndicateurs::iterator itRsi = rsi->begin()+nombre-10;
    EvolutionIndicateurs::iterator itEma= ema->begin()+nombre-10;
    EvolutionIndicateurs::iterator itMacd=macd->begin()+nombre-10;
    EvolutionCours::iterator itDate = evolution.begin()+nombre-1;// pour récupérer la date de la dernière bougie affichée pour update transactions

    //On udate user et transactionsviewer
    user->update(itDate->getDate());
    transactionsviewer->updateTransactions();
    transactionsviewer->updateMontantBaseOwned(-1);
    transactionsviewer->updateMontantContrepartieOwned(-1);
    transactionsviewer->updateMontantTotalOwned();

    //On créé les 9 bougies antérieures au jour courant
    for(int i=0;i<9;i++){
         Bougie* bougie = new Bougie(it->getOpen(), it->getHigh(), it->getLow(), it->getClose(),it,itRsi->getValue(),itEma->getValue(),itMacd->getValue());
         series->append(bougie);
         *dates << it->getDate().toString("yy.MM.dd");
         connect(bougie,&Bougie::clickBougie,this, &EvolutionViewer::viewCoursOHLC);
         itRsi++;
         itEma++;
         itMacd++;
         it++;
    }
    //On affiche uniquement la valeur du cours d'ouverture pour la bougie correspondant au jour courant
    //Le fait de mettre Open uniquement dans les attributs de bougie et pas de cours nous permet de modifier uniquement l'affichage, le fond restant le même
    Bougie* bougie = new Bougie(it->getOpen(), it->getOpen(), it->getOpen(), it->getOpen(),it,itRsi->getValue(),itEma->getValue(),itMacd->getValue()); // permet d'afficher uniquement le prix d'ouverture pour la dernière bougie
    series->append(bougie);
    *dates << it->getDate().toString("yy.MM.dd");
    connect(bougie,&Bougie::clickBougie,this, &EvolutionViewer::viewCoursOHLC);
    }
    else{ //sinon c'est qu'on a choisit une date très proche de celle de début et donc on a pas assez de bougies à afficher
        EvolutionCours::iterator it = evolution.begin(); // On accède au premier cours que l'on a ajouté
        EvolutionIndicateurs::iterator itRsi= rsi->begin();
        EvolutionIndicateurs::iterator itEma= ema->begin();
        EvolutionIndicateurs::iterator itMacd=macd->begin();

        EvolutionCours::iterator itDate = evolution.begin()+nombre-1;// pour récupérer date dernière bougie affichée pour update transactions

        user->update(itDate->getDate());
        transactionsviewer->updateTransactions();
        transactionsviewer->updateMontantBaseOwned(-1);
        transactionsviewer->updateMontantContrepartieOwned(-1);
        transactionsviewer->updateMontantTotalOwned();

        for(int i=0;i<nombre-1;i++){
             Bougie* bougie = new Bougie(it->getOpen(), it->getHigh(), it->getLow(), it->getClose(),it,itRsi->getValue(),itEma->getValue(),itMacd->getValue());
             series->append(bougie);
             *dates << it->getDate().toString("yy.MM.dd");
             connect(bougie,&Bougie::clickBougie,this, &EvolutionViewer::viewCoursOHLC);
             itRsi++;
             itEma++;
             itMacd++;
             it++;
        }
        Bougie* bougie = new Bougie(it->getOpen(), it->getOpen(), it->getOpen(), it->getOpen(),it,itRsi->getValue(),itEma->getValue(),itMacd->getValue()); // permet d'afficher uniquement le prix d'ouverture pour la dernière bougie
        series->append(bougie);
        *dates << it->getDate().toString("yy.MM.dd");
        connect(bougie,&Bougie::clickBougie,this, &EvolutionViewer::viewCoursOHLC);
    }
    chart->removeAxis(axisX); //On enlève l'axe du graphique pour le remettre une fois mis à jour
    axisX->clear(); //On enleve les anciennes dates de l'axe
    axisX->append(*dates); //On met les nouvelles
    chart->addSeries(series); //On rajoute la série
    chart->setAxisX(axisX,series); //L'axe
    volumeviewer->Update(nombre); //On update le graphique de volumes
}

//Méthode appelée lorsque l'on clique sur le bouton nextday, elle incrémente l'attribut last et appelle Update pour mettre à jour tous les graphiques
void EvolutionViewer::nextDay(){
    last++;
    Update(last);
    this->rsiviewer->Update(rsi,last);
    this->emaviewer->UpdateEMA(ema,last);
    this->macdviewer->UpdateMACD(macd,last);
}

//Méthode appelée lorsque l'on clique sur le bouton previousday, elle décrémente l'attribut last et appelle Update pour mettre à jour tous les graphiques
void EvolutionViewer::previousDay(){
    last--;
    Update(last);
    this->rsiviewer->Update(rsi,last);
    this->emaviewer->UpdateEMA(ema,last);
    this->macdviewer->UpdateMACD(macd,last);
}

//Affiche les boutton de deplacement pour le graphique Bougie
void EvolutionViewer::ShowButton_day(){
    buttonNextDay->setVisible(true);
    buttonPreviousDay->setVisible(true);
}

//Cache les boutton de deplacement pour le graphique Bougie
void EvolutionViewer::HideButon_day(){
    buttonNextDay->setVisible(false);
    buttonPreviousDay->setVisible(false);
}

//Handle charger d'envoyer à Simulation le tableau des transaction et le nombre de transaction apr signal
void EvolutionViewer::handle_signal_Sauv_tab(){
    emit Sauv_tab(transactionsviewer->getNbTransactions(),transactionsviewer->GetTransactions() );
}

//Handle charger d'envoyer à Simulation la lsite des donnees
void EvolutionViewer::handle_signal_Sauv_donnees(){
    emit Sauv_donnees(GetEvolution());
}


bool Bougie::isToupie() const {
    double meche = cours->getHigh() - cours->getLow() ;
    double corps = abs(cours->getOpen() - cours->getClose());
    if(corps <= 0.5*meche) return true;
        return false;

}

bool Bougie::isDoji() const {
    double meche = cours->getHigh() - cours->getLow() ;
    double corps = abs(cours->getOpen() - cours->getClose());
    if(corps < 0.05*meche) return true;
        return false;
}

bool Bougie::isDragonflyDoji() const {
    double mecheSup = cours->getHigh() - max(cours->getOpen(),cours->getClose());
    double mecheInf = min(cours->getOpen(), cours->getClose()) - cours->getLow();
    double corps = abs(cours->getOpen() - cours->getClose());
    if(isDoji() && mecheSup==0 && mecheInf > 5*corps) return true;
        return false;
}

bool Bougie::isGraveStoneDoji() const {
    double mecheSup = cours->getHigh() - max(cours->getOpen(),cours->getClose());
    double mecheInf = min(cours->getOpen(), cours->getClose()) - cours->getLow();
    double corps = abs(cours->getOpen() - cours->getClose());
    if (isDoji() && mecheInf==0 && mecheSup >5*corps) return true;
        return false;
}

bool Bougie::isInvertedHammer() const {
    double mecheSup = cours->getHigh() - max(cours->getOpen(),cours->getClose());
    double mecheInf = min(cours->getOpen(), cours->getClose()) - cours->getLow();
    double corps = abs(cours->getOpen() - cours->getClose());
    if(mecheSup>1*corps && mecheInf < 0.1*corps) return true;
        return false;

}

bool Bougie::isHanggingMan() const {
    double corps = abs(cours->getOpen() - cours->getClose());
    double mecheInf = min(cours->getOpen(), cours->getClose()) - cours->getLow();
    if(isHammer() &&  mecheInf > 2*corps)  return true;
        return false;
}

bool Bougie::isHammer() const {
    double mecheSup = cours->getHigh() - max(cours->getOpen(),cours->getClose());
    double mecheInf = min(cours->getOpen(), cours->getClose()) - cours->getLow();
    double corps = abs(cours->getOpen() - cours->getClose());
    if(mecheSup>0.1*corps && mecheInf <corps) return true;
        return false;
}

bool Bougie::isInvertedBlackHammer() const {
    if (isInvertedHammer() && (cours->getOpen() > cours->getClose())) return true;
        return false;
}

bool Bougie::isLongLowerShadow() const {
    double meche = cours->getHigh() - cours->getLow() ;
    double mecheInf = min(cours->getOpen(), cours->getClose()) - cours->getLow();
    if( mecheInf/meche > double(2)/3) return true;
        return false;
}

bool Bougie::isLongUpperShadow() const {
    double meche = cours->getHigh() - cours->getLow() ;
    double mecheSup = cours->getHigh() - max(cours->getOpen(),cours->getClose());
    if(mecheSup/meche> double(2)/3) {
        return true;
    }
    else {
        return false;
    }

}

bool Bougie::isMarubozu() const {

    double mecheSup = cours->getHigh() - max(cours->getOpen(),cours->getClose());
    double mecheInf = min(cours->getOpen(), cours->getClose()) - cours->getLow();
    double corps = abs(cours->getOpen() - cours->getClose());
    if(mecheSup<0.005*corps && mecheInf<0.005*corps) return true;
        return false;
}

bool Bougie::isShootingStar() const {
    double corps = abs(cours->getOpen() - cours->getClose());
    double mecheSup = cours->getHigh() - max(cours->getOpen(),cours->getClose());
    double mecheInf = min(cours->getOpen(), cours->getClose()) - cours->getLow();
    if(mecheSup>2*corps && corps*0.1<mecheSup+mecheInf)  return true;
        return false;
}

bool Bougie::isShavenBottom() const {
    if(isInvertedHammer() && ((cours->getOpen()==cours->getLow()) || (cours->getClose()==cours->getLow()))) return true;
        return false;
}

bool Bougie::isShavenHead() const {
    if(isHammer() && ((cours->getHigh()==cours->getOpen() || cours->getHigh()==cours->getClose())))    return true;
        return false;
}

QString Bougie::forme() const {
    if(isMarubozu()) return "Marubozu";
    if(isDragonflyDoji()) return "Dragonfly Doji";
    if(isGraveStoneDoji()) return "Grave stone doji";
    if(isDoji()) return "Doji";
    if(isHanggingMan()) return "Hangging man";
    if(isShavenHead()) return "Shaven head";
    if(isShavenBottom()) return "Shaven bottom";
    if(isShootingStar()) return "Shooting star";
    if(isLongUpperShadow()) return "Long upper shadow";
    if(isLongLowerShadow()) return "Long lower shadow";
    if(isInvertedBlackHammer()) return "Inverted black hammer";
    if(isInvertedHammer()) return "Inverted hammer";
    if(isHammer()) return "Hammer";
    if(isToupie()) return "Toupie";
    return "Pas de forme particulière";
}

void Bougie::analyseForm(){
    QString form = forme();
    form += "\nRSI: " + QString::number(getRSI());
    form += "\nEMA: " + QString::number(getEMA());
    form += "\nMACD: " + QString::number(getMACD());
    QToolTip::showText(QCursor::pos() , form, nullptr, QRect(), 50000);
}





















