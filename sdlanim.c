#include "SDL.h"

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
//#include <SDL/SDL_mixer.h>

#define SCREEN_WIDTH  720	
#define SCREEN_HEIGHT 797
#define SPRITE_WIDTH 32
#define SPRITE_HEIGHT 32

#define BOARD_LEFT 100 //test
#define BOARD_TOP  0
#define BOARD_RIGHT 720
#define BOARD_BOTTOM 797

#define PAC_NY 23 
#define PAC_NX 24 


int gameover;// previousTime, currentTime entier qui stocke le temps
int move; //gere le déplacement du pacman

int pac_array[PAC_NY][PAC_NX]; // tab pour les possibilités du move du pac
float dirX, dirY,x,y;

/* source and destination rectangles */
SDL_Rect rcSrc, rcSprite, rcCandy, rcG1, rcG2,rcG3;

int a;

int Convertir(float nb) {
	nb += 0.5;
	int a = (int) nb;
	return a;
}

Uint32 getpixel(SDL_Surface *map, int x, int y) {
    if (x<0 || y<0 || x>=map->w || y>=map->h) return 0;
    Uint8 *p = (Uint8 *)map->pixels + y*map->pitch + x*map->format->BytesPerPixel;
    return p[0] | p[1] << 8 | p[2] << 16; // TODO if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
}


void HandleEvent(SDL_Surface *map, SDL_Event event)
{
	switch (event.type) {
		/* close button clicked */
		case SDL_QUIT:
			gameover = 1;
			break;
			
		/* handle the keyboard */
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
				case SDLK_q:
					gameover = 1;
					break;
				case SDLK_LEFT:
					move=1;
					rcSrc.y = SPRITE_HEIGHT;
					rcSrc.x = rcSrc.x - SPRITE_WIDTH;
					if (rcSrc.x < 0) {
						rcSrc.x = 5 * SPRITE_WIDTH;
					}
					if (!getpixel(map, rcSprite.x-1, rcSprite.y)) {
					 	rcSprite.x -= 5;
					}
					
					break;
				case SDLK_RIGHT:
					move=1;
					rcSrc.y = 0;
			//		currentTime = SDL_GetTicks();
			//if(currentTime - previousTime > 50 ){ //temps en ms entre chq animation				
						rcSrc.x = rcSrc.x + SPRITE_WIDTH;
						if(rcSrc.x > 5 * SPRITE_WIDTH){
						rcSrc.x = 0;
						}
					//previousTime = currentTime;
					//}
					if (!getpixel(map, rcSprite.x+33, rcSprite.y)) {
					 	rcSprite.x += 5;
					}
					
		        
					break;
				case SDLK_UP:
					move=1;
					rcSrc.y = 2 * SPRITE_HEIGHT;
					rcSrc.x = rcSrc.x + SPRITE_WIDTH;
					if ( rcSrc.x > 5 * SPRITE_WIDTH) {
						rcSrc.x = 0;
					}
					if (!getpixel(map, rcSprite.x-1, rcSprite.y-1)) {
					 	rcSprite.y -= 5;
					}


					break;
				case SDLK_DOWN:
					move=1;
					rcSrc.y = 3 * SPRITE_HEIGHT;
					rcSrc.x = rcSrc.x + SPRITE_WIDTH;
					if ( rcSrc.x > 5 * SPRITE_WIDTH) {
						rcSrc.x = 0;
					}
					if (!getpixel(map, rcSprite.x, rcSprite.y+33)) {
					 	rcSprite.y += 5;
					}
					
					break;
			}
			break;
	}
}

int main(int argc, char* argv[])
{
	SDL_Surface *screen, *temp, *sprite, *candy, *map, *g1, *g2, *g3;
	SDL_Rect rcmap;
	int colorkey;
	int i,j;

	/* print candy */

	/*
	for (i=0; i<5; i++) {
		for (j=0;j<5;j++) {
		pac_array[i][j] = candy;
		}	
	}
	*/
	
	/*initialize move of pacman */
	move=0;
	
	/* initialize SDL */
	SDL_Init(SDL_INIT_VIDEO);

	/* set the title bar */
	SDL_WM_SetCaption("Pacman", "Pacman");

	/* create window */
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);

	/* set keyboard repeat */
	SDL_EnableKeyRepeat(30,30);

	/* load sprite */
	temp   = SDL_LoadBMP("pacman.bmp");
	sprite = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
