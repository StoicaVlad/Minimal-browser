#include "utils.h"
#include "fct.h"
#include<stdio.h>

int main(int argc,char* argv[])
{
	//fisierele de input/output
	FILE *in=fopen(argv[1],"r");
	FILE *out=fopen(argv[2],"wt");

//cozile de istoric global,descarcare continua,descarcare finalizata
	AQ *ist_global=InitQ(sizeof(WebPg)),*c_d=InitQ(sizeof(Resource)),
	*d_f=InitQ(sizeof(Resource));

//listaTab este lista de tab-uri
	LT p,q,listaTab=(LT)malloc(sizeof(LT));
	listaTab=NULL;
//p e folosti pentru a retine tab-ul curent

	int index;
	unsigned int band = 1024;
	char cerinta[10000],*url,sep[]=" \n",*cuv;

//se adauga tab-ul gol in lista
	listaTab=AddTab(listaTab);
	url=(char*)malloc(sizeof(char)*50);
	//se parcurge lista si p ia vaoloarea tab-ului gol
		p=listaTab;
		for( ;p->next;p=p->next)
		{}
	//se primeste cerinta din fisierul de in
	while(fgets(cerinta, sizeof(cerinta), stdin))
	{

		cuv=strtok(cerinta,sep);
		while(cuv)
		{
			if(strcmp(cuv,"set_band")==0)
			{
				cuv=strtok(NULL,sep);
				band=atoi(cuv);
			}
			if(strcmp(cuv,"newtab")==0)
			{
				listaTab=AddTab(listaTab);
				p=listaTab;
				//cand se adauga un nou tab, p ia valoarea lui
				while(p->next)
					p=p->next;
			}
			if(strcmp(cuv,"deltab")==0)
			{
				//cand se elimina un tab,p ia valoarea penultimului tab
				q=listaTab;
				while(q->next)
					q=q->next;
				if(p==q)

				{
					p=listaTab;
					while(p->next->next)
					p=p->next;
				}
				listaTab=ElimTab(listaTab);
			}
			if(strcmp(cuv,"print_open_tabs")==0)
				ShowTabs(listaTab,out);
			if(strcmp(cuv,"goto")==0)
			{
				cuv=strtok(NULL,sep);
				url=cuv;
				//deschide o pagina web cu url-ul "url"
				p=NWBP(url,p,c_d,d_f,band);
				//adauga in istoricul global pagina web deschisa
				IntrQ(ist_global,p->info->current_page);
			}
			if(strcmp(cuv,"change_tab")==0)
			{
				cuv=strtok(NULL,sep);
				index=atoi(cuv);
				p=listaTab;
				int i=0;
				//se parcurge lista pana la tabul cu indexul "index"
				while(p!=NULL && i!=index)
				{	
					p=p->next; 
					i++;
				}
			//p retine valoarea tabului curent
			}
			if(strcmp(cuv,"back")==0)
				p=BackWP(p,out);
			if(strcmp(cuv,"forward")==0)
				p=FWP(p,out);
			if(strcmp(cuv,"history")==0)
				ist_global=History(ist_global,out);
			if(strcmp(cuv,"del_history")==0)
			{
				int nr_ent;
				cuv=strtok(NULL,sep);
				//nr_ent este numarul de pagini ce va fi sters
				nr_ent=atoi(cuv);
				Del_History(nr_ent,ist_global);
			}
			if(strcmp(cuv,"list_dl")==0)
				Afis_Resurse(p,out);
			if(strcmp(cuv,"download")==0)
			{
				cuv=strtok(NULL,sep);
				index=atoi(cuv);
			//se adauga in coada resursa de la index
				c_d=Download( c_d, p, index);
			}
			if(strcmp(cuv,"downloads")==0)
			{
				//se afiseaza coada de descarcari curente
				c_d=Afis_Downloads_Crt(c_d, out, band);
				//se afiseaza coada de descarcari complete
				d_f=Afis_Downloads_Comp(d_f,out);
			}
			if(strcmp(cuv,"wait")==0)
			{
				cuv=strtok(NULL,sep);
				//index e timpul de asteptare
				index=atoi(cuv);
				Wait(c_d,d_f,band,index);
			}
			cuv=strtok(NULL,sep);
		}
	}
	fclose(in);
	fclose(out);
	return 0;
}