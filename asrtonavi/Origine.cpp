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

void Menu() {
    while (true) {
        // Cancella lo schermo con il colore nero
        Clear(Black);

        // Disegna i messaggi del menu principale
        DrawString(IMM2D_WIDTH /2, IMM2D_HEIGHT /3, "Benvenuto nel Gioco!", "Arial", 24, White, true);
        DrawString(IMM2D_WIDTH /2, IMM2D_HEIGHT  /2, "Premi spazio per iniziare", "Arial", 18, White, true);

        // Presenta il contenuto sullo schermo
        Present();

        //Spazio per iniziare a giocare
        const char key = LastKey();
        if (key==' ') {
            Wait(500); // Breve pausa per evitare input multipli
            break; // Esci dal menu principale
        }

        Wait(10); // Riduce il consumo di CPU
    }
}
void run() {

    Menu();
    // Imposto immagine sfondo
    const Image sfondo = LoadImage(sfondopng);

    // Coordinate iniziali dell'astronave
    int x = 150;
    int y = 100;

    // Coordinate iniziali del proiettile (-1 indica che il proiettile non esiste)
    int px = -1;
    int py = -1;

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

        // Spara un proiettile con il tasto spazio
        if (key == ' ' && py < 0) {
            px = x + 12;  // Parte dal centro dell'astronave
            py = y - 10;  // Leggermente sopra la posizione dell'astronave
        }

        // Muove il proiettile verso l'alto
        if (py >= 0) py -= 5;

        // Cancella il proiettile quando esce dallo schermo
        if (py < 0) {
            px = -1;
            py = -1;
        }

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

        //Immagine proiettili
        const Image proiettileImg = LoadImage("unnamed.png");

         // Disegna il proiettile se è attivo
         if (py >= 0) {
             DrawImage(px, py, proiettileImg); // Disegna l'immagine
         }

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


