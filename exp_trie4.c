#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Trie4 Trie4;
typedef struct Trie4Node Trie4Node;

struct Trie4 {
	Trie4Node *children[4];
	int nodes;
};

struct Trie4Node {
	Trie4Node *children[4];
	bool end;
};

int parseBaseTrie4(char);
Trie4 *createTrie4();
Trie4Node *createTrie4Node();
void insertTrie4(Trie4 *, char *, int);
bool searchTrie4(Trie4 *, char *, int);
void destroyTrie4(Trie4 *);
void destroyTrie4Node(Trie4Node *);

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
	
	double trie4InsertionTime = 0;
	double trie4SearchTime = 0;
	int trie4Nodes = 0;
	double trie4SpaceKB = 0;
	// roda o experimento diversas vezes e tira a média dos resultados
	for (int i = 0; i < rep; i++) {
		Trie4 *trie4 = createTrie4();
		
		// INSERÇÃO
		FILE *seq_file = fopen(seq_filename, "r");
		// encontra o k-mers e inserções na primeira linha
		fscanf(seq_file, "%d", &k);
		fscanf(seq_file, "%d", &insertions);
		// lê a sequência e adiciona na trie
		char seq[k + 1];
		start = clock();
		while (fscanf(seq_file, "%s", seq) == 1) {
			insertTrie4(trie4, seq, k);
		}
		finish = clock();
		trie4InsertionTime += (double)(finish - start) / (rep * CLOCKS_PER_SEC);
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
			searchTrie4(trie4, srch, k);
		}
		finish = clock();
		trie4SearchTime += (double)(finish - start) / (rep * CLOCKS_PER_SEC);
		fclose(srch_file);
		
		trie4Nodes = trie4->nodes;
		// registra o espaço ocupado pela árvore (desconsiderando o nó "raíz" da trie)
		trie4SpaceKB += (double)(trie4->nodes * sizeof(Trie4Node)) / (rep * 1024);
		
		destroyTrie4(trie4);
	}
	
	printf("%d,%.3f,%d,%.3f,%d,%.2f\n", insertions, trie4InsertionTime, searches, trie4SearchTime, trie4Nodes, trie4SpaceKB);
	
	return 0;
}

int parseBaseTrie4(char base) {
	switch (base) {
		case 'A': return 0;
		case 'C': return 1;
		case 'G': return 2;
		case 'T': return 3;
		default: return -1;
	}
}

Trie4 *createTrie4() {
	return calloc(1, sizeof(Trie4));
}

Trie4Node *createTrie4Node() {
	return calloc(1, sizeof(Trie4Node));
}

void insertTrie4(Trie4 *trie4, char *seq, int k) {
	Trie4Node **current = trie4->children;
	Trie4Node *last = NULL;
	for (int i = 0; i < k; i++) {
		int index = parseBaseTrie4(seq[i]);
		if (index < 0) return;
		// cria um novo caso não exista e aumenta o número de nós
		if (current[index] == NULL) {
			current[index] = createTrie4Node();
			trie4->nodes++;
		}
		last = current[index];
		current = current[index]->children;
	}
	last->end = true;
}

bool searchTrie4(Trie4 *trie4, char *seq, int k) {
	Trie4Node *current = NULL;
	for (int i = 0; i < k; i++) {
		int index = parseBaseTrie4(seq[i]);
		if (index < 0) return false; // base não existe
		if (i == 0) {
			current = trie4->children[index];
		} else {
			current = current->children[index];
		}
		if (current == NULL) return false; // não encontrou precocemente
	}
	// se o nó final tiver end = true, então encontrou
	return current->end;
}

void destroyTrie4Node(Trie4Node *node) {
    if (node == NULL) return;
    for (int i = 0; i < 4; i++) {
        destroyTrie4Node(node->children[i]);
    }
    free(node);
}

void destroyTrie4(Trie4 *trie4) {
    if (trie4 == NULL) return;
    for (int i = 0; i < 4; i++) {
        destroyTrie4Node(trie4->children[i]);
    }
    free(trie4);
}