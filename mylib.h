#ifndef MY_LIB_H
#define MY_LIB_H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

typedef volatile unsigned char vu8;
typedef volatile unsigned short vu16;
typedef volatile unsigned int vu32;

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;

typedef volatile signed char vs8;
typedef volatile signed short vs16;
typedef volatile signed int vs32;

#define REG_DISPCNT *(u16*) 0x4000000
#define SCANLINECOUNTER *(vu16 *)0x4000006

#define BG2_ENABLE (1 << 10)
#define MODE3 3

#define RGB(r, g, b) ((r) | ((g) << 5) | ((b) << 10))
//gsc is grayscalecolor
#define GSC(gsc) ((gsc) | ((gsc) << 5) | ((gsc) << 10))
#define WIDTH 240
#define HEIGHT 160

/* Colours */

#define BLACK 	0
#define WHITE 	RGB(31,31,31)

#define RED 	RGB(31,0,0)
#define GREEN 	RGB(0,31,0)
#define BLUE 	RGB(0,0,31)
#define RGBRAND RGB(rand()%32,rand()%32,rand()%32)

/* Buttons */

#define BUTTON_A      (1<<0)
#define BUTTON_B      (1<<1)
#define BUTTON_SELECT (1<<2)
#define BUTTON_START  (1<<3)
#define BUTTON_RIGHT  (1<<4)
#define BUTTON_LEFT   (1<<5)
#define BUTTON_UP     (1<<6)
#define BUTTON_DOWN   (1<<7)
#define BUTTON_R      (1<<8)
#define BUTTON_L      (1<<9)

#define BUTTONS (*(vu16*)0x04000130)

#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)

extern u16 key_curr, key_prev;

#define KEY_MASK 0x03FF

//player constants
#define PLAYER_RADIUS 5

/* Ignore this for now LOL STRUCT*/
typedef struct
{
	const volatile void *src;
	volatile void *dst;
	volatile u32 cnt;
} DMAREC;

//player struct
typedef struct
{
	int score;
	int hp;
	int spd;
	int rotspd;
	int x;
	int y;
	int prevx;
	int prevy;
	int rot;
	int dead;
	int cool;
} PLAYER;
//enemy struct
//typedef struct
//{
//	int size;
//	int spd;
//	int hp;
//	int x;
//	int y;
//	int dir;
//	int dead;
//} ENEMY1;
//asteroid struct
typedef struct
{
	int size;
	int xspd;
	int yspd;
	int x;
	int y;
	int dir;
	int prevx;
	int prevy;
	int dead;
	int hit;
} ASTEROIDS;
//bullet struct
typedef struct
{
	int size;
	int spd;
	int xspd;
	int yspd;
	int x;
	int y;
	int prevx;
	int prevy;
	int dead;
} BULLETS;



#define DMA ((volatile DMAREC *)0x040000B0)

/* DMA channel 0 register definitions*/
#define REG_DMA0SAD         *(vu32*)0x40000B0  /* source address*/
#define REG_DMA0DAD         *(vu32*)0x40000B4  /* destination address*/
#define REG_DMA0CNT         *(vu32*)0x40000B8  /* control register*/

/* DMA channel 1 register definitions*/
#define REG_DMA1SAD         *(vu32*)0x40000BC  /* source address*/
#define REG_DMA1DAD         *(vu32*)0x40000C0  /* destination address*/
#define REG_DMA1CNT         *(vu32*)0x40000C4  /* control register*/

/* DMA channel 2 register definitions*/
#define REG_DMA2SAD         *(vu32*)0x40000C8  /* source address*/
#define REG_DMA2DAD         *(vu32*)0x40000CC  /* destination address*/
#define REG_DMA2CNT         *(vu32*)0x40000D0  /* control register*/

/* DMA channel 3 register definitions */
#define REG_DMA3SAD         *(vu32*)0x40000D4   /* source address*/
#define REG_DMA3DAD         *(vu32*)0x40000D8  /* destination address*/
#define REG_DMA3CNT         *(vu32*)0x40000DC  /* control register*/

/* Defines*/
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

/* Destination address flags */
#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

/* Source address flags */
#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

/* How much to copy flags */
#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

/* When to DMA flags */
#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)

#define DMA_ON (1 << 31)

#define START_ON_FIFO_EMPTY 0x30000000

//my defines
#define FOREVER while(1)

	
	

	extern u16* videoBuffer;

//IMAGES MUST BE PUT HERE
extern const u16 title[38400];
extern const u16 pausescreen[9600];
extern const u16 playershipsprite[800];
extern const u16 gamefinish[230400];
extern const u16 winscreen[384000];
extern const u16 livesimage[864];

//prototypes
void setPixel(int, int, u16);
void waitForVblank();
void drawImage3(int, int, int, int, const u16*);
void drawImage3Tile(int, int, int, int, const u16*,int);
void drawRect(int, int, int, int, u16);
void drawRectHollow(int, int, int, int, u16);
void drawClr(u16);

//player things
void resetPlayer();

//keystuff
void key_poll();
u32 key_curr_state();
u32 key_prev_state();
u32 key_is_down(u32);
u32 key_is_up(u32);
u32 key_was_down(u32);
u32 key_was_up(u32);
u32 key_transit(u32);
u32 key_held(u32);
u32 key_hit(u32);
u32 key_released(u32);

//asteroid stuff
void drawAsteroid(int, int, int, const u16*);
void drawAni(int dx, int dy,int ani, int maxani, int swidth, int sheight, const u16* image, int iwidth, u16 trans);
void drawAniScreen(int ani, int maxani, const u16* image, int iwidth);
int min(int a, int b);

#endif 
