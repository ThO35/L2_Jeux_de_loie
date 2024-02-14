#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 10
#define OIEMAX 94

/**
 * @brief Crée un tableau de caractères avec des caractères spécifiques à des positions prédéfinies.
 *
 * Cette fonction initialise un tableau de caractères de longueur 100 et attribue des
 * caractères spécifiques ('T', 'R', 'P', 'H', 'L', 'X') à certaines positions. Les positions
 * restantes sont remplies avec le caractère 'O' toute les 9 cas jusqua 94.
 *
 * @param tableau Un pointeur vers le tableau de caractères à créer.
 */
void creation_tab(char *tableau) {
    tableau[53] = 'T';
    tableau[8] = 'R';
    tableau[74] = 'P';
    tableau[31] = 'H';
    tableau[65] = 'L';
    tableau[94] = 'X';
    int pose = 0;
    while (pose < OIEMAX) {
        int randompos = 9;
        if (pose + randompos != 53 && pose + randompos != 8 && pose + randompos != 74 &&
            pose + randompos != 31 && pose + randompos != 65 && pose + randompos != 94) {
            pose += randompos;
            if (pose < 94) {
                tableau[pose] = 'O';
            }
        }
    }
    tableau[100] = '\0';
}

/**
 * @brief Gère les collisions entre les joueurs sur le plateau de jeu.
 * Cette fonction parcourt les positions des joueurs et détecte les collisions.
 * En cas de collision avec un autre joueur, le joueur courant prend la place de la nouvelle position,
 * et l'autre joueur est renvoyé à l'ancienne position du joueur courant.
 *
 * @param positions Un tableau représentant les positions des joueurs sur le plateau.
 * @param nbjoueurs Le nombre total de joueurs.
 * @param joueurcourant Indique qui doit jouer.
 * @param nouvelleposition La nouvelle position du joueur courant.
 * @param ancienneposition L'ancienne position du joueur courant.
 */
void collision (int positions[],int nbjoueurs, int joueurcourant, int nouvelleposition,int ancienneposition) {
    // Fonction pour gérer les collisions entre les joueurs sur le plateau de jeu
    for (int i = 0; i < nbjoueurs; i++) {
        
        if (i != joueurcourant && positions[i] == nouvelleposition ) {
            // Déplacer le joueur i à l'ancienne position du joueur courant
            positions[i] = ancienneposition;
            // Optionnellement, appliquer l'effet de la case au joueur i
            // (vous voudrez peut-être implémenter cela en fonction du type de case)
        }
    }
}

/**
 * @brief Vérifie les cases spéciales du plateau de jeu et applique les effets correspondants.
 *
 * Cette fonction gère différents événements spéciaux en fonction de la nouvelle position
 * d'un joueur sur le plateau de jeu. Les événements incluent le joueur tombant dans le trou,
 * prenant un raccourci, allant en prison, se reposant à l'hôtel, se retrouvant dans le labyrinthe,
 * touchant la tête de mort, et atterrissant sur une case Oie.
 *
 * @param plateau Un tableau représentant le plateau de jeu.
 * @param position Un tableau représentant les positions des joueurs sur le plateau.
 * @param joueurcourant  Indique qui doit jouer.
 * @param attente Un tableau indiquant l'etat des joueur .
 * @param resdes La sommme des 2 lancer de dés du joueur courant.
 * @param nbjoueurs Le nombre total de joueurs.
 * @param newposition La nouvelle position du joueur courant.
 * @return 1 si un événement spécial a été géré, 0 sinon.
 */
