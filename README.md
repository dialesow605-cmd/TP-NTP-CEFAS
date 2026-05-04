# Projet 10 : Synchronisation de Temps NTP

## Description
Application client-serveur permettant de synchroniser l'horloge d'un client 
sur celle d'un serveur en calculant le Round Trip Time (RTT).

## Compilation
Utilisez gcc sous Linux :
```bash
gcc -Wall serveur.c -o serveur
gcc -Wall client.c -o client