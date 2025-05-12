![pacmanlogo](https://loodibee.com/wp-content/uploads/Pac-Man-Logo.png)

## Regole
- Ogni 10000 punti si guadagna una vita (modificabile a 15000, 20000 o niente)
- Ci devono essere frutti
    - Cherry = 100 punti
    - Strawberry = 300 punti
    - Orange = 500 punti
    - Apple = 700 punti
    - Melon = 1000 punti
    - Galaxian = 2000 punti
    - Bell = 3000 punti
    - Key = 5000 punti
- Pac-dot = 10 punti
- Power pellet = 50 punti
- Fantasmi (si ricomincia dal primo fantasma ogni volta che si mangia un power pellet)
    - Primo fantasma = 200 punti
    - Secondo fantasma = 400 punti
    - Terzo fantasma = 800 punti
    - Quarto fantasma = 1600 punti

## Meccaniche
- In alcuni punti della mappa ci sono dei tunnel che permettono di passare da un lato all'altro della mappa
- Quando 70 e 170 pac-dot sono stati mangiati compare un frutto sotto la casa dei fantasmi per 9-10 secondi e poi sparisce se non racccolto
- Creazione e possibilità di giocare livelli custom

## Osservazioni
- Quando Pac-Man è contro a un muro l'animazione è ferma
- L'high score viene visualizzato per tutta la partita e si aggiorna subito nel caso in cui si superi
- Deduco che ci debbano essere più livelli (storicamente 256 però guardando questo video ([link](https://www.youtube.com/watch?v=AuoH0vz3Mqk)) è sempre lo stesso livello ripetuto con lo stesso layout)
- Ogni fantasma ha un proprio comportamento
- Il testo del punteggio "incrementa" verso sinistra
- I fantasmi escono dalla casa secondo quanti pacdot sono stati mangiati:
    - 0 pacdot = Blinky (rosso) e Pinky (rosa)
    - 30 pacdot = Inky (azzurro)
    - 60 pacdot = Clyde (arancione)

## Link utili
- Video di gameplay 1: [link](https://www.youtube.com/watch?v=i_OjztdQ8iw)
- Video di gameplay 2: [link](https://www.youtube.com/watch?v=AuoH0vz3Mqk)
- Wiki: [link](https://pacman.holenet.info/)

## Roadmap
- [x] Sistema layout mappa (mappa di default) con file di testo
- [x] Aggiungere pac-man con il movimento classico
- [x] Aggiungere sistema di punteggio
- [ ] Aggiungere frutti
- [x] Aggiungere fantasmi con movimento classico
- [x] Aggiungere collisioni con i fantasmi
- [x] Aggiungere reazione fantasmi al power pellet
- [ ] Aggiungere schermate varie (game over, pausa, ecc.)
- [ ] Aggiungere schermata titolo
- [ ] Aggiungere editor livelli
- [ ] Aggiungere schermata di selezione livello