int  verification_case(char plateau[], int position[], int joueurcourant, int attente[], int resdes, int nbjoueurs, int newposition) {
    // Vérifier les cases spéciales et appliquer les effets
    // Si le joueur tombe dans le trou (case 53)
    int ancienneposition = position[joueurcourant];
    position[joueurcourant]=newposition;
    if (newposition == 53) {
        printf("Le joueur %d est tombé dans le trou (case 53).\n", joueurcourant + 1);
        attente[joueurcourant] = -1;

        // Attendre qu'un autre joueur prenne sa place pour se déplacer à nouveau
        for (int i = 0; i < nbjoueurs; i++) {
            if (i != joueurcourant && attente[i] == -1) {
                attente[i] = 1;
            }
        }
        return 1;
    }

    // Raccourci vers la case 16 (case 8)
    if (newposition == 8) {
        printf("Le joueur %d prend le raccourci vers la case 16 (case 8).\n", joueurcourant + 1);
        newposition = 16;
        attente[joueurcourant] = 1;
        position[joueurcourant]=newposition;
        attente[joueurcourant]=1;
        collision(position, nbjoueurs, joueurcourant, newposition, ancienneposition);
        return 1;
    }

    // Prison (case 74)
    if (newposition == 74) {
        printf("Le joueur %d est en prison (case 74).\n", joueurcourant + 1);
        attente[joueurcourant] = -2;
        int nbreJcase = 0;  
        int joueurlibre[2];  // Initialiser le tableau à des valeurs invalides
        
        // Compter le nombre de joueurs en attente dans la prison et enregistrer les deux premiers
        for (int i = 0; i < nbjoueurs && nbreJcase < 2; i++) {
            if (attente[i] == -2) {
                joueurlibre[nbreJcase] = i;
                nbreJcase += 1;
            }
        }
        
        // Si deux joueurs sont en attente dans la prison, les libérer
        if (nbreJcase == 2) {
            for (int i = 0; i < 2; i++) {
                if (joueurlibre[i] != -1 && attente[joueurlibre[i]] == -2) {
                    attente[joueurlibre[i]] = 1;
                    printf("Le joueur %d est libéré de la prison.\n", joueurlibre[i] + 1);
                }
            }
        }
        return 1;
    }

    // Hotel (case 31)
    if (newposition == 31) {
        printf("Le joueur %d est à l'hôtel (case 31) et passe son tour deux fois.\n", joueurcourant + 1);
        attente[joueurcourant] = 2; // Le joueur passe son tour deux fois
        return 1;
    }

    // Labyrinthe (case 65)
    if (newposition == 65) {
        printf("Le joueur %d est dans le labyrinthe (case 65) et recule de 13 cases.\n", joueurcourant + 1);
        position[joueurcourant] = 65-13; // Recule de 13 cases (malus)
        attente[joueurcourant]=1;
        collision(position, nbjoueurs, joueurcourant, newposition, ancienneposition);
        return 1;
    }

    // Tête de mort (case 94)
    if (newposition == 94) {
        printf("Le joueur %d a touché la tête de mort (case 94) et retourne à la case 0.\n", joueurcourant + 1);
        newposition = 0; // Retourne à la case 0
        position[joueurcourant]=0;
        attente[joueurcourant]=1;
        collision(position, nbjoueurs, joueurcourant, newposition, ancienneposition);
        return 1;
    }

    // Case Oie (marquée avec 'o')
    if (plateau[newposition] == 'O') {
        printf("Le joueur %d atterrit sur une case Oie (marquée avec 'o')\n", joueurcourant + 1);
        int add = 0;
        for (int i = 0; i < resdes; i++) {
            if (newposition <= 99 && add == 0) {
                newposition++;
            } else {
                newposition--;
                add = 1;
            }
        } // Avance du même nombre de cases que le lancer de dés
        if (verification_case(plateau, position, joueurcourant, attente,  resdes, nbjoueurs, newposition)==0){
            collision(position, nbjoueurs, joueurcourant, newposition, ancienneposition);
        }
        return 1;
    }
    return 0;
}

/**
 * @brief Saisit la valeur du dé à partir de l'entrée utilisateur.
 *
 * Cette fonction demande à l'utilisateur de saisir la valeur d'un dé entre 1 et 6.
 * La saisie est répétée jusqu'à ce que l'utilisateur entre une valeur valide ou 'q' pour quitter.
 *
 * @param nbredes Le numéro du dé en cours.
 * @return La valeur du dé saisie par l'utilisateur (entre 1 et 6), ou -1 si l'utilisateur veut quitter.
 */
int saisir_Valeur_des(int nbredes) {
    char caractere;
    do {
        printf("Entrez la valeur du dé %d entre 1 et 6 (ou 'q' pour quitter) : \n", nbredes);
        scanf(" %c", &caractere); // Added a space before %c to consume the newline character
        if (caractere == 'q') return -1;
    } while (!(caractere >= '1' && caractere <= '6'));
    return (int) caractere - '0';
} 

