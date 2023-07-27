//
//  main.c
//  Trie
//
//  Created by CX Lin on 2023/07/10.
//

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "trie.h"

int main(int argc, const char *argv[]) {
    TRIE *trie = NULL;
    TRIE *permute_trie = NULL;
    FILE *fp;
    char buff[100];

    // input validation
    if(argc != 2) {
        fprintf(stderr, "Error: Incorrect input.\n");
        fprintf(stderr, "Correct Usage: ./file [filename]\n");
        return 1;
    }
    // open the words file
    fp = fopen(argv[1], "rt");
    if(!fp) {
        fprintf(stderr, "File open error: %s\n", argv[1]);
        return 1;
    }
    // create structs
    trie = CreateTrieNode();        // original trie
    permute_trie = CreateTrieNode();// trie for permuterm index
    // insert the word into trie
    fprintf(stdout, "Loading words to trie ...\t");
    while(fscanf(fp, "%s", buff) == 1) {
        bool isInsert = InsertTrie(trie, buff);     // insert original word
        if(isInsert) {
            InsertPermuterms(permute_trie, buff);   // insert permuted words
        }
    }
    fprintf(stdout, "Done\n");
    // close the file stream
    fclose(fp);

    // fetch the user's input
    fprintf(stdout, "Press ctrl + D to quit\n");
    fprintf(stdout, "Query\n> ");
    while(fscanf(stdin, "%s", buff) == 1) {
        // input validation
        if(!InputValidation(buff)) {
            fprintf(stdout, "Invalid input. Please enter only alphabets.\n> ");
            continue;
        }
        // query
        if(strchr(buff, '*')) {
            SearchWildcardTrie(permute_trie, buff);
        }
        else {
            bool isFound = SearchTrie(trie, buff);
            fprintf(stdout, "[%s] was%s found!\n", buff, (isFound ? "" : " not"));
        }
        fprintf(stdout, "\nQuery\n> ");
    }
    fprintf(stdout, "\nFreeing memory ...\t");

    // free the memory
    DestroyTrie(trie);
    DestroyTrie(permute_trie);
    fprintf(stdout, "Done\n");
    fprintf(stdout, "Thanks for using trie\n");

    return 0;
}