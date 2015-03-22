# multi_core
Jean-Serge Monbailly et Arthur Dewarumez

Organisation du projet
======================
Le projet est organisé en deux principaux répertoires :
 1. un répertoire contenant les sources du projet. Ce dernier est divisé en sous répertoires, chacun contenant une partie du projet. Ainsi, vous y trouverez :
* disque : qui contient les sources correspondant à toute la gestion des disques, du matériel au systéme de fichiers.
* ordonnancement : qui s'occupe de la partie de gestion de contexte (sorte de processus) et leur ordonnancement.
* shell : qui fourni un shell simpliste pour lancer des commandes qui seront ordonnancées par le code de gestion de contexte
* core_test : qui contient le résultat des exercices permettant de se familiariser avec la bibliothèque.
* test : qui contient des sources écrites à des fins de tests.
 2. un répertoire contenant les fichiers headers. Ce répertoire n'est pas subdivisé en sous-répertoires.

Compilation et principaux exécutable
============================

Pour compiler l'intégralité du projet : make all
Pour nettoyer le projet : make clean
Pour créer un disque et l'initiliser avec un système de fichiers : create_volume.sh
Pour lancer le shell : shell.bin.

Solutions envisagées
=================
Notre projet n'est pas fonctionnel. Les algorithmes et solutions décrit sont partiellement implémenté voir pas.



Problèmes rencontrés
====================
Lors de ce projet, nous avons rencontré plusieurs difficultés. La tâche réalisée n'est pas anodine et nous a demandé beaucoup de réflexion pour choisir une solution raisonnable en prenant en compte la performance et le temps de développement.

De plus, certains bug dans les bibliothèques nous ont fait perdre du temps. Nous avons, dans un premier temps, recherché l'origine de l'erreur dans notre code. Ce qui nous à considérablement mis en retard.


Conclusion
==========
Ce projet a été très instructif, il nous a donné la possibilité de nous rendre compte de la complexité d'un système d'ordonnencement multicoeurs, ainsi que des problèmatiques qui y sont liées, tels que la répartition équitable des contextes sur les coeurs, la gestion des coeurs inutilisés. 
