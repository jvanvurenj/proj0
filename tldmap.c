/* AUTHOR: Jackson VanVuren
DUCKID: 951453829
CIS415 Project 0

Note: This is all my own work, except for the hash function, which is based off of Dan Bernstein's algorithm (example- http://www.cse.yorku.ca/~oz/hash.html).
Additionally, the general format is based off of our in class queue example by Joe Sventek.
*/



#include "tldmap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct tdata{
	long size;
	long used;
	TLDNode **thearr;

} TData;

struct tldnode {
	struct tldnode *next;
	char *key;
	long data;
};

/*
 * TLDMap_create generates a map structure for storing counts against
 * top level domains (TLDs)
 *
 * creates an empty TLDMap
 * returns a pointer to the list if successful, NULL if not
 */
const TLDMap *TLDMap_create(void){
	TLDMap *tmap = (TLDMap *)malloc(sizeof(TLDMap));

	if (tmap != NULL){
		TData *tldata = (TData *)malloc(sizeof(TData));

		if (tldata != NULL){
			TLDNode **arr = (TLDNode **)malloc(sizeof(TLDNode *));
			if (arr!=NULL){
				tldata->size = 1;
				tldata->used = 0;
				tldata->thearr = arr;
				tldata->thearr[0] = NULL;

				//*tmap = allfunctions;

				tmap->self = tldata;

			}
			else{
				free(tldata);
				free(tmap);
				tmap = NULL;
			}
		}
		else{
			free(tmap);
			tmap = NULL;
		}
	}
	return tmap;

}


/*
 * destroy() destroys the list structure in `tld'
 *
 * all heap allocated storage associated with the map is returned to the heap
 */
static void destroy(const TLDMap *tld){
	TData *tldata = (TData *)tld->self;
	long i;

	for(i=0; i<tldata->size; i++){
		
		TLDNode *x = tldata->thearr[i];
		TLDNode *temp = tldata->thearr[i];
		while(temp!=NULL){
			x = temp->next;
			free(temp->key);
			free(temp);
			temp = x;
		}
		tldata->thearr[i] = NULL;
	}
	free(tldata->thearr);
	free(tldata);
	free((void *)tld); //cast to void because const
}

/*
 * insert() inserts the key-value pair (theTLD, v) into the map;
 * returns 1 if the pair was added, returns 0 if there already exists en
 * entry corresponding to TLD
 */
static int insert(const TLDMap *tld, char *theTLD, long v){
	TData *tldata = (TData *)tld->self;
	long i;
	TLDNode *x, *temp;
	if(tldata->size = tldata->used){
		TLDNode **newarr = (TLDNode **)malloc(2*tldata->size*sizeof(TLDNode *));
		if(newarr!=NULL){
			for (i=0; i<tldata->size*2;i++){
				newarr[i] = NULL;
			}
			for(i=0; i<tldata->size; i++){
			
				for(temp = tldata->thearr[i]; temp!=NULL; temp = x){
					x = temp->next;
					/* Hash function based off of Dan Bernstein's algorithm (example- http://www.cse.yorku.ca/~oz/hash.html)*/
					long hashedkey = 0;
					char *tempar;
					for(tempar = temp->key; *tempar != '\0'; tempar++){
						hashedkey = (((33*hashedkey)+*tempar)%(tldata->size*2));
					}
					temp->next = newarr[hashedkey];
					newarr[hashedkey] = temp;
				}
			}
		}
		else{
			return 0;
		}
		free(tldata->thearr);
		tldata->thearr = newarr;
		tldata->size = tldata->size * 2;
	}
	int j = lookup(tld, theTLD, v);
	if(j){
		return 0;
	}
	else{
		TLDNode *new = (TLDNode *)malloc(sizeof(TLDNode));
		if (new!=NULL){
			char *dupkey = strdup(theTLD);
			if(dupkey!=NULL){
				new->key = dupkey;
				new->data = v;
				/* Hash function based off of Dan Bernstein's algorithm (example- http://www.cse.yorku.ca/~oz/hash.html)*/
				long hashedkey = 0;
				char *tempar;
				for(tempar = theTLD; *tempar != '\0'; tempar++){
					hashedkey = (((33*hashedkey)+*tempar)%(tldata->size));
				}


				new->next = tldata->thearr[hashedkey];
				tldata->thearr[hashedkey] = new;
				tldata->used++;
				return 1;

			}
			else{
				free(new);
				return 0;
			}
		}
		else{
			return 0;
		}
	}
}

/*
 * reassign() replaces the current value corresponding to theTLD in the
 * map with v.
 * returns 1 if the value was replaced, returns 0 if no such key exists
 */
static int reassign(const TLDMap *tld, char *theTLD, long v){
	TData *tldata = (TData *)tld->self;
	int j = lookup(tld, theTLD, v);
	if(!j){
		return 0;
	}
	/* Hash function based off of Dan Bernstein's algorithm (example- http://www.cse.yorku.ca/~oz/hash.html)*/
	long hashedkey = 0;
	char *tempar;
	for(tempar = theTLD; *tempar != '\0'; tempar++){
		hashedkey = (((33*hashedkey)+*tempar)%(tldata->size));
	}
	tldata->thearr[hashedkey]->data = v;
	return 1;
}

/*
 * lookup() returns the current value associated with theTLD in *v
 * returns 1 if the lookup was successful, returns 0 if no such key exists
 */
int lookup(const TLDMap *tld, char *theTLD, long *v){
	TData *tldata = (TData *)tld->self;
	/* Hash function based off of Dan Bernstein's algorithm (example- http://www.cse.yorku.ca/~oz/hash.html)*/
	long hashedkey = 0;
	char *tempar;
	for(tempar = theTLD; *tempar != '\0'; tempar++){
		hashedkey = (((33*hashedkey)+*tempar)%(tldata->size));
	}
	*v = hashedkey;
	if (tldata->thearr[hashedkey]->data!=NULL){
		return 1;
	}
	else{
		return 0;
	}
	
}
/*
 * itCreate() creates an iterator over the map
 * returns the iterator if successful, NULL if unsuccessful
 */

static const Iterator *itCreate(const TLDMap *tld){
	TData *tldata = (TData *)tld->self;
	const Iterator *it = NULL;
	void **tempar = (void **)malloc(sizeof(TLDNode*) * tldata->used);
	if (tempar != NULL){

		long i;
		TLDNode *x, *temp;
		for(i=0; i<tldata->size; i++){
				
			for(temp = tldata->thearr[i]; temp!=NULL; temp = x){
				x = temp->next;
				tempar[i] = temp;
			}
		}
		it = Iterator_create(tldata->used, tempar);
		if (it==NULL){
			free(tempar);
			return NULL;
		}
		return it;
	}
	else{
		return NULL;
	}
}

/*static TLDMap allfunctions = {
	NULL, destroy, insert, reassign, lookup, itCreate
};
/*
/*
 * TLDNode_tldname returns the tld associated with the TLDNode
 */
char *TLDNode_tldname(TLDNode *node){
	return node->key;
}

/*
 * TLDNode_count returns the value currently associated with that TLD
 */
long TLDNode_count(TLDNode *node){
	return node->data;
}



int main(int argc, char *argv[]) {
	TLDMap *map = TLDMap_create();
	destroy(map);
	return 0;
}