/**
 * @brief Saisit le nombre de joueurs avec une limite entre 2 et 4.
 *
 * Cette fonction demande à l'utilisateur de saisir le nombre de joueurs, min 2 et max4.
 * La saisie est répétée jusqu'à ce que l'utilisateur entre une valeur valide.
 *
 * @return Le nombre de joueurs saisi par l'utilisateur (entre 2 et 4).
 */
int nbrjoueur(){
    char caractere;
    do {
        printf("Entrez le nombre de joueurs entre 2 et 4 : \n");
        scanf(" %c", &caractere);
    } while (!(caractere >= '2' && caractere <= '4'));
    return (int) caractere - '0';
}

/**
 * @brief Fait avancer le joueur sur le plateau en fonction des résultats du lancer de dés.
 *
 * Cette fonction ajuste la position d'un joueur en fonction de la somme des dés et de certaines
 * conditions spéciales. Elle prend en compte les règles du premier tour, les cases oie spéciales,
 * les collisions avec d'autres joueurs, et les effets spéciaux des différentes cases.
 *
 * @param plateau Un tableau représentant le plateau de jeu.
 * @param positions Un tableau représentant les positions des joueurs sur le plateau.
 * @param attente Un tableau indiquant l'etat des joueur.
 * @param des Un tableau contenant les résultats des 2 lancer de dés.
 * @param joueurcourant Indique qui doit jouer.
 * @param nbjoueurs Le nombre total de joueurs.
 * @param premiertour Un indicateur qui permet de dire combien de tour total 
 * @return -1 si le joueur ne peut pas se déplacer (attente), 0 s'il n'y a pas d'événement spécial,
 *         1 si un événement spécial a été géré, et -2 si le joueur a atteint la case finale.
 */
int avancerJoueur(char plateau[], int positions[], int attente[], int des[], int joueurcourant, int nbjoueurs, int premiertour) {
    int ancienneposition = positions[joueurcourant];
    int newposition = positions[joueurcourant] + des[0] + des[1];

    // Ajuster la position si la somme des dés dépasse 99
    if (newposition > 99) {
        newposition = 99 - (newposition - 99);
    }

    if (attente[joueurcourant] != 0) {
        return -1; // Le joueur attend, ne peut pas se déplacer
    } else {
        if (premiertour == 1) {
            // Vérifier si les valeurs des dés répondent à certaines conditions pour le premier tour

            if ((des[0] == 3 || des[1] ==3) && (des[1] == 6 || des[0] == 6)) {
                newposition = 40; // Atterrir sur une case oie
                collision(positions, nbjoueurs, joueurcourant, newposition, ancienneposition);
            } else if ((des[0] == 4 || des[1] == 4) && (des[0] == 5 || des[1] == 5)) {
                newposition = 89; // Atterrir sur une case oie
                collision(positions, nbjoueurs, joueurcourant, newposition, ancienneposition);
            }
        } 
        if (verification_case(plateau, positions, joueurcourant, attente, des[0] + des[1], nbjoueurs, newposition)==0){
            collision(positions, nbjoueurs, joueurcourant, newposition, ancienneposition);
        }
    }

    if (positions[joueurcourant] == 99) {
        printf("BRAVO!\n");
        return -1;
    }
    return 1; 
}

/**
 * @brief Remplit une matrice 2D en spirale.
 *
 * Cette fonction remplit une matrice 2D en spirale, en partant du coin supérieur gauche
 * et en se déplaçant vers la droite, vers le bas, vers la gauche, puis vers le haut.
 *
 * @param spiral La matrice 2D à remplir en spirale.
 * @note La taille de la matrice est définie par la constante SIZE.
 */
void createSpiral(int spiral[SIZE][SIZE]) {
    int valeur = 0;
    int ligne, colonne;
    int startRligne = 0, startColonne = 0;
    int finligne = SIZE - 1, fincolonne = SIZE - 1;

    while (startRligne <= finligne && startColonne <= fincolonne) {
        for (colonne = startColonne; colonne <= fincolonne; colonne++) {
            spiral[startRligne][colonne] = valeur++;
        }
        startRligne++;

        for (ligne = startRligne; ligne <= finligne; ligne++) {
            spiral[ligne][fincolonne] = valeur++;
        }
        fincolonne--;

        if (startRligne <= finligne) {
            for (colonne = fincolonne; colonne >= startColonne; colonne--) {
                spiral[finligne][colonne] = valeur++;
            }
            finligne--;
        }

        if (startColonne <= fincolonne) {
            for (ligne = finligne; ligne >= startRligne; ligne--) {
                spiral[ligne][startColonne] = valeur++;
            }
            startColonne++;
        }
    }
}

