//
//  main.c
//  Trie
//
//  Created by CX Lin on 2023/07/27.
//

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