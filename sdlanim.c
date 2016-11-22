#include "SDL.h"
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//musique#include <SDL/SDL_mixer.h>

#include "liste_point.h"

#define SCREEN_WIDTH  768
#define SCREEN_HEIGHT 640
#define SPRITE_WIDTH 31
#define SPRITE_HEIGHT 31
#define SPRITE_SIZE 31
#define NY 20
#define NX 24
#define G1_WIDTH 32
#define G1_HEIGHT 34
#define COL 24
#define LIG 20
#define TIME_BTW_ANIMATIONS 40
#define TIME_BTW_MOVEMENTS 10
#define taille 32

int gameover;
int moveRight, moveLeft, moveUp, moveDown; //gere le déplacement du pacman
int move;
int currentTime, previousTime; //gerer le temps entre les deplacements
int currentTimeAnim, previousTimeAnim; //gerer le temps entre les animations
float x,y;

/* source and destination rectangles */
SDL_Rect rcSrc,rcWall,rcWall2, rcBloc, rcSprite,rcG1, rcSG1, rcG2,rcG3, rcCandy;
int i,j;

//musiqueMix_Music *music,*start;
//musiqueint Mix_OpenAudio(int frequency, Uint16 format, int channels, int chunksize);	

/*int a;
int Convertir(float nb)
{
	nb += 0.5;
	int a = (int) nb;
	return a;
}
*/



void deplacement(SDL_Rect *fant, int x, int y){
	fant->x += x;
	fant->y += y;
}

void deplacementFantome (liste_point L, SDL_Rect *fant) {

	if (!est_vide(L)){
		if (prem(L).x == (fant->x / taille) + 1) { // droite
			deplacement(fant,TIME_BTW_MOVEMENTS,0);
			if ((fant->x/32) == prem(L).x) {
				//m = fant.x/32;
				L = reste(L);
			}
			/*if ((fant.y/32) == prem(L).y) {
				//n = fant.y/32;
				L = reste(L);
			}*/
		}
		if (prem(L).x == (fant->x / taille) - 1) { // gauche
			deplacement(fant,-TIME_BTW_MOVEMENTS,0);
			if ((fant->x/32) == prem(L).x) {
				L = reste(L);
			}
			
		}
		if (prem(L).y == (fant->y / taille) + 1) {	//bas
			deplacement(fant,0,TIME_BTW_MOVEMENTS);
			if ((fant->y/32) == prem(L).y) {
				L = reste(L);
			}
			
		}
		if (prem(L).y == (fant->y / taille) - 1) { // haut
			deplacement(fant,0,-TIME_BTW_MOVEMENTS);
			if ((fant->y/32) == prem(L).y) {

				L = reste(L);
			}
		}
	}


//		if((prem(L).x == fant.x) & (prem(L).y == fant.y) )
}

