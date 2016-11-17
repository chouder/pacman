#include "SDL.h"
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
//#include <SDL/SDL_mixer.h>

#define SCREEN_WIDTH  768
#define SCREEN_HEIGHT 640
#define SPRITE_WIDTH 32
#define SPRITE_HEIGHT 32
#define SPRITE_SIZE 32
#define NY 20
#define NX 24
#define G1_WIDTH 32
#define G1_HEIGHT 34

#define TIME_BTW_ANIMATIONS 40
#define TIME_BTW_MOVEMENTS 5

int gameover;
int moveRight, moveLeft, moveUp, moveDown; //gere le déplacement du pacman
int move;
int currentTime, previousTime; //gerer le temps entre les deplacements
int currentTimeAnim, previousTimeAnim; //gerer le temps entre les animations
float dirX, dirY,x,y;

/* source and destination rectangles */
SDL_Rect rcSrc,rcWall,rcWall2, rcBloc, rcSprite,rcG1, rcSG1, rcG2,rcG3, rcCandy;
int i,j;
int pos_Wall[NY][NX];

int a;
int Convertir(float nb)
{
	nb += 0.5;
	int a = (int) nb;
	return a;
}

void HandleAnimations();

void HandleEvent(SDL_Surface *map, SDL_Event event)
{
//int n = ((rcSprite.x+16)/32) +0.5;
//int m = ((rcSprite.y+16)/32) +0.5;

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
				
				}
			break;
		}
}

void HandleMovements()
{

    if(moveUp){
        currentTime = SDL_GetTicks();
         if(currentTime - previousTime > TIME_BTW_MOVEMENTS){
            rcSprite.y -= 1;
            previousTime = currentTime;
         }
         HandleAnimations();
		}
    if(moveDown){
        currentTime = SDL_GetTicks();
        if(currentTime - previousTime > TIME_BTW_MOVEMENTS){
            rcSprite.y += 1;
            previousTime = currentTime;
         }
        HandleAnimations();
    }
    if(moveLeft){
        currentTime = SDL_GetTicks();
        if(currentTime - previousTime > TIME_BTW_MOVEMENTS){
            rcSprite.x -= 1;
            previousTime = currentTime;
         }
         HandleAnimations();
    }
    if(moveRight){
        currentTime = SDL_GetTicks();
        if(currentTime - previousTime > TIME_BTW_MOVEMENTS){
            rcSprite.x += 1;
            previousTime = currentTime;
        }
        HandleAnimations();
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
	SDL_Surface *screen, *map, *temp, *wall, *wall2, *bloc, *sprite, *g1, *g2, *g3, *candy;
	SDL_Rect rcmap;
	int colorkey;
	int i,j;


	/*initialize move of pacman */
	move=0;

	/* initialize SDL */
	SDL_Init(SDL_INIT_VIDEO);

	/* set the title bar */
	SDL_WM_SetCaption("Pacman", "Pacman");

	/* create window */
	screen = SDL_SetVideoMode(SCREEN_WIDTH, 640, 0, 0);

	/* set keyboard repeat */
	SDL_EnableKeyRepeat(30,30);

	/* load sprite */
	temp   = SDL_LoadBMP("pacman.bmp");
	sprite = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	/* setup sprite colorkey and turn on RLE*/
	colorkey = SDL_MapRGB(screen->format, 0,0,0);
	SDL_SetColorKey(sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

	/* load Wall */
	temp   = SDL_LoadBMP("wall.bmp");
	wall = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* load Wall2 */
	temp   = SDL_LoadBMP("wall2.bmp");
	wall2 = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	/* load bloc */
	temp   = SDL_LoadBMP("bloc.bmp");
	bloc = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);


	/* load candy */
	temp   = SDL_LoadBMP("bonbon.bmp");
	candy = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

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
	temp  = SDL_LoadBMP("mapb.bmp");
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


	int pos_Wall[NY][NX]= {
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,4,0,4,0,4,0,4,0,4,0,4,1,0,4,0,4,0,4,0,4,0,4,1},
	{1,0,1,1,0,1,1,1,1,1,1,4,1,4,1,1,1,1,1,0,1,1,0,1},
	{1,4,1,1,0,1,1,1,1,1,1,0,1,0,1,1,1,1,1,0,1,1,4,1},
	{1,0,0,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,0,1},
	{1,4,1,1,0,1,1,1,1,1,1,4,1,4,1,1,1,1,1,0,1,1,4,1},
	{1,0,1,1,4,0,4,0,4,0,1,0,1,0,1,4,0,4,0,4,1,1,0,1},
	{1,4,0,1,0,1,3,1,0,4,0,4,0,4,0,4,1,3,1,0,1,0,4,1},
	{1,1,0,1,4,1,4,1,4,0,1,1,0,1,1,0,1,4,1,0,1,0,1,1},
	{0,4,0,4,0,4,0,4,0,4,1,0,0,0,1,4,0,4,0,4,0,4,0,4},
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
	
	int cpt=0;
	int n,m;
	/* message pump */
	while (!gameover)
	{
		SDL_Event event;

		if (move) {
		
			move = 0;
		}

		/* look for an event */
		if (SDL_PollEvent(&event)) {
			HandleEvent(map, event);
		}

       		 HandleMovements();
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

	n = ((rcSprite.x+16)/32) +0.5;
        m = ((rcSprite.y+16)/32) +0.5;
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

	if ( pos_Wall[m][n] == 4 ){
		cpt ++;
		printf("cpt = %d\n",cpt);
		pos_Wall[m][n]=0;
		if (cpt == 101 ) {
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
	SDL_FreeSurface(sprite);
	SDL_FreeSurface(wall);
	SDL_FreeSurface(wall2);
	SDL_FreeSurface(bloc);
	SDL_FreeSurface(candy);
	SDL_FreeSurface(g1);
	SDL_FreeSurface(g2);
	SDL_FreeSurface(g3);
	SDL_FreeSurface(map);
	SDL_Quit();
	return 0;
}

