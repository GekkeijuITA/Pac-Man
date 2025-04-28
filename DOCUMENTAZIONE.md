# Tappa n.2
## Obiettivo
L'obiettivo in questa tappa del progetto "Pac-Man" è quello di implementare Pac-Man con il movimento, la collisione con le pareti e il teletrasporto da un lato all'altro della mappa. E l'illusione che Pac-Man mangi i pallini.
## Ostacoli
L'ostacolo principale è stato quello di implementare la funzione del movimento per avere un movimento in linea con la griglia.
## Soluzione
Per rendere l'implementazione del movimento più semplice ho deciso di usare i movimenti in numeri interi, in questo modo però ci sono stati dei problemi di conversione con la setPosition della classe Sprite. Per risolvere a questo problema tengo in memoria due posizioni: una in float e una in int. La posizione in float viene usata come accumulatore per il movimento e la posizione in int viene usata per disegnare lo sprite. 

![demo](demo.png)