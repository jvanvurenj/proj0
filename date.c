/* AUTHOR: Jackson VanVuren
CIS415 Project 0

Note: This is all my own work, except the general format is based off of our in class queue example by Joe Sventek.
*/



#include "date.h"
#include <stdlib.h>
#include <stdio.h>


typedef struct d_data {
	int month;
	int day;
	int year;
} D_Data;
/*
 * duplicate() creates a duplicate of `d'
 * returns pointer to new Date structure if successful,
 *         NULL if not (memory allocation failure)
 */
static const Date *duplicate(const Date *d) {
	D_Data *d1 = (D_Data *)d->self;
	Date *newd = (Date *)malloc(sizeof(Date));
	if (newd != NULL){
		*newd = *d;
		D_Data *dd = (D_Data *)malloc(sizeof(D_Data));
		if (dd != NULL){
			*dd = *d1;
			newd->self = dd;

		}
		else{
			free(newd);
			newd = NULL;
		}
	}
	return newd;
}

/*
 * compare() compares two dates, returning <0, 0, >0 if
 * date1<date2, date1==date2, date1>date2, respectively
 */
static int compare(const Date *date1, const Date *date2) {
	D_Data *d1 = (D_Data *)date1->self;
	D_Data *d2 = (D_Data *)date2->self;
	int d1year=d1->year;
	int d2year = d2->year;
	if(d1year < d2year){
		return -1;
	}
	if (d1->year > d2->year){
		return 1;
	}
	if (d1->month < d2->month){
		return -1;;
	}
	if (d1->month > d2->month){
		return 1;
	}
	if (d1->day < d2->day){
		return -1;
	}
	if (d1->day > d1->day){
		return 1;
	}
	return 0;

}

/*
 * destroy() returns any storage associated with `d' to the system
 */
static void destroy(const Date *d) {
	D_Data *d1 = (D_Data *)d->self;
	free(d->self);
	free((void *)d);
}

/*
 * Date_create() creates a Date structure from `datestr`
 * `datestr' is expected to be of the form "dd/mm/yyyy"
 * returns pointer to Date structure if successful,
 *         NULL if not (syntax error)
 */
const Date *Date_create(char *datestr) {
	int i;
	char ary[3];
	char ar1[3];
	char ar2[5];
	int count;
	Date *d = (Date *)malloc(sizeof(Date));
	if (d != NULL){
		D_Data *dd = (D_Data *)malloc(sizeof(D_Data));
		if (dd != NULL){
			for(i = 0; i<10; i++){
				if( i<2){
					ary[i] = datestr[i];
				}
				else if(i<5){
					if(datestr[i]=='/'){
						dd->day = atoi(ary);

						count = 0;
					}
					else{
						ar1[count] = datestr[i];
						count++;
					}
				}
				else{
					if(datestr[i]=='/'){
						dd->month = atoi(ar1);
						count = 0;
					}
					else{
						ar2[count] = datestr[i];
						count++;
					}
				}
			}
			dd->year = atoi(ar2);
			d->self = dd;
		}
		else{
			free(d);
			d = NULL;
		}
	}
	return d;
}



