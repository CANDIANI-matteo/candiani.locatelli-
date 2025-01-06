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
int viteAsteroidi[100];
// Aggiungi un array per tenere traccia dei colpi ricevuti da ciascun asteroide
int colpiRicevuti[100] = { 0 }; // Inizializza a zero

int punti = 0; // Variabile per il punteggio

void Menu() {
    while (true) {

        const Image menu = LoadImage("sfondoHome.jpg");

        // Disegna l'immagine di sfondo del menu
        DrawImage(0, 0, menu);

        // Disegna i messaggi del menu principale
        DrawString(IMM2D_WIDTH /2, IMM2D_HEIGHT /3, "Benvenuto nel Gioco!", "Arial", 24, Magenta, true);
        DrawString(IMM2D_WIDTH /2, IMM2D_HEIGHT  /2, "Premi spazio per iniziare", "Arial", 18, Magenta, true);

        // Presenta il contenuto sullo schermo
        Present();


        //Spazio per iniziare a giocare
        const char key = LastKey();
        if (key==' ') {
            Wait(5); // Breve pausa per evitare input multipli
            break; // Esci dal menu principale
        }

        Wait(10); // Riduce il consumo di CPU
    }
}
void Pause(){

    bool pausa = true; // Imposta inizialmente il gioco in pausa
    // Imposto immagine sfondo
    const Image sfondo = LoadImage(sfondopng);

    DrawImage(0, 0, sfondo);

    DrawString(IMM2D_WIDTH / 2, IMM2D_HEIGHT / 3, "Gioco in pausa!", "Arial", 24, Magenta, true);
    Present();  // Mostra il messaggio di pausa

    // Aspetta che l'utente prema 'P' per riprendere il gioco
    while (LastKey() != 'p' && LastKey() != 'P') {
        Wait(10); // Riduce il carico CPU durante l'attesa
    }

    Clear(Black);  // Rimuove il messaggio di pausa dal display
    Present();     // Rende lo schermo pulito senza la pausa
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
            viteAsteroidi[numeroAsteroidi] = asteroidiGrandi[numeroAsteroidi] ? 100 : 50;
            colpiRicevuti[numeroAsteroidi] = 0; // Inizializza i colpi ricevuti
            numeroAsteroidi++;
        }

        // Movimento dell'astronave usando l'ultimo tasto premuto
        const char key = LastKey();
        if (key == Left) x = x - 5;
        if (key == Right) x = x + 5;
        if (key == Up) y = y - 5;
        if (key == Down) y = y + 5;

        if (key == 'p' || key == 'P') {
            Pause();
        }

        // Premi spazio per sparare
        if (key == ' ' && py < 0) {
            px = x + 12;  // Parte dal centro dell'astronave
            py = y - 10;  // Leggermente sopra la posizione dell'astronave
        }


        // Muove il proiettile verso l'alto
        if (py >= 0) py -= 4;

        //Cancella il proiettile quando esce dallo schermo
        if (px < 0 || py < 0 || px >= IMM2D_WIDTH || py >= IMM2D_HEIGHT) {
            px = -1;
            py = -1;
        }

        
        // Gestisci la collisione del proiettile con gli asteroidi
        for (int i = 0; i < numeroAsteroidi; i++) {
            if (px != -1 && py != -1) { // Se il proiettile è attivo
                int dimensioneAsteroide;

                // Determina la dimensione dell'asteroide
                if (asteroidiGrandi[i]) {
                    dimensioneAsteroide = 20; // Asteroide grande
                }
                else {
                    dimensioneAsteroide = 10; // Asteroide piccolo
                }

                const int tolleranza = 3;  // Un margine di errore per rendere la collisione più permissiva

                // Controllo della collisione tra proiettile e asteroide con tolleranza
                if (px < asteroidiX[i] + dimensioneAsteroide + tolleranza && px + 5 > asteroidiX[i] - tolleranza &&
                    py < asteroidiY[i] + dimensioneAsteroide + tolleranza && py + 5 > asteroidiY[i] - tolleranza) {

                    // Incrementa i colpi ricevuti
                    colpiRicevuti[i]++;

                    // Determina il numero di colpi necessari per distruggere l'asteroide
                    int colpiNecessari;
                    if (asteroidiGrandi[i]) {
                        colpiNecessari = 2; // Due colpi per un asteroide grande
                    }
                    else {
                        colpiNecessari = 1; // Un colpo per un asteroide piccolo
                    }

                    // Se l'asteroide ha ricevuto il numero di colpi necessari
                    if (colpiRicevuti[i] >= colpiNecessari) {
                        // Distruggi l'asteroide
                        asteroidiY[i] = IMM2D_HEIGHT + 1; // Mandi fuori dallo schermo
                        colpiRicevuti[i] = 0; // Resetta i colpi ricevuti
                        if (asteroidiGrandi[i]) {
                            punti += 2;  // Se l'asteroide è grande, assegna 2 punti
                        }
                        else {
                            punti += 1;  // Se l'asteroide è piccolo, assegna 1 punto
                        }
                    }

                    // Disattiva il proiettile per evitare più collisioni
                    px = -1;
                    py = -1;
                }
            }
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

        //Uso c_str coverte la stringa in una array di caratteri

        // Disegna i comandi nell'angolo in basso al centro
        string comandi = "P - Pausa | Spazio - Sparare | Freccette - Movimento";
        DrawString(IMM2D_WIDTH - 150, IMM2D_HEIGHT - 20, comandi.c_str(), "Arial",8, White, true);

        // Usa to_string per convertire il punteggio in stringa
        string punteggio = "Punti: " + to_string(punti);

        // Mostra il punteggio nell'angolo in alto a destra 
        DrawString(IMM2D_WIDTH - 40, 10, punteggio.c_str(), "Arial", 10, White, true);

        //Dichiarazione e inizializzazione
        string gameOver = "GAME OVER";

        // Controlla se l'astronave è uscita dai limiti dello schermo
        if (x < 0 || x >= IMM2D_WIDTH || y < 0 || y >= IMM2D_HEIGHT) {
            // Fine del gioco
            esecuzione = false;

            // Cancella lo schermo
            Clear(Black);

            // Scrive "Hai perso"
            DrawString(IMM2D_WIDTH / 2, IMM2D_HEIGHT / 3, "GAME OVER", "Arial", 24, Red, true);
            //Visualizza il punteggio in basso al centro
             DrawString(IMM2D_WIDTH / 2 , IMM2D_HEIGHT -20, punteggio.c_str(), "Arial", 10, White, true);

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
                DrawString(IMM2D_WIDTH / 2, IMM2D_HEIGHT / 3, "GAME OVER", "Arial", 24, Red, true);
                //Visualizza il punteggio in basso al centro
                DrawString(IMM2D_WIDTH / 2 , IMM2D_HEIGHT -20, punteggio.c_str(), "Arial", 10, White, true);
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


