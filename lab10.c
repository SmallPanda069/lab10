#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE_OF_ALPHABET 26

// Trie node structure
struct TrieNode {
    struct TrieNode *kid[SIZE_OF_ALPHABET];
    int endingWord;
    int cout;
};

// Initializes a trie node
struct TrieNode* createTrieNode() {
    struct TrieNode *node = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    if (node) {
        node->endingWord = 0;
        node->cout = 0;
        for (int i = 0; i < SIZE_OF_ALPHABET; i++) {
            node->kid[i] = NULL;
        }
    }
    return node;
}

// Inserts a word into the trie
void insert(struct TrieNode *root, char *word) {
    struct TrieNode *current = root;
    int length = strlen(word);
    for (int i = 0; i < length; i++) {
        int index = word[i] - 'a';
        if (!current->kid[index]) {
            current->kid[index] = createTrieNode();
        }
        current = current->kid[index];
    }
    current->endingWord = 1;
    current->cout++;
}

// Counts the occurrences of a word in the trie
int numberOfOccurrences(struct TrieNode *root, char *word) {
    struct TrieNode *current = root;
    int length = strlen(word);
    for (int i = 0; i < length; i++) {
        int index = word[i] - 'a';
        if (!current->kid[index]) {
            return 0; // Word not found
        }
        current = current->kid[index];
    }
    if (current != NULL && current->endingWord) {
        return current->cout;
    } else {
        return 0;
    }
}

// Deallocates memory allocated for the trie
void deallocateTrie(struct TrieNode *root) {
    if (!root) return;
    for (int i = 0; i < SIZE_OF_ALPHABET; i++) {
        if (root->kid[i]) {
            deallocateTrie(root->kid[i]);
        }
    }
    free(root);
}

// Reads dictionary from file and stores words in array
int readDictionary(char *filename, char **pInWords) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int cout = 0;
    char word[100]; // Assuming max word length is 100 characters
    while (fscanf(file, "%s", word) != EOF) {
        pInWords[cout] = strdup(word);
        cout++;
    }

    fclose(file);
    return cout;
}

int main(void) {
    char *inWords[256];

    // Read the number of words in the dictionary
    int numberofWord = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numberofWord; ++i) {
        printf("%s\n", inWords[i]);
    }

    struct TrieNode *root = createTrieNode();
    for (int i = 0; i < numberofWord; i++) {
        insert(root, inWords[i]);
    }

    // Test word occurrences
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(root, pWords[i]));
    }

    deallocateTrie(root);

    return 0;
}