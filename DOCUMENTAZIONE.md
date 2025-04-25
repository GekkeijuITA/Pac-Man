# Tappa n.1
## Obiettivo
L'obiettivo in questa tappa del progetto "Pac-Man" è quello di creare un sistema di layout della mappa che permetta di visualizzare il labirinto di gioco. Il layout sarà definito in un file e verrà caricato all'avvio del gioco. In seguito, si prevede di implementare un sistema per creare livelli personalizzati.
## Ostacoli
L'ostacolo principale per questa tappa è la definizione del sistema con cui salvare/caricare il layout della mappa. E l'efficienza del caricamento delle texture.
## Soluzione
Per la definizione del layout della mappa, si è deciso di utilizzare un file di testo contentente una matrice di numeri interi da 0 a 9. Ogni numero rappresenta un tipo di cella del labirinto.
Per il caricamento delle texture, si è deciso di caricarle una volta sola all'avvio del gioco e di salvarle in una lista. In questo modo, si evita di caricare le texture ogni volta che si deve disegnare il labirinto.