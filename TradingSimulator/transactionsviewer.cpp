#ifndef TRANSACTIONSVIEWER_CPP
#define TRANSACTIONSVIEWER_CPP
#include "transactionsviewer.h"

//Constructeur de la classe TransactionsViewer
TransactionsViewer::TransactionsViewer(EvolutionCours* c, User* u,int* l){
    user = u;
    cours = c;
    last = l;
    listeTransactions = new QVBoxLayout;

    buttonBuy = new QPushButton("Buy");
    connect(buttonBuy, SIGNAL (released()),this, SLOT (handleButtonBuy()));

    buttonSell = new QPushButton("Sell");
    connect(buttonSell, SIGNAL (released()),this, SLOT (handleButtonSell()));

    montantBaseOwnedMessage = new QLabel ("montant devise base Owned");
    montantContrepartieOwnedMessage = new QLabel("montant devise contrepartie Owned");
    montantTotalOwnedMessage = new QLabel("montant devise contrepartie Owned");


    montantTotalOwned = new QLineEdit;
    montantTotalOwned->setReadOnly(true);

    montantContrepartieOwned = new QLineEdit;
    montantContrepartieOwned->setReadOnly(true);



    montantBaseOwned = new QLineEdit;
    montantBaseOwned->setReadOnly(true);

    //Creation des attribut du mode automatique
    auto_begin = new QPushButton("begin");
    connect(auto_begin, SIGNAL (released()),this, SLOT (handle_automatique())); //Permet de demarer la simulation automatique.

    selection = new QComboBox;  //Permet la seleciton de la fonction a utiliser.
    selection->addItem("Variation_1Bougie");
    selection->addItem("Variation_3Bougie");
    selection->addItem("Variation_5Bougie");
    selection->addItem("tendance_5Bougie");
    selection->addItem("tendance_10Bougie");
    selection->addItem("Comparaison_mtn_10bougie");

    //On creer un liste d'entier permettant de choisir le nombre de tansaction a faire ( 1 par jour).
    QStringList jours;
    QString jour;
    for(unsigned int i=1;i<100;i++){
        jour=QString::number(i);
        jours<<jour;
    }
    nombre_jour = new QComboBox;
    nombre_jour->addItems(jours);


    montantTotalOwnedMessage = new QLabel("Montant total");
    montantContrepartieMessage = new QLabel("Montant devise contrepartie");

    montantContrepartie = new QLineEdit;

    //Mise en page
    baseTotal = new QVBoxLayout;
    contrepartieTotal = new QVBoxLayout;

    contrepartieTotal->addWidget(montantContrepartieMessage);
    contrepartieTotal->addWidget(montantContrepartie);

    form = new QHBoxLayout;
    form->addLayout(contrepartieTotal);
    form->addWidget(buttonBuy);
    form->addWidget(buttonSell);
    form->addWidget(montantContrepartieOwnedMessage);
    form->addWidget(montantContrepartieOwned);
    form->addWidget(montantBaseOwnedMessage);
    form->addWidget(montantBaseOwned);
    form->addWidget(montantTotalOwnedMessage);
    form->addWidget(montantTotalOwned);
    form->addWidget(selection);
    form->addWidget(nombre_jour);
    form->addWidget(auto_begin);

    fenetre = new QVBoxLayout;

    fenetre->addLayout(form);
    fenetre->addLayout(listeTransactions);
    window = new QWidget;
    window->setLayout(fenetre);
}

//Permet d'update l'affichage du montant de devise de base l'utilisateur, cette méthode est appelée lorsque l'on change ce montant
void TransactionsViewer::updateMontantBaseOwned(double oldBase){
    if(oldBase>0){
        montantBaseOwned->setText(QString::number(user->getMontantBase())+"("+QString::number(user->getMontantBase()-oldBase)+")");}
    else{
        montantBaseOwned->setText(QString::number(user->getMontantBase()));}}

//Permet d'update l'affichage du montant de devise de contrepartie de l'utilisateur, cette méthode est appelée lorsque l'on change ce montant
void TransactionsViewer::updateMontantContrepartieOwned(double oldContrepartie){
    if(oldContrepartie>0){
        montantContrepartieOwned->setText(QString::number(user->getMontantContrepartie())+"("+QString::number(user->getMontantContrepartie()-oldContrepartie)+")");}
    else{
        montantContrepartieOwned->setText(QString::number(user->getMontantContrepartie()));}}

//Permet d'ajouter une transaction dans l'affichage
void TransactionsViewer::addTransaction(Transaction* transaction,bool type) {
        if (nbMaxTransactions == nbTransactions) { // agrandissement du tableau
                QLineEdit** newtab = new QLineEdit*[nbMaxTransactions + 100];
                for (unsigned int i = 0; i < nbTransactions; i++) newtab[i] = transactions[i];
                // mise � jour des attributs
                QLineEdit** old = transactions;
                transactions = newtab;
                nbMaxTransactions += 100;
                delete[] old; // destruction de l'ancien tableau
        }
        transactions[nbTransactions] = new QLineEdit;

        transactions[nbTransactions]->setText(transaction->toString(type));
        transactions[nbTransactions]->setReadOnly(true);

        listeTransactions->addWidget(transactions[nbTransactions]);
        nbTransactions++;}

