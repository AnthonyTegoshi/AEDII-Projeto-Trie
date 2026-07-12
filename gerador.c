#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
	if (argc != 4) {
		printf("Argumentos: {tamanho da sequência} {número de sequências} {arquivo com resultado}");
		return 1;
	}
	
	const char *bases[] = {"A", "C", "G", "T"};
	int k = atoi(argv[1]);
	int n = atoi(argv[2]);
	char *filename = argv[3];
	
	srand(time(NULL));
	
	FILE *file = fopen(filename, "w");
	// A primeira linha contém o k-mers e número de sequências
	fprintf(file, argv[1]);
	fprintf(file, " ");
	fprintf(file, argv[2]);
	fprintf(file, "\n");
	// Gera aleatoriamente as sequências com rand()
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < k; j++) {
			fprintf(file, bases[rand() % 4]);
		}
		fprintf(file, "\n");
	}
	fclose(file);
	
	return 0;
}