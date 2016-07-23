/**
 * \file trie.c
 *
 * A simple trie for doing quick lookups on ASCII strings.
 *
 * \author eaburns
 * \date 25-08-2010
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "trie.h"

/*
 * Allocates a new string that contains a copy of the first 'len'
 * characters of 'str'.  Returns NULL on error.
 */
static char *dup_prefix(const char *str, unsigned int len)
{
    char *d;

    d = malloc((len + 1) * sizeof(*d));
    if (!d) {
	perror("malloc failed");
	return NULL;
    }

    strncpy(d, str, len);
    d[len] = '\0';

    return d;
}


void trie_init(struct trie *t)
{
    unsigned int i;

    for (i = 0; i < NELMS; i += 1)
	t->kids[i] = NULL;
    t->key = "";
    t->data = NULL;
}


void trie_free(struct trie *t)
{
    unsigned int i;

    if (!t)
	return;

    for (i = 0; i < NELMS; i += 1) {
	if (t->kids[i]) {
	    trie_free(t->kids[i]);
	    free((void *) t->kids[i]->key);
	    free(t->kids[i]);
	}
    }
}


/*
 * Actually perform the insert.
 *
 * Return 0 on success and 1 on failure.
 */
static int _trie_insert(struct trie *t, const char *key, void *data,
			unsigned int depth)
{
    unsigned int e;

    if (depth == strlen(key) + 1) {
	assert(strcmp(t->key, key) == 0);
	t->data = data;
	return 0;
    }

    e = key[depth];
    assert(e < NELMS);
    if (!t->kids[e]) {
	struct trie *k = malloc(sizeof(*k));
	if (!k) {
	    perror("malloc failed");
	    return 1;
	}
	trie_init(k);
	k->key = dup_prefix(key, depth);
	if (!k->key) {
	    free(k);
	    return 1;
	}
	t->kids[e] = k;
    }

    return _trie_insert(t->kids[e], key, data, depth + 1);
}


int trie_insert(struct trie *t, const char *key, void *data)
{
    return _trie_insert(t, key, data, 0);
}

/*
 * Actually look a key up in the trie.
 *
 * Return the data associated with the key on success or NULL on
 * failure.
 */
static void *_trie_lookup(struct trie *t, const char *key,
			  unsigned int depth)
{
    unsigned int e;

    if (depth == strlen(key) + 1) {
	assert(strcmp(t->key, key) == 0);
	return t->data;
    }

    e = key[depth];
    if (!t->kids[e])
	return NULL;

    return _trie_lookup(t->kids[e], key, depth + 1);
}

void *trie_lookup(struct trie *t, const char *key)
{
    return _trie_lookup(t, key, 0);
}


int trie_iter(struct trie *t, int (*f) (const char *, void *d, void *aux),
	      void *aux)
{
    unsigned int i;

    if (t->data)
	f(t->key, t->data, aux);

    for (i = 0; i < NELMS; i += 1) {
	if (t->kids[i]) {
	    int err = trie_iter(t->kids[i], f, aux);
	    if (err)
		return 1;
	}
    }

    return 0;
}
