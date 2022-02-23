# Chess

Mon implémentation de l'algorithme Alpha-Beta pour jouer aux échecs
===

## Utilisation

Sous Linux, il faut _évidemment_ recompiler l'exécutable, à l'aide de ```qmake && make```, un script arrivera bientôt pour l'automatiser.

Sous Windows, il suffit de lancer le fichier en conservant la structure de données, c'est à dire les images dans le dossier ```img``` et l'exécutable dans ```win```.

L'utilisation en console n'est plus possible pour le moment.

## AlphaBeta

Pour l'instant la seule intelligence artificielle implémentée est un AlphaBeta, de profondeur 6 (tourne en ~10s par tours sur un _Intel i5 10th gen_, peut être réduit en changeant la valeur par défaut dans _Player.hpp_).

### Fonction d'évaluation

Plutôt basique, elle évalue un board de la façon suivante :

 - Ajoute (resp. retire) 900 points pour une reine de sa couleur (resp. de la couleur adverse)
 - Idem pour les pions, tours, cavaliers et fous, avec respectivement 100, 500, 320 et 330 points
 - Idem pour le roi, avec 200000 points (en effet, pour des questions de performance, l'algorithme ne teste que les mouvements pseudo-légaux).
 - Ajoute (resp. retire) 1 point par case menacée par ses pièces (resp. les pièces des adversaires).
 - Ajoute à chaque des points propre à chaque pièces en fonction de leur postition sur le plateau pour encourager l'IA à développer ses pièces.
 - S'il s'apprête à jouer un coup qui donne une égalité, on y retire 3000 points.

## TODO

 - Prendre en compte les fins de partie
 - Ajouter une barre de chargement lorsque l'IA joue
 - Ajouter un indicateur pour savoir qui est le joueur qui doit jouer
 - Ajouter un menu au démarrage pour choisir de commencer une partie, reprendre la dernière, regarder l'IA jouer etc.
 - Utiliser la recherche AlphaBeta à profondeur incrémentale pour ré-ordonner les branches avant la prochaine recherche.
 - Prendre en compte les chaînes de prises
 - Ajouter la prise en passant
 - Changer le comportement en fin de partie pour mettre en échec
 - Paralléliser le code avec OpenMP
 - Implémenter un MCTS
