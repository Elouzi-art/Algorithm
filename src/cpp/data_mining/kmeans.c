/**
 * @file kmeans_1d.c
 * @brief Implémentation de l'algorithme des k‑moyennes (k‑means) sur des données unidimensionnelles.
 *
 * Les données sont stockées dans un tableau d'entiers. L'algorithme partitionne les points
 * en k groupes en minimisant la distance absolue (L1) entre chaque point et le centre de son groupe.
 * Les centres sont mis à jour comme l'arrondi de la moyenne des points du groupe.
 * La convergence est atteinte lorsque la variation de la moyenne de chaque groupe est inférieure à 0.001.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

/* ==========================================================================
 * Structures de données
 * ========================================================================== */

/**
 * @struct point
 * @brief Nœud d'une liste chaînée représentant un point (valeur entière).
 */
typedef struct point {
    int abscissa;          /**< Valeur du point. */
    struct point* next;    /**< Pointeur vers l'élément suivant. */
} Point;

/**
 * @struct Group
 * @brief Groupe de l'algorithme k‑means.
 *
 * Chaque groupe possède un centre (entier), une liste chaînée des points qui lui sont attribués,
 * et l'ancienne moyenne (utilisée pour tester la convergence).
 */
typedef struct Group {
    int center;          /**< Centre actuel du groupe (entier arrondi de la moyenne). */
    Point* list;         /**< Liste des points attribués au groupe. */
    float old_avg;       /**< Moyenne précédente pour vérifier la convergence. */
} Group;

/* ==========================================================================
 * Prototypes des fonctions
 * ========================================================================== */

Point* create_node(int x);
void push_node(Point** head, int x);
void free_list(Point** head);
float calculate_average(Point* list);
int count_points(Point* list);
void clear_group_list(Point** head);
void print_group_points(Point* list);
int find_nearest_group(Group* groups, int k, int value);
bool has_converged(Group* groups, int k);
void update_centers(Group* groups, int k);
void print_group_info(Group* groups, int k, const char* title);
void print_final_results(Group* groups, int k, int iterations);
void cleanup_groups(Group* groups, int k);
void assign_point(Group* groups, int k, int *data, int data_size, bool from_scratch);

/* ==========================================================================
 * Fonction principale
 * ========================================================================== */

int main() {
    int data[100];                     /**< Tableau de 100 points aléatoires. */
    int data_size = sizeof(data) / sizeof(data[0]);

    /* Initialisation du générateur aléatoire et remplissage des données */
    srand(time(NULL));
    for (int i = 0; i < data_size; ++i)
        data[i] = rand() % 1234;

    int k, max_iterations = 100;

    printf("Enter number of groups (k): ");
    scanf("%d", &k);

    if (k <= 0 || k > data_size) {
        printf("Invalid number of groups. Must be between 1 and %d\n", data_size);
        return 1;
    }

    /* Allocation du tableau de groupes */
    Group* groups = malloc(k * sizeof(Group));
    if (!groups) {
        fprintf(stderr, "Error allocating memory\n");
        return 1;
    }

    /* Assignation initiale : les k premiers points servent de centres initiaux */
    assign_point(groups, k, data, data_size, true);
    print_group_info(groups, k, "Initial configuration");

    /* Boucle principale de l'algorithme k‑means */
    int iteration = 0;
    do {
        iteration++;
        update_centers(groups, k);          /* Recalcul des centres */
        assign_point(groups, k, data, data_size, false); /* Réaffectation des points */
        // print_group_info(groups, k, "Iteration");   /* Décommenter pour afficher chaque itération */
    } while (!has_converged(groups, k) && iteration < max_iterations);

    /* Affichage des résultats finaux */
    print_final_results(groups, k, iteration);
    cleanup_groups(groups, k);

    return 0;
}

/* ==========================================================================
 * Implémentation des fonctions
 * ========================================================================== */

