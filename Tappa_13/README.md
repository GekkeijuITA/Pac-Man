# Tappa n.13: Miglioramenti all'IA dei fantasmi
L'obiettivo di questa tappa è quella di migliorare l'attuale intelligenza artificiale dei fantasmi cambiando quella attuale (randomica) in una più vicina a quella originale.

## Cambiamenti generali
In questa tappa è stata aggiunta la funzione `update(float elapsed)` che sarà usata dal Game Manager e gestisce i timer interni del fantasma (ad esempio, il timer per il comportamento spaventato o la gestione degli stati CHASE e SCATTER), gestisce anche il movimento e l'animazione.

Per gestire il comportamento dei singoli fantasmi è stata aggiunta una funzione `virtual behaviour()` che sarà implementata da ciascun fantasma e verrà chiamata da `move()`.

Per la modalità SCATTER il comportamento è lo stesso per ogni fantasma: prende la posizione di un `IntRect` che rappresenta l'area in cui il fantasma deve muoversi che ogni fantasma ha nel proprio costruttore, e quando è in modalità SCATTER calcola la BFS per arrivare nell'angolo in alto a sinistra (dell'`IntRect`) e se quella posizione specifica è considerato un muro allora il programma cerca la tile più vicina libera e imposta quella come target della BFS e una volta al suo interno si muove liberamente fino a quando è dentro all'area altrimenti ricalcola la BFS per tornare dentro.

La scelta randomica della direzione dei fantasmi è stata sostituita con una più simile all'originale: randomicamente sceglie una direzione, se quella direzione è un muro allora in senso orario trova la prima direzione libera e prende quella.

## Blinky
Blinky (il fantasma rosso) ha come comportamento quello di seguire direttamente il giocatore. Per fare ciò, quando è allineato alla cella e la posizione del giocatore è cambiata, calcola il percorso usando la ricerca BFS.

![Blinky Behaviour](images/blinky-behaviour.gif)
---
**Fonti utilizzate in questa Tappa:**
* [Wiki di Pac-Man](https://pacman.holenet.info/)
---