#ifndef INDICATEUR_H
#define INDICATEUR_H
#include"includes.h"
#include "trading.h"

//Classe représentant l'indicateur pour une bougie : elle possède une valeur et une date
class Indicateur {
    double value;
    QDate date;
public:
    Indicateur(double v,const QDate& d);
    Indicateur() {}
    unsigned int volume;
    double getValue() const {return value; }
    void setValue(double v){value=v;}
    QDate getDate() const { return date; }
    void setDate(const QDate& d) { date=d; }
};

//Classe représentant une suite d'indicateurs, dans le cas de l'application, elle contient les indicateurs pour tous les cours d'un fichier, elle est dotée d'un iterateur sur la classe Indicateur
class EvolutionIndicateurs{
protected:
    Indicateur* indicateurs = nullptr;
    unsigned int nbIndicateurs = 0;
    unsigned int nbMaxIndicateurs = 0;
public:
    EvolutionIndicateurs(){}
    void addIndicateur(double v,const QDate& d);
    ~EvolutionIndicateurs();
    unsigned int getNbIndicateurs() const { return nbIndicateurs; }
    using iterator = Indicateur*;
    iterator begin() { return iterator(indicateurs); }
    iterator end() { return iterator(indicateurs + nbIndicateurs); }
    using const_iterator = const Indicateur*;
    const_iterator begin() const { return indicateurs; }
    const_iterator end() const { return indicateurs + nbIndicateurs; }
    const_iterator cbegin() const { return indicateurs; }
    const_iterator cend() const { return indicateurs + nbIndicateurs; }

};


//Classe héritant de EvolutionIndicateurs et représentant le RSI
class RSI : public EvolutionIndicateurs{
private:
    unsigned int lookback;
public:
    RSI(unsigned int lbp){lookback=lbp;}
    void Calcul(EvolutionCours* cours);

};

//Classe héritant de EvolutionIndicateurs et représentant l'EMA
class EMA : public EvolutionIndicateurs{
private:
    unsigned int period;
public:
    EMA(unsigned int per){period=per;}
    double* Calcul(EvolutionCours *cours,unsigned int period);
    void setPeriod(unsigned int p){period=p;}

};

//Classe héritant de EvolutionIndicateurs et représentant le MACD
class MACD : public EvolutionIndicateurs{
private:
    unsigned int shortPeriod;
    unsigned int longPeriod;
public:
    MACD(unsigned int sPer, unsigned int lPer){shortPeriod=sPer;longPeriod=lPer;}
    void Calcul(EvolutionCours* cours,EMA* ema1,EMA* ema2);
};







#endif // INDICATEUR_H
