# Tappa n.4
## Obiettivo
L'obiettivo in questa tappa del progetto "Pac-Man" è quello di implementare un minimo di ui come ad esempio il punteggio, l'high score, il numero di vite rimanenti e i frutti/oggetti raccolti. Inoltre sono state apportate alcune modifiche per prendere la texture di pacman da un foglio di sprite.
## Ostacoli
Leak di memoria dovuto alla matrice che contiene la mappa.
## Soluzione
Per risolvere questo problema ho cambiato mettendo la matrice come una matrice di vettori anzichè una matrice a dimensione fissa.

![demo](demo.png)