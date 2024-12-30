//dimesioni dell'esecutore
#define IMM2D_WIDTH 300
#define IMM2D_HEIGHT 150

#define IMM2D_IMPLEMENTATION
#include <iostream>
#include "immediate2d.h"
using namespace std;

//immagine dell'astronave
static constexpr const char astronavepng[] = "immagineastronave.PNG";

void run() {
	//coordinate iniziali dell'astronave (inseguito si modificano)
    int x = 145;
    int y = 100;

	//carica l'immagine dell'astronave
    const Image astronave = LoadImage(astronavepng);
    
    //per un codice dove l'atronave non lampeggia 
    UseDoubleBuffering(true);
	//il ciclo continua finche la variabile esecuzione è true
    bool esecuzione = true;



    while (esecuzione) {
		//l'ultimo tasto premuto permette il movimento dell'astronave
        const char key = LastKey();
        if (key == Left) x -= 5;
        if (key == Right) x += 5;
        if (key == Up) y -= 5;
        if (key == Down) y += 5;

        // cancella la precedente astronave
        Clear(Black);
        
		// Disegna l'immagine alle coordinate alle nuove coordinate
        DrawImage(x, y, astronave);

        
        Present();

        
    }
}

