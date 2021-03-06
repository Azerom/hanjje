#include "menu.h"

void menuHead(char chaine[80])
{
    printf("\x1b[31m                     .__\x1b[0m          .__                       __.__        \n\x1b[31m         ____ ___  __|__|____ \x1b[0m    |  |__ _____    ____     |__|__| ____  \n\x1b[31m       _/ __ \\\\  \\/  /  \\__  \\\x1b[0m    |  |  \\\\__  \\  /    \\    |  |  |/ __ \\ \n\x1b[31m       \\  ___/ >    <|  |/ __ \\_\x1b[0m  |   Y  \\/ __ \\|   |  \\   |  |  \\  ___/ \n\x1b[31m        \\___  >__/\\_ \\__(____  / /\\\x1b[0m___|  (____  /___|  /\\__|  |__|\\___  >\n\x1b[31m            \\/      \\/       \\/  \\/ \x1b[0m   \\/     \\/     \\/\\______|       \\/ \n");
    printf("\n\n\x1b[0m~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    int i;
    int taille = strlen(chaine);
    for (i = 0; i < (40 - (taille/2)); i++)
    {
        printf(" ");
    }
    printf("%s\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n", chaine);
}

void menuPrincipal(Partie * partie)
{
    char a;

    do
    {

        system("clear");
        menuHead("Menu Principal");
        printf("\t1=Jouer une partie\n\t2=Voir l'historique des parties\n\t3=Quitter\n" );
        scanf("%c",&a);

        switch (a)
        {
        case '1' :

            menuJeu(partie);

            break;

        case '2':

            menuHistorique();

            break;

        case '3' :
            system("clear");
            break;
        default :
            system("clear");
            break;
        }

    }
    while (a != '3');
}

void menuJeu(Partie * partie)
{
    char b;
    int alea, tmp;
    char addresse[150] = "niveaux/";
    char addresseSave[150];
    char difficulteAdresse[3] [50] = {"facile/", "moyen/", "difficile/"};
    char fichierAdresses[3][50] = { "1.pbm", "2.pbm", "3.pbm"};
    do
    {

        system("clear");
        menuHead("Menu de partie");
        printf("\t1=Nouvelle partie\n\t2=Charger une partie\n\t3=Revenir au menu \n " );
        scanf("\n%c",&b);

        switch (b)
        {

        case '1' :
            partie->pseudo[0] = '\0';
            do
            {
                system("clear");
                viderTampon();
                menuHead("Nouvelle Partie");
                printf("Entrez votre pseudo (50 caractères maximum):\n ");
                scanf("%[^\n]50s", partie->pseudo);
            }
            while(partie->pseudo[0] == '\0');
            do
            {
                viderTampon();
                system("clear");
                menuHead("Nouvelle Partie");
                printf("Quelle difficulté du niveau voulez vous ?\n\t1=FACILE\n\t2=MOYEN\n\t3=DIFFICILE\n");
                scanf ("%d",&tmp);
                partie->difficulte = tmp;

            }
            while (partie->difficulte != 1 && partie->difficulte != 2 && partie->difficulte != 3);
            do
            {
                viderTampon();
                system("clear");
                menuHead("Nouvelle Partie");
                printf("Quelle taille de d'affichage des resultats voulez vous ? (Fonctionalite inopérante)\n\t1=5*3\n\t2=7*4\n\t3=9*5\n\t4=11*6\n ");
                scanf ("%1d",&tmp);
                partie->tailleResultats = tmp;

            }
            while (partie->tailleResultats != 1 && partie->tailleResultats != 2 && partie->tailleResultats != 3 && partie->tailleResultats != 4);

            system("clear");

            //Choix aléatoire du niveau à réaliser
            partie->date = time(NULL);
            srand(partie->date);
            alea = rand()%2;
            addresse[0] = 'n';
            addresse[1] = 'i';
            addresse[2] = 'v';
            addresse[3] = 'e';
            addresse[4] = 'a';
            addresse[5] = 'u';
            addresse[6] = 'x';
            addresse[7] = '/';
            addresse[8] = '\0';
            strcat(addresse, difficulteAdresse[partie->difficulte-1]);
            strcat(addresse, fichierAdresses[alea]);

            //Lecture du niveau
            lectureNiveau(addresse, &(partie->pattern));

            //Initialisation de la grille actuel
            partie->actuel.x = partie->pattern.x;
            partie->actuel.y = partie->pattern.y;
            partie->actuel.grille = initialisationGrilleChar(partie->actuel.x, partie->actuel.y);

            //Calcul des indices
            calculIndice(partie);

            //Envoie a la fonction de jeu
            menuPartie(partie);
            break;

        case '2' :
            system("clear");
            //-----En travaux
            viderTampon();
            menuHead("Chargement partie");
            printf("Addresse de la save :\n");
            scanf("%s", addresseSave);
            chargement(partie, addresseSave);
            SystemPause();
            partie->date = time(NULL) - partie->temp;

            menuPartie(partie);
            //sauvegarde();
            break;

        case '3':
            system("clear");
            break;
        default :
            break;
        }

    }
    while (b != '3');

}

void menuHistorique()
{
    char a;

    do
    {

        system("clear");
        menuHead("Menu Historique");
        printf("\t1=Trier par date\n\t2=Trier par score\n\t3=Trier par pseudo\n\t4=Quitter\n " );
        scanf("%c",&a);

        FILE * fichier = NULL;

        switch (a)
        {
        case '1' :
            system("clear");
            menuHead("Historique");
            fichier = fopen("historique.log", "r");
            ElementHistorique * historique = NULL;
            historique = (ElementHistorique*)lireHistorique(historique, fichier);
            fclose(fichier);
            afficherHistorique(historique);
            freeHistorique(historique);
            SystemPause();
            break;

        case '2' :

            system("clear");
            break;

        case '3' :
            system("clear");
            break;
        case '4' :

            break;
        default :
            system("clear");
            break;
        }

    }
    while (a != '4');
}
void afficherHistorique(ElementHistorique * historique)
{
    time_t date;
    char tabDifficulte[3][50] = {"Facile", "Moyen", "Difficile"};
    while(historique != NULL)
    {
        date = historique->date;
        char format[128];
        struct tm t_local;
        // On récupère la date et l'heure actuelles.

        t_local=*localtime(&date);
        strftime(format, 50, "%d/%m/%y %X", &t_local);
        printf("%s", format);

        date = historique->temp;
        t_local=*localtime(&date);
        strftime(format, 50, "%X", &t_local);
        printf(" : %s en %s, finit en %s\n", historique->pseudo, tabDifficulte[historique->difficulte-1], format);

        historique = historique->suivant;
    }

}
void menuPartie(Partie * partie)
{
    system("clear");
    int retour = hanjie(partie);
    char addresse [150];
    partie->temp = time(NULL) - partie->date;

    switch(retour)
    {
    case 2:
        system("clear");
        menuHead("Sauvegarde");
        printf("Addresse de la save :\n");
        scanf(" %s", addresse);
        sauvegarde(partie, addresse);
        break;
    case 3:
        system("clear");
        menuHead("Bravo vous avez reussi !!");
        affichage(partie);
        SystemPause();
        system("clear");
        menuHead("Bravo vous avez reussi !!");
        score(partie);
        enregistrerHistorique(partie);
        SystemPause();
        break;
    }
}
