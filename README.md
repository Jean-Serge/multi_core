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
====================================

Pour compiler l'intégralité du projet : make all
Pour nettoyer le projet : make clean
Pour créer un disque et l'initiliser avec un système de fichiers : create_volume.sh
Pour lancer le shell : shell.bin.

Solutions envisagées
====================
Notre projet n'est pas fonctionnel. Les algorithmes et solutions décrit sont partiellement implémenté voir pas.

Gestion des accès au disque :
-----------------------------
Nous voulions protéger le système lors de multiples accès aux disques en ré-implémentant des sémaphores.

Nous aurions :
* Un contexte_disque courant (en attente d'IRQ disque)
* Une liste de contexte_disque en attente (attendant de lancer une opération sur le disque)
* Une fonction à ajouter dans l'IRQVECTOR gérant le 4.


 1. Juste avant un appel au disque nécessitant un temps de travail de sa part, le contexte courant serait ajouté à une liste de contextes 'attente_disques'.
    Ces contextes ne sont pas appelable par le fonction yield.
 2. Dans la fonction yield(), s'il n'y a aucun contexte_disque courant le premier élément de la liste en attente (si non-vide) sera exécuté
 3. Ce contexte lancerait une opération sur le disque et serait le nouveau contexte_disque
 4. Lorsqu'une IRQ venant du disque serait levée, contexte_disque en attente serait immédiatement rechargé (interrompant le contexte courant) 

Cette action serait évidemment une fonction passée à l'IRQVECTOR.
Ainsi, chaque contexte devrait attendre la levée de l'IRQ afin de continuer son traitement.

Politique d'équilibrage des contextes :
---------------------------------------
Afin de limiter la charge sur un coeur en particulier nous voulions implémenter un système simple :

Nous voulions disposer d'une variable indiquant le numéro du coeur sur lequel créer le nouveau contexte.
Cette variable serait incrémentée après chaque création de contexte.

Nous comptions chercher un système plus efficace à implémenter quand nous aurions un programme fonctionnel, nous n'avon pas eu cette opportunité.

Problèmes rencontrés
====================
Lors de ce projet, nous avons rencontré plusieurs difficultés. La tâche réalisée n'est pas anodine et nous a demandé beaucoup de réflexion pour choisir une solution raisonnable en prenant en compte la performance et le temps de développement.

De plus, certains bugs dans les bibliothèques nous ont fait perdre du temps. Nous avons, dans un premier temps, cherché l'origine de l'erreur dans notre code ce qui nous à considérablement ralentis.


Conclusion
==========
Ce projet a été très instructif, il nous a donné la possibilité de nous rendre compte de la complexité d'un système d'ordonnancement multicoeurs, ainsi que des problématiques qui y sont liées, telles que la répartition équitable des contextes sur les coeurs et la gestion des coeurs inutilisés. 
