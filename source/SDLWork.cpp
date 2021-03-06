#include <SDL.h>
#include <SDL_thread.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <SDL_mixer.h>
#include <math.h> 
#include <errno.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <iostream>
#include <math.h>  
#include <Vector>
#include "SDLWork.hpp"
#include "utils.hpp"

extern SDLB GOD;
extern LTexture gTextTexture;
extern LTexture Farest;
extern std::string rootdirectory;

void SDLB::intA(){
	//Start up SDL and create window
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());

	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
#ifdef __SWITCH__
		gWindow = SDL_CreateWindow("sdl2_gles2", 0, 0, 1280, 720, 0);
#else
		gWindow = SDL_CreateWindow("RipJKNX", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
#endif // SWITCH


		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());

		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());

			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());

				}

				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());

				}
				//Initialize SDL_mixer
				if (Mix_OpenAudio(48000, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
				{
					printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
					
				}
#ifdef __SWITCH__
				//Load music
				if (isFileExist(rootdirectory+"wada.ogg")){
					gMusic = Mix_LoadMUS((rootdirectory+"wada.ogg").c_str());
				} else {
					gMusic = Mix_LoadMUS("romfs:/wada.ogg");
				}

#else
				gMusic = Mix_LoadMUS("C:/respaldo2017/C++/test/Debug/wada.ogg");
#endif // SWITCH
				
				if (gMusic == NULL)
				{
					printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
					
				}
				if (Mix_PlayingMusic() == 0)
				{
					//Play the music
					if(isFileExist(rootdirectory+"play"))
					Mix_PlayMusic(gMusic, -1);
				}
			}
		}
	}

#ifdef __SWITCH__

	gFont = TTF_OpenFont("romfs:/lazy.ttf", 16);
	gFont2 = TTF_OpenFont("romfs:/lazy2.ttf", 150);
	gFontcapit = TTF_OpenFont("romfs:/lazy2.ttf", 100);
	gFont3 = TTF_OpenFont("romfs:/lazy2.ttf", 40);
	gFont4 = TTF_OpenFont("romfs:/lazy2.ttf", 30);
	digifont = TTF_OpenFont("romfs:/digifont.otf", 16);

#else
	gFont = TTF_OpenFont("C:\\respaldo2017\\C++\\test\\Debug\\lazy.ttf", 16);
	digifont = TTF_OpenFont("C:\\respaldo2017\\C++\\test\\Debug\\digifont.otf", 16);
	gFont2 = TTF_OpenFont("C:\\respaldo2017\\C++\\test\\Debug\\lazy2.ttf", 150);
	gFontcapit = TTF_OpenFont("C:\\respaldo2017\\C++\\test\\Debug\\lazy2.ttf", 100);
	gFont3 = TTF_OpenFont("C:\\respaldo2017\\C++\\test\\Debug\\lazy2.ttf", 40);
#endif // SWITCH
}
void SDLB::Image(std::string path,int X, int Y,std::string Text,bool off){

		SDL_Surface* DrawImg;
		DrawImg = IMG_Load(path.c_str());
		int DW=0,DH=0;
		if (DrawImg == NULL)
		{
			printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		}else{
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
		SDL_Texture* CLUST = SDL_CreateTextureFromSurface(gRenderer, DrawImg);
		if (off){SDL_SetTextureColorMod(CLUST, 150, 150, 150);}

		SDL_Rect ImagetRect2 = {X, Y, DrawImg->w, DrawImg->h};
		SDL_RenderCopy(gRenderer, CLUST , NULL, &ImagetRect2);
		SDL_DestroyTexture(CLUST);
		DW=DrawImg->w;
		DH=DrawImg->h;
		}
		
		if (Text.length()){
			SDL_Surface* textSurface = TTF_RenderText_Blended(gFont, Text.c_str(), { 50, 50, 50 });
			if (textSurface == NULL)
			{
				printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
			}
			else
			{
				SDL_Rect TextRect = {X+DW+3, Y +(DH/3), textSurface->w, textSurface->h};
				//Create texture from surface pixels
				SDL_Texture* TextureT = SDL_CreateTextureFromSurface(gRenderer, textSurface);
				//Render to screen
				SDL_RenderCopy(gRenderer, TextureT, NULL, &TextRect);
				SDL_DestroyTexture(TextureT);
			}
			SDL_FreeSurface(textSurface);
		}
		SDL_FreeSurface(DrawImg);
		
		
}

