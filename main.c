//
//  main.c
//  Trie
//
//  Created by CX Lin on 2023/07/10.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define MAXSIZE     27  // 'a' - 'z' and EOW
#define EOW         '$' // end of word

// used in the following functions: TrieInsert, TrieSearch, TriePrefixList
#define GetIndex(x) ((x == EOW) ? (MAXSIZE - 1) : (x - 'a'))

/* TRIE structure deifinition */
typedef struct TrieNode {
    char *entry;
    struct TrieNode *subtrees[MAXSIZE];
} TRIE;

/* prototype declarations */

// allocate dynamic memory for a trie node and return its address
// return node pointer
// NULL if overflow
TRIE *CreateTrieNode(void);

// delete all data in trie and recycle memory
void DestroyTrie(TRIE *root);

// insert new entry into the trie
// return true success
// otherwise false
bool InsertTrie(TRIE *root, char *str);

// retrieve trie for the requested key
// return true key found
// otherwise false
bool SearchTrie(TRIE *root, char *str);

// make permuterms for given str and insert them into trie
// ex) "abc" -> "abc$", "bc$a", "c$ab", "$abc"
void InsertPermuterms(TRIE *root, char *str);

// format the output string
// ex) "k$boo" -> "book"
void PrintWord(const char *word);

// print all entries in trie using preorder traversal
void ListTrie(TRIE *root);

// print all entries starting with str (as prefix) in trie
// ex) "abb" -> "abbas", "abbasid", ...
// using TrieList function
void TriePrefixList(TRIE *root, char *str);

// wildcard search
// ex) "ab*", "*ab", "a*b", "*ab*"
// using triePrefixList function
void SearchWildcardTrie(TRIE *root, char *str);

// input validation
// return true is valid
// otherwise false
bool InputValidation(char *str);

int main(int argc, const char *argv[]) {
    TRIE *trie = NULL;
    TRIE *permute_trie = NULL;
    FILE *fp;
    char buff[100];
    
    // input validation
    if(argc != 2) {
        fprintf(stderr, "Error: Incorrect input.\n");
        fprintf(stderr, "Correct Usage: ./file [filename]\n");
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

TRIE *CreateTrieNode(void) {
    TRIE *trie = NULL;
    trie = (TRIE *)calloc(1, sizeof(TRIE));
    return trie;
}

void DestroyTrie(TRIE *root) {
    if(root == NULL) {
        return;
    }
    for(int i = 0; i < MAXSIZE; ++i) {
        if(root->subtrees[i]) {
            DestroyTrie(root->subtrees[i]);
        }
    }
    free(root->entry);
    root->entry = NULL;
    free(root);
    root = NULL;
}

bool InsertTrie(TRIE *root, char *str) {
    int strSize = (int) strlen(str);
    char buff[100] = {'\0'};
    int buffSize = 0;

    // check string and tolower
    for(int i = 0; i < strSize; ++i) {
        if(isalpha(str[i])) {
            str[i] = tolower(str[i]);
        }
        else if(str[i] != '$') {
            return false;
        }
    }

    // insert the word to trie
    TRIE *node = root;
    int index;
    for(int i = 0; i < strSize; ++i) {
        index = GetIndex(str[i]);
        if(!node->subtrees[index]) {
            node->subtrees[index] = CreateTrieNode();
        }
        node = node->subtrees[index];
        buff[buffSize++] = str[i];
    }

    // create this word if entry is empty
    if(!node->entry) {
        node->entry = strdup(buff);
    }

    return true;
}

void InsertPermuterms(TRIE *root, char *str) {
    int strSize = (int) strlen(str);
    char permutedStr[strSize + 2];
    permutedStr[strSize + 1] = '\0';
    
    // generate permuted str
    // ex) "abc" -> "abc$", "bc$a", "c$ab", "$abc"
    for(int i = 0; i <= strSize; ++i) {
        strcpy(permutedStr, str + i);
        permutedStr[strSize - i] = '$';
        strncpy(permutedStr + strSize - i + 1, str, i);
        
        // insert permuted str to trie
        InsertTrie(root, permutedStr);
    }
}

bool SearchTrie(TRIE *root, char *str) {
    int strSize = (int) strlen(str);
    
    TRIE *node = root;
    if(root == NULL || str == NULL) {
        return false;
    }
    
    // verify the prefix of str
    for(int i = 0; i < strSize; ++i) {
        int index = GetIndex(str[i]);
        if(!node->subtrees[index]) {
            return false;
        }
        node = node->subtrees[index];
    }
    
    return (node->entry ? true : false);
}

void PrintWord(const char *word) {
    // find the position of '$' in the string
    const char* dollarSign = strchr(word, '$');

    // if '$' is found in the string
    if (dollarSign != NULL) {
        // print the part after the '$'
        fprintf(stdout, "%s", dollarSign + 1);
        
        // print the part before the '$'
        fwrite(word, sizeof(char), dollarSign - word, stdout);
        
        // print a newline for good measure
        fprintf(stdout, "\n");
    }
    else {
        // if '$' is not found in the string
        fprintf(stdout, "%s\n", word);
    }
}

void ListTrie(TRIE *root) {
    if(!root) {
        return;
    }
    // preorder traversal
    if(root->entry) {
        if(strchr(root->entry, '$')) {
            PrintWord(root->entry);
        }
    }
    for(int i = 0; i < MAXSIZE; ++i) {
        if(root->subtrees[i]) {
            ListTrie(root->subtrees[i]);
        }
    }
}

void TriePrefixList(TRIE *root, char *str) {
    int strSize = (int) strlen(str);
    TRIE *node = root;
    
    // go to the corresponding leaf
    if(root == NULL || str == NULL) {
        return;
    }
    for(int i = 0; i < strSize; ++i) {
        int index = GetIndex(str[i]);
        if(!node->subtrees[index]) {
            return;
        }
        node = node->subtrees[index];
    }

    ListTrie(node);
}

void SearchWildcardTrie(TRIE *root, char *str) {
    int strSize = (int) strlen(str);
    const char *ptr = strchr(str, '*');
    
    // create search format
    // ex) "ab*c" -> "c$ab"
    char newStr[strSize + 1];
    int beforeLength = (int) (ptr - str);
    int afterLength = (int) (strlen(str)) - beforeLength - 1;
    if (afterLength > 0) {
        strncpy(newStr, ptr + 1, afterLength);
    }
    newStr[afterLength] = '$';
    if (beforeLength > 0) {
        strncpy(newStr + afterLength + 1, str, beforeLength);
    }
    newStr[strSize] = '\0';

    TriePrefixList(root, newStr);
}

bool InputValidation(char *str) {
    int asteriskCount = 0;
    for (int i = 0; str[i]; i++) {
        // return false if not alpha nor '*'
        if (!isalpha(str[i]) && str[i] != '*') {
            return false;
        }
        // count '*'
        if (str[i] == '*') {
            asteriskCount++;
        }
        // return false if '*' appears multiple times
        if (asteriskCount > 1) {
            return false;
        }
        // tolower string
        str[i] = tolower((unsigned char) str[i]);
    }
    return true;
}
