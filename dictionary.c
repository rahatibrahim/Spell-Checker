// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 11500;

// Hash table
node *table[N];

// Number of words
int nofw = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int c;
    
    for (node *tmp = table[hash(word)]; tmp != NULL; tmp = tmp->next)
    {
        c = strcasecmp(tmp->word, word);
        
        if (c == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Source: djb2 by Dan Bernstein
    
    unsigned long hash = 5381;

    int c = *word;
    c = tolower(c);
    
    while (*word != 0)
    {
        hash = ((hash << 5) + hash) + c;
        
        c = *word++;
        c = tolower(c);
    }
    
    return hash % N;
}

FILE *dict;
// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    char string[LENGTH + 1];
    
    // Declaring all the elments of array NULL
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }
    
    // Open dictionary
    dict = fopen(dictionary, "r");
    
    // Check if return value is NULL
    if (!dict)
    {
        return false;
    }
    else
    {
        // Scan the dictionary
        while (fscanf(dict, "%s", string) != EOF)
        {
            // Create a memory
            node *n = malloc(sizeof(node));
            if (n == NULL)
            {
                return false;
            }
            
            strcpy(n->word, string);
            
            // Adding node to the table
            n->next = table[hash(string)];
            table[hash(string)] = n;
            
            nofw++;
        }
    }
    
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return nofw;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        while (table[i] != NULL)
        {
            node *tmp = table[i]->next;
            free(table[i]);
            table[i] = tmp;
        }
        
        if (table[i] != NULL)
        {
            return false;
        }
    }
    
    return true;
}
