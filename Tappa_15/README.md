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
- **Gestione della rotellina del mouse**: implementato il supporto a `sf::Event::MouseWheelScrolled` negli editor e nei selettori (Map Editor, Scenario Editor, Level Selector, GameState). In questo modo l’utente può scorrere le pagine di mappe/scenari usando la rotellina del mouse, rendendo più veloce la navigazione.


L'editor di scenari è stato implementato usando come base l'editor di mappe sviluppato nelle tappe precedenti, con alcune modifiche per adattarlo alla gestione delle playlist.
Inoltre sono state apportate modifiche al selezionatore di mappe per poter distinguere tra la selezione di una singola mappa e quella di multiple mappe per uno scenario.

---

# Fonti utilizzate in questa Tappa
- [Documentazione di SFML](https://www.sfml-dev.org/documentation/3.0.0/structsf_1_1Event_1_1MouseWheelScrolled.html#a02d581d6baf283dcbf6ea38a6e1f8d04)

---