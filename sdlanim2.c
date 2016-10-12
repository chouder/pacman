#include "SDL.h"
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
//#include <SDL/SDL_mixer.h>

#define SCREEN_WIDTH  720	
#define SCREEN_HEIGHT 600
#define SPRITE_WIDTH 32
#define SPRITE_HEIGHT 32

int gameover, previousTime, currentTime; // entier qui stocke le temps


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
					if(currentTime - previousTime > 50 ){ // temps en ms entre chaque animation				
						rcSrc.x = rcSrc.x + SPRITE_WIDTH;
						if(rcSrc.x > 5 * SPRITE_WIDTH){
						rcSrc.x = 0;
						}
					previousTime = currentTime;
					}
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
	SDL_Surface *screen, *temp, *sprite, *grass;
	SDL_Rect rcGrass;
	int colorkey;

	/* initialize SDL */
	SDL_Init(SDL_INIT_VIDEO);

	/* set the title bar */
	SDL_WM_SetCaption("SDL Animation", "SDL Animation");

	/* create window */
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);

	/* set keyboard repeat */
	SDL_EnableKeyRepeat(70, 70);

	/* load sprite */
	temp   = SDL_LoadBMP("pacman2.bmp");
	sprite = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* setup sprite colorkey and turn on RLE */
	//colorkey = SDL_MapRGB(screen->format, 255,255,255);
	//SDL_SetColorKey(sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

	/* load grass */
	temp  = SDL_LoadBMP("l1.bmp");
	grass = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* set sprite position */
	rcSprite.x = 150;
	rcSprite.y = 150;

	/* set animation frame */
	rcSrc.x = 0; //modif
	rcSrc.y = 0;
	rcSrc.w = SPRITE_WIDTH;
	rcSrc.h = SPRITE_HEIGHT;

	rcGrass.x = 0;
	rcGrass.y = 0;

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

		/* draw the grass */
		/*for (int x = 0; x < SCREEN_WIDTH / SPRITE_SIZE; x++) {
			for (int y = 0; y < SCREEN_HEIGHT / SPRITE_SIZE; y++) {
				rcGrass.x = x * SPRITE_SIZE;
				rcGrass.y = y * SPRITE_SIZE;
				SDL_BlitSurface(grass, NULL, screen, &rcGrass);
			}
		}*/
		SDL_BlitSurface(grass,NULL,screen,&rcGrass);
		/* draw the sprite */
		SDL_BlitSurface(sprite, &rcSrc, screen, &rcSprite);

		/* update the screen */
		SDL_UpdateRect(screen, 0, 0, 0, 0);
	}

	/* clean up */
	SDL_FreeSurface(sprite);
	SDL_FreeSurface(grass);
	SDL_Quit();

	return 0;
}