/* setup sprite colorkey and turn on RLE*/ 
	colorkey = SDL_MapRGB(screen->format, 0,0,0);	
	SDL_SetColorKey(sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
	
	/* load candy */
	temp   = SDL_LoadBMP("bonbon.bmp");
	candy = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	/* setup candy colorkey and turn on RLE*/ 
	colorkey = SDL_MapRGB(screen->format, 0,0,0);	
	SDL_SetColorKey(candy, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

	/* load G1 */
	temp   = SDL_LoadBMP("g1.bmp");
	g1 = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* setup g1 colorkey and turn on RLE*/
	colorkey = SDL_MapRGB(screen->format, 0,0,0);	
	SDL_SetColorKey(g1, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);


	/* load g2 */
	temp   = SDL_LoadBMP("g2.bmp");
	g2 = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* setup g2 colorkey and turn on RLE*/ 
	colorkey = SDL_MapRGB(screen->format, 0,0,0);	
	SDL_SetColorKey(g2, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);


	/* load g3 */
	temp   = SDL_LoadBMP("g3.bmp");
	g3 = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* setup g3 colorkey and turn on RLE*/ 
	colorkey = SDL_MapRGB(screen->format, 0,0,0);	
	SDL_SetColorKey(g3, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
	
	/* load map */
	temp  = SDL_LoadBMP("map.bmp");
	map = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* set sprite position */
	rcSprite.x = SCREEN_WIDTH/2 - 16; 
	rcSprite.y = SCREEN_HEIGHT/2 + 32; 

	/* set candy positon */
	rcCandy.x = SCREEN_WIDTH/2 - 80;
	rcCandy.y = SCREEN_HEIGHT/2 + 40;
	
	/* set G1 position */		//red
	rcG1.x = SCREEN_WIDTH/2 -64; 
	rcG1.y = SCREEN_HEIGHT/2 -40; 

	/* set G2 positon */		//blue
	rcG2.x = SCREEN_WIDTH/2-16;
	rcG2.y = SCREEN_HEIGHT/2 -40;

	/* set  positon */		//white
	rcG3.x = SCREEN_WIDTH/2+32;
	rcG3.y = SCREEN_HEIGHT/2 -40;

	
	/* set animation frame */
	rcSrc.x = 0; 
	rcSrc.y = 0;
	rcSrc.w = SPRITE_WIDTH;
	rcSrc.h = SPRITE_HEIGHT;

	rcmap.x = 0;
	rcmap.y = 0;

	gameover = 0;
	
	//previousTime = currentTime = 0;
	/* message pump */
	while (!gameover)
	{
		SDL_Event event;

		if (move) {
			//printf("x=%f et y=%f ",x,y);
			
			/*if (rcSprite.x < BOARD_LEFT){
				rcSprite.x = BOARD_LEFT ;
				move=0;	
			}*/

			// Je cherche à bloquer le pacman quand il tombe sur une case = 0    
			/*
			if (pac_array[11][12] == 0){
				rcSprite.y += 5;			
			}
			*/
  		
			move = 0;
		}

		/* look for an event */
		if (SDL_PollEvent(&event)) {
			HandleEvent(map, event);
		}

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
		/* draw the sprite */
		SDL_BlitSurface(sprite, &rcSrc, screen, &rcSprite);

		/* draw the candy */
		SDL_BlitSurface(candy, NULL, screen, &rcCandy);
		
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
	SDL_FreeSurface(sprite);
	SDL_FreeSurface(candy);
	SDL_FreeSurface(g1);
	SDL_FreeSurface(g2);
	SDL_FreeSurface(g3);
	SDL_FreeSurface(map);
	SDL_Quit();

	return 0;
}


/*****************/
/**Regle du Jeu***/
/*****************/
/*
C'est un pacman, les règles sont bien connues.
Le pacman se déplace avec les touches ou avec un joypad et mange des pastilles.
Les fantômes ont deux états :
En état "normal" il sont en couleurs et se déplacent de manière à se rapprocher du pacman.
Si le pacman rencontre un fantôme en état "normal", la partie est perdue.
Il y a une super-pastille à chaque coin de l'écran. Si le pacman en mange une, les fantômes passent à l'état "effrayé" pendant une durée déterminée.
Dans cet état les fantômes sont bleus et ralentis.
Ils se déplacent de manière à s'éloigner du pacman.
Si le pacman rencontre un fantôme en état "effrayé", le fantôme est mangé.
Ce fantôme retourne alors au centre (on ne voit que ses yeux durant ce retour) et redevient à l'état "normal".
La partie est gagnée lorsque le pacman a mangé toutes les pastilles du jeu.
*/