//Permet de supprimer toutes les transactions affichées, la méthode est utilisée quand on clique sur previous day afin d'actualiser la listes des transactions
void TransactionsViewer::deleteTransactions(){
    iterator it=begin();
    while(it!=end()){
        delete *it;
        it++;
    }
nbTransactions=0;
}

//Permet d'update la liste des transactions affichées par rapport à la liste des transactions présente dans l'objet User
void TransactionsViewer::updateTransactions() {
    deleteTransactions();
    EvolutionTransactions::iterator it=user->getEvolution()->begin();
    while(it!=user->getEvolution()->end()){
        addTransaction(it,it->getType());
        it++;
    }
    window->adjustSize();
}



void TransactionsViewer::handleButtonSell(){

    EvolutionCours::iterator it=cours->begin()+(*last)-1;
    double oldBase = user->getMontantBase();
    double oldContrepartie = user->getMontantContrepartie();
    bool result=user->sell(montantContrepartie->text().toDouble(),it->getOpen(),it->getDate());
    updateMontantBaseOwned(oldBase);
    updateMontantContrepartieOwned(oldContrepartie);
    if(result){ // Si la vente a réussi, on affiche la transaction
    addTransaction(user->getEvolution()->end()-1,false);}
    updateMontantTotalOwned();

}

void TransactionsViewer::handleButtonBuy(){

    EvolutionCours::iterator it=cours->begin()+(*last)-1;
    double oldBase = user->getMontantBase();
    double oldContrepartie = user->getMontantContrepartie();

    bool result=user->buy(montantContrepartie->text().toDouble(),it->getOpen(),it->getDate());
    updateMontantBaseOwned(oldBase);
    updateMontantContrepartieOwned(oldContrepartie);
    if(result){ // Si l'achat a réussi, on affiche la transaction
    addTransaction(user->getEvolution()->end()-1,true);}
    updateMontantTotalOwned();
}

//Permet la mise a jour du mode slectionner . Automatique ou manuel.
void TransactionsViewer::update_mode(){
    if (mode_choisit){//Si on choisit le mode auto, on cache les widget du mode manuel.
        nombre_jour->setVisible(true);
        selection->setVisible(true);
        auto_begin->setVisible(true);
        buttonBuy->setVisible(false);
        buttonSell->setVisible(false);
        montantContrepartieMessage->setVisible(false);
        montantContrepartie->setVisible(false);
    }else {//on choisit mode manuel, on cache les widget du mode automatique.
        nombre_jour->setVisible(false);
        selection->setVisible(false);
        auto_begin->setVisible(false);
        buttonBuy->setVisible(true);
        buttonSell->setVisible(true);
        montantContrepartieMessage->setVisible(true);
        montantContrepartie->setVisible(true);
    }
}

//Pour les transaction en automatique. On effectue la transaction du jour en se basant sur les données du lendemain, car les données de la journée en cour ne sont pas encore disponnible

//On regarde la derniere bougie, et on achete si elle est decroissante
int TransactionsViewer::Variation_1Bougie(){
    EvolutionCours::iterator it=cours->begin()+(*last)-1;//pointe sur la derniere bougie affiché ( donc celle ou on n'affiche que le prix d'ouverture)
    it --;  //pointe sur la derniere bougie complete affiché ( celle d'hier).

    if (it->getClose() < it->getOpen()){//Cas decroissant
        return 1;//on achete
    }else {
        return 0; //On vend
    }
}

//On regarde les 3 dernière bougie est on achete si les 3 sont decroissante ( de meme pour vente). Sinon on ne fait rien.
int TransactionsViewer::Variation_3Bougie(){
   EvolutionCours::iterator it=cours->begin()+(*last)-3;    //pointe sur la derniere bougie affiché -2, car on souhaite regarder les 3 derniere bougie
    it --;  //pointe sur la bougie de 3j avant.
    int tendance=0;
    for (int j=0; j<3;j++){ //On balaye les 3 dernieres bougies
        if ((it+j)->getClose() < (it+j)->getOpen()){//Cas decroissant
            tendance ++;
        }else{
            tendance --;
        }
    }
    if (tendance == 3){ //Les 3dernieres bougies sont toute decroissantes.
        return 1;   //on achete
    }else if(tendance == -3){
        return 0;   //on vend
    }else {
        return -1;  //on ne fait aucune transaction.
    }
}

