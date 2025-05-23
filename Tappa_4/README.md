# Tappa n.4
L'obiettivo in questa tappa è stato quello di implementare un minimo di ui come ad esempio il punteggio, l'high score, il numero di vite rimanenti e i frutti/oggetti raccolti. Inoltre sono state apportate alcune modifiche per prendere la texture di pacman da un foglio di sprite.

Durante l'esecuzione del programma si verificava un leak di memoria dovuto alla matrice che contiene la mappa.
Per risolvere questo problema ho cambiato mettendo la matrice come una matrice di vettori anzichè una matrice a dimensione fissa.

![demo](images/demo.png)