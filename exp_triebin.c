#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct TrieBin TrieBin;
typedef struct TrieBinNode TrieBinNode;

struct TrieBin {
	TrieBinNode *children[2];
	int nodes;
};

struct TrieBinNode {
	TrieBinNode *children[2];
	bool end;
};

bool parseBaseTrieBin(char, int *);
TrieBin *createTrieBin();
TrieBinNode *createTrieBinNode();
void insertTrieBin(TrieBin *, char *, int);
bool searchTrieBin(TrieBin *, char *, int);
void destroyTrieBin(TrieBin *);
void destroyTrieBinNode(TrieBinNode *);

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Argumentos: {arquivo com sequências} {arquivo com consultas}");
		return 1;
	}
	
	const int rep = 5; // número de vezes para repetir cada experimento (e tirar a média)
	clock_t start, finish; // para medir tempo de execução
	char *seq_filename = argv[1];
	char *srch_filename = argv[2];
	int k; // k-mers
	int insertions, searches; // quantas inserções/buscas tem nos testes
	
	double trieBinInsertionTime = 0;
	double trieBinSearchTime = 0;
	int trieBinNodes = 0;
	double trieBinSpaceKB = 0;

	for (int i = 0; i < rep; i++) {
		TrieBin *trieBin = createTrieBin();
		
		// INSERÇÃO
		FILE *seq_file = fopen(seq_filename, "r");
		// encontra o k-mers e inserções na primeira linha
		fscanf(seq_file, "%d", &k);
		fscanf(seq_file, "%d", &insertions);
		// lê a sequência e adiciona na trie
		char seq[k + 1];
		start = clock();
		while (fscanf(seq_file, "%s", seq) == 1) {
			insertTrieBin(trieBin, seq, k);
		}
		finish = clock();
		trieBinInsertionTime += (double)(finish - start) / (rep * CLOCKS_PER_SEC);
		fclose(seq_file);
		
		// BUSCA
		FILE *srch_file = fopen(srch_filename, "r");
		// encontra o k-mers e buscas na primeira linha
		fscanf(srch_file, "%d", &k);
		fscanf(srch_file, "%d", &searches);
		// lê a sequência e adiciona na trie
		char srch[k + 1];
		start = clock();
		while (fscanf(srch_file, "%s", srch) == 1) {
			searchTrieBin(trieBin, srch, k);
		}
		finish = clock();
		trieBinSearchTime += (double)(finish - start) / (rep * CLOCKS_PER_SEC);
		fclose(srch_file);
		
		trieBinNodes = trieBin->nodes;
		// espaço ocupado pelos nós
		trieBinSpaceKB += (double)(trieBin->nodes * sizeof(TrieBinNode)) / (rep * 1024);
		
		destroyTrieBin(trieBin);
	}
	
	printf("%d,%.3f,%d,%.3f,%d,%.2f\n", insertions, trieBinInsertionTime, searches, trieBinSearchTime, trieBinNodes, trieBinSpaceKB);
	
	return 0;
}

bool parseBaseTrieBin(char base, int *bits) {
	switch (base) {
		case 'A':
			bits[0] = 0;
			bits[1] = 0;
			return true;
		case 'C':
			bits[0] = 0;
			bits[1] = 1;
			return true;
		case 'G':
			bits[0] = 1;
			bits[1] = 0;
			return true;
		case 'T':
			bits[0] = 1;
			bits[1] = 1;
			return true;
		default:
			return false;
	}
}

TrieBin *createTrieBin() {
	return calloc(1, sizeof(TrieBin));
}

TrieBinNode *createTrieBinNode() {
	return calloc(1, sizeof(TrieBinNode));
}

void insertTrieBin(TrieBin *trieBin, char *seq, int k) {
	TrieBinNode **current = trieBin->children;
	TrieBinNode *last = NULL;
	for (int i = 0; i < k; i++) {
		// cada base exige 2 níveis
		int bits[2];
		if (!parseBaseTrieBin(seq[i], bits)) return;
		for (int j = 0; j < 2; j++) {
			int index = bits[j];
			// cria um novo caso não exista e aumenta o número de nós
			if (current[index] == NULL) {
				current[index] = createTrieBinNode();
				trieBin->nodes++;
			}
			last = current[index];
			current = current[index]->children;
		}
	}
	last->end = true;
}

bool searchTrieBin(TrieBin *trieBin, char *seq, int k) {
	TrieBinNode *current = NULL;
	for (int i = 0; i < k; i++) {
		int bits[2];
		if (!parseBaseTrieBin(seq[i], bits)) return false; // base não existe
		for (int j = 0; j < 2; j++) {
			int index = bits[j];
			if (i == 0 && j == 0) {
				current = trieBin->children[index];
			} else {
				current = current->children[index];
			}
			if (current == NULL) return false; // não encontrou precocemente
		}
	}
	// se o nó final tiver end = true, então encontrou
	return current->end;
}

void destroyTrieBinNode(TrieBinNode *node) {
    if (node == NULL) return;
    for (int i = 0; i < 2; i++) {
        destroyTrieBinNode(node->children[i]);
    }
    free(node);
}

void destroyTrieBin(TrieBin *trieBin) {
    if (trieBin == NULL) return;
    for (int i = 0; i < 2; i++) {
        destroyTrieBinNode(trieBin->children[i]);
    }
    free(trieBin);
}