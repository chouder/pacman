#include "SDL.h"
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL/SDL_mixer.h>

#include "liste_point.h"

#define SCREEN_WIDTH  768
#define SCREEN_HEIGHT 640
#define SPRITE_WIDTH 31
#define SPRITE_HEIGHT 31
#define SPRITE_SIZE 31
#define NY 20
#define NX 24
#define G1_WIDTH 31
#define G1_HEIGHT 31
#define COL 24
#define LIG 20
#define TIME_BTW_ANIMATIONS 40
//#define TIME_BTW_MOVEMENTS 10
int TIME_BTW_MOVEMENTS = 5 ;
#define taille 31

int gameover;
int moveRight, moveLeft, moveUp, moveDown; //gere le déplacement du pacman
int move;
int currentTime, previousTime, fant_time; //gerer le temps entre les deplacements
int currentTimeAnim, previousTimeAnim; //gerer le temps entre les animations
float x,y;
int time_game;
int time_game2;

/* source and destination rectangles */
SDL_Rect rcSrc,rcWall,rcWall2, rcBloc, rcSprite,rcG1, rcSG1, rcG2,rcG3, rcCandy, rcCandy2;
int i,j;

Mix_Music *music,*start, *scream,*pilule,*die;
int Mix_OpenAudio(int frequency, Uint16 format, int channels, int chunksize);	

void deplacement(SDL_Rect *fant, int x, int y){
	fant->x += x;
	fant->y += y;
}

void deplacementFantomeBlanc(int tab[NY][NX],SDL_Rect *fant, int *a, int *b)
{
	if(*a == 0){
		
		if(moveLeft){
			if(tab[fant->y/32][fant->x / 32 + 1] != 1 && tab[fant->y/32][fant->x / 32 +1] != 3)
				*a = 1;
		}
		if(moveRight){
			if(tab[fant->y/32][fant->x/32 - 1] != 1 && tab[fant->y/32][fant->x/32 - 1] != 3)
				*a = 2;
		}
		if(moveUp){
			if(tab[fant->y/32 + 1][fant->x/32] != 1 && tab[fant->y/32 + 1][fant->x/32] != 3)
				*a = 3;
		}
		if(moveDown){
			if(tab[fant->y/32 - 1][fant->x/32] != 1 && tab[fant->y/32 - 1][fant->x/32] != 3)
				*a = 4;
		}
	}

	if(*a == 1){
		deplacement(fant,1,0);
		*b += 1;
	}
	if(*a == 2){
		deplacement(fant,-1,0);
		*b += 1;
	}
	if(*a == 3){
		deplacement(fant,0,1);
		*b += 1;
	}
	if(*a == 4){
		deplacement(fant,0,-1);
		*b += 1;
	}
	if(*b == 32){
		*a = 0;
		*b = 0;
	}
}

liste_point deplacementFantomeR(liste_point L, SDL_Rect *fant, int *a, int *b) {
	if (!est_vide(L)){
		if(*a == 0){
			if (prem(L).x == ((fant->x) / taille) + 1) { // droite
				*a = 1;
			}
			if (prem(L).x == ((fant->x) / taille) - 1) { // gauche
				*a = 2;
			}
			if (prem(L).y == ((fant->y) / taille) + 1) {	//bas
				*a = 3;
			}
			if (prem(L).y == ((fant->y) / taille) - 1) { // haut
				*a = 4;
			}
		}
		if(*a == 1){
			deplacement(fant,1,0);
			*b += 1;
		}
		if(*a == 2){
			deplacement(fant,-1,0);
			*b += 1;
		}
		if(*a == 3){
			deplacement(fant,0,1);
			*b += 1;
		}
		if(*a == 4){
			deplacement(fant,0,-1);
			*b += 1;
		}
		if(*b == 32){
			*a = 0;
			*b = 0;
			 L = reste(L);
		}
		/*if (prem(L).x == (fant->x / taille) + 1) { // droite
			//printf("DROITE\n");
			deplacement(fant,32,0);
			
			if (((fant->x)/31) == prem(L).x) {
				//m = fant.x/32;
				L = reste(L);
				return L;
			}
		}
		if (prem(L).x == (fant->x / taille) - 1) { // gauche
			//printf("GAUCHE\n");
			deplacement(fant,-32,0);
			if (((fant->x)/31) == prem(L).x) {
				L = reste(L);
				return L;
			}
			
		}
		if (prem(L).y == (fant->y / taille) + 1) {	//bas
			//printf("BAS\n");
			deplacement(fant,0,32);
			if (((fant->y)/31) == prem(L).y) {
				L = reste(L);
				return L;
			}
			
		}
		if (prem(L).y == (fant->y / taille) - 1) { // haut
			//printf("HAUT\n");
			deplacement(fant,0,-32);
			if (((fant->y)/31) == prem(L).y) {
				L = reste(L);
				return L;
			}
		}*/
	}
	return L;
}

