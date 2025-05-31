# Tappa n.9: Le Schermate di Gioco

L'obiettivo di questa tappa del progetto Pac-Man è stato implementare le **varie schermate** che scandiscono l'esperienza di gioco: il menu principale, la schermata di pausa, quella di game over e quella schermata di vittoria.

## Riorganizzazione del Codice e Gestione degli Stati

Per gestire al meglio queste nuove schermate e le diverse fasi del gioco, ho ulteriormente **suddiviso le responsabilità** all'interno del codice. Adesso c'è un file che gestisce lo **stato della finestra** (quindi se siamo in gioco, nel menu principale, ecc.). Ho anche creato un file dedicato al **Debug**, che disegna una griglia nella finestra.

Come conseguenza di questa riorganizzazione, ho creato i file che rappresentano lo **stato di Gioco** (`GameState`, che in realtà è stato un cambio di nome per la logica di gioco esistente) e `MainMenuState` per il menu principale. Ho anche sviluppato un file `GameMenu` che si occupa di disegnare un menu come **overlay di gioco**; questo lo uso per il menu di pausa e per quello di vittoria. Per il game over, invece, ho optato per il metodo classico: scrivere la scritta direttamente sulla mappa, mantenendo un'estetica fedele all'originale.

![Menu Principale](images/mainmenu.png)
![Game Over](images/gameover.png)
![Menu di Pausa](images/pausemenu.png)
![Schermata di Vittoria](images/victorymenu.png)

---
**Fonti utilizzate in questa Tappa:**
Nessuna fonte esterna è stata utilizzata per questa tappa, in quanto si tratta principalmente di riorganizzazione del codice e implementazione di funzionalità che usano competenze già acquisite nelle tappe precedenti.
---