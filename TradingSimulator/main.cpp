#include "evolutionviewer.h"
#include "includes.h"
#include "indicateur.h"
#include "indicateursviewer.h"
#include "note.h"
#include "pilote.h"
#include "trading.h"
#include "transactionsviewer.h"
#include "user.h"
#include "volumeviewer.h"

int main(int argc, char* argv[]) {
    DevisesManager& m=DevisesManager::getManager();
    m.creationDevise("USD", "Dollar Américain","USA");
    m.creationDevise("BTC", "BitCoin","monnaie virtuelle");
    const PaireDevises& BTC_USD=m.getPaireDevises("BTC", "USD");
    EvolutionCours e(BTC_USD);

    QApplication app(argc, argv);

    EvolutionViewer v(e);

    Pilote p(&v);

    p.GetWindow()->show();
    return app.exec();
}
