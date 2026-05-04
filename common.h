#ifndef COMMON_H
#define COMMON_H

// Cette ligne permet d'utiliser clock_gettime sans erreurs
#define _POSIX_C_SOURCE 199309L

#include <stdio.h>      // Pour l'affichage (printf)
#include <stdlib.h>     // Pour la gestion de la mémoire et exit()
#include <string.h>     // Pour les fonctions de texte et memset()
#include <unistd.h>     // Pour usleep() et les fonctions système
#include <time.h>       // Pour la gestion précise du temps (timespec)
#include <sys/types.h>
#include <sys/socket.h> // Pour les sockets (socket, bind, send, recv)
#include <netinet/in.h> // Pour les structures d'adresses internet
#include <arpa/inet.h>  // Pour la conversion d'adresses IP (inet_pton)

// Structure du paquet NTP simplifiée pour notre projet
struct ntp_packet {
    struct timespec t1; // Temps de départ du client
    struct timespec t2; // Temps d'arrivée au serveur
    struct timespec t3; // Temps de départ du serveur
};

/**
 * Fonction utilitaire pour vérifier si une commande a échoué.
 * Elle affiche l'erreur et arrête le programme proprement.
 */
static inline void check_error(int status, const char *msg) {
    if (status < 0) {
        perror(msg);
        exit(EXIT_FAILURE);
    }
}

#endif