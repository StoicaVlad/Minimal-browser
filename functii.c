/*Stoica Vlad Marian-311CB*/
#include "utils.h"
#include "fct.h"
#include<stdio.h>

//initializeaza o stiva generica
void* InitS(size_t d)
{
	ASt s;
	s=(ASt)malloc(sizeof(TStiva));
	if(!s) return NULL;
	s->dime=d;
	s->vf=NULL;
	return (void*)s;
}

//elimina un element din stiva generica
int Pop(void* s,void* ae)
{
	if( ((TStiva*)s)->vf==NULL ) return 0;
	memcpy(ae, ((TStiva*)s)->vf->info, ((TStiva*)s)->dime);
	ACelSt aux=((TStiva*)s)->vf;
	((TStiva*)s)->vf=aux->urm;
	free(aux->info);
	free(aux);
	return 1;
}

//adauga un element in stiva generica
int Push(void* a,void* ae)
{
	ACelSt aux=(ACelSt)malloc(sizeof(TCelSt));
	if(!aux) return 0;
	aux->info=(void*)malloc(((TStiva*)a)->dime);
	memcpy(aux->info,ae,((TStiva*)a)->dime);
	aux->urm=((TStiva*)a)->vf;
	((TStiva*)a)->vf=aux;
	return 1;
}

//introduce un element intr-o coada generica
int IntrQ(void* a,void* ae)
{
	ACel aux=(ACel)malloc(sizeof(TCel));
	if(!aux) return 0;
	aux->info=(void*)malloc(((TCoada*)a)->dime);
	if(!aux->info) return 0;
	memcpy(aux->info,ae,((TCoada*)a)->dime);
	aux->urm=NULL;
	//primul element introdus in coada
	if( ((TCoada*)a)->ic == NULL && ((TCoada*)a)->sc==NULL)
	{
		((TCoada*)a)->ic=aux;
		((TCoada*)a)->sc=aux;
	}
	//daca e o coada nevida
	else
	{
		((TCoada*)a)->sc->urm=aux;
		((TCoada*)a)->sc=aux;
	}
	return 1;
}

//initializeaza o coada generica
void* InitQ(size_t d)
{
	AQ c;
	c=(AQ)malloc(sizeof(TCoada)*10);
	if(!c) return NULL;
	c->dime=d;
	c->ic=NULL;
	c->sc=NULL;
	return (void*)c;
}

//extrage un element dintr-o coada generica
int ExtrQ(void* a,void* ae)
{
	ACel aux=((TCoada*)a)->ic;
	((TCoada*)a)->ic=aux->urm;
	memcpy(ae,aux->info,((TCoada*)a)->dime);
	free(aux->info);
	free(aux);
	return 1;
}

//reseteaza/reinitializeaza o coada generica
void ResetQ(void* a)
{
	((TCoada*)a)->ic=NULL;
	((TCoada*)a)->sc=NULL;
}

//distruge o coada generica
void DistrQ(void* a)
{
	ResetQ(a);
	free(a);
}

//aloca memorie pentru un tab
LT AlocLT()
{
	LT aux=(LT)malloc(sizeof(LT));
	if(!aux) return 0; //verifica alocarea
	aux->info=(Tabs)malloc(sizeof(Tabs));
	if(!aux->info) return 0; //verifica alocarea
	//aloca o pagina web si o initializeaza ca null
	aux->info->current_page=(WebPg*)malloc(sizeof(WebPg));
	aux->info->current_page=NULL;
	//initializeaza cele 2 stive specifice
	aux->info->back_stack=InitS(sizeof(WebPg));
	aux->info->forward_stack=InitS(sizeof(WebPg));
	aux->next=NULL;
	return aux;
}

//adauga un nou tab in lista
LT AddTab(LT lista)
{
	LT p,TabNou=AlocLT();//se aloca noul tab
	if(!TabNou) return 0;
	if(!lista)//verifica daca lista e vida
	{
		lista=TabNou;
	}
	else
	//adauga tab-ul pe ultimul loc din lista
	{
		p=lista;
		while(p->next)
			p=p->next;
		p->next=TabNou;
	}
	return lista;
}

