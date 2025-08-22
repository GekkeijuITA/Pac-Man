# Tappa n.14: Suoni e miglioramenti vari
L'obbiettivo di questa tappa è quella di implementare i suoni nel gioco, l'idea è quella di utilizzare una classe `SoundManager` che si occupa di caricare i file audio e di renderli disponibili a tutte gli altri file.

## Sound Manager
Il `SoundManager` è stato implementato seguendo la struttura di `TileFactory`. Visto che `sf::Sound` non può essere copiato e per funzionare deve avere un buffer sempre "vivo", ho dovuto fare una struct che contenesse sia il suono sia il buffer su cui si basa. Per rendere l'incapsulamento più efficace in questa struct ho messo alcune funzioni utili per gestire il suono. Per questa parte di tappa ho usato una soluzione proposta su Stack Overflow, ma visto che usava una versione più datata di `SFML` ho dovuto adattare per `SFML 3.0.0` come ad esempio il fatto che il costruttore `sf::Sound()` non esiste più e bisogna passargli il buffer da subito.

Ho implementato le funzioni per riprodurre i suoni (anche in loop) in modo semplice passando il nome del suono e alcune funzioni di supporto come `isSoundPlaying` e `getSoundStatus`.

I suoni che sono stati implementati sono:
- Fantasmi (normali, spaventati e mangiati)
- Il "waka" di Pac-Man
- La morte di Pac-Man
- La canzone iniziale
- Pac-Man che mangia il frutto e i fantasmi

In più ho messo un suono per quando si preme il tasto "invio" nei menu.

## Vita Extra
Per implementare la logica della vita extra è stato abbastanza semplice: controlliamo quando Pac-Man raggiunge un punteggio di 10000 e dopodichè facciamo partire il suono dell'"estensione" e facciamo un semplice `lives++` per incrementare.

## Piccole migliorie
Per rendere il codice più leggibile e mantenibile ho centralizzato la gestione dei timer di pacman e per i frutti, adesso entrambi utilizzano una funzione `update()`.

La logica di movimento per pacman è stata spostata da `GameState::update()` a `Pacman::update()` e la logica del powerpellet anche.

La logica di disegno dei frutti è stata spostata in `Fruit::draw()` che gestisce anche il lampeggio. La logica di comparsa e scomparsa (quindi anche la gestione dei timer) è stata centralizzata in `Fruit::update()`.

---
**Fonti utilizzate in questa Tappa:**
* [Tutorial audio SFML](https://www.sfml-dev.org/tutorials/3.0/audio/sounds/)
* [Suoni](https://www.sounds-resource.com/arcade/pacman/sound/10603/)
* [Vita Extra](https://www.youtube.com/watch?v=nkV6BedgwRY)
* [Organizzazione audio SFML](https://stackoverflow.com/questions/27235897/how-do-i-properly-organize-and-implement-sfml-audio-in-c)
---