/**
 * @brief Affiche la matrice en spirale avec les positions des joueurs.
 *
 * Cette fonction affiche la matrice en spirale, en mettant en évidence 
 * les positions des joueurs.
 *
 * @param spiral La matrice 2D en spirale.
 * @param position Un tableau représentant les positions des joueurs sur le plateau.
 * @param nbrjoueur Le nombre total de joueurs.
 */
void printSpiral(int spiral[SIZE][SIZE], int position[], int nbrjoueur) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int temp = -1; 
            for (int m = 0; m < nbrjoueur; m++) {
                int joueur = position[m];
                if (spiral[i][j] == joueur) {
                    temp = m;
                    break;
                }
            }
            if (temp != -1) {
                printf("|%2d", temp+1);
            } else {
                printf("|%2d", spiral[i][j]);
            }
        }
        printf("|\n");

        for (int j = 0; j < SIZE; j++) {
            printf("---");
        }
        printf("\n");
    }
}

/**
 * @brief Gère le passage au joueur suivant en prenant en compte les états d'attente spéciaux.
 *
 * Cette fonction ajuste les compteurs d'attente en fonction de l'état du joueur suivant et du joueur
 * actuel, et indique si le joueur actuel doit passer au suivant.
 *
 * @param attente Un tableau indiquant l'etat des joueur.
 * @param nbjoueurs Le nombre total de joueurs.
 * @param suivant L'indice du joueur suivant dans le tableau d'attente.
 * @param joueurcourant L'indice du joueur actuel dans le tableau d'attente.
 * @return 0 si le joueur actuel ne doit pas passer au suivant, 1 sinon.
 */
int joueurSuivant(int attente[], int nbjoueurs, int suivant,int joueurcourant) {
   
    
    if (attente[suivant]==2){
        attente[suivant]--;
        return 0;
    }
    
    if (attente[suivant] == 1) {
        if (attente[joueurcourant]==0) attente[joueurcourant]=1;
        attente[suivant]=0;
    }
    if (attente[suivant] == 0) {
        if (attente[joueurcourant]>0) attente[joueurcourant]=1;
        attente[suivant]=0;
        return 1;
    }
    return 0;
}

/**
 * @brief Vérifie si l'extension d'un fichier est ".txt".
 *
 * Cette fonction prend un tableau d'arguments de ligne de commande et la longueur de l'extension.
 * Elle vérifie si les quatre derniers caractères du deuxième argument correspondent à l'extension ".txt".
 *
 * @param argv Un tableau d'arguments de ligne de commande.
 * @param lenext La longueur de l'extension.
 * @return 0 si l'extension est ".txt", 1 sinon.
 */
int bad_ext(char * argv[], int lenext){

    if (argv[1][lenext-4] != '.' || argv[1][lenext-3] != 't' || argv[1][lenext-2] != 'x' || argv[1][lenext-1] != 't'){ //teste l'extension du fichier
            fprintf(stderr,"Fichier de sauvegarde non compatible");
            printf("\n");
            return 1;
    }
    return 0;
}

/**
 * @brief Charge des données à partir d'un fichier et simule le déroulement du jeu.
 *
 * Cette fonction lit les informations d'un fichier (représentant les dés et les états du jeu)
 * et simule le déroulement du jeu en fonction des données chargées.
 *
 * @param fichier Le fichier à partir duquel les données sont lues.
 * @param tab Le tableau de jeu.
 * @param position Les positions actuelles des joueurs.
 * @param attente Un tableau indiquant l'etat des joueur.
 * @param des Les valeurs des dés.
 * @param nbjoueurs Le nombre total de joueurs.
 * @param nbrtour Le nombre de tours joués.
 * @param joueurcourant L'indice du joueur courant.
 * @param game L'état du jeu.
 * @return 1 si la fonction s'exécute normalement, -1 en cas d'erreur.
 */
