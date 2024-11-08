#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Variables globales pour suivre la mémoire
size_t memory_used = 0;
size_t peak_memory_used = 0;
int max_recursion_depth = 0;
int current_recursion_depth = 0;

// Fonction de remplacement pour malloc avec comptage
void* malloc_count(size_t size) {
    memory_used += size;
    if (memory_used > peak_memory_used) {
        peak_memory_used = memory_used;
    }
    printf("Mémoire allouée : %zu octets\n", size);
    return malloc(size);
}

// Fonction de remplacement pour free avec comptage
void free_count(void* ptr, size_t size) {
    memory_used -= size;
    printf("Mémoire libérée : %zu octets\n", size);
    free(ptr);
}

// Fonction récursive pour calculer la LCS
int lcs_recursive(char *X, char *Y, int m, int n) {
    if (m == 0 || n == 0)
        return 0;

    // Mise à jour de la profondeur de récursion
    current_recursion_depth++;
    if (current_recursion_depth > max_recursion_depth) {
        max_recursion_depth = current_recursion_depth;
    }

    int result;
    if (X[m - 1] == Y[n - 1])
        result = 1 + lcs_recursive(X, Y, m - 1, n - 1);
    else
        result = (lcs_recursive(X, Y, m, n - 1) > lcs_recursive(X, Y, m - 1, n)) ?
                 lcs_recursive(X, Y, m, n - 1) : lcs_recursive(X, Y, m - 1, n);

    // Réduction de la profondeur de récursion
    current_recursion_depth--;

    return result;
}
// Fonction LCS avec matrice complète
int lcs_full_matrix(char *X, char *Y) {
    int m = strlen(X);
    int n = strlen(Y);
    int** L = (int**)malloc_count((m + 1) * sizeof(int*));
    for (int i = 0; i <= m; i++) {
        L[i] = (int*)malloc_count((n + 1) * sizeof(int));
    }
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0 || j == 0)
                L[i][j] = 0;
            else if (X[i - 1] == Y[j - 1])
                L[i][j] = L[i - 1][j - 1] + 1;
            else
                L[i][j] = (L[i - 1][j] > L[i][j - 1]) ? L[i - 1][j] : L[i][j - 1];
        }
    }
    int result = L[m][n];
    for (int i = 0; i <= m; i++) {
        free_count(L[i], (n + 1) * sizeof(int));
    }
    free_count(L, (m + 1) * sizeof(int*));
    return result;
}

// Fonction LCS optimisée en espace
int lcs_optimized(char *X, char *Y) {
    int m = strlen(X);
    int n = strlen(Y);
    int* prev = (int*)malloc_count((n + 1) * sizeof(int));
    int* curr = (int*)malloc_count((n + 1) * sizeof(int));
    for (int j = 0; j <= n; j++) {
        prev[j] = 0;
    }
    for (int i = 1; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (j == 0)
                curr[j] = 0;
            else if (X[i - 1] == Y[j - 1])
                curr[j] = prev[j - 1] + 1;
            else
                curr[j] = (prev[j] > curr[j - 1]) ? prev[j] : curr[j - 1];
        }
        for (int j = 0; j <= n; j++) {
            prev[j] = curr[j];
        }
    }
    int result = prev[n];
    free_count(prev, (n + 1) * sizeof(int));
    free_count(curr, (n + 1) * sizeof(int));
    return result;
}

int main() {
    char X[] = "ACTGKKSBHYTGHBKLT";
    char Y[] = "GACTOIUFTGBNLDLKHGSR";

    // Ouvrir le fichier CSV en mode écriture
    FILE *fichier = fopen("resultats.csv", "w");
    if (fichier == NULL) {
        printf("Erreur d'ouverture du fichier.\n");
        return 1;
    }

    // Écrire les en-têtes du fichier CSV
    fprintf(fichier, "Algorithme,Temps (secondes),Mémoire maximale (octets),Longueur LCS\n");

    // Mesurer le temps et la mémoire pour l'algorithme récursif
     clock_t start = clock();
    int lcs_length_rec = lcs_recursive(X, Y, strlen(X), strlen(Y));
    clock_t end = clock();
    double time_spent_rec = (double)(end - start) / CLOCKS_PER_SEC;

    // Affichage des résultats
    printf("Profondeur maximale de récursion: %d\n", max_recursion_depth);
    fprintf(fichier, "Récursif,%f,%d,%d\n", time_spent_rec, max_recursion_depth, lcs_length_rec);


    // Réinitialiser le compteur de mémoire
    memory_used = 0;
    peak_memory_used = 0;

    // Mesurer le temps et la mémoire pour l'algorithme avec matrice complète
    start = clock();
    int lcs_length_full = lcs_full_matrix(X, Y);
    end = clock();
    double time_spent_full = (double)(end - start) / CLOCKS_PER_SEC;
    fprintf(fichier, "Matrice complète,%f,%zu,%d\n", time_spent_full, peak_memory_used, lcs_length_full);

    // Réinitialiser le compteur de mémoire
    memory_used = 0;
    peak_memory_used = 0;

    // Mesurer le temps et la mémoire pour l'algorithme optimisé en espace
    start = clock();
    int lcs_length_opt = lcs_optimized(X, Y);
    end = clock();
    double time_spent_opt = (double)(end - start) / CLOCKS_PER_SEC;
    fprintf(fichier, "Optimisé,%f,%zu,%d\n", time_spent_opt, peak_memory_used, lcs_length_opt);

    // Fermer le fichier CSV
    fclose(fichier);

    printf("Les résultats ont été enregistrés dans resultats.csv\n");
    return 0;
}
