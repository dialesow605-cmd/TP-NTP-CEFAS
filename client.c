#include "common.h"

int main(int argc, char *argv[]) {
    // Vérification de l'argument (l'adresse IP du serveur)
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <adresse_ip_serveur>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sockfd;
    struct sockaddr_in servaddr;
    struct ntp_packet packet;
    struct timespec t4;
    socklen_t len = sizeof(servaddr);

    // 1. Création du socket UDP
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    check_error(sockfd, "Erreur lors de la création du socket");

    // 2. Configuration de l'adresse du serveur
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(1234);
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
        fprintf(stderr, "Adresse IP invalide : %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // 3. Noter T1 et envoyer le paquet
    clock_gettime(CLOCK_REALTIME, &packet.t1);
    sendto(sockfd, &packet, sizeof(packet), 0, (const struct sockaddr *)&servaddr, len);

    // 4. Recevoir la réponse et noter T4
    printf("En attente de la réponse du serveur...\n");
    check_error(recvfrom(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&servaddr, &len), "Erreur réception");
    clock_gettime(CLOCK_REALTIME, &t4);

    // 5. Calculs mathématiques (NTP)
    // Conversion des structures timespec en secondes (double)
    double t1 = packet.t1.tv_sec + packet.t1.tv_nsec / 1e9;
    double t2 = packet.t2.tv_sec + packet.t2.tv_nsec / 1e9;
    double t3 = packet.t3.tv_sec + packet.t3.tv_nsec / 1e9;
    double t4_val = t4.tv_sec + t4.tv_nsec / 1e9;

    // Calcul du Round Trip Time (RTT) et de l'Offset (Décalage)
    double rtt = (t4_val - t1) - (t3 - t2);
    double offset = ((t2 - t1) + (t3 - t4_val)) / 2.0;

    // 6. Affichage des résultats
    printf("\n--- Synchronisation NTP ---\n");
    printf("T1 (Départ Client) : %.6f s\n", t1);
    printf("T2 (Arrivée Serveur): %.6f s\n", t2);
    printf("T3 (Départ Serveur) : %.6f s\n", t3);
    printf("T4 (Retour Client)  : %.6f s\n", t4_val);
    printf("---------------------------\n");
    printf("RTT (Délai réseau)  : %.6f s\n", rtt);
    printf("Offset (Décalage)   : %.6f s\n", offset);
    printf("---------------------------\n");

    close(sockfd);
    return 0;
}