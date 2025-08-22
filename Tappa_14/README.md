# Tappa n.14: Suoni e miglioramenti vari
L'obbiettivo di questa tappa è quella di implementare i suoni nel gioco, l'idea è quella di utilizzare una classe `SoundManager` che si occupa di caricare i file audio e di renderli disponibili a tutte gli altri file.

## Sound Manager
Il `SoundManager` è stato implementato seguendo la struttura di `TileFactory`. 
- **Problema e soluzione**: Poiché `sf::Sound` non può essere copiato, è stata creata una struct `Audio` che incapsula sia il buffer (`sf::SoundBuffer`) sia l'oggetto suono (`sf::Sound`), risolvendo così il problema di rendere i suoni disponibili in tutto il gioco.
- **Adattamento a SFML 3.0.0**: La soluzione implementata è stata adattata da un esempio di Stack Overflow (vedere la sezione [Fonti utilizzate in questa Tappa](#fonti-utilizzate-in-questa-tappa)), tenendo conto delle modifiche introdotte nella versione 3.0.0 di SFML, come la necessità di passare il buffer direttamente nel costruttore di `sf::Sound`.

Per rendere l'incapsulamento più efficace laa struct `Audio` include alcune funzioni utili per gestire il suono come `play()`,`stop()`,`pause()`,`setLoop()` e `getStatus()`. 

I suoni che sono stati implementati sono:
- Fantasmi (normali, spaventati e mangiati)
- Il "waka" di Pac-Man
- La morte di Pac-Man
- La canzone iniziale
- Pac-Man che mangia il frutto e i fantasmi

In più è stato messo un suono per quando si preme il tasto "invio" nei menu.

## Vita Extra
La logica per l'implementazione della vita extra si attiva al raggiungimento di un punteggio specifico. Quando il punteggio di Pac-Man raggiunge i 10000 punti, viene riprodotto un suono di "estensione" e il numero di vite del giocatore viene incrementato di uno.

## Piccole migliorie
Per migliorare la leggibilità e la manutenibilità del codice, sono state apportate le seguenti modifiche:
- **Centralizzazione della logica**: la gestione dei timer e la funzione `update()` sono state centralizzate all'interno delle rispettive classi `Pacman` e `Fruit`
- **Incapsulamento del comportamento**: la logica di movimento e la gestione dei power pellet di Pac-Man sono state spostate da `GameState::update()` a `Pacman::update()`, incapsulando il comportamento del personaggio nel suo file
- **Separazione delle responsabilità**: la logica di disegno, inclusa l'animazione di lampeggio dei frutti, è stata spostata in `Fruit::draw()`, mentre la logica di comparsa e scomparsa (con la gestione dei timer) è stata centralizzata in `Fruit::update()`.

---
# Fonti utilizzate in questa Tappa
* [Tutorial audio SFML](https://www.sfml-dev.org/tutorials/3.0/audio/sounds/)
* [Suoni](https://www.sounds-resource.com/arcade/pacman/sound/10603/)
* [Vita Extra](https://www.youtube.com/watch?v=nkV6BedgwRY)
* [Organizzazione audio SFML](https://stackoverflow.com/questions/27235897/how-do-i-properly-organize-and-implement-sfml-audio-in-c)
---