int load(FILE *fichier, char tab[], int position[], int attente[], int des[], int nbjoueurs, int nbrtour, int joueurcourant,int game){
    fgetc(fichier);

    char c = fgetc(fichier); //contient toujours une valeur de dé
    printf("%c\n",c);

    while (c != EOF){ //écriture des dés dans le tab dés

        des[0] = (int) c - '0';   
        
        fgetc(fichier);


        c = fgetc(fichier); //deuxième dé
        printf("%c\n",c);

        des[1] = (int) c - '0';

        fgetc(fichier);
        

        if (nbjoueurs==2){
            if ((position[0] == 53 && position[1] == 74) || (position[1]==53 && position[0] == 74)){
                printf("bande de noobs");
                return -1;
            }
        }

        printf("des[0] %d\n",des[0]);
        printf("des[1] %d\n\n",des[1]);
        //à partir de là c pareil que dans le main
        game =avancerJoueur(tab,  position, attente, des, joueurcourant,  nbjoueurs,nbrtour);
        //int spiral[SIZE][SIZE];
        //createSpiral(spiral);
        //printSpiral(spiral, position, nbjoueurs);
        printf("%d,%d,%d,%d\n",position[0],position[1],position[2],position[3]);
        printf("%d,%d,%d,%d\n",attente[0],attente[1],attente[2],attente[3]);
        int suivant=(joueurcourant+1)%nbjoueurs;
        
        int cmpt=1;
        
        do {
            suivant=(joueurcourant+cmpt)%nbjoueurs;
            cmpt++;
        }while(joueurSuivant( attente,  nbjoueurs,  suivant, joueurcourant)==0);
        if (suivant==0){
            nbrtour++;
        }
        joueurcourant=suivant;
        c = fgetc(fichier);
    }
    return 1;
}

/**
 * @brief   Fonction personnalisée pour séparer une chaîne en utilisant la virgule comme délimiteur
 *
 * @param   input   La chaîne d'entrée à diviser
 * @param   value1  Pointeur vers la variable où stocker la première valeur
 * @param   value2  Pointeur vers la variable où stocker la deuxième valeur
 *
 * Cette fonction prend une chaîne de caractères en entrée et extrait deux valeurs en utilisant la virgule comme délimiteur.
 * Les valeurs extraites sont stockées dans les variables pointées par value1 et value2.
 */
void splitString(const char *input, int *value1, int *value2) {
    // Variables pour stocker les indices de début et de fin des valeurs
    int start = 0, end = 0;

    // Trouver la position de la virgule
    while (input[end] != ',' && input[end] != '\0') {
        end++;
    }

    // Extraire la première valeur
    *value1 = atoi(input);

    // Si la virgule a été trouvée, extraire la deuxième valeur
    if (input[end] == ',') {
        start = end + 1;
        *value2 = atoi(input + start);
    }
}


