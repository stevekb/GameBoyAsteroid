#include <stdlib.h>
#include "mylib.h"



u16* videoBuffer = (u16*) 0x6000000;



u16 key_curr=0, key_prev=0;

void setPixel(int x, int y, u16 color)
{
	videoBuffer[y * 240 + x] = color;
}

void waitForVblank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

//required drawImage3 function
void drawImage3(int r, int c, int width, int height, const u16* image)
{
	for(int i = 0; i<height; i++)
	{
		DMA[3].src = image+(i*width);
		DMA[3].dst = videoBuffer+(r*240+c)+i*240;
		DMA[3].cnt = width | DMA_ON | DMA_NOW | DMA_16 | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT;
	}
}


void drawRect(int x, int y, int width, int height, u16 color)
{

	setPixel(x,y,color);
	DMA[3].src = videoBuffer+x+y*240;
	for(int i = 0; i<height; i++)
	{
		DMA[3].dst = videoBuffer+(y*240+x)+i*240;
		DMA[3].cnt = width | DMA_ON | DMA_NOW | DMA_16 | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT;
	}
}

void drawRectHollow(int x, int y, int width, int height, u16 color)
{

	setPixel(x,y,color);
	DMA[3].src = videoBuffer+x+y*240;
	
	//top
	DMA[3].dst = videoBuffer+(y*240+x)+1;
	DMA[3].cnt = (width-1) | DMA_ON | DMA_NOW | DMA_16 | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT;
	
	//bot
	DMA[3].dst = videoBuffer+(y*240+x)+(height-1)*240;
	DMA[3].cnt = width | DMA_ON | DMA_NOW | DMA_16 | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT;
	
	for(int i = 1; i<height-1; i++)
	{
		DMA[3].dst = videoBuffer+(y*240+x)+i*240;
		DMA[3].cnt = 1 | DMA_ON | DMA_NOW | DMA_16 | DMA_SOURCE_FIXED | DMA_DESTINATION_FIXED;
		DMA[3].dst = videoBuffer+(y*240+x)+i*240+width-1;
		DMA[3].cnt = 1 | DMA_ON | DMA_NOW | DMA_16 | DMA_SOURCE_FIXED | DMA_DESTINATION_FIXED;
	}
}

void drawClr(u16 color)
{

	setPixel(0,0,color);
	DMA[3].src = videoBuffer;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = 240*160 | DMA_ON | DMA_NOW | DMA_16 | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT;
}


//key stuff
void key_poll()
{
	key_prev= key_curr;
	key_curr= ~BUTTONS & KEY_MASK;
}

//Basic state checks
u32 key_curr_state()         {   return key_curr;          }
u32 key_prev_state()         {   return key_prev;          }
u32 key_is_down(u32 key)     {   return  key_curr & key;   }
u32 key_is_up(u32 key)       {   return ~key_curr & key;   }
u32 key_was_down(u32 key)    {   return  key_prev & key;   }
u32 key_was_up(u32 key)      {   return ~key_prev & key;   }
//Transitional state checks
u32 key_transit(u32 key)	{   return ( key_curr ^  key_prev) & key;  }
u32 key_held(u32 key)		{   return ( key_curr &  key_prev) & key;  }
// key hit now but not before
u32 key_hit(u32 key)		{   return ( key_curr &~ key_prev) & key;  }
u32 key_released(u32 key)	{   return (~key_curr &  key_prev) & key;  }



//draw asteroid
void drawAsteroid(int x, int y, int size, const u16* image){
	image+=rand()%600;
	//int mysize = min(size,WIDTH-x);
	for(int i = 0; i<size; i++)
	{
		DMA[3].src = image+(i*size);
		DMA[3].dst = videoBuffer+(y*240+x)+i*240;
		DMA[3].cnt = size | DMA_ON | DMA_NOW | DMA_16 | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT;
	}
}

int min(int a, int b){
	if(a>b){return b;
	}else{return a;}
}


//player stuff

void drawAni(int dx, int dy,int ani, int maxani, int swidth, int sheight, const u16* image, int iwidth, u16 trans)
{
	//dx dyis where to draw on screen
	//ani is current animation position
	//max ani is maximum animation
	//swidth and sheight are the animation picture's width and height
	//image is image to use
	//iwidth is the image file's actual width
	//trans is the colour that represents transparant and that should be ignored

	int anix = (ani%maxani)%iwidth;
	int aniy = (ani%maxani)/iwidth;
	u16 col = 0;
	for(int i = 0; i<sheight; i++)
	{
		for(int j = 0; j<swidth; j++)
		{
			
				DMA[3].src = image+(i*iwidth+j)    +(anix*swidth) +(aniy*iwidth*sheight);//+160*240;
				col=*(u16*)(image+(i*iwidth+j)    +(anix*swidth) +(aniy*iwidth*sheight));
				if( col!=trans ){
				DMA[3].dst = videoBuffer+(dy*240+dx)+(i*240+j);//offset by i;
				DMA[3].cnt = 1 | DMA_ON | DMA_NOW | DMA_16 | DMA_SOURCE_FIXED | DMA_DESTINATION_FIXED;
				}
		}

	}
}

void drawAniScreen(int ani, int maxani, const u16* image, int iwidth)
{
	//dx dyis where to draw on screen
	//ani is current animation position
	//max ani is maximum animation
	//swidth and sheight are the animation picture's width and height
	//image is image to use
	//iwidth is the image file's actual width
	//trans is the colour that represents transparant and that should be ignored

	int anix = (ani%maxani)%WIDTH;
	int aniy = (ani%maxani)/WIDTH;
	for(int i = 0; i<HEIGHT; i++)
	{
					
				DMA[3].src = image+(i*iwidth)    +(anix*WIDTH) +(aniy*WIDTH*HEIGHT);//+160*240;
				
				DMA[3].dst = videoBuffer+(i*240);//offset by i;
				DMA[3].cnt = WIDTH | DMA_ON | DMA_NOW | DMA_16 | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT;
		

	}
}
