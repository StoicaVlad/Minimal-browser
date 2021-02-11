/*Stoica Vlad Marian 311CB*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#ifndef _DOWNLOAD_RESOURCE_
#define _DOWNLOAD_RESOURCE_

typedef struct{
    char name[100];
    unsigned long dimension;
    unsigned long currently_downloaded;
} Resource;

//structura de tip Pagina Web
typedef struct{
	char url[100];
	int num_res;
	Resource* resources;
} WebPg;

//structura pentru o celula din stiva
typedef struct celst{
	struct celst *urm;
	void* info;
} TCelSt, *ACelSt;

//structura pentru stiva generica
typedef struct stiva{
	size_t dime;
	ACelSt vf;
} TStiva, *ASt;

//structura pentru o celula din coada
typedef struct cel{
	struct cel *urm;
	void* info;
} TCel, *ACel;

//structura pentru coada generica
typedef struct coada{
	size_t dime;
	ACel ic, sc;
} TCoada, *AQ;

//structura pentru un tab
typedef struct TTabs{
	WebPg* current_page;
	ASt* back_stack;
	ASt* forward_stack;
	struct TTabs* next;
} *Tabs;

//structura pentru o lista  de tab-uri
typedef struct LTabs{
	Tabs info;
	struct LTabs *next;
} *LT;

Resource* get_page_resources(const char *URL_Name, int *n);

#endif /* _DOWNLOAD_RESOURCE_ */
