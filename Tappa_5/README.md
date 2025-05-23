## Tappa n.5: I Fantasmi

In questa tappa, l'obiettivo è stato concentrarsi sull'implementazione dei **fantasmi** e, soprattutto, del loro **movimento**. Per ora, ho ignorato i loro comportamenti specifici, concentrandomi solo sul farli muovere in modo **casuale** all'interno della mappa.

Ho creato un file generico per i fantasmi (`Ghost`) e poi dei file specifici per ogni fantasma presente nel gioco. Questi file specifici passano al `Ghost` generico la posizione delle diverse texture nello sprite-sheet per l'orientamento di ogni fantasma, rendendo il tutto modulare.

### La Sfida: Far Uscire i Fantasmi di Casa

L'ostacolo principale qui è stato trovare il modo migliore per far **uscire i fantasmi dalla loro "casa"** senza che rimanessero intrappolati.

La soluzione che ho adottato è stata la seguente:

1.  **Memorizzo le Porte:** Salvo in un vettore tutte le tile che considero le "porte" della casa dei fantasmi.
2.  **Calcolo la Via d'Uscita:** Una volta che ho impostato la mappa per un fantasma, calcolo quale di queste tile di uscita è la più vicina alla posizione iniziale del fantasma stesso.
3.  **Gestione dello Stato:** Ogni fantasma ha uno stato che indica se è ancora "in casa" o meno. C'è anche un "dot limit": una soglia di pallini mangiati da Pac-Man che, una volta superata, fa scattare l'uscita del fantasma.
4.  **Uscita Controllata:** Quando questa soglia viene superata, il fantasma esce dalla casa avvicinandosi alla tile di uscita selezionata. Imposto la sua direzione "manualmente" per guidarlo fuori. Una volta uscito, lo stato del fantasma viene cambiato in "non in casa" e, da quel momento in poi, le porte della casa vengono considerate come muri, impedendo al fantasma di rientrare.

---

![Demo con i fantasmi in movimento](images/demo.png)