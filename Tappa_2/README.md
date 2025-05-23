# Tappa n.2: Il Sistema di Disegno della Mappa

In questa tappa, mi sono concentrato sulla creazione del **sistema di disegno per la mappa** del gioco. L'obiettivo era permettere di visualizzare il mondo di gioco e tutte le sue caratteristiche in modo chiaro. Ho progettato questo sistema per essere il più **modulare e facilmente estendibile** possibile, pensando già a tutte le funzionalità che avremmo potuto aggiungere in futuro.

### La Sfida della Visualizzazione e la Soluzione

Durante lo sviluppo, ho incontrato un problema con la visualizzazione della mappa: **usciva dal campo visivo del giocatore**.

Per risolvere questo problema, ho utilizzato una **vista** che ha il compito di "comprimere" la mappa, facendola rientrare perfettamente nell'area visibile al giocatore.

---

![Sistema Mappa in azione](images/map-system.png)