//afiseaza tab-urile deschise
void ShowTabs(LT lista,FILE *of)
{
	int nrt=0;
	LT p=lista;
	//se parcurge lista de tab-uri
	for( ;p; p=p->next)
	{
	//se verifica daca in tab e o pagina web deschisa
		if(!p->info->current_page)
		{
			fprintf(of, "(%d: empty)\n",nrt);
			printf( "(%d: empty)\n",nrt);
		}
		else
		{
			fprintf(of, "(%d: %s)\n",nrt,p->info->current_page->url);
			printf("(%d: %s)\n",nrt,p->info->current_page->url);
		}
		nrt++; //nrt e index-ul tab-ului
	}
}

//elimina ultimul tab din lista
LT ElimTab(LT lista)
{
	LT p=lista;
	//verifica daca lista e vida
	if(!p) return 0;
	//verifica daca e un singur tab in lista
	if(!p->next)
	{
		lista=NULL;
		return lista;
	}
	//parcurge lista pana la penultimul tab si il elimina pe ultimul
	while(p->next->next!=NULL)
	{
		p=p->next;
	}
	free(p->next);
	p->next=NULL;
	return lista;
}

//deschide o noua pagina web (goto)
LT NWBP(char *url,LT Tab,AQ *c_d, AQ *d_f, int band)
{
	char* URL=url;
	int i;

	WebPg *PG=(WebPg*)malloc(sizeof(WebPg));

	PG->num_res=0;
	PG->resources=NULL;
	strcpy(PG->url,URL);
	//adauga resursele pentru pagina si numarul acestora
	PG->resources=get_page_resources(PG->url,&PG->num_res);

	/*pentru fiecare resursa,currently_downloaded va reprezenta dimensiunea
	de memorie ramasa de descarcat,nu cat a fost descarcat*/
	for(i=0;i<PG->num_res;i++)
		(PG->resources+i)->currently_downloaded=(PG->resources+i)->dimension;

	//functia de wait daca exista o pagina deja in coada de descarcari
	if( ((TCoada*)c_d)->ic )
	Wait(c_d,d_f,band,1);

	//verifica daca exista o alta pagina web deja deschisa
	if(!Tab->info->current_page)
		Tab->info->current_page=PG;
	else
	//daca exista,o impinge pe cea veche in stiva de back
	{
		Push(Tab->info->back_stack,Tab->info->current_page);
		Tab->info->current_page=PG;
	}
	//sitva de forward ia NULL la o noua pagina deschisa
	((TStiva*)Tab->info->forward_stack)->vf=NULL;
	return Tab;
}

LT BackWP(LT Tab,FILE* of)
{
	//verifica daca nu mai sunt pagini in stiva de back
	if( !((TStiva*)Tab->info->back_stack)->vf )
	{
		fprintf(of,"can't go back,no pages in stack\n");
		return Tab;
	}
	//impinge in stiva de forward pagina curenta
	Push(Tab->info->forward_stack,Tab->info->current_page);

	//elimina din stiva de back prima pagina si o adauga drept pagina curenta
	if(((TStiva*)Tab->info->back_stack)->vf->urm)
	Pop(Tab->info->back_stack,Tab->info->current_page);
	else
	{
		Tab->info->current_page=(((TStiva*)Tab->info->back_stack)->vf->info);
		((TStiva*)Tab->info->back_stack)->vf=NULL;
	}
	return Tab;
}

LT FWP(LT Tab,FILE* of)
{
	//verifica daca nu mai sunt pagini in stiva de forward
	if( !((TStiva*)Tab->info->forward_stack)->vf )
	{
		fprintf(of,"can't go forward,no pages in stack\n");
		return Tab;
	}
	//impinge in stiva de back pagina curenta
	Push(Tab->info->back_stack,Tab->info->current_page);
//elimina din stiva de forward prima pagina si o adauga drept pagina curenta
	if(((TStiva*)Tab->info->forward_stack)->vf->urm)
	Pop(Tab->info->forward_stack,Tab->info->current_page);
	else
	{
		Tab->info->current_page=(((TStiva*)Tab->info->forward_stack)->vf->info);
		((TStiva*)Tab->info->forward_stack)->vf=NULL;
	}
	return Tab;
}

