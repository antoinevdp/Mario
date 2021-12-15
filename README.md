# Mario

Game developped by two ESME students for a school project.

! WARNING !

Pour lancer notre projet:
-Executer: gcc main.c -o output -lncurses
puis: ./output

Commandes:
'q' pour aller à gauche
'd' pour aller à droite
'x' pour quitter

Lorsque le joueur attrappe le champignon, il a une vie supplémentaire.

Si au lancement d'une map, le terminale affiche le personnage mais sans fond, décor,
il y a un problème de type de end line des fichiers .txt

Exemple:
- En executant dans un terminale:
- file <nom_du_fichier>.txt
- On obtient : out.txt: ASCII text, with CRLF line terminators
Nous devons avoir : map.txt: ASCII text

Solutions:

- Ouvrir un terminale
- Se rendre dans le dossier bin du projet
- Executer: sudo apt-get install dos2unix
- Executer: dos2unix <nom_du_fichier>.txt

Exemple: dos2unix map1.txt

Cela a pour effet de changer les lines terminators
