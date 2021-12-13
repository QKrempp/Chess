# Chess

Mon implémentation de l'algorithme Alpha-Beta pour jouer aux échecs
===

## Utilisation

Il suffit de lancer Interface.py (nécessite Tkinter, et peut être de remplacer ```import tkinter``` par ```import Tkinter``` sous Windows) pour lancer une nouvelle partie.
Il est probable que l'interface plante en cas d'échecs et mat ou de pat.
Pour l'instant, l'utilisation en console en lançant le binaire _Chess.xpp_ nécessite de recompiler le projet en changeant dans le ```main()``` l'un des joueurs ```AlphaBeta``` par un ```Human``` et d'ajouter un ```std::cout << b << std::endl;``` dans le main, avant la ligne ```a->play(&b);``` pour ne pas jouer à l'aveuglette.

### Options de CLI

 - ```-b``` permet d'exporter l'état du _board_ vers le fichier _data/board.txt_ après avoir joué.
 - ```-c``` permet de jouer en ligne de commande (joueur contre IA)
 - ```-i``` permet d'initialiser la partie avec les coups notés dans le fichier _data/game.txt_, ou dans le ficher spécifié juste après l'option
 - ```-m``` permet d'exporter le tableau des coups jouables (_moves_) vers le fichier _data/moves.txt_ après avoir joué
 - ```-s``` permet de ne faire jouer qu'un seule coup à l'IA ou le nombre spécifié juste après l'option
 - ```-u``` permet de simplement générer les fichiers _data/board.txt_ et _data/moves.txt_ à partir de la partie annotée dans le fichier _data/game.txt_

### Exemples

```./Chess.xpp -i "my_custom_game.txt" -s 3 -m -b ``` permet de demander à l'IA de jouer trois coups à partir de la partie consignée dans le fichier *"my_custom_game.txt"* puis d'exporter les mouvements possibles au tour suivant ainsi que la position des pièces.

```./Chess.xpp -i -c``` permet de continuer la partie consignée dans le fichier _data/game.txt_ en CLI.

## AlphaBeta

Pour l'instant la seule intelligence artificielle implémentée est un AlphaBeta, de profondeur 6 (tourne en ~10s par tours sur un _Intel i5 10th gen_, peut être réduit en changeant la valeur par défaut dans _Player.hpp_).

### Fonction d'évaluation

Plutôt basique, elle évalue un board de la façon suivante :

 - Ajoute (resp. retire) 100 points pour une reine de sa couleur (resp. de la couleur adverse)
 - Idem pour les pions, tours, cavaliers et fous, avec respectivement 10, 70, 50 et 50 points
 - Idem pour le roi, avec 10000 points (en effet, pour des questions de performance, l'algorithme ne teste que les mouvements pseudo-légaux).
 - Ajoute (resp. retire) 1 point par case menacée par ses pièces (resp. les pièces des adversaires).
 - S'il s'apprête à jouer un coup qui donne une égalité, on y retire 3000 points.

## TODO

 - Prendre en compte les fins de partie
 - Ajouter une barre de chargement lorsque l'IA joue
 - Ajouter un indicateur pour savoir qui est le joueur qui doit jouer
 - Retravailler l'affichage des coups jouables
 - Ajouter un menu au démarrage pour choisir de commencer une partie, reprendre la dernière, regarder l'IA jouer etc.
 - Paralléliser le code avec OpenMP
 - Implémenter un MCTS