int main (int argc, char *argv[]){
    if (argc>2 && argv[1][0]=='-'&& argv[1][1] == 'W'){
        if (argc>=5)
            srand(atoi(argv[4]));
        else  srand(time(NULL));

        int valeur1, valeur2;
        splitString(argv[2], &valeur1, &valeur2);

        
        int nombre_boucle;
        nombre_boucle = valeur2;

        if (!(valeur1 <= 4 && valeur1 >= 2)){
            print("Nombre de joueurs trop élevé ou trop petit.");
            return 0;
        }

        float somme=0;
        int tout_premier_tour=0;
        int nombre_de_tour_min;
        int nombre_de_tour_max;
        int nombre_de_fois_jeux_bloque=0;
        int nbjoueurs = valeur1;

        for (int i =0; i<nombre_boucle;i++){
            char tab[100];
            creation_tab(tab);
            int position []={0,0,0,0};
            int attente []={0,1,1,1};
            int des[2] = {0,0};
            int nbrtour=1;
            int joueurcourant=0;
            int game=1;
            while (game > 0){
                 if (nbjoueurs==2){
                    if ((position[0] == 53 && position[1] == 74) || (position[1]==53 && position[0] == 74)){
                        printf("Aucun gagnant");
                        game=-2;
                        break;
                    }
                }
                des[0]=rand()%7;
                des[1]=rand()%7;

             
                game =avancerJoueur(tab,  position, attente, des, joueurcourant,  nbjoueurs,nbrtour);
                int suivant=(joueurcourant+1)%nbjoueurs;
                int cmpt=1;
                do {
                    suivant=(joueurcourant+cmpt)%nbjoueurs;
                    cmpt++;
                }while(joueurSuivant( attente,  nbjoueurs,  suivant, joueurcourant)==0);
                if (suivant==0){
                    nbrtour++;
                }
                joueurcourant=suivant;
    
    
            }
 

    
            if (game ==-1 ){
                for (int i =0;i<nbjoueurs;i++){
                    if (tout_premier_tour==0){
                        nombre_de_tour_max=nbrtour;
                        nombre_de_tour_min=nbrtour;
                        somme+=nbrtour;
                        tout_premier_tour+=1;
                    }
                    else{
                        if (nbrtour<nombre_de_tour_min) nombre_de_tour_min=nbrtour;
                        if (nbrtour>nombre_de_tour_max) nombre_de_tour_max=nbrtour;
                        somme+=nbrtour;
                    }

                }
            }
            else nombre_de_fois_jeux_bloque++;
        }
    printf ("Nombre de simulation: %d  et avec: %d joueur\n",nombre_boucle,nbjoueurs);
    printf ("Nombre tour moyen %f\n",somme/nombre_boucle);
    printf ("NOmbre max: %d\n",nombre_de_tour_max);
    printf ("Nombre min : %d\n",nombre_de_tour_min);

    }
    
    else{
        char tab[100];
        creation_tab(tab);
        int position []={0,0,0,0};
        int attente []={0,1,1,1};
        int des[2] = {0,0};
        int nbjoueurs;
        int nbrtour=1;
        int joueurcourant=0;
        int game=1;

    
    FILE *fichier = NULL;
    fichier = fopen(argv[1],"r");
    
    if (fichier == NULL){ //le fichier de save n existe pas
        

        
        nbjoueurs = nbrjoueur();
        
        if (argc > 1){
            fichier = fopen(argv[1],"a");//on réouvre le fichier pour écrire dedans
        } else {
            fichier = fopen("sauvegarde.txt","a");
        }

        fprintf(fichier, "J0\n");
        fprintf(fichier,"%d\n",nbjoueurs);
    }

    else { //le fichier existe
        
        int lenext = 0; 
        while (argv[1][lenext] != '\0'){
            lenext ++;
        }

        if (bad_ext(argv,lenext)) game = -1; //mauvaise extension
        

        else {  //le fichier existe et contient déjà des infos
            printf("Sauvegarde trouvée <chargement>\n\n");
            char s;
            fgets(&s,4,fichier); 

            char tmp = fgetc(fichier);
            nbjoueurs = tmp - '0';

            game = load(fichier,tab,position,attente,des,nbjoueurs,nbrtour,joueurcourant,game);
            
        }
    }
    
    fclose(fichier);
    if (argc > 1){
        fichier = fopen(argv[1],"a");//on réouvre le fichier pour écrire dedans
    } else {
        fichier = fopen("sauvegarde.txt","a");
    }

    while (game > 0){
        if (nbjoueurs==2){
            if ((position[0] == 53 && position[1] == 74) || (position[1]==53 && position[0] == 74)){
                printf("Aucun gagnant");
                game=-2;
                break;
            }
        }
        printf ("Tour du joueur numéro %d\n",joueurcourant+1);

        des[0]=saisir_Valeur_des(1);

        if (des[0] == -1){
            game=-3;
            break;
        }

        des[1]=saisir_Valeur_des(2);

        if (des[1] == -1) {
            game=-3;
            break;
        }

        fprintf(fichier,"%d %d\n",des[0],des[1]);

        game =avancerJoueur(tab,  position, attente, des, joueurcourant,  nbjoueurs,nbrtour);
        int spiral[SIZE][SIZE];
        createSpiral(spiral);
        printSpiral(spiral, position, nbjoueurs);
        int suivant=(joueurcourant+1)%nbjoueurs;
        
        int cmpt=1;
        
        do {
            suivant=(joueurcourant+cmpt)%nbjoueurs;
            cmpt++;
        }while(joueurSuivant( attente,  nbjoueurs,  suivant, joueurcourant)==0);
        if (suivant==0){
            nbrtour++;
        }
        joueurcourant=suivant;
    
    }
    fclose(fichier);

    
    if (game ==-1 ){
        for (int i =0;i<nbjoueurs;i++){
            if (position[i]==99)printf ("Le gagnant est le joueur %d en %d tours",i+1,nbrtour);

        }
    }
    if (game == -3){
        printf ("La sauvegarde est dans le fichier sauvegarde.txt");
    }
    }
}