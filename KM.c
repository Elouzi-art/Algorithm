#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>


typedef struct point {
    int abscissa;
    struct point* next;
} Point;

typedef struct Group {
    int center;
    Point* list;
    float old_avg;
} Group;

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
void assign_point(Group* groups, int k , int *data, int data_size, bool from_scratch);

int main() {
    int data[100];
    int data_size = sizeof(data) / sizeof(data[0]);

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

    Group* groups = malloc(k * sizeof(Group));
    if (!groups) {
        fprintf(stderr, "Error allocating memory\n");
        return 1;
    }

    assign_point(groups, k, data, data_size, true);
    print_group_info(groups, k, "Initial configuration");

    int iteration = 0;
    do {
        iteration++;
        update_centers(groups, k);
        assign_point(groups, k, data, data_size, false);
       // print_group_info(groups, k, "Iteration");
    } while (!has_converged(groups, k) && iteration < max_iterations);

    print_final_results(groups, k, iteration);
    cleanup_groups(groups, k);

    return 0;
}

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

void push_node(Point** head, int x) {
    Point* new_node = create_node(x);
    if (!*head) {
        *head = new_node;
        return;
    }
    new_node->next = *head;
    *head = new_node;
}

void free_list(Point** head) {
    if (!*head) return;
    Point* temp;
    while (*head != NULL) {
        temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}

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

int count_points(Point* list) {
    int count = 0;
    Point* tmp = list;
    while (tmp != NULL) {
        count++;
        tmp = tmp->next;
    }
    return count;
}

void clear_group_list(Point** head) {
    free_list(head);
    *head = NULL;
}

void print_group_points(Point* list) {
    Point* current = list;
    while (current != NULL) {
        printf("%d ", current->abscissa);
        current = current->next;
    }
}

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

bool has_converged(Group* groups, int k) {
    for (int i = 0; i < k; i++) {
        float current_avg = calculate_average(groups[i].list);
        if (fabs(current_avg - groups[i].old_avg) > 0.001)
            return false;
    }
    return true;
}

void update_centers(Group* groups, int k) {
    for (int i = 0; i < k; i++) {
        groups[i].old_avg = calculate_average(groups[i].list);
        groups[i].center = (int)round(groups[i].old_avg);
    }
}

void print_group_info(Group* groups, int k, const char* title) {
    printf("\n%s:\n", title);
    for (int i = 0; i < k; i++) {
        printf("Group %d (center: %d): ", i, groups[i].center);
        print_group_points(groups[i].list);
        printf(" | Avg: %.2f\n", calculate_average(groups[i].list));
    }
}

void print_final_results(Group* groups, int k, int iterations) {
    printf("\n========== FINAL RESULTS ==========\n");
    printf("Converged after %d iterations\n\n", iterations);

    /*
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

void cleanup_groups(Group* groups, int k) {
    for (int i = 0; i < k; i++)
        free_list(&(groups[i].list));
    free(groups);
}


void assign_point(Group* groups, int k , int *data, int data_size, bool from_scratch){

    Point* all_points = NULL;

    if (from_scratch)
    {
       for (int i = 0; i < k && i < data_size; ++i)
        {
            groups[i].center = data[i];
            groups[i].list = NULL;
            groups[i].old_avg = 0;
        }

    for(int i = 0; i < data_size; ++i)
        push_node(&all_points, data[i]);    

    }else{

        for (int i = 0; i < k; ++i)
        {
            Point* current = groups[i].list;
            while(current){

                push_node(&all_points, current->abscissa);
                current = current->next;
            }
            clear_group_list(&(groups[i].list));
        }
    }

    Point* tmp = all_points;

    while(tmp){

        int nearest = find_nearest_group(groups, k, tmp->abscissa);
        push_node(&(groups[nearest].list), tmp->abscissa);
        tmp = tmp->next;
    }
    free_list(&all_points);
    
}