void SDLB::Cover(std::string path,int X, int Y,std::string Text,int HS){

		SDL_Surface* DrawImg = NULL;
		DrawImg = IMG_Load(path.c_str());
		int WS=0;
		
		if (DrawImg == NULL)
		{
			printf("Unable to load image %s ! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		}else{
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
		SDL_Texture* CLUST = SDL_CreateTextureFromSurface(gRenderer, DrawImg);
			
		WS = (DrawImg->w * (HS * 1000 /DrawImg->h) /1000);
		
		
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 100);
		SDL_Rect HeaderRect = {X+5,Y+5, WS+6, HS+6};
		SDL_RenderFillRect(gRenderer, &HeaderRect);
		
		SDL_Rect ImagetRect2 = {X, Y, WS, HS};
		SDL_RenderCopy(gRenderer, CLUST , NULL, &ImagetRect2);
		SDL_DestroyTexture(CLUST);

		}

		if (Text.length()){
			SDL_Surface* textSurface = TTF_RenderText_Blended(gFont, Text.c_str(), { 50, 50, 50 });
			if (textSurface == NULL)
			{
				printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
			}
			else
			{
				SDL_Rect TextRect = {X+WS+3, Y +(HS/3), textSurface->w, textSurface->h};
				//Create texture from surface pixels
				SDL_Texture* TextureT = SDL_CreateTextureFromSurface(gRenderer, textSurface);
				//Render to screen
				SDL_RenderCopy(gRenderer, TextureT, NULL, &TextRect);
				SDL_DestroyTexture(TextureT);
			}
			SDL_FreeSurface(textSurface);
		}
		SDL_FreeSurface(DrawImg);
}

void SDLB::PleaseWait(std::string text,bool render){
	//Clear screen
	SDL_SetRenderDrawColor(gRenderer, 0x55, 0x55, 0x55, 0xFF);
	SDL_RenderClear(gRenderer);

	//wallpaper
	Farest.render((0), (0));	
	SDL_Rect fillRect = { 0, 720/2 - 25, 1280, 50 };
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
	SDL_RenderFillRect(gRenderer, &fillRect);
	
	gTextTexture.loadFromRenderedText(gFont3, text.c_str(), { 50, 50, 50 });
	gTextTexture.render(1280/2 - gTextTexture.getWidth()/2, 720/2 - gTextTexture.getHeight() / 2);
	if (render)	SDL_RenderPresent(gRenderer);
}

void SDLB::deint(){
	//Free global font
	TTF_CloseFont(gFont);
	gFont = NULL;
	TTF_CloseFont(digifont);
	digifont = NULL;

	TTF_CloseFont(gFontcapit);
	gFontcapit = NULL;
	TTF_CloseFont(gFont2);
	gFont2 = NULL;
	TTF_CloseFont(gFont3);
	gFont3 = NULL;
	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
	//Free the music
	Mix_FreeMusic(gMusic);
	gMusic = NULL;
	Mix_Quit();
	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

}

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	mX = 0;
	mY = 0;
	SelIns = -1;
	mark=true;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile(std::string path)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());remove(path.c_str());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(GOD.gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}
bool LTexture::loadFromFileCustom(std::string path, int h, int w)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		if(std::string(IMG_GetError()) == "Unsupported image format") remove(path.c_str());
	}
	else
	{
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(GOD.gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = w;
			mHeight = h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}
bool LTexture::loadFromRenderedText(TTF_Font *fuente, std::string textureText, SDL_Color textColor)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Blended(fuente, textureText.c_str(), textColor);

	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(GOD.gRenderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return mTexture != NULL;
}
bool LTexture::loadFromRenderedTextWrap(TTF_Font *fuente, std::string textureText, SDL_Color textColor, Uint32 size)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderUTF8_Blended_Wrapped(fuente, textureText.c_str(), textColor, size);

	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(GOD.gRenderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return mTexture != NULL;
}
void LTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
		mX = 0;
		mY = 0;
		SelIns = -1; 
	}
}
void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	if(SP()){setColor(77,166,255);}
	else setColor( 255, 255, 255);

	//Render to screen
	SDL_RenderCopyEx(GOD.gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
	//tactil stuff
	mX = x;mY = y;	SelIns = GOD.GenState;
}

void LTexture::render_T(int x, int y, std::string text, bool presed)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	
	if (presed ){setColor( 150, 150, 150);}
	else {setColor( 255, 255, 255);}
	
	if(SP()){setColor( 77,166,255);}
	
	//Render to screen
	SDL_RenderCopy(GOD.gRenderer, mTexture, NULL, &renderQuad);
	
	if (text.length()){
		SDL_Surface* textSurface = TTF_RenderText_Blended(GOD.gFont, text.c_str(), { 50, 50, 50 });
		if (textSurface == NULL)
		{
			printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
		}
		else
		{
			SDL_Rect TextRect = {x+mWidth+3, y+(mHeight/3), textSurface->w, textSurface->h};
			//Create texture from surface pixels
			SDL_Texture* TextureT = SDL_CreateTextureFromSurface(GOD.gRenderer, textSurface);
			//Render to screen
			SDL_RenderCopy(GOD.gRenderer, TextureT, NULL, &TextRect);
			SDL_DestroyTexture(TextureT);
		}
		SDL_FreeSurface(textSurface);
	}
	//tactil stuff
	mX = x;mY = y;	SelIns = GOD.GenState;
}

