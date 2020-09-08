#include "trading.h"

//On definit les fonction de nos classes de bases.

Devise::Devise(const QString& c, const QString& m, const QString& z) :
        monnaie(m), zone(z) {
        if (c.size() != 3) throw TradingException("code devise incorrect");
        for (unsigned int i = 0; i < 3; i++) {
                if (c[i] <= 'A' || c[i] >= 'Z') throw TradingException("code devise incorrect");

        }
    code=c;
}
PaireDevises::PaireDevises(const Devise& b, const Devise& c, const QString& s) :
        base(&b), contrepartie(&c), surnom(s) {}

QString PaireDevises::toString() const{
    QString str;
    str=getBase().getCode()+"/"+getContrepartie().getCode();
    if (getSurnom()!="") str+="("+getSurnom()+")";
    return str;
}

void CoursOHLC::setCours(double o, double h, double l, double c,unsigned int v) {
        if (o < 0 || h < 0 || l < 0 || c<0 || l>h) throw TradingException("cours OHLC incorrect");
                open = o; high = h; low = l; close = c; volume = v;
}

CoursOHLC::CoursOHLC(double o, double h, double l, double c, const QDate& d,unsigned int v):date(d),volume(v) {
    if (o < 0 || h < 0 || l < 0 || c<0 || l>h) throw TradingException("cours OHLC incorrect");
        open = o; high = h; low = l; close = c;
}


void EvolutionCours::addCours(double o, double h, double l, double c, const QDate& d,unsigned int v) {
        if (nbMaxCours == nbCours) { // agrandissement du tableau
                CoursOHLC* newtab = new CoursOHLC[nbMaxCours + 100];
                for (unsigned int i = 0; i < nbCours; i++) newtab[i] = cours[i];
                // mise � jour des attributs
                CoursOHLC* old = cours;
                cours = newtab;
                nbMaxCours += 100;
                delete[] old; // destruction de l'ancien tableau
        }
        // ajout du cours
        cours[nbCours].setCours(o, h, l, c, v);
    cours[nbCours].setDate(d);
        nbCours++;}

EvolutionCours::~EvolutionCours() {
        delete[] cours;
}

EvolutionCours::EvolutionCours(const EvolutionCours& e) :
        paire(e.paire),
    cours(new CoursOHLC[e.nbCours]),
        nbCours(e.nbCours),
    nbMaxCours(e.nbCours)
{
        for (unsigned int i = 0; i < nbCours; i++) cours[i] = e.cours[i];
}

EvolutionCours& EvolutionCours::operator=(const EvolutionCours& e) {
        if (this != &e) { // se prot�ger de l'ato-affectation
                paire = e.paire;
                nbCours = 0;
                for (unsigned int i = 0; i < e.nbCours; i++)
            addCours(e.cours[i].getOpen(), e.cours[i].getHigh(),e.cours[i].getLow(), e.cours[i].getClose(),e.cours[i].getDate(),e.cours[i].getVolume());
        }
        return *this;
}




const Devise& DevisesManager::creationDevise(const QString& c, const QString& m, const QString& z) {
        if (nbDevises == nbMaxDevises) { // agrandissement tableau
                Devise** newtab = new Devise*[nbMaxDevises + 10];
                for (unsigned int i = 0; i < nbDevises; i++) newtab[i] = devises[i];
                // mise � jour des attributs
                Devise** old = devises;
                nbMaxDevises += 10;
                devises = newtab;
                delete[] old;
        }
        // c�ration et ajout de la nouvelle devise...
        devises[nbDevises] = new Devise(c, m, z);
        return *devises[nbDevises++];
}

const Devise& DevisesManager::getDevise(const QString& c)const {
        for (unsigned int i = 0; i < nbDevises; i++)
                if (devises[i]->getCode() == c) return *devises[i];
        throw TradingException("devise inexistante");
}

const PaireDevises& DevisesManager::getPaireDevises(const QString & c1, const QString & c2) const {
        const Devise& d1 = getDevise(c1);
        const Devise& d2 = getDevise(c2);
        for (unsigned int i = 0; i < nbPaires; i++)
                if (paires[i]->getBase().getCode() == c1 &&
                        paires[i]->getContrepartie().getCode() == c2)
                        return *paires[i];
        // si la paire de devises n'est pas trouv�e, il faut la cr�er
        if (nbPaires == nbMaxPaires) { // agrandissement du tableau
                PaireDevises** newtab = new PaireDevises*[nbMaxPaires + 10];
                for (unsigned int i = 0; i < nbPaires; i++) newtab[i] = paires[i];
                PaireDevises** old = paires;
                nbMaxPaires += 10;
                paires = newtab;
                delete[] old;
        }
        paires[nbPaires] = new PaireDevises(d1, d2);
        return *paires[nbPaires++];
}

DevisesManager::~DevisesManager() {
        for (unsigned int i = 0; i < nbPaires; i++) delete paires[i];
        for (unsigned int i = 0; i < nbDevises; i++) delete devises[i];
        delete[] paires; delete[] devises;
}

DevisesManager::Handler DevisesManager::handler=
DevisesManager::Handler();
