# Tappa n.15: Editor di scenari
L'obbiettivo di questa tappa è quella di implementare un editor di scenari: ovvero uno strumento che consenta di creare delle "playlist" di mappe da giocare in sequenza con la possibilità di scegliere se giocare una sequenza in loop (fino al livello 256) oppure no.

## Specifiche
- **Creazione di playlist di mappe**: gli utenti possono selezionare una sequenza di mappe da giocare in un ordine specifico. Salvando all'utente verrà chiesto, oltre al nome della playlist, se è uno scenario in loop oppure no.
- **Salvataggio degli scenari**: le playlist vengono salvate in un file di testo (ad esempio `my_scenario.txt`) con il seguente formato:
    ```
    1
    classic.txt,map_1.txt,...
    ```
    Dove la prima riga indica se lo scenario è in loop (1) o no (0), e la seconda riga contiene i nomi delle mappe separate da virgole.
- **Cambiamenti nel menu principale**: è stata aggiunta una nuova voce di menu per accedere all'editor di scenari. Inoltre, quando si seleziona "Gioca", viene chiesto se si vuole giocare uno scenario o una singola mappa.
- **Gestione del mouse nei menu**: è stato introdotto il supporto completo al mouse per tutti i menu (menu principale, menu di gioco, editor di mappe e editor di scenari).  
  Gli utenti possono ora navigare le opzioni passando il cursore sopra le voci e selezionarle tramite click sinistro.  
  La gestione degli eventi è stata centralizzata nello `StateManager`, che smista i movimenti e i click del mouse allo stato corretto.  
  Inoltre, la classe `GameMenu` è stata rifattorizzata per calcolare automaticamente le aree di selezione delle opzioni, migliorando l’esperienza utente.


L'editor di scenari è stato implementato usando come base l'editor di mappe sviluppato nelle tappe precedenti, con alcune modifiche per adattarlo alla gestione delle playlist.
Inoltre sono state apportate modifiche al selezionatore di mappe per poter distinguere tra la selezione di una singola mappa e quella di multiple mappe per uno scenario.

---

# Fonti utilizzate in questa Tappa
Nessuna fonte esterna è stata utilizzata per questa tappa.

---