//afiseaza istoricul global
AQ* History(AQ* coada,FILE* of)
{
	/*creeaza o noua coada iar,pe masura ce se elimina elementele
	din prima coada,se adauga in coada noua x,care va fi intoarsa*/
	AQ *p=InitQ(sizeof(WebPg)),*x=InitQ(sizeof(WebPg)),*q=InitQ(sizeof(WebPg));
	p=coada;
	while( ((TCoada*)p)->ic )
	{
		fprintf(of,"%s\n",((WebPg*)((TCoada*)p)->ic->info)->url);
		ExtrQ(p,q);
		IntrQ(x,q);
	}
	return x;
}

//elimina pagini din istoricul global
void Del_History(int nr_ent,AQ* coada)
{
	AQ* p=InitQ(sizeof(WebPg));
	int i;
	//daca nr_ent este 0,atunci se resteaza coada
	if(nr_ent==0)
		ResetQ(coada);
	else
	{
		//daca e coada istoricului nula,se termina functia
		if(!((TCoada*)coada)->ic)
			return;
		else
		/*se elimina din coada nr_ent elemente*/
		for(i=1;i<=nr_ent;i++)
			{
				//verifica daca e un singur element in coada
				if(!((TCoada*)coada)->ic->urm)
				{
					ResetQ(coada);
					break;
				}
				else
				ExtrQ(coada,p);
			}
	}
	free(p);
}

//afiseaza resursele disponibile pe pagina
void Afis_Resurse(LT Tab,FILE* of)
{
	int i;
	WebPg* PG=Tab->info->current_page;
	if(!PG) return;
	AQ* coada_resurse=InitQ(sizeof(Resource)),c_aux=InitQ(sizeof(Resource)),
	q=InitQ(sizeof(Resource));
	Resource *res;
	res=Tab->info->current_page->resources;
	//daca resursele nu sunt alocate sau pagina nu are resurse
	if(strlen(res->name)<1)
		return;
	//introduce prima resursa in coada_resurse
	IntrQ(coada_resurse,res);
	for(i=1;i<PG->num_res;i++)
	{
		//parcurge toate resursele din pagina si le introduce in coada
		IntrQ(coada_resurse,(res+i));
	}
	i=0;
	//parcurge coada de resurse si afiseaza resursele
	while( ((TCoada*)coada_resurse)->ic )
	{
		fprintf(of,"[%d - \"%s\" : %ld]\n",i,
			((Resource*)((TCoada*)coada_resurse)->ic->info)->name,
			((Resource*)((TCoada*)coada_resurse)->ic->info)->dimension);
		printf("[%d - \"%s\" : %ld]\n",i,
			((Resource*)((TCoada*)coada_resurse)->ic->info)->name,
			((Resource*)((TCoada*)coada_resurse)->ic->info)->dimension);
		ExtrQ(coada_resurse,q);
		IntrQ(c_aux,q);
		i++;
	}
}

AQ* Afis_Downloads_Crt(AQ* c_d,FILE* of,int band)
{
	//parcurge coada de download curent si afiseaza continutul sau
	AQ *p=InitQ(sizeof(Resource)),*q=InitQ(sizeof(Resource));
	while( ((TCoada*)c_d)->ic )
	{
		printf("[\"%s\" : %ld/%ld]\n",((Resource*)((TCoada*)c_d)->ic->info)->name,
			((Resource*)((TCoada*)c_d)->ic->info)->currently_downloaded,
			((Resource*)((TCoada*)c_d)->ic->info)->dimension);
		fprintf(of,"[\"%s\" : %ld/%ld]\n",((Resource*)((TCoada*)c_d)->ic->info)->name,
			((Resource*)((TCoada*)c_d)->ic->info)->currently_downloaded,
			((Resource*)((TCoada*)c_d)->ic->info)->dimension);
		ExtrQ(c_d,p);
		IntrQ(q,p);
	}
	//intoarce coada q care va fi exact coada c_d la inceput
	return q;
}