liste_point pathfinding(int map[NY][NX],int dx, int dy, int fx, int fy)
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
                    if (i>0 && (map[i-1][j] == 0 || map[i-1][j] == 4) && dist[i-1][j]>d+1)
                    {
                        dist[i-1][j] = d+1;
                    }
                    if (j>0 && (map[i][j-1] == 0 || map[i][j-1] == 4)&& dist[i][j-1]>d+1)
                    {
                        dist[i][j-1] = d+1;
                    }
                    if (i<LIG-1 && (map[i+1][j] == 0 || map[i+1][j] == 4) && dist[i+1][j]>d+1)
                    {
                        dist[i+1][j] = d+1;
                    }
                    if (j<COL -1 && (map[i][j+1] == 0 || map[i][j+1] == 4) && dist[i][j+1]>d+1)
                    {
                        dist[i][j+1] = d+1;
                    }
                }
            }
		}
	} // end for

	d = dist[fx][fy];

	if (d>=1000){
		return l_vide();
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






void HandleAnimations();

void HandleEvent(SDL_Surface *map, SDL_Event event)
{
	switch (event.type) {
		default:
			break;
		/* close button clicked */
		case SDL_QUIT:
			gameover = 1;
			break;

		/* handle the keyboard */
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
				default:
					break;
				case SDLK_ESCAPE:
				case SDLK_q:
					gameover = 1;
					break;


				case SDLK_LEFT:

				    if(moveUp){
                       			 moveUp = 0;
				    }
				    if(moveDown){
                        		moveDown = 0;
				    }
				    if(moveRight){
                      			  moveRight = 0;
				    }


					moveLeft=1;


				 	
					move = 1;			
					break;
				case SDLK_RIGHT:

				    if(moveUp){
                     			   moveUp = 0;
				    }
				    if(moveDown){
                     			   moveDown = 0;
				    }
				    if(moveLeft){
                     			   moveLeft = 0;
				    }
					moveRight=1;
					move = 1;
				 
					break;
				case SDLK_UP:

				    if(moveLeft){
                      			  moveLeft = 0;
				    }
				    if(moveDown){
                      			  moveDown = 0;
				    }
				    if(moveRight){
                      			  moveRight = 0;
				    }
					moveUp=1;
					move = 1;
				
					break;
				case SDLK_DOWN:

				    if(moveUp){
                      			  moveUp = 0;
				    }
				    if(moveLeft){
                       			 moveLeft = 0;
				    }
				    if(moveRight){
                       			 moveRight = 0;
				    }
					moveDown=1;
					move = 1;
				
					break;

				case SDLK_w:	// LEFT
					move=1;
					rcSG1.y = 0;
					rcSG1.x = rcSG1.x - 32;
					if (rcSG1.x <= 0) {
						rcSG1.x = 32;
					}

					 	rcG1.x -= 5;

					break;
				case SDLK_c:	//RIGHT
					move=1;
					rcSG1.y = G1_HEIGHT;
					rcSG1.x =  G1_WIDTH;
					if(rcSG1.x >= G1_WIDTH){
						rcSG1.x = 0;
					}
					 rcG1.x += 5;
					break;
				case SDLK_s:	//UP
					move=1;
					rcSG1.y = 3 * G1_HEIGHT;
					rcSG1.x =  G1_WIDTH;
					if ( rcSG1.x >= G1_WIDTH) {
						rcSG1.x = 0;
					}
				 	rcG1.y -= 5;
					break;
				case SDLK_x:	//DOWN
					move=1;
					rcSG1.y = 4 * G1_HEIGHT;
					rcSG1.x = G1_WIDTH;
					if ( rcSG1.x >= G1_WIDTH) {
						rcSG1.x = 0;
					}
					 rcG1.y += 5;

					break;
				case SDLK_m: 
					//musiqueif(Mix_PausedMusic() == 1)
					//musique{
					//musique	Mix_ResumeMusic(); 
					//musique	printf("p1");
					//musique}
					//musiqueelse
					//musique{
					//musique	Mix_PauseMusic(); 
					//musique	printf("p2"); 
					//musique}
					break;
				
				}
			break;
		}
}

