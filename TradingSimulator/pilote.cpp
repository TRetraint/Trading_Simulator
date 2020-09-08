#include "pilote.h"

//Constructeur de la classe Pilote
Pilote::Pilote(EvolutionViewer *v,QWidget *parent) :
    QWidget (parent){

    message = new QLabel("Bienvenue dans Trading Simulator",this);
    viewer=v;
    connect(viewer->GetSim()->getNote_affiche(), SIGNAL(sauv_note()), this, SLOT(ajuster_window()) );     //Permet de reajuster la fenetre lorsque on ferme l'editeur de text ( feuille d enote).

    user = new User;


    buttonDisplay = new QPushButton("display graphic");
    connect(buttonDisplay, SIGNAL (released()),this, SLOT (handleButtonDisplay())); //connecte le signal émis par le boutton au handler de la lasse Pilote


    buttonFile = new QPushButton("choose file");
    connect(buttonFile, SIGNAL (released()),this, SLOT (handleButtonFile()));

    buttonDate = new QPushButton("choose date");
    connect(buttonDate, SIGNAL (released()),this, SLOT (handleButtonDate()));

    buttonMontant = new QPushButton("choose montant");
    connect(buttonMontant, SIGNAL (released()),this, SLOT (handleButtonMontant()));

    /*!
     * Creation de la partie pour choisir le mode
    */
    mode_titre = new QLabel("Choisissez le Mode");

    /*!
     * Creation des boutton pour choisir le mode manuel ou automatique
    */
    Button_auto = new QPushButton("automatique");
    connect(Button_auto, SIGNAL(released()), this, SLOT(handle_Button_auto()));

    Button_manuel = new QPushButton ("manuel");
    connect(Button_manuel, SIGNAL(released()), this, SLOT(handle_Button_manuel()));

    /*!
     * Creation de la partie affichant le mode en cours.
    */
    mode_choisit_label = new QLabel("Mode CHoisit :");

    mode_choisit_edit = new QLineEdit;
    mode_choisit_edit->setText("Manuel");   //Le mode par defaut est manuel
    mode_choisit_edit->setReadOnly(true);

    /*!
     * Boutton permettant de passer a la suite
    */
    choisir_mode = new QPushButton("choose mode");
    connect(choisir_mode, SIGNAL(released()), this, SLOT(handleChoix_mode()));

    //Mise en page
    mode =  new QWidget;
    mode->setVisible(false);//La partie est invisible jusqu'a ce que on ait choisit le montant

    liste_button_mode = new QHBoxLayout;
    liste_button_mode -> addWidget(Button_manuel);
    liste_button_mode -> addWidget(Button_auto);

    choix_mode = new QHBoxLayout;
    choix_mode->addWidget(mode_choisit_label);
    choix_mode->addWidget(mode_choisit_edit);

    affiche_mode = new QVBoxLayout(mode);
    affiche_mode->addWidget(mode_titre);
    affiche_mode->addLayout(liste_button_mode);
    affiche_mode->addLayout(choix_mode);
    affiche_mode->addWidget(choisir_mode);


    //
    yearBoxMessage = new QLabel("Year");
    monthBoxMessage = new QLabel("Month");
    dayBoxMessage = new QLabel("Day");



    QStringList years;
    QString year;
    for(unsigned int i=0;i<100;i++){
        year=QString::number(2019-i);
        years<<year;
    }

    QStringList months;
    QString month;
    for(unsigned int i=1;i<12;i++){
        month=QString::number(i);
        months<<month;
    }

    QStringList days;
    QString day;
    for(unsigned int i=1;i<31;i++){
        day=QString::number(i);
        days<<day;
    }

   yearBox = new QComboBox;
   yearBox->addItems(years);

   monthBox = new QComboBox;
   monthBox->addItems(months);

   dayBox = new QComboBox;
   dayBox->addItems(days);

   yearLayout = new QVBoxLayout;
   yearLayout->addWidget(yearBoxMessage);
   yearLayout->addWidget(yearBox);


   monthLayout = new QVBoxLayout;
   monthLayout->addWidget(monthBoxMessage);
   monthLayout->addWidget(monthBox);


   dayLayout = new QVBoxLayout;
   dayLayout->addWidget(dayBoxMessage);
   dayLayout->addWidget(dayBox);


   dateTotal = new QWidget;

   dateForm = new QHBoxLayout(dateTotal);
   dateForm->addLayout(dayLayout);
   dateForm->addLayout(monthLayout);
   dateForm->addLayout(yearLayout);

   montantBaseMessage = new QLabel("Montant devie de base");
   montantContrepartieMessage = new QLabel("Montant devise de contrepartie");


   montantBase = new QLineEdit;
   montantBase->setText("0");

   montantContrepartie = new QLineEdit;
   montantContrepartie->setText("1000000");



   montantContrepartieLayout= new QVBoxLayout;
   montantContrepartieLayout->addWidget(montantContrepartieMessage);
   montantContrepartieLayout->addWidget(montantContrepartie);

   montantBaseLayout= new QVBoxLayout;
   montantBaseLayout->addWidget(montantBaseMessage);
   montantBaseLayout->addWidget(montantBase);

   montantTotal = new QWidget;
   montantForm = new QHBoxLayout(montantTotal);

   montantForm->addLayout(montantBaseLayout);
   montantForm->addLayout(montantContrepartieLayout);

    //Mise en page
   fenetre = new QVBoxLayout;
   window = new QWidget;
   window2 = new QWidget;

   fenetre->addWidget(message);
   fenetre->addWidget(buttonFile);
   fenetre->addWidget(dateTotal);
   fenetre->addWidget(buttonDate);
   fenetre->addWidget(montantTotal);
   fenetre->addWidget(buttonMontant);
   fenetre->addWidget(mode);
   fenetre->addWidget(buttonDisplay);
   window->setLayout(fenetre);

   total = new QVBoxLayout;
   total->addWidget(viewer->GetWindow());
   total->addWidget(viewer->getRsiViewer()->GetWindow());
   total->addWidget(viewer->getMacdViewer()->GetWindow());
   total->addWidget(viewer->getEmaViewer()->GetWindow());
   total->addWidget(viewer->getVolumeViewer()->GetWindow());
   window2->setLayout(total);
   montantTotal->setVisible(false);
   buttonMontant->setVisible(false);
   buttonDisplay->setVisible(false);
   buttonDate->setVisible(false);
   dateTotal->setVisible(false);
}

