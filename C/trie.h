/**
 * \file trie.h
 *
 * A trie for quick lookups indexed on strings.
 *
 * \author eaburns
 * \date 25-08-2010
 */

#if !defined(_TRIE_H_)
#define _TRIE_H_

#define NELMS 256

struct trie {
    void *data;
    const char *key;
    struct trie *kids[NELMS];
};

/* Initialize a new trie. */
void trie_init(struct trie *t);

/* Free the memory associated with the given trie. */
void trie_free(struct trie *t);

/* Insert a binding of key=data into the trie. */
int trie_insert(struct trie *t, const char *key, void *data);

/* Look for the data associated with the given key. */
void *trie_lookup(struct trie *t, const char *key);

/* Iterater the function 'f' over each element of the trie. */
int trie_iter(struct trie *t, int (*f) (const char *, void *d, void *aux),
	      void *aux);

#endif				/* !_TRIE_H_ */
