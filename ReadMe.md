# Exercice n°1

```./osmGraph.exe extract data/test.osm data/test_extract.graph```

```./osmGraph.exe simplify data/test_extract.graph```

```./osmGraph.exe visualize data/test_extract.graph```

![Image à la fin de l'exercice 1](/data/Img/VisualizeTest.png "Plan 1")

# Exercice n°2

## Identifiez où sont définies les structures principales du graphe (WeightedGraph / PositionedGraph) et expliquez brièvement leur rôle et comment elles sont utilisées.

Dans weightedGraph.hpp et positionedGraph.hpp

Comme son nom l'indique, weightedGraph permet de stocker un graphe pondéré.
Ca permet de faire les connexions entre des éléments comme vu en cours : avoir la distance entre chaque point et passer d'un point à un autre.

On obtient la même chose que dans le graph du cours mais avec des connexions pouvant aller dans les deux sens : 
![Schéma cours](/data/Img/GraphCours.png "Plan 1")

Pour positionedGraph, le code ajoute les coordonnées (x, y) aux points, permettant d'obtenir la carte à la fin.

## Expliquez en quelques lignes le rôle des modules: extraction OSM, simplification, visualisation.

**extraction OSM :**

Ce module permet de lire des fichiers XML et selectionner les infos importantes en enlevant tout ce qui est inutile (visible les batiments et les rails avec OSM::remove_building_ways(ways); et OSM::remove_railway_ways(ways);).
Ensuite il convertit les données 3D de la vie réel en 2D.
Puis ça donne un graph exploitable.

**simplification :**

Permet d'enlever tous les points inutiles (comme ceux au même endroit) pour que ce soit plus rapide et lisisble.

**visualisation :**

Permet de voir le graph, de cliquer dessus, de calculer des chemins, de zommer etc

## Expliquez ce que vous comprenez des différentes étapes de simplification implémentées (fichier src/simplification/simplify.cpp) et les raisons pour lesquelles elles sont utilisées (leur impact sur la structure du graphe, les avantages/inconvénients, etc.).

1. Garder la zone où tous les éléments sont regroupés, on enlève les éléments isolés
=> Permet d'enlever le bruit, les endroits isolés etc
2. Supprimer les petits bouts de route sur les bords en fusionnant les points proches
=> Permet d'enlever les détails inutiles
3. Supprimer les point inutiles sur les lignes droites
Exemple : A _____________ B _____________ C devient A _____________________________ C
Ca marche en vérifiant l'angle entre les points, s'il est proche de 180° ou non
=> simplifie les lignes
4. Fusionner les points proches (pas forcément les bords)
=> Simplifier de nouveau, code plus épuré
5. On refait l'étape 3 pour simplifier les nouveaux points formés