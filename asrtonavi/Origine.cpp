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

// Per memorizzare gli asteroidi
int asteroidiX[100];
int asteroidiY[100];
Image asteroidiImg[100];
bool asteroidiGrandi[100];
int numeroAsteroidi = 0;

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

    srand(time(0)); // Inizializza il generatore di numeri casuali

    while (esecuzione) {


        // Genera nuovi asteroidi casualmente
        if (rand() % 30 == 0 && numeroAsteroidi < 100) {
            asteroidiX[numeroAsteroidi] = rand() % IMM2D_WIDTH;
            asteroidiY[numeroAsteroidi] = 0;
            asteroidiGrandi[numeroAsteroidi] = rand() % 2 == 0;
            asteroidiImg[numeroAsteroidi] = asteroidiGrandi[numeroAsteroidi] ? meteoritegrande : meteoritepiccolo;
            numeroAsteroidi++;
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
        for (int i = 0; i < numeroAsteroidi; i++) {
            asteroidiY[i] += 1; // Velocità di discesa
            if (asteroidiGrandi[i]) {
                DrawImage(asteroidiX[i], asteroidiY[i], asteroidiImg[i]); // Dimensione maggiore per meteoriti grandi
            }
            else {
                DrawImage(asteroidiX[i], asteroidiY[i], asteroidiImg[i]); // Dimensione ridotta per meteoriti piccoli
            }
            Wait(3); // Perché se no scendono troppo velocemente
        }

        // Disegna l'immagine dell'astronave alle nuove coordinate
        DrawImage(x, y, astronave);

        // Controlla se l'astronave è uscita dai limiti dello schermo
        if (x < 0 || x >= IMM2D_WIDTH || y < 0 || y >= IMM2D_HEIGHT) {
            // Fine del gioco
            esecuzione = false;

            // Cancella lo schermo
            Clear(Black);

            // Scrive "Hai perso"
            DrawString(IMM2D_WIDTH / 2, IMM2D_HEIGHT / 2, "GAME OVER", "Arial", 24, Red, true);

            // Stampa il messaggio
            Present();
        }

        // Controlla se l'astronave è stata colpita da un asteroide
        for (int i = 0; i < numeroAsteroidi; i++) {
            int dimensioneAsteroide = asteroidiGrandi[i] ? 20 : 10;
            if (x < asteroidiX[i] + dimensioneAsteroide && x + 20 > asteroidiX[i] && y < asteroidiY[i] + dimensioneAsteroide && y + 20 > asteroidiY[i]) {
                // Fine del gioco
                esecuzione = false;

                // Cancella lo schermo
                Clear(Black);

                // Scrive "Hai perso"
                DrawString(IMM2D_WIDTH / 2, IMM2D_HEIGHT / 2, "GAME OVER", "Arial", 24, Red, true);

                // Stampa il messaggio
                Present();
            }
        }

        // Presenta il disegno sullo schermo
        Present();

        // Rimuovi gli asteroidi che sono usciti dallo schermo
        int nuovoNumeroAsteroidi = 0;
        for (int i = 0; i < numeroAsteroidi; i++) {
            if (asteroidiY[i] <= IMM2D_HEIGHT) {
                asteroidiX[nuovoNumeroAsteroidi] = asteroidiX[i];
                asteroidiY[nuovoNumeroAsteroidi] = asteroidiY[i];
                asteroidiImg[nuovoNumeroAsteroidi] = asteroidiImg[i];
                asteroidiGrandi[nuovoNumeroAsteroidi] = asteroidiGrandi[i];
                nuovoNumeroAsteroidi++;
            }
        }
        numeroAsteroidi = nuovoNumeroAsteroidi;
    }



}

int main() {
    // Esegui il gioco
    run();
    return 0;
}