liste_point pathfinding(int map[NY][NX],int dy, int dx, int fy, int fx)
{
	int i,j;

	liste_point LF;
	point pNode;
	LF = l_vide();

	int dist[NY][NX];
	int d;
	
	for (i=0; i<NY; i++)
		for (j=0; j<NX; j++)
        {
			dist[i][j] = 40000;
		}

	dist[dy][dx] = 0;
	
	for (d=0;d<NY*NX;d++)
	{
		for (i=0; i<NY; i++)
		{
			for (j=0; j<NX; j++)
			{
				if(dist[i][j] == d)
				{
				    if (i>0 && (map[i-1][j] != 1 && map[i-1][j] != 3) && dist[i-1][j]>d+1)
				    {
				        dist[i-1][j] = d+1;
				    }
				    if (j>0 && (map[i][j-1] != 1 && map[i][j-1] != 3)&& dist[i][j-1]>d+1)
				    {
				        dist[i][j-1] = d+1;
				    }
				    if (i<NY-1 && (map[i+1][j] != 1 && map[i+1][j] != 3) && dist[i+1][j]>d+1)
				    {
				        dist[i+1][j] = d+1;
				    }
				    if (j<NX -1 && (map[i][j+1] != 1 && map[i][j+1] != 3) && dist[i][j+1]>d+1)
				    {
				        dist[i][j+1] = d+1;
				    }
				}
            }
		}
	}	
	d = dist[fy][fx];

	if (d>=1000){
		return l_vide();
	}
	
    pNode = remplisPoint(fx,fy); // met l'arrivé dans la liste
    LF = cons(pNode,LF);
	
	for (;d>0;d--) {
		if (fy>0 && dist[fy-1][fx] == d-1){
			fy--;
			pNode = remplisPoint(fx,fy);
			LF = cons(pNode,LF);
			continue;
        }
		if (fx>0 && dist[fy][fx-1] == d-1){
			fx--;
			pNode = remplisPoint(fx,fy);
			LF = cons(pNode,LF);
			continue; }
		if (fy<NY+1 && dist[fy+1][fx] == d-1) {
			fy++;
			pNode = remplisPoint(fx,fy);
			LF = cons(pNode,LF);
			continue; }
		if (fx<NX+1  && dist[fy][fx+1] == d-1) {
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
					break;
				case SDLK_m: 
					if(Mix_PausedMusic() == 1)
					{
						Mix_ResumeMusic(); 
						printf("p1");
					}
					else
					{
						Mix_PauseMusic(); 
						printf("p2"); 
					}
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

	if ((pos_Wall[m][n] == 0 || pos_Wall[m][n] == 4 || pos_Wall[m][n] == 6) && (pos_Wall[m2][n2] == 0 || pos_Wall[m2][n2] == 4 || pos_Wall[m2][n2] == 6)){
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
	if ((pos_Wall[m][n] == 0 || pos_Wall[m][n] == 4|| pos_Wall[m2][n2] == 6) && (pos_Wall[m2][n2] == 0 || pos_Wall[m2][n2] == 4 || pos_Wall[m2][n2] == 6)){
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
	if ((pos_Wall[m][n] == 0 || pos_Wall[m][n] == 4 || pos_Wall[m2][n2] == 6) && (pos_Wall[m2][n2] == 0 || pos_Wall[m2][n2] == 4 || pos_Wall[m2][n2] == 6)){
        currentTime = SDL_GetTicks();
        if(currentTime - previousTime > TIME_BTW_MOVEMENTS){
            rcSprite.x -= 1;
            previousTime = currentTime;
         }
         HandleAnimations();
    }
	}
    if(moveRight){
	int n = ((rcSprite.x+31+1)/32);
	int m = ((rcSprite.y)/32);

	int n2 = ((rcSprite.x+31+1)/32);
	int m2 = ((rcSprite.y+31)/32);
	if ((pos_Wall[m][n] == 0 || pos_Wall[m][n] == 4 || pos_Wall[m2][n2] == 6) && (pos_Wall[m2][n2] == 0 || pos_Wall[m2][n2] == 4 || pos_Wall[m2][n2] == 6)){
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

int main()
{
	SDL_Surface *screen, *map, *temp, *wall, *wall2, *bloc, *sprite, *g1, *g2, *g3, *candy, *candy2, *menu, *gover;
	SDL_Rect rcmap;
	int colorkey;
	int i,j;
	int deplaSG1,deplaCG1,deplaSG2,deplaCG2,deplaSG3,deplaCG3;
	liste_point liste_coord;

	/*initialize move of pacman */
	move=0;

	/* initialize SDL */
	//SDL_Init(SDL_INIT_VIDEO);
	/* initialize SDL */
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
   	 Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);  
    	 Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
    	 music = Mix_LoadMUS("sons/point.mp3");
	 start = Mix_LoadMUS("sons/start.mp3");
    	 scream = Mix_LoadMUS("sons/scream.wav");
    	 pilule = Mix_LoadMUS("sons/pilule.wav");
    	 die = Mix_LoadMUS("sons/die.mp3");

	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1){
		printf("%s", Mix_GetError());
	}

	/* set the title bar */
	SDL_WM_SetCaption("Pacman", "Pacman");

	/* create window */
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);

	/* set keyboard repeat */
	SDL_EnableKeyRepeat(30,30);

 	/*load menu */ 
 	 temp=SDL_LoadBMP("images/1.bmp");
 	menu= SDL_DisplayFormat(temp);
 	 SDL_FreeSurface(temp);

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

	/* load Wall */
	temp   = SDL_LoadBMP("images/gameover.bmp");
	gover = SDL_DisplayFormat(temp);
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
	
	/* load candy special */
	temp   = SDL_LoadBMP("images/bonbon2.bmp");
	candy2 = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* load G1 */
	temp   = SDL_LoadBMP("images/g1_f.bmp");
	g1 = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* setup g1 colorkey and turn on RLE*/
	colorkey = SDL_MapRGB(screen->format, 0,0,0);
	SDL_SetColorKey(g1, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);


	/* load g2 */
	temp   = SDL_LoadBMP("images/g2_f.bmp");
	g2 = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* setup g2 colorkey and turn on RLE*/
	colorkey = SDL_MapRGB(screen->format, 0,0,0);
	SDL_SetColorKey(g2, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);


	/* load g3 */
	temp   = SDL_LoadBMP("images/g3_f.bmp");
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
	fant_time = 0;
	time_game = 0;
	time_game2 = 0;

	/* set sprite position */
	/*rcSprite.x = SCREEN_WIDTH/2;
	rcSprite.y = SCREEN_HEIGHT/2 +32;*/

	/* set G position */

	/*
	rcG1.x = SCREEN_WIDTH/2 -32;
	rcG1.y = SCREEN_HEIGHT/2-32;


	rcG2.x = SCREEN_WIDTH/2;
	rcG2.y = SCREEN_HEIGHT/2 -31;


	rcG3.x = SCREEN_WIDTH/2+32;
	rcG3.y = SCREEN_HEIGHT/2 -32;
	*/

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
	{1,6,0,4,0,4,0,4,0,4,0,4,1,0,4,0,4,0,4,0,4,0,6,1},
	{1,0,1,1,4,1,1,1,1,1,1,4,1,4,1,1,1,1,1,4,1,1,0,1},
	{1,4,1,1,0,1,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,1,4,1},
	{1,0,0,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,0,1},
	{1,4,1,1,0,1,1,1,1,1,1,4,1,4,1,1,1,1,1,4,1,1,4,1},
	{1,0,1,1,4,0,4,0,4,0,1,0,1,0,1,4,0,4,0,0,1,1,0,1},
	{1,4,0,1,0,1,3,1,0,4,0,4,0,4,0,4,1,3,1,4,1,0,4,1},
	{1,1,0,1,4,1,4,1,4,0,1,1,5,1,1,0,1,4,1,0,1,0,1,1},
	{0,4,0,4,0,4,0,4,0,4,1,7,8,9,1,4,0,4,0,4,0,4,0,4},
	{1,1,0,1,4,1,1,1,0,0,1,1,1,1,1,0,1,1,1,0,1,0,1,1},
	{1,4,0,1,0,4,0,4,0,4,0,4,10,4,0,4,0,4,0,4,1,4,0,1},
	{1,0,1,1,4,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,1,1,4,1},
	{1,4,1,1,0,1,4,0,4,0,4,0,4,0,4,0,4,0,1,4,1,1,0,1},
	{1,0,1,1,4,1,0,0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,4,1},
	{1,4,0,4,0,4,0,4,0,4,0,4,1,4,0,4,0,4,0,4,0,4,0,1},
	{1,0,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,4,1},
	{1,4,1,1,1,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,1,1,0,1},
	{1,0,6,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,6,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};

	int cpt=0;
	int n,m;
	int a,b;
	int c,d;
	int e,f;

	deplaSG1 = 0;
	deplaCG1 = 0;
	deplaSG2 = 0;
	deplaCG2 = 0;
	deplaSG3 = 0;
	deplaCG3 = 0;


	Mix_PlayMusic(start, 1);
	SDL_BlitSurface(menu,NULL,screen,NULL);
  	SDL_Flip(screen);
  	SDL_Delay(5000);

	 /*message pump */
	while (!gameover)
{
	b = (rcG1.x+16)/32;
	a = (rcG1.y+16)/32;

	d = (rcG2.x+16)/32;
	c = (rcG2.y+16)/32;

	f = (rcG3.x+16)/32;
	e = (rcG3.y+16)/32;
	
	n = ((rcSprite.x+16)/32);
	m = ((rcSprite.y+16)/32);

		SDL_Event event;
		/* look for an event */
		if (SDL_PollEvent(&event)) {
			HandleEvent(map, event);
		}

		time_game = SDL_GetTicks();

		if(time_game == 5 ||time_game % 5 == 0){
			liste_coord = pathfinding(pos_Wall, a, b, m, n);
			liste_coord = deplacementFantomeR(liste_coord, &rcG1, &deplaSG1,&deplaCG1);

			liste_coord = pathfinding(pos_Wall, c, d, m, n);
			liste_coord = deplacementFantomeR(liste_coord, &rcG2, &deplaSG2,&deplaCG2);

			liste_coord = pathfinding(pos_Wall, e, f, m, n);
			liste_coord = deplacementFantomeR(liste_coord, &rcG3, &deplaSG3,&deplaCG3);
 			//deplacementFantomeBlanc(pos_Wall,&rcG2, &deplaSG2,&deplaCG2);
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
				rcCandy.x = j * 32+8;
				rcCandy.y = i * 32+8;
				SDL_BlitSurface(candy, NULL, screen, &rcCandy);
			}
			if ( pos_Wall[i][j] == 6 ){
				rcCandy2.x = j * 32+8;
				rcCandy2.y = i * 32+8;
				SDL_BlitSurface(candy2, NULL, screen, &rcCandy2);
			}
			/* draw the GHOST 1 */
			if ( pos_Wall[i][j] == 7 ){
				rcG1.x = j * 32;
				rcG1.y = i * 32;
				SDL_BlitSurface(g1, NULL, screen, &rcG1);
			}
			/* draw the GHOST 2 */
			if ( pos_Wall[i][j] == 8 ){
				rcG2.x = j * 32;
				rcG2.y = i * 32;
				SDL_BlitSurface(g2, NULL, screen, &rcG2);
			}
			/* draw the GHOST 3 */
			if ( pos_Wall[i][j] == 9 ){
				rcG3.x = j * 32;
				rcG3.y = i * 32;
				SDL_BlitSurface(g3, NULL, screen, &rcG3);
			}
			/* draw the sprite */
			if ( pos_Wall[i][j] == 10 ){
				rcSprite.x = j * 32;
				rcSprite.y = i * 32;
				SDL_BlitSurface(sprite, &rcSrc, screen, &rcSprite);
			}			
		}
	}


	if ( pos_Wall[m][n] == 4){
		cpt ++;
		printf("cpt = %d\n",cpt);
		pos_Wall[m][n]=0;
		Mix_PlayMusic(music, 1);
						
	}

	if (pos_Wall[m][n]==6) {
		pos_Wall[m][n]=0;
		Mix_PlayMusic(pilule, 1);
		TIME_BTW_MOVEMENTS -= 4;
		printf(" Le pacman ++++\n");
		cpt += 10;
		printf("cpt = %d\n",cpt);
		time_game = SDL_GetTicks();
		if(time_game2 == 2000 ||time_game2 % 2000 == 0){
			TIME_BTW_MOVEMENTS = 10;
			printf(" Le pacman ------ \n");
			time_game2 = 0;
		} 
	}
	if (cpt >= 140 ) {
		printf("VICTORY ");
	}

	if (pos_Wall[m][n] == pos_Wall[a][b] || pos_Wall[m][n] == pos_Wall[c][d] || pos_Wall[m][n] == pos_Wall[e][f]){
		printf("GAMEOVER");
		//gameover = 1;
		if (cpt == 5 ) {
			/* load screamer */
			temp = SDL_LoadBMP("images/scream.bmp");
			gover = SDL_DisplayFormat(temp);
			SDL_FreeSurface(temp);
			Mix_PlayMusic(scream, 1); 
			SDL_BlitSurface(gover,NULL,screen,NULL);
			SDL_Flip(screen);
			SDL_Delay(2500);
		}
		Mix_PlayMusic(die, 1);
	}
		
		/* update the screen */
		SDL_UpdateRect(screen, 0, 0, 0, 0);
	
}
	/* clean up */
	SDL_FreeSurface(sprite);
	SDL_FreeSurface(wall);
	SDL_FreeSurface(wall2);
	SDL_FreeSurface(bloc);
	SDL_FreeSurface(candy);
	SDL_FreeSurface(g1);
	SDL_FreeSurface(g2);
	SDL_FreeSurface(g3);
	SDL_FreeSurface(map);
	SDL_FreeSurface(menu);
	SDL_FreeSurface(gover);
   	Mix_FreeMusic(music);
	Mix_FreeMusic(start);
	Mix_FreeMusic(scream);
	Mix_FreeMusic(pilule);
	Mix_FreeMusic(die);
	SDL_Quit();
	return 0;
}