AQ* Afis_Downloads_Comp(AQ* d_f,FILE* of)
{
	//parcurge si afiseaza coada de download-uri complete
	AQ *p=InitQ(sizeof(Resource)),*q=InitQ(sizeof(Resource));
	while( ((TCoada*)d_f)->ic )
	{
		printf("[\"%s\" : completed]\n",((Resource*)((TCoada*)d_f)->ic->info)->name);
		fprintf(of,"[\"%s\" : completed]\n",((Resource*)((TCoada*)d_f)->ic->info)->name);
		ExtrQ(d_f,p);
		IntrQ(q,p);
	}
	//intoarce coada q care va fi exact coada d_f la inceput
	return q;
}

//descarca resursursa din pagina aflata la index-ul "index"
AQ* Download(AQ* coada,LT Tab,int index)
{
	AQ* aux=InitQ(sizeof(Resource)),p=InitQ(sizeof(Resource));
	int cond=0;
	//verifica daca tab-ul curent are pagina deschisa
	if(!Tab->info->current_page)
		return coada;
	Resource* res=Tab->info->current_page->resources;
	//verifica daca, coada de descarcari e vida
	if(!((TCoada*)coada)->ic)
			IntrQ(coada,(res+index));
	else
	{
		//adauga la inceputul cozii aux resursa de la index
		if((res+index)->currently_downloaded <= 
			((Resource*)((TCoada*)coada)->ic->info)->currently_downloaded )
		{
			IntrQ(aux,(res+index));
			//parcurge coada si o adauga in aux
			while( ((TCoada*)coada)->ic )
			{
				ExtrQ(coada,p);
				IntrQ(aux,p);

			}
			/*noua coada cu resurse de dimensiune de descarcat crescatoare
			va fi aux*/
			return aux;
		}
		else
			{
				/*parcurge coada de descarcat pana gaseste doua valori
				intre care sa pozitioneze resursa aux*/
				while(((TCoada*)coada)->ic)
				{
					if((res+index)->currently_downloaded <= 
						((Resource*)((TCoada*)coada)->ic->info)->
						currently_downloaded && cond==0)
						{
							IntrQ(aux,(res+index));
							cond=1;
						}
					ExtrQ(coada,p);
					IntrQ(aux,p);
				}
				//daca nu a gasit,pozitioneaza resursa de la index la sfarsit
				if(cond==0)
					IntrQ(aux,(res+index));
				return aux;
			}
	}
	return coada;
}

/*functia de asteptare a timpului*/
void Wait(AQ* c_d,AQ* d_f,int bandwith,int time)
{
	AQ* aux=InitQ(sizeof(Resource));
	int loc=time*bandwith;

//cat timp exista resurse
	while(loc>0)
	{
/*daca resursa e mai mica,o scoarte din coada de descarcat si o trece in
coada de finalizat apoi descarca din urmatoarea datele ramase "pe cablu"*/
		if( ((Resource*)((TCoada*)c_d)->ic->info)->currently_downloaded < loc )
		{
			loc=loc-((Resource*)((TCoada*)c_d)->ic->info)->currently_downloaded;
			if( ((TCoada*)c_d)->ic == ((TCoada*)c_d)->sc)
			{
				ExtrQ(c_d,aux);
				IntrQ(d_f,aux);
				break;
			}
			else
				ExtrQ(c_d,aux);
			IntrQ(d_f,aux);
		}
		else
		/*altfel,scade din prima resursa loc*/
		{
			((Resource*)((TCoada*)c_d)->ic->info)->currently_downloaded =
			((Resource*)((TCoada*)c_d)->ic->info)->currently_downloaded - loc;
			loc = 0;
		}
	}
}