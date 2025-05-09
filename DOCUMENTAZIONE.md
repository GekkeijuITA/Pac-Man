# Tappa n.6
## Obiettivo
L'obiettivo in questa tappa del progetto "Pac-Man" è quello di implementare la collisione dei fantasmi con pacman con la gestione della vita di pacman. In caso di collisione, pacman perde una vita e viene riportato alla posizione iniziale. Se pacman ha esaurito tutte le vite, settiamo un flag che indica che il gioco è terminato.
## Ostacoli
Nessun ostacolo particolare.
## Soluzione
Per controllare se il fantasma mangiava pacman, controllo la posizione di pacman e del fantasma tramite il calcolo della distanza euclidea tra i due oggetti e se questa distanza è minore di un certo valore ``COLLIDE_BOX`` allora viene triggerato un reset del round (quindi il reset delle posizioni) e il decremento della vita di pacman. Se pacman ha esaurito tutte le vite, viene settato il flag ``game_over`` a true e il gioco termina (in questo caso viene messo in pausa).
![demo](demo.png)