bool LTexture::render_AH(int x, int y, int w, int h, bool type)
{
		//tactil stuff
		mX = x;mY = y;	SelIns = GOD.GenState;
		static bool anend = false;
		static int delayp = 0;
		int sizeH = 0;
		int HP = h < 0 ? h *- 1 : h;
		if(type){
			if (delayp < HP &&!anend)
			{
				delayp+=30;
				sizeH = h < 0 ? delayp*-1 : delayp;
			} else {
				anend = true;
				sizeH = h;
				//delayp = h; 
			}
			{//SIDE
			SDL_SetRenderDrawColor(GOD.gRenderer, 0, 0, 0, 220);
			SDL_Rect HeaderRect = {x+w,y, 2, sizeH*-1};
			SDL_RenderFillRect(GOD.gRenderer, &HeaderRect);
			
			SDL_SetRenderDrawColor(GOD.gRenderer, 0, 0, 0, 220);
			HeaderRect = {x-2,y, 2, sizeH*-1};
			SDL_RenderFillRect(GOD.gRenderer, &HeaderRect);
			

			//TOP
			SDL_SetRenderDrawColor(GOD.gRenderer, 0, 0, 0, 220);
			HeaderRect = {x-2,y-2+sizeH*-1, w+2, 2};
			SDL_RenderFillRect(GOD.gRenderer, &HeaderRect);
			}
			
			
			SDL_SetRenderDrawColor(GOD.gRenderer, 50, 50, 50, 200);
			SDL_Rect HeaderRect = {x,y+sizeH*-1, w, sizeH};
			SDL_RenderFillRect(GOD.gRenderer, &HeaderRect);
			return anend;
		} else{
			if (anend){
				if (delayp <= 0)
				{
					delayp=0;
					anend = false;
				} else {
					delayp-=30;
					sizeH = h < 0 ? delayp*-1 : delayp;					

					{//SIDE
					SDL_SetRenderDrawColor(GOD.gRenderer, 0, 0, 0, 220);
					SDL_Rect HeaderRect = {x+w,y, 3, sizeH*-1};
					SDL_RenderFillRect(GOD.gRenderer, &HeaderRect);
					
					SDL_SetRenderDrawColor(GOD.gRenderer, 0, 0, 0, 220);
					HeaderRect = {x-2,y, 2, sizeH*-1};
					SDL_RenderFillRect(GOD.gRenderer, &HeaderRect);

					//TOP
					SDL_SetRenderDrawColor(GOD.gRenderer, 0, 0, 0, 220);
					HeaderRect = {x-2,y-2+sizeH*-1, w+2, 2};
					SDL_RenderFillRect(GOD.gRenderer, &HeaderRect);
					}

					SDL_SetRenderDrawColor(GOD.gRenderer, 50, 50, 50, 200);
					SDL_Rect HeaderRect = {x,y+sizeH*-1, w, sizeH};
					SDL_RenderFillRect(GOD.gRenderer, &HeaderRect);
				}
			}
		}

return false;
}

void LTexture::render_VOX(SDL_Rect Form ,int R, int G, int B, int A)
{
	//Set rendering space and render to screen
	SDL_SetRenderDrawColor(GOD.gRenderer, R, G, B, A);
	SDL_RenderFillRect(GOD.gRenderer, &Form);
	//tactil stuff
	mX = Form.x; mY = Form.y;	SelIns = GOD.GenState;
}

bool LTexture::SP()
{
	//return on negative touch
	if (GOD.TouchX <= 0||GOD.TouchY <= 0||mWidth <= 0||mHeight <= 0) return false;
	if (SelIns != GOD.GenState) return false;
	if (!mark) return false;
	
	//check if touched
	if(GOD.TouchX > mX-3 && GOD.TouchX < mX + mWidth +3 && GOD.TouchY > mY-3 && GOD.TouchY < mY + mHeight +3){
		printf("TouchX:%d  TouchY:%d\nB_X:%d  B_Y:%d\nB_W:%d  B_H:%d  \n",GOD.TouchX,GOD.TouchY,mX,mY,mWidth,mHeight);
		return true;
	}
return false;
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

int LTexture::getX()
{
	return mX;
}

int LTexture::getY()
{
	return mY;
}



