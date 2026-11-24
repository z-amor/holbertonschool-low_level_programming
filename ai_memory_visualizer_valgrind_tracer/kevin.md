Si vous parlez bien de la Tâche 0 d'AI Memory Visualizer, voici ce qu'il faut comprendre pour y voir clair : il n'y a aucun code C à corriger ni à pousser. Votre seul livrable est un rapport d'enquête rédigé dans le fichier analysis/memory_maps.md.

Voici la marche à suivre concrète étape par étape :

les fichiers analyser sont:

programs/stack_example.c (obligatoire)

programs/aliasing_example.c (obligatoire)

programs/heap_example.c (optionnel pour la tâche 0, mais obligatoire pour la tâche 1)

Ce que vous devez faire pour chacun de ces fichiers :

Compiler et exécuter : Allez dans programs/, tapez make et lancez les programmes normalement pour voir ce qu'ils affichent (ou s'ils plantent).

Repérer les moments clés : Lisez le code et notez les étapes où la mémoire bouge (appel/retour d'une fonction, récursivité, malloc/free, ou quand deux pointeurs pointent vers la même variable).

Générer la memory map avec une IA en lui donnant le code et en lui lui demandant de vous dessiner/décrire l'état de la mémoire étape par étape : ce qu'il y a dans la Stack, ce qu'il y a dans le Heap, quels pointeurs sont des alias (pointent au même endroit) et à quel moment précis chaque donnée devient invalide.

MAIS§ Ne copiez pas l'IA aveuglément ! Relisez son explication ligne par ligne : elle va forcément faire une erreur, une omission ou une approximation (par exemple sur la durée de vie d'une variable locale après un return, ou sur qui possède un malloc). Vous devez documenter au moins une erreur de l'IA dans votre fichier : ce qu'elle a dit, pourquoi c'est faux (ou approximatif), et votre correction.

Quoi pousser sur GitHub me direz-vous?
Uniquement analysis/memory_maps.md. Ne modifiez pas les fichiers .c et ne poussez pas les exécutables compilés.