//
//  main.c
//  Trie
//
//  Created by CX Lin on 2023/07/27.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "trie.h"

TRIE *CreateTrieNode(void) {
    return (TRIE *)calloc(1, sizeof(TRIE));
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

    // check string and tolower
    for(int i = 0; i < strSize; ++i) {
        if(isalpha(str[i])) {
            str[i] = tolower(str[i]);
        }
        else if(str[i] != '$') {
            return false;
        }
    }

    // navigate through the trie character by character based on the str's prefix
    TRIE *node = root;
    int index;
    for(int i = 0; i < strSize; ++i) {
        index = GetIndex(str[i]);
        if(!node->subtrees[index]) {
            node->subtrees[index] = CreateTrieNode();
        }
        node = node->subtrees[index];
    }

    // create the entry word if it is empty
    if(!node->entry) {
        node->entry = strdup(str);
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
    
    return node->entry;
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
