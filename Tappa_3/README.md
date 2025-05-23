# Tappa n.3
L'obiettivo in questa tappa è stato quello di implementare Pac-Man con il movimento tipico (compreso il "buffer" per la prossima direzione), la collisione con le pareti e il teletrasporto da un lato all'altro della mappa. E l'illusione che Pac-Man mangi i pallini.

L'ostacolo principale è stato quello di implementare la funzione del movimento per avere un movimento fluido in linea con la griglia e anche il cambio di direzione appena possibile usando il "buffer".

Per implementare il movimento fluido uso due vettori: uno di interi e uno di float. Il primo è il movimento che Pac-Man compie nella matrice e il secondo serve per rendere fluida l'animazione di movimento. Per il cambio di direzione appena possibile, uso una variabile di tipo Direction che contiene la direzione in cui Pac-Man deve andare appena il blocco in quella direzione è libero, per fare ciò ho usato una funzione che controlla se il blocco in questione non è un muro e se il Pac-Man è allineato al centro della tile (con un margine di tolleranza). Questo è possibile perchè il vettore di float contiene valori da -1 a 1 circa.

![demo](images/demo.png)