//Méthode permettant de charger les données d'un fichier dans l'application, on l'appelle en donnant le chemin de celui-ci, les délimiteurs du fichier entre les colonnes et entre les mois,jours et années dans la date
void Pilote::LoadFile(QString path,QChar delim, QChar delimd, QDate dateCondition){

    QString ligne,date; //permet de récupérer la première ligne du fichier
    QDate dateLigne; //recevra la qdate correspondant à chaque ligne
    QString y,m,d,o,h,l,c,adj,v;
    int i=0; // sert à compter le nombre de cours ajoutés
    bool found=false; // sert à savoir si on a trouvé le premier jour postérieur à la date demandée
    QFile file(path); //On ouvre le fichier en lecture
    EvolutionCours *cours=viewer->GetEvolution();
    RSI *rsi=viewer->getRSI();
    EMA *ema=viewer->getEMA();
    EMA *ema1=viewer->getEMA1();
    EMA *ema2=viewer->getEMA2();
    MACD* macd=viewer->getMACD();
    if(file.open(QIODevice::ReadOnly)) //si l'ouverture a réussi
    {
        ligne=file.readLine(); //On met la première ligne du fichier dans "ligne"
        while(!file.atEnd()){
            ligne=file.readLine();
            date=ligne.section(delim,0,0); // on coupe la string entre les délimiteurs, ce qui nous permet d'isoler les données que l'on souhaite
            y=date.section(delimd,0,0);
            m=date.section(delimd,1,1);
            d=date.section(delimd,2,2);
            o=ligne.section(delim,1,1);
            h=ligne.section(delim,2,2);
            l=ligne.section(delim,3,3);
            c=ligne.section(delim,4,4);
            adj=ligne.section(delim,5,5);
            v=ligne.section(delim,6,6);
            dateLigne = QDate(y.toInt(),m.toInt(),d.toInt());

            cours->addCours(o.toDouble(),h.toDouble(),l.toDouble(),c.toDouble(),dateLigne,v.toUInt());
            rsi->addIndicateur(0,dateLigne);
            ema->addIndicateur(0,dateLigne);
            ema1->addIndicateur(0,dateLigne);
            ema2->addIndicateur(0,dateLigne);
            macd->addIndicateur(0,dateLigne);
            i++;

            if(!found && dateLigne>dateCondition){
                viewer->setLast(i);
                found=true;
            }
        }
        //On calcul les indicateurs correspondans aux cours
        this->viewer->getRSI()->Calcul(this->viewer->GetEvolution());
        this->viewer->getEMA()->Calcul(this->viewer->GetEvolution(),9);
        this->viewer->getMACD()->Calcul(this->viewer->GetEvolution(),ema1,ema2);
        //On update tous les viewers
        this->viewer->Update(viewer->getLast()); //On met à jour les graphiques avec les nouvelles données de l'objet EvolutionCours qu'ils contiennent;
        this->viewer->getRsiViewer()->Update(viewer->getRSI(),viewer->getLast());
        this->viewer->getEmaViewer()->UpdateEMA(viewer->getEMA(),viewer->getLast());
        this->viewer->getMacdViewer()->UpdateMACD(viewer->getMACD(),viewer->getLast());


    }
   else{
        QMessageBox::critical(nullptr, "Erreur"," Impossible d'ouvrir le fichier");
    }
    file.close();  // on ferme le fichier
}
