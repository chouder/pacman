
#include <stdio.h>
#include <stdlib.h>
#include "liste_point.h"

#define COL 24
#define LIG 20


liste_point pathfinding(int map[COL][LIG],int dx, int dy, int fx, int fy)
{
	int i,j;

	liste_point	LF;
	point pNode;
	LF = l_vide();

	int dist[COL][LIG];
	int d;

	for (i=0; i<COL; i++)
		for (j=0; j<LIG; j++)
        {
			dist[i][j] = 40000;
		}

	dist[dx][dy] = 0;

	for (d=0;d<COL*LIG;d++)
	{
		for (i=0; i<COL; i++)
		{
			for (j=0; j<LIG; j++)
			{
				if(dist[i][j] == d)
                {
                    if (i>0 && map[i-1][j] == 0 && dist[i-1][j]>d+1)
                    {
                        dist[i-1][j] = d+1;
                    }
                    if (j>0 && map[i][j-1]== 0 && dist[i][j-1]>d+1)
                    {
                        dist[i][j-1] = d+1;
                    }
                    if (i<LIG-1 && map[i+1][j]== 0 && dist[i+1][j]>d+1)
                    {
                        dist[i+1][j] = d+1;
                    }
                    if (j<COL -1 && map[i][j+1]== 0 && dist[i][j+1]>d+1)
                    {
                        dist[i][j+1] = d+1;
                    }
                }
            }
		}
	} // end for

	d = dist[fx][fy];

	if (d>=1000) {
		return 0;
	}

    pNode = remplisPoint(fx,fy); // met l'arrivé dans la liste
    LF = cons(pNode,LF);

	for (;d>0;d--) {
		if (fy>0 && dist[fx][fy-1] == d-1){
			fy--;
			pNode = remplisPoint(fx,fy);
			LF = cons(pNode,LF);
			continue;
        }
		if (fx>0 && dist[fx-1][fy] == d-1){
			fx--;
			pNode = remplisPoint(fx,fy);
			LF = cons(pNode,LF);
			continue; }
		if (fy<COL+1 && dist[fx][fy+1] == d-1) {
			fy++;
			pNode = remplisPoint(fx,fy);
			LF = cons(pNode,LF);
			continue; }
		if (fx<LIG+1  && dist[fx+1][fy] == d-1) {
			fx++;
			pNode = remplisPoint(fx,fy);
			LF = cons(pNode,LF);
			continue; }
	}

	LF = reste(LF); // supprime le premier element, car on deja dessus
	return LF;
}

int main()
{
    int tab[COL][LIG];
    liste_point L;
    L = l_vide();

    int i,j;
    for(i=0;i<COL;i++)
    {
        for(j=0;j<LIG;j++)
        {
            tab[i][j] = 0;
        }
    }

    int u;

    for(u=0;u<5;u++)
    {
        tab[u][3] = 1;
    }

    L = pathfinding(tab,1,1,1,10);
    afficher_point_liste(L);

    while(!est_vide(L))
    {
        tab[prem(L).x][prem(L).y] = 2;
        L = reste(L);
    }

    for(i=0;i<LIG;i++)
    {
        for(j=0;j<COL;j++)
        {
            printf(",%d",tab[j][i]);
        }
        printf("\n");
    }
    return 0;
}