void HandleMovements(int pos_Wall[NY][NX])
{

    if(moveUp){
	int n = ((rcSprite.x)/32);
	int m = ((rcSprite.y-1)/32); 

	int n2 = ((rcSprite.x+31)/32);
	int m2 = ((rcSprite.y-1)/32);

	if ((pos_Wall[m][n] == 0 || pos_Wall[m][n] == 4) && (pos_Wall[m2][n2] == 0 || pos_Wall[m2][n2] == 4)){
        currentTime = SDL_GetTicks();
         if(currentTime - previousTime > TIME_BTW_MOVEMENTS){
            rcSprite.y -= 1;
            previousTime = currentTime;
         }
         HandleAnimations();
		}
	}
    if(moveDown){
	int n = ((rcSprite.x)/32);
	int m = ((rcSprite.y+31+1)/32);

	int n2 = ((rcSprite.x+31)/32);
	int m2 = ((rcSprite.y+31+1)/32);

	if ((pos_Wall[m][n] == 0 || pos_Wall[m][n] == 4) && (pos_Wall[m2][n2] == 0 || pos_Wall[m2][n2] == 4)){
        currentTime = SDL_GetTicks();
        if(currentTime - previousTime > TIME_BTW_MOVEMENTS){
            rcSprite.y += 1;
            previousTime = currentTime;
         }
        HandleAnimations();
    }
	}
    if(moveLeft){
	int n = ((rcSprite.x-1)/32);
	int m = ((rcSprite.y)/32);

	int n2 = ((rcSprite.x-1)/32);
	int m2 = ((rcSprite.y+31)/32);
	if ((pos_Wall[m][n] == 0 || pos_Wall[m][n] == 4) && (pos_Wall[m2][n2] == 0 || pos_Wall[m2][n2] == 4)){
        currentTime = SDL_GetTicks();
        if(currentTime - previousTime > TIME_BTW_MOVEMENTS){
            rcSprite.x -= 1;
            previousTime = currentTime;
         }
         HandleAnimations();
    }
	}
    if(moveRight){
	int n = ((rcSprite.x+1)/32);
	int m = ((rcSprite.y)/32);

	int n2 = ((rcSprite.x+31+1)/32);
	int m2 = ((rcSprite.y+31)/32);

	//printf("pos_Wall[%d][%d] = %d, pos_Wall2[%d][%d] = %d\n",m,n,pos_Wall[m][n],m2,n2,pos_Wall[m2][n2]);
	if ((pos_Wall[m][n] == 0 || pos_Wall[m][n] == 4) && (pos_Wall[m2][n2] == 0 || pos_Wall[m2][n2] == 4)){
        currentTime = SDL_GetTicks();
        if(currentTime - previousTime > TIME_BTW_MOVEMENTS){
            rcSprite.x += 1;
            previousTime = currentTime;
        }
        HandleAnimations();
    }
	}
}
void HandleAnimations()
{
    if(moveUp){
        rcSrc.y = 2 * SPRITE_HEIGHT;
        currentTimeAnim = SDL_GetTicks();
        if(currentTimeAnim - previousTimeAnim > TIME_BTW_ANIMATIONS){
            rcSrc.x = rcSrc.x + SPRITE_WIDTH;
            previousTimeAnim = currentTimeAnim;
        }

        if ( rcSrc.x >= 5 * SPRITE_WIDTH) {
            rcSrc.x = 0;
        }
    }
    if(moveDown){
        rcSrc.y = 3 * SPRITE_HEIGHT;
        currentTimeAnim = SDL_GetTicks();
        if(currentTimeAnim - previousTimeAnim > TIME_BTW_ANIMATIONS){
            rcSrc.x = rcSrc.x + SPRITE_WIDTH;
            previousTimeAnim = currentTimeAnim;
        }
        if ( rcSrc.x >= 5 * SPRITE_WIDTH) {
            rcSrc.x = 0;
        }
    }
	
    if(moveLeft){
        rcSrc.y = SPRITE_HEIGHT;
        currentTimeAnim = SDL_GetTicks();
        if(currentTimeAnim - previousTimeAnim > TIME_BTW_ANIMATIONS){
            rcSrc.x = rcSrc.x - SPRITE_WIDTH;
            previousTimeAnim = currentTimeAnim;
        }
        if (rcSrc.x < 0) {
            rcSrc.x = 5 * SPRITE_WIDTH;
        }
    }
  
    if(moveRight){
        rcSrc.y = 0;
        currentTimeAnim = SDL_GetTicks();
        if(currentTimeAnim - previousTimeAnim > TIME_BTW_ANIMATIONS){
            rcSrc.x = rcSrc.x + SPRITE_WIDTH;
            previousTimeAnim = currentTimeAnim;
        }
        if(rcSrc.x > 5 * SPRITE_WIDTH){
            rcSrc.x = 0;
        }
    }
}