//On regarde les 5 dernière bougie est on achete si les 5 sont decroissante ( de meme pour vente). Sinon on ne fait rien.
int TransactionsViewer::Variation_5Bougie(){
   EvolutionCours::iterator it=cours->begin()+(*last)-5;    //pointe sur la derniere bougie affiché -5, car on souhaite regarder les 5 derniere bougie
    it --;  //pointe sur la bougie de 5j avant.

    int tendance=0;
    for (int j=0; j<5;j++){//On balaye les 5 dernieres bougies
        if ((it+j)->getClose() < (it+j)->getOpen()){//Cas decroissant
            tendance ++;
        }else{
            tendance --;
        }
    }
    if (tendance == 5){     //Les 5 dernieres bougies sont toute decroissantes.
        return 1;   //on achete
    }else if(tendance == -5){
        return 0;   //on vend
    }else {
        return -1;  //on ne fait aucune transaction.
    }
}

//On regarde la tendance sur 5 derniere bougie. On achete si la tendance est a la baisse, on vend sinon. Dans tous les cas il y aura une transaction.
int TransactionsViewer::tendance_5Bougie(){
    EvolutionCours::iterator it=cours->begin()+(*last)-5;
    it --;  //pointe sur la bougie de 5j avant.

    int tendance=0;
    for (int j=0; j<5;j++){
        if ((it+j)->getClose() < (it+j)->getOpen()){//Cas decroissant
            tendance ++;
        }else {
            tendance --;
        }
    }
    if (tendance > 0){ //tendance decroissante
        return 1;   //achat
    }else{
        return 0;   //vente
    }
}

//On regarde la tendance sur 10 derniere bougie. On achete si la tendance est a la baisse, on vend sinon. Dans tous les cas il y aura une transaction.
int TransactionsViewer::tendance_10Bougie(){
    EvolutionCours::iterator it=cours->begin()+(*last)-10;
    it --;  //pointe sur la bougie de 5j avant.

    int tendance=0;
    for (int j=0; j<10;j++){
        if ((it+j)->getClose() < (it+j)->getOpen()){//Cas decroissant
            tendance ++;
        }else {
            tendance --;
        }
    }
    if (tendance > 0){ //tendance decroissante
        return 1;   //achat
    }else{
        return 0;   //vente
    }
}

//On regarde l'evolution entre hier et il y a 10 jours. On achete si c'est decroissante. On vend sinon.
int TransactionsViewer::Comparaison_mtn_10bougie(){
    EvolutionCours::iterator it=cours->begin()+(*last)-10;
    it --;  //pointe sur la bougie de 10j avant.

    if ((it+9)->getClose() < it->getOpen()){//Cas decroissant
        return 1;   //achat
    }else{
        return 0;   //vente
    }
}


/*Remarque les achat se font toujours par 1000*/
//Permet de lancer la simulation automatique
void TransactionsViewer::handle_automatique(){

    EvolutionCours::iterator it=cours->begin()+(*last)-1;
    double oldBase = user->getMontantBase();
    double oldContrepartie = user->getMontantContrepartie();

    int valeur_retour=-1;   //-1: pas de trasaction  /  1: achat  /  0: vente

    /*Appel de la Fonction choisit*/
    /*retourne 1 si il faut acheter 0 sinon*/
    int index = selection->currentIndex() +1;   //Car index commence a 0 , et non 1. Il contient l'index de la fonction choisit.

    switch (index) {
        case 1 :{
            valeur_retour = Variation_1Bougie();
            break;
        }
        case 2 :{
            valeur_retour = Variation_3Bougie();
            break;
        }
        case 3 :{
            valeur_retour = Variation_5Bougie();
            break;
        }
        case 4 :{
            valeur_retour = tendance_5Bougie();
            break;
        }
        case 5 :{
            valeur_retour = tendance_10Bougie();
            break;
        }
        case 6 :{
            valeur_retour = Comparaison_mtn_10bougie();
            break;
        }
    };

    //Boucle pour effectuer plusieur transaction selon le nombre de jours choisit ( 1 transaction par jour).
    for (int i=0; i < nombre_jour->currentText().toInt() ; i++){
        switch (valeur_retour){
            case 1:{
                user->buy(1000,it->getOpen(),it->getDate());
                updateMontantBaseOwned(oldBase);
                updateMontantContrepartieOwned(oldContrepartie);
                if(user->getMontantBase()>=1000/it->getOpen()){
                addTransaction(user->getEvolution()->end()-1,true);}
                break;
            }
            case 0:{
                user->sell(1000,it->getOpen(),it->getDate());
                updateMontantBaseOwned(oldBase);
                updateMontantContrepartieOwned(oldContrepartie);
                if(user->getMontantContrepartie()>=1000){
                addTransaction(user->getEvolution()->end()-1,false);}
                break;
            }
            case -1:{
                break;
            }
        }
        emit(nextDay());    //On passe au jour suivant.
    }
}

#endif // TRANSACTIONSVIEWER_CPP
