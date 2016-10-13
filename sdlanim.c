#include "SDL.h"
/*
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL/SDL_mixer.h>
*/
#define SCREEN_WIDTH  720	
#define SCREEN_HEIGHT 797
#define SPRITE_WIDTH 32
#define SPRITE_HEIGHT 32

int gameover, previousTime, currentTime; // entier qui stocke le temps
int move; //gere le déplacement du pacman

/* source and destination rectangles */
SDL_Rect rcSrc, rcSprite;

void HandleEvent(SDL_Event event)
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
					
						rcSrc.y = SPRITE_HEIGHT;
						rcSrc.x = rcSrc.x - SPRITE_WIDTH;
						if (rcSrc.x < 0) {
							rcSrc.x = 5 * SPRITE_WIDTH;
						}
					
					rcSprite.x -= 5;
					break;
				case SDLK_RIGHT:
					rcSrc.y = 0;
					currentTime = SDL_GetTicks();
					//if(currentTime - previousTime > 50 ){ // temps en ms entre chaque animation				
						rcSrc.x = rcSrc.x + SPRITE_WIDTH;
						if(rcSrc.x > 5 * SPRITE_WIDTH){
						rcSrc.x = 0;
						}
					//previousTime = currentTime;
					//}
					rcSprite.x += 5;
					break;
				case SDLK_UP:
					rcSrc.y = 2 * SPRITE_HEIGHT;
					rcSrc.x = rcSrc.x + SPRITE_WIDTH;
					if ( rcSrc.x > 5 * SPRITE_WIDTH) {
						rcSrc.x = 0;
					}
					rcSprite.y -= 5;
					break;
				case SDLK_DOWN:
					rcSrc.y = 3 * SPRITE_HEIGHT;
					rcSrc.x = rcSrc.x + SPRITE_WIDTH;
					if ( rcSrc.x > 5 * SPRITE_WIDTH) {
						rcSrc.x = 0;
					}
					rcSprite.y += 5;
					break;
			}
			break;
	}
}

int main(int argc, char* argv[])
{
	SDL_Surface *screen, *temp, *sprite, *map;
	SDL_Rect rcmap;
	int colorkey;
	
	/*initialize move of pacman */
	move=0;
	
	/* initialize SDL */
	SDL_Init(SDL_INIT_VIDEO);

	/* set the title bar */
	SDL_WM_SetCaption("SDL Animation", "SDL Animation");

	/* create window */
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);

	/* set keyboard repeat */
	SDL_EnableKeyRepeat(70,70);

	/* load sprite */
	temp   = SDL_LoadBMP("pacman.bmp");
	sprite = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* setup sprite colorkey and turn on RLE*/ 
	colorkey = SDL_MapRGB(screen->format, 0,0,0);	//for eviter d'avoir le carré noir
	SDL_SetColorKey(sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);
	
	/* load map */
	temp  = SDL_LoadBMP("map.bmp");
	map = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* set sprite position */
	rcSprite.x = SCREEN_WIDTH/2 - 16; //position of pacman | 16 = decalage pour atteindre la moitie du screen
	rcSprite.y = SCREEN_HEIGHT/2 + 35; // 19 pour atteindre le bon milieu du screen

	/* set animation frame */
	rcSrc.x = 0; //modif
	rcSrc.y = 0;
	rcSrc.w = SPRITE_WIDTH;
	rcSrc.h = SPRITE_HEIGHT;

	rcmap.x = 0;
	rcmap.y = 0;

	gameover = 0;

	previousTime = currentTime = 0;
	/* message pump */
	while (!gameover)
	{
		SDL_Event event;
		
		/* look for an event */
		if (SDL_PollEvent(&event)) {
			HandleEvent(event);
		}

		/* collide with edges of screen */
		if (rcSprite.x <= 0)
			rcSprite.x = 0;
		if (rcSprite.x >= SCREEN_WIDTH - SPRITE_WIDTH) 
			rcSprite.x = SCREEN_WIDTH - SPRITE_WIDTH;

		if (rcSprite.y <= 0)
			rcSprite.y = 0;
		if (rcSprite.y >= SCREEN_HEIGHT - SPRITE_HEIGHT) 
			rcSprite.y = SCREEN_HEIGHT - SPRITE_HEIGHT;

		/* draw the map */
		/*for (int x = 0; x < SCREEN_WIDTH / SPRITE_SIZE; x++) {
			for (int y = 0; y < SCREEN_HEIGHT / SPRITE_SIZE; y++) {
				rcmap.x = x * SPRITE_SIZE;
				rcmap.y = y * SPRITE_SIZE;
				SDL_BlitSurface(map, NULL, screen, &rcmap);
			}
		}*/
		SDL_BlitSurface(map,NULL,screen,&rcmap);
		/* draw the sprite */
		SDL_BlitSurface(sprite, &rcSrc, screen, &rcSprite);

		/* update the screen */
		SDL_UpdateRect(screen, 0, 0, 0, 0);
	}

	/* clean up */
	SDL_FreeSurface(sprite);
	SDL_FreeSurface(map);
	SDL_Quit();

	return 0;
}
/*****************/
/* Regle du Jeu***/
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