int main(int argc, char* argv[])
{
	SDL_Surface *screen, *map, *temp, *wall, *wall2, *bloc, *sprite, *g1, *g2, *g3, *candy;//musique *menu;
	SDL_Rect rcmap;
	int colorkey;
	int i,j;
	liste_point liste_coord;
	


	/*initialize move of pacman */
	move=0;

	/* initialize SDL */
	SDL_Init(SDL_INIT_VIDEO);
	 /* initialize SDL */
   	//musique SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    	//musique Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);  
    	//musique Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
    	//musique music = Mix_LoadMUS("sons/point.mp3");
	//musique start = Mix_LoadMUS("sons/start.mp3");

   	//musique  if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1){
   	//musique  printf("%s", Mix_GetError());
   	//musique  }
  	//musiqueMix_PlayMusic(start, 1);

	/* set the title bar */
	SDL_WM_SetCaption("Pacman", "Pacman");

	/* create window */
	screen = SDL_SetVideoMode(SCREEN_WIDTH, 640, 0, 0);

	/* set keyboard repeat */
	SDL_EnableKeyRepeat(30,30);

 	/*load menu */ 
 	//musique temp=SDL_LoadBMP("1.bmp");
 	//musiquemenu= SDL_DisplayFormat(temp);
 	//musique SDL_FreeSurface(temp);

	/* load sprite */
	temp   = SDL_LoadBMP("images/pacmanf_modif.bmp");
	sprite = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	/* setup sprite colorkey and turn on RLE*/
	colorkey = SDL_MapRGB(screen->format, 0,0,0);
	SDL_SetColorKey(sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

	/* load Wall */
	temp   = SDL_LoadBMP("images/wall.bmp");
	wall = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* load Wall2 */
	temp   = SDL_LoadBMP("images/wall2.bmp");
	wall2 = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* load bloc */
	temp   = SDL_LoadBMP("images/bloc.bmp");
	bloc = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);


	/* load candy */
	temp   = SDL_LoadBMP("images/bonbon.bmp");
	candy = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* load G1 */
	temp   = SDL_LoadBMP("images/g1.bmp");
	g1 = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* setup g1 colorkey and turn on RLE*/
	colorkey = SDL_MapRGB(screen->format, 0,0,0);
	SDL_SetColorKey(g1, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);


	/* load g2 */
	temp   = SDL_LoadBMP("images/g2.bmp");
	g2 = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* setup g2 colorkey and turn on RLE*/
	colorkey = SDL_MapRGB(screen->format, 0,0,0);
	SDL_SetColorKey(g2, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);


	/* load g3 */
	temp   = SDL_LoadBMP("images/g3.bmp");
	g3 = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* setup g3 colorkey and turn on RLE*/
	colorkey = SDL_MapRGB(screen->format, 0,0,0);
	SDL_SetColorKey(g3, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

	/* load map */
	temp  = SDL_LoadBMP("images/mapb.bmp");
	map = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

        /*initialise le temps*/
        currentTime = 0;
        previousTime = 0;
        currentTimeAnim = 0;
        previousTimeAnim = 0;

	/* set sprite position */
	rcSprite.x = SCREEN_WIDTH/2;
	rcSprite.y = SCREEN_HEIGHT/2 +32;

	/* set G1 position */
	rcG1.x = SCREEN_WIDTH/2 -32;
	rcG1.y = SCREEN_HEIGHT/2-32;

	/* set G2 positon */
	rcG2.x = SCREEN_WIDTH/2;
	rcG2.y = SCREEN_HEIGHT/2 -31;

	/* set  positon */
	rcG3.x = SCREEN_WIDTH/2+32;
	rcG3.y = SCREEN_HEIGHT/2 -32;


	/* set animation frame */
	rcSrc.x = 0;
	rcSrc.y = 0;
	rcSrc.w = SPRITE_WIDTH;
	rcSrc.h = SPRITE_HEIGHT;

	/* set animation frame */
	rcSG1.x = 0;
	rcSG1.y = 0;
	rcSG1.w = G1_WIDTH;
	rcSG1.h = G1_HEIGHT;

	rcmap.x = 0;
	rcmap.y = 0;

	gameover = 0;
	
	liste_coord = l_vide();

	int pos_Wall[NY][NX]= {
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,4,0,4,0,4,0,4,0,4,0,4,1,0,4,0,4,0,4,0,4,0,4,1},
	{1,0,1,1,0,1,1,1,1,1,1,4,1,4,1,1,1,1,1,0,1,1,0,1},
	{1,4,1,1,0,1,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,1,4,1},
	{1,0,0,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,0,1},
	{1,4,1,1,0,1,1,1,1,1,1,4,1,4,1,1,1,1,1,0,1,1,4,1},
	{1,0,1,1,4,0,4,0,4,0,1,0,1,0,1,4,0,4,0,4,1,1,0,1},
	{1,4,0,1,0,1,3,1,0,4,0,4,0,4,0,4,1,3,1,0,1,0,4,1},
	{1,1,0,1,4,1,4,1,4,0,1,1,5,1,1,0,1,4,1,0,1,0,1,1},
	{0,4,0,4,0,4,0,4,0,4,1,5,5,5,1,4,0,4,0,4,0,4,0,4},
	{1,1,0,1,4,1,1,1,0,0,1,1,1,1,1,0,1,1,1,0,1,0,1,1},
	{1,4,0,1,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,1,4,0,1},
	{1,0,1,1,4,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,1,4,1},
	{1,4,1,1,0,1,4,0,4,0,4,0,4,0,4,0,4,0,1,4,1,1,0,1},
	{1,0,1,1,4,1,0,0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,4,1},
	{1,4,0,4,0,4,0,4,0,4,0,4,1,4,0,4,0,4,0,4,0,4,0,1},
	{1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,4,1},
	{1,4,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,0,1},
	{1,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};
	

	/*int pos_Wall[NX][NY]= {
	{1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1,1,1},
	{1,4,0,4,0,4,0,4,1,4,1,4,0,4,0,4,0,4,0,1},
	{1,0,1,1,0,1,1,0,0,0,0,0,1,1,1,0,1,1,4,1},
	{1,4,1,1,0,1,1,1,1,4,1,1,1,1,1,4,1,1,0,1},
	{1,0,0,0,4,0,4,0,4,0,4,0,4,0,4,0,1,1,4,1},
	{1,4,1,1,0,1,0,3,1,4,1,4,1,1,1,4,1,1,0,1},
	{1,0,1,1,4,1,4,3,4,0,1,0,1,4,0,0,1,1,4,1},
	{1,4,1,1,0,1,0,3,1,4,1,4,1,0,0,4,1,1,0,1},
	{1,0,1,1,4,1,4,0,4,0,0,0,1,4,1,0,1,1,4,1},
	{1,4,1,1,0,1,0,4,0,4,0,4,0,0,1,4,1,1,0,1},
	{1,0,1,1,4,1,1,0,1,1,1,0,1,4,1,0,1,1,4,1},
	{1,4,4,0,0,4,0,4,1,5,1,4,1,0,1,4,0,0,0,1},
	{1,1,1,1,4,1,1,0,5,5,1,0,1,4,1,1,1,1,4,1},
	{1,0,4,0,0,4,0,4,1,5,1,4,1,0,1,4,0,0,0,1},
	{1,4,1,1,4,1,1,0,1,1,1,0,1,4,1,0,1,1,4,1},
	{1,0,1,1,0,1,4,4,0,4,0,4,0,0,1,4,1,1,0,1},
	{1,4,1,1,4,1,0,1,1,0,1,0,1,4,1,0,1,1,4,1},
	{1,0,1,1,0,1,4,3,4,4,1,4,1,0,0,4,1,1,0,1},
	{1,4,1,1,4,1,0,1,1,0,1,0,1,1,1,0,1,1,4,1},
	{1,0,0,0,0,0,4,0,0,4,0,4,0,4,0,4,1,1,0,1}};*/

/*
	
	int z,y;
	for(z=0;z<NY;z++)
	{
		for(y=0;y<NX;y++)
			printf(",%d",pos_Wall[z][y]);
		printf("\n");
	}

	printf("(6,8)= %d, (6,9)  = %d\n",pos_Wall[6][8],pos_Wall[6][9]);

*/
	
	int cpt=0;
	int n,m;

	int a,b;
	a = (rcG1.x/32)+0.5;
	b = (rcG1.y/32)+0.5;
	//musiqueSDL_BlitSurface(menu,NULL,screen,NULL);
  	//musiqueSDL_Flip(screen);
  	//musiqueSDL_Delay(5000);
	 /*message pump */

	while (!gameover)
{		
		
		
		SDL_Event event;
		/* look for an event */
		if (SDL_PollEvent(&event)) {
			HandleEvent(map, event);
		}


		
		//liste_coord = pathfinding(pos_Wall, rcG1.x, rcG1.y, rcSprite.x, rcSprite.y);

		//gameover = 1;

		if (move) {

			move = 0;
		}

		

       		 HandleMovements(pos_Wall);
		/* collide with edges of screen */
		if (rcSprite.x <= 0)
			rcSprite.x = SCREEN_WIDTH - SPRITE_WIDTH -1;
		if (rcSprite.x >= SCREEN_WIDTH - SPRITE_WIDTH)
			rcSprite.x = 1;

		if (rcSprite.y <= 0)
			rcSprite.y = 0;
		if (rcSprite.y >= SCREEN_HEIGHT - SPRITE_HEIGHT)
			rcSprite.y = SCREEN_HEIGHT - SPRITE_HEIGHT;


		/* draw the map */
		SDL_BlitSurface(map,NULL,screen,&rcmap);


		//printf("m= %d n= %d \n", m,n);

	for(i=0; i<NY ; i++){
		for(j=0;j<NX;j++){


			if ( pos_Wall[i][j] == 1 ){
				rcBloc.x = j * 32;
				rcBloc.y = i * 32;
				SDL_BlitSurface(bloc, NULL, screen, &rcBloc);
			}
			if ( pos_Wall[i][j] == 2){
				rcWall.x = j * 32;
				rcWall.y = i * 32;
				SDL_BlitSurface(wall, NULL, screen, &rcWall);
			}
			if ( pos_Wall[i][j] == 3){
				rcWall2.x = j * 32;
				rcWall2.y = i * 32;
				SDL_BlitSurface(wall2, NULL, screen, &rcWall2);
			}

			if ( pos_Wall[i][j] == 4 ){
				//printf("OK");
				rcCandy.x = j * 32+8;
				rcCandy.y = i * 32+8;
				SDL_BlitSurface(candy, NULL, screen, &rcCandy);
			}
			//if ( pos_Wall[m][n] != 0 ){
				//printf("PAS BON");
				//rcSprite.y = 0;

			//}
			

			

		}
	}

	
	
	/*if ( pos_Wall[m][n] == 4 || pos_Wall[m][n] ==0 ){
		for(i=0; i<NY ; i++){
			for(j=0;j<NX;j++){
				if (pos_Wall[i+1][j]==0 || pos_Wall[i+1][j]==4){
					if (pos_Wall[i-1][j]==0 || pos_Wall[i-1][j]==4{
						if (pos_Wall[i][j-1]==0 || pos_Wall[i][j-1]==4){
							if (pos_Wall[i][j+1]==0 || pos_Wall[i][j+1]==4){
								printf("ca marche");
							}
						}
					}
				}	

			}
		}
	}*/


	 n = ((rcSprite.x+16)/32);
	 m = ((rcSprite.y+16)/32);
	//printf("m= %d n= %d \n", m,n);
	if ( pos_Wall[m][n] == 4 ){
		cpt ++;
		printf("cpt = %d\n",cpt);
		pos_Wall[m][n]=0;
		//musiqueMix_PlayMusic(music, 1);
		if (cpt >= 100 ) {
			printf("VICTORY ");
		}				
	}
				
			
			
		
	


	/*for (i=0; i< NY;i++){
		for (j=0;j<NX;j++){
			printf("%d",pos_Wall[i][j]);
		}
	}
	*/

		/* draw the sprite */
		SDL_BlitSurface(sprite, &rcSrc, screen, &rcSprite);



		/* draw the GHOST 1 */
		SDL_BlitSurface(g1, NULL, screen, &rcG1);

		/* draw the GHOST 2 */
		SDL_BlitSurface(g2, NULL, screen, &rcG2);

		/* draw the GHOST 3 */
		SDL_BlitSurface(g3, NULL, screen, &rcG3);

		/* update the screen */
		SDL_UpdateRect(screen, 0, 0, 0, 0);
	
}
	/* clean up */

	afficher_point_liste(liste_coord);

	SDL_FreeSurface(sprite);
	SDL_FreeSurface(wall);
	SDL_FreeSurface(wall2);
	SDL_FreeSurface(bloc);
	SDL_FreeSurface(candy);
	SDL_FreeSurface(g1);
	SDL_FreeSurface(g2);
	SDL_FreeSurface(g3);
	SDL_FreeSurface(map);
	//musiqueSDL_FreeSurface(menu);
   	//musiqueMix_FreeMusic(music);
	SDL_Quit();
	return 0;
}

