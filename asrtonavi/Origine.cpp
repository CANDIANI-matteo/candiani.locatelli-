#define IMM2D_WIDTH 300
#define IMM2D_HEIGHT 150

#define IMM2D_IMPLEMENTATION
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "immediate2d.h"
using namespace std;

// Immagine sfondo
static constexpr const char sfondopng[] = "immaginesfondo.png";
// Immagine dell'astronave
static constexpr const char astronavepng[] = "immagineastronave.PNG";
// Immagine meteorite piccolo
static constexpr const char meteoritepiccolopng[] = "immaginemeteoritepiccolo.png";
// Immagine meteorite grande
static constexpr const char meteoritegrandepng[] = "immaginemeteoritegrande.png";

// Struct per gestire gli asteroidi
struct Asteroide {
    int x, y; // Coordinate dell'asteroide
    Image img; // Immagine dell'asteroide
};

void run() {
    // Imposto immagine sfondo
    const Image sfondo = LoadImage(sfondopng);

    // Coordinate iniziali dell'astronave
    int x = 150;
    int y = 100;

    // Carica le immagini dell'astronave e dei meteoriti
    const Image astronave = LoadImage(astronavepng);
    const Image meteoritepiccolo = LoadImage(meteoritepiccolopng);
    const Image meteoritegrande = LoadImage(meteoritegrandepng);

    // Per evitare che l'astronave lampeggi
    UseDoubleBuffering(true);

    // Variabile per tenere il ciclo in esecuzione
    bool esecuzione = true;

    // Per memorizzare gli asteroidi
    vector<Asteroide> asteroidi;
    srand(time(0)); // Inizializza il generatore di numeri casuali

    while (esecuzione) {
        // Genera nuovi asteroidi casualmente
        if (rand() % 30 == 0) {
            Asteroide nuovoAsteroide;
            nuovoAsteroide.x = rand() % IMM2D_WIDTH;
            nuovoAsteroide.y = 0;
            nuovoAsteroide.img = (rand() % 2 == 0) ? meteoritepiccolo : meteoritegrande;
            asteroidi.push_back(nuovoAsteroide);
        }

        // Movimento dell'astronave usando l'ultimo tasto premuto
        const char key = LastKey();
        if (key == Left) x = x - 5;
        if (key == Right) x = x + 5;
        if (key == Up) y = y - 5;
        if (key == Down) y = y + 5;

        // Cancella lo schermo precedente
        Clear(Black);

        // Disegna l'immagine di sfondo
        DrawImage(0, 0, sfondo);

        // Aggiorna e disegna gli asteroidi
        for (auto& asteroide : asteroidi) {
            asteroide.y += 1; // Velocità di discesa
            DrawImage(asteroide.x, asteroide.y, asteroide.img);
            Wait(10); // Perché se no scende troppo velocemente
        }

        // Disegna l'immagine dell'astronave alle nuove coordinate
        DrawImage(x, y, astronave);

        // Presenta il disegno sullo schermo
        Present();

        // Controlla se l'astronave è uscita dai limiti dello schermo
        if (x < 0 || x >= IMM2D_WIDTH || y < 0 || y >= IMM2D_HEIGHT) {
            // Fine del gioco
            esecuzione = false;

            // Cancella lo schermo
            Clear(Black);

            // Scrive "Hai perso"
            DrawString(IMM2D_WIDTH / 2, IMM2D_HEIGHT / 2, "Hai perso", "Arial", 24, Red, true);

            // Stampa il messaggio
            Present();
        }

        // Rimuovi gli asteroidi che sono usciti dallo schermo
        asteroidi.erase(remove_if(asteroidi.begin(), asteroidi.end(), [](const Asteroide& a) {
            return a.y > IMM2D_HEIGHT;
            }), asteroidi.end());
    }
}
