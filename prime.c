#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

void write_primes_range_to_file(int start, int end, const char* filename) {
    if (end < 2 || start > end) return;

    int sieve_limit = (int)sqrt(end) + 1;

    int base_size = (sieve_limit - 1) / 2;
    bool* base_prime = malloc(base_size);
    for (int i = 0; i < base_size; i++)
        base_prime[i] = true;

    for (int i = 3; i * i <= sieve_limit; i += 2) {
        if (base_prime[(i - 3) / 2]) {
            for (int j = i * i; j <= sieve_limit; j += 2 * i)
                base_prime[(j - 3) / 2] = false;
        }
    }

    int range = end - start + 1;
    bool* is_prime = malloc(range);
    for (int i = 0; i < range; i++)
        is_prime[i] = true;

    if (start == 0) is_prime[0] = false;
    if (start <= 1 && end >= 1) is_prime[1 - start] = false;

    for (int i = 0; i < base_size; i++) {
        if (base_prime[i]) {
            int p = 2 * i + 3;

            int start_index = (start + p - 1) / p * p;
            if (start_index < p * p) start_index = p * p;

            for (int j = start_index; j <= end; j += p) {
                is_prime[j - start] = false;
            }
        }
    }

    FILE* f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "Fehler beim Öffnen der Datei.\n");
        free(base_prime);
        free(is_prime);
        return;
    }

    if (start <= 2 && end >= 2)
        fprintf(f, "2\n");

    for (int i = 0; i < range; i++) {
        int number = start + i;
        if (number > 2 && number % 2 == 0)
            continue;

        if (is_prime[i])
            fprintf(f, "%d\n", number);
    }

    fclose(f);
    free(base_prime);
    free(is_prime);
}

int main() {
    int start, end;
    printf("Von welcher Zahl starten? ");
    scanf("%d", &start);
    printf("Bis zu welcher Zahl suchen? ");
    scanf("%d", &end);

    write_primes_range_to_file(start, end, "primes.txt");

    printf("Fertig! Primzahlen stehen in 'primes.txt'\n");
    return 0;
}
