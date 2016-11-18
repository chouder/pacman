sdlanim:	sdlanim.c
	 g++ sdlanim.c `sdl-config --cflags --libs`  -o sdlanim

	#g++ sdlanim.c -lSDL_mixer -lm `sdl-config --cflags --libs`  -o sdlanim 


clean:
	rm sdlanim
