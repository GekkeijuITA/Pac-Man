# Tappa n.14: Suoni e miglioramenti vari
L'obbiettivo di questa tappa è quella di implementare i suoni nel gioco, l'idea è quella di utilizzare una classe `SoundManager` che si occupa di caricare i file audio e di renderli disponibili a tutte gli altri file.

## Sound Manager
Il file `SoundManager` è stato costruito seguendo l'esempio di `TileFactory`. La parte più complicata è stata quella di rendere disponibile il suono a tutti gli altri file perchè `sf::Sound` non può essere copiato e non esiste all'infuori dello scope. Per ovviare a questo problema è stata utilizzata una mappa a cui associamo il nome del suono (che diamo noi) e una struct `Audio` che contiene il buffer e il suono:
```
struct Audio {
    std::shared_ptr<sf::SoundBuffer> buffer;
    std::unique_ptr<sf::Sound> sound;
};
```
Per rendere il buffer riutilizzabile in tutti i file l'ho dovuto mettere come puntatore condiviso mentre il suono come puntatore unico perchè appunto non è possibile copiare `sf::Sound`. La funzione che ha permesso di avere il suono nella mappa è stata `std::move`.

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

---
**Fonti utilizzate in questa Tappa:**
* [Tutorial audio SFML](https://www.sfml-dev.org/tutorials/3.0/audio/sounds/)
* [Suoni](https://www.sounds-resource.com/arcade/pacman/sound/10603/)
* [Vita Extra](https://www.youtube.com/watch?v=nkV6BedgwRY)
---