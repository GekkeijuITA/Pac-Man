# Tappa n.7: Il Power Pellet e la Reazione dei Fantasmi

L'obiettivo di questa tappa del progetto Pac-Man è stato l'implementazione del **power pellet**. Quando Pac-Man lo mangia, i fantasmi devono cambiare il loro stato, la texture e la velocità. Tutti i fantasmi si spaventano (anche quelli ancora in casa), ma una volta che vengono "mangiati" da Pac-Man e ritornano alla loro base, non sono più spaventati dal power pellet in corso. Il power pellet ha una durata limitata, ma questa si resetta ogni volta che Pac-Man ne mangia un altro. Ho anche dovuto tenere conto della "streak" dei fantasmi mangiati durante *un singolo* power pellet, e la formula per calcolare il punteggio che ho usato è $$200\cdot 2^{\text{streak}} \quad \text{con streak}\in\{0,3\}$$

Quando un fantasma entra in modalità `SCARED`, deve immediatamente invertire la sua direzione di movimento.

### La Sfida: Il Ritorno a Casa dei Fantasmi Spaventati

Una funzionalità che si è rivelata piuttosto difficile da implementare è stata il **pathfinding** dei fantasmi quando devono ritornare alla loro casa dopo essere stati mangiati.

Per risolvere questo problema, ho utilizzato un algoritmo **BFS (Breadth-First Search)**. Questo algoritmo mi permette di calcolare la tile `GHOST_DOOR` più vicina alla posizione attuale del fantasma in quel momento, fornendo un percorso efficiente per il suo ritorno.

![Demo con i power pellet e i fantasmi spaventati](images/demo.png)

---
**Fonti utilizzate in questa Tappa:**
* [Algoritmo BFS](https://medium.com/@RobuRishabh/classic-graph-algorithms-c-9773f2841f2e)
---