#include "common.h"

int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    struct ntp_packet packet;
    socklen_t len = sizeof(cliaddr);

    // 1. Création du socket UDP
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    check_error(sockfd, "Échec de la création du socket");

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(1234);

    // 2. Liaison (Bind)
    check_error(bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)), "Échec du bind");

    printf("Serveur NTP prêt sur le port 1234...\n");

    // 3. Boucle infinie pour répondre aux clients
    while (1) {
        // Attendre un paquet du client
        ssize_t n = recvfrom(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&cliaddr, &len);
        if (n < 0) continue;

        // T2 : Noter l'heure d'arrivée
        clock_gettime(CLOCK_REALTIME, &packet.t2);
        printf("Requête reçue ! Calcul du temps en cours...\n");

        // Simulation d'un léger délai de traitement
        usleep(1000); 

        // T3 : Noter l'heure de départ
        clock_gettime(CLOCK_REALTIME, &packet.t3);

        // Renvoyer le paquet avec T1 (déjà dedans), T2 et T3
        sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&cliaddr, len);
    }

    close(sockfd);
    return 0;
}