/**
 * @brief Crée un nouveau nœud Point avec la valeur donnée.
 * @param x Valeur du point.
 * @return Pointeur vers le nœud alloué.
 */
Point* create_node(int x) {
    Point* node = malloc(sizeof(Point));
    if (!node) {
        fprintf(stderr, "Error \"malloc\" failed\n");
        exit(EXIT_FAILURE);
    }
    node->abscissa = x;
    node->next = NULL;
    return node;
}

/**
 * @brief Ajoute un nouveau point en tête d'une liste chaînée.
 * @param head Pointeur vers la tête de la liste (modifiable).
 * @param x    Valeur à insérer.
 */
void push_node(Point** head, int x) {
    Point* new_node = create_node(x);
    if (!*head) {
        *head = new_node;
        return;
    }
    new_node->next = *head;
    *head = new_node;
}

/**
 * @brief Libère toute la mémoire occupée par une liste chaînée.
 * @param head Pointeur vers la tête de la liste (sera mis à NULL).
 */
void free_list(Point** head) {
    if (!*head) return;
    Point* temp;
    while (*head != NULL) {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}

/**
 * @brief Calcule la moyenne des valeurs d'une liste chaînée.
 * @param list Tête de la liste.
 * @return Moyenne (float), ou 0 si la liste est vide.
 */
float calculate_average(Point* list) {
    float sum = 0;
    int count = 0;
    Point* tmp = list;
    while (tmp != NULL) {
        sum += tmp->abscissa;
        tmp = tmp->next;
        count++;
    }
    if (count == 0) return 0;
    return sum / count;
}

/**
 * @brief Compte le nombre d'éléments dans une liste.
 * @param list Tête de la liste.
 * @return Nombre de points.
 */
int count_points(Point* list) {
    int count = 0;
    Point* tmp = list;
    while (tmp != NULL) {
        count++;
        tmp = tmp->next;
    }
    return count;
}

/**
 * @brief Vide complètement une liste (libère la mémoire et met la tête à NULL).
 * @param head Pointeur vers la tête de la liste.
 */
void clear_group_list(Point** head) {
    free_list(head);
    *head = NULL;
}

/**
 * @brief Affiche les valeurs d'une liste chaînée sur la sortie standard.
 * @param list Tête de la liste.
 */
void print_group_points(Point* list) {
    Point* current = list;
    while (current != NULL) {
        printf("%d ", current->abscissa);
        current = current->next;
    }
}

/**
 * @brief Trouve l'index du groupe dont le centre est le plus proche d'une valeur donnée.
 * @param groups Tableau de groupes.
 * @param k      Nombre de groupes.
 * @param value  Valeur à tester.
 * @return Index du groupe le plus proche.
 */
int find_nearest_group(Group* groups, int k, int value) {
    int nearest = 0;
    float min_dist = fabs(groups[0].center - value);
    for (int i = 1; i < k; i++) {
        float dist = fabs(groups[i].center - value);
        if (dist < min_dist) {
            min_dist = dist;
            nearest = i;
        }
    }
    return nearest;
}

/**
 * @brief Vérifie si l'algorithme a convergé (variation des moyennes < 0.001).
 * @param groups Tableau de groupes.
 * @param k      Nombre de groupes.
 * @return true si tous les groupes ont convergé, false sinon.
 */
bool has_converged(Group* groups, int k) {
    for (int i = 0; i < k; i++) {
        float current_avg = calculate_average(groups[i].list);
        if (fabs(current_avg - groups[i].old_avg) > 0.001)
            return false;
    }
    return true;
}

/**
 * @brief Met à jour les centres des groupes :
 *        calcule la nouvelle moyenne et l'arrondit pour obtenir le nouveau centre.
 * @param groups Tableau de groupes.
 * @param k      Nombre de groupes.
 */
void update_centers(Group* groups, int k) {
    for (int i = 0; i < k; i++) {
        groups[i].old_avg = calculate_average(groups[i].list);
        groups[i].center = (int)round(groups[i].old_avg);
    }
}

/**
 * @brief Affiche l'état courant des groupes (centre et liste des points).
 * @param groups Tableau de groupes.
 * @param k      Nombre de groupes.
 * @param title  Titre affiché avant les détails.
 */
void print_group_info(Group* groups, int k, const char* title) {
    printf("\n%s:\n", title);
    for (int i = 0; i < k; i++) {
        printf("Group %d (center: %d): ", i, groups[i].center);
        print_group_points(groups[i].list);
        printf(" | Avg: %.2f\n", calculate_average(groups[i].list));
    }
}

/**
 * @brief Affiche les résultats finaux : nombre d'itérations et éventuellement les groupes.
 *        (La partie détaillée est commentée pour alléger la sortie.)
 * @param groups     Tableau de groupes.
 * @param k          Nombre de groupes.
 * @param iterations Nombre d'itérations effectuées.
 */
void print_final_results(Group* groups, int k, int iterations) {
    printf("\n========== FINAL RESULTS ==========\n");
    printf("Converged after %d iterations\n\n", iterations);

    /* 
       Cette partie est commentée ; décommentez‑la pour afficher tous les détails.
    for (int i = 0; i < k; i++) {
        printf("Group %d:\n", i);
        printf("  Center: %d\n", groups[i].center);
        printf("  Average: %.2f\n", calculate_average(groups[i].list));
        printf("  Number of points: %d\n", count_points(groups[i].list));
        printf("  Points: ");
        print_group_points(groups[i].list);
        printf("\n\n");
    }
    */
}

/**
 * @brief Libère la mémoire de tous les groupes.
 * @param groups Tableau de groupes.
 * @param k      Nombre de groupes.
 */
void cleanup_groups(Group* groups, int k) {
    for (int i = 0; i < k; i++)
        free_list(&(groups[i].list));
    free(groups);
}

/**
 * @brief Fonction cruciale d'assignation des points aux groupes.
 *
 * Selon le paramètre from_scratch, elle initialise les centres avec les k premières
 * données ou récupère les points déjà attribués. Dans les deux cas, elle vide les listes
 * des groupes, reconstruit une liste globale de tous les points, puis les réaffecte
 * au groupe dont le centre est le plus proche (distance L1).
 *
 * @param groups      Tableau de groupes.
 * @param k           Nombre de groupes.
 * @param data        Tableau des données (pour l'initialisation).
 * @param data_size   Taille du tableau de données.
 * @param from_scratch Si true, initialisation complète ; sinon, réaffectation à partir des listes existantes.
 */
void assign_point(Group* groups, int k, int *data, int data_size, bool from_scratch) {
    Point* all_points = NULL;

    if (from_scratch) {
        /* Initialisation : les k premiers points deviennent les centres ; les listes sont vidées */
        for (int i = 0; i < k && i < data_size; ++i) {
            groups[i].center = data[i];
            groups[i].list = NULL;
            groups[i].old_avg = 0;
        }

        /* On construit une liste contenant toutes les données */
        for (int i = 0; i < data_size; ++i)
            push_node(&all_points, data[i]);
    } else {
        /* Réaffectation : on extrait tous les points des listes des groupes */
        for (int i = 0; i < k; ++i) {
            Point* current = groups[i].list;
            while (current) {
                push_node(&all_points, current->abscissa);
                current = current->next;
            }
            clear_group_list(&(groups[i].list));  /* on vide la liste du groupe */
        }
    }

    /* Parcours de la liste globale et assignation de chaque point au groupe le plus proche */
    Point* tmp = all_points;
    while (tmp) {
        int nearest = find_nearest_group(groups, k, tmp->abscissa);
        push_node(&(groups[nearest].list), tmp->abscissa);
        tmp = tmp->next;
    }

    /* Nettoyage de la liste globale temporaire */
    free_list(&all_points);
}
