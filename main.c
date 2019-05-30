//Stephen Kelly-Bazan
#include <stdlib.h>
#include "mylib.h"
#include <debugging.h>
#include "title.h"
#include "pausescreen.h"
#include "playershipsprite.h"
#include "winscreen.h"
#include "livesimage.h"

//IMAGES MUST BE PUT HERE
extern const u16 title[38400];
extern const u16 pausescreen[9600];
extern const u16 playershipsprite[800];
extern const u16 gamefinish[230400];
extern const u16 winscreen[384000];
extern const u16 livesimage[864];

int main(void)
{
	REG_DISPCNT = MODE3 | BG2_ENABLE;// enable mode 3 and bg2



	// VARAIBLES TO USE
	u8 state =0;//begin title state 0 (used to determine state of game)

	u8 paused = 0;	

	u8 myrand = 0;

	u16 graystatic[1000];//max will be 20x20
	u16 i =0;
	u16 j =0;
	u16 k =0;
	u16 ani =0;
	//do this once
	for(i=0;i<(1000);i++)//make it divisible by 5
	{
		myrand = rand()%16+8;//makes it in mid 50% of gray
		graystatic[i]=GSC(myrand);
	}
	//object list arrays

	PLAYER player[1];
	player[0].score = 0;
	player[0].hp = 5;
	player[0].spd = 2;
	player[0].rotspd = 5;
	player[0].x = WIDTH/2-PLAYER_RADIUS;
	player[0].y = HEIGHT/2-PLAYER_RADIUS;
	player[0].prevx = WIDTH/2-PLAYER_RADIUS;
	player[0].prevy = HEIGHT/2-PLAYER_RADIUS;
	player[0].dead = 0;
	player[0].rot = 0;
	player[0].cool=0;

	ASTEROIDS asteroids[32];//max large asteroids is 5
	//ENEMY1 enemy1[20];
	BULLETS bullets[20];

	FOREVER
	{
		//game stuff here
		//waitForVblank();

		//update graystatic



		//GAME LOGIC
		//UPDATE KEYSTATES
		key_poll();
		//default logic (checked regardless of state)
		if( key_hit(BUTTON_SELECT) ){
			state=0;
		}
		//state logic
		//note for efficiency switch state 1 to top since it should be the most used
		if(state ==0){//reset state title screen
			//reset some vars
			paused =0;


			//fillers for unused atm
			//set up player stuff
			player[0].score = 0;
			player[0].hp = 5;
			player[0].spd = 1;
			player[0].rotspd = 5;
			player[0].x = WIDTH/2-PLAYER_RADIUS;
			player[0].y = HEIGHT/2-PLAYER_RADIUS;
			player[0].prevx = WIDTH/2-PLAYER_RADIUS;
			player[0].prevy = HEIGHT/2-PLAYER_RADIUS;
			player[0].dead = 0;
			player[0].rot = 0;

			//declare all the asteroids stuff
			for(i=0;i<32;i++){
				asteroids[i].dead=1;
			}
			//make first 5 not dead and large
			//notice that x and y position are scaled by 10 (so it's easier to make it move
			//slower than 1pixel at a time
			for(i=0;i<4;i++){//change to 4 later
				asteroids[i].dead=0;
				asteroids[i].size=20;
				asteroids[i].x=rand()%(240-20)*10;
				asteroids[i].y=rand()%(160-20)*10;
				if(						
						(player[0].x+10 	> asteroids[i].x/10)&& //if left player x is greater that asteroid left x
						(player[0].x 	< asteroids[i].x/10+asteroids[i].size)&&
						(player[0].y+10	> asteroids[i].y/10)&&
						(player[0].y	< asteroids[i].y/10+asteroids[i].size)
						)
				{
					asteroids[i].x=0;
					asteroids[i].y=0;
				}
				do{
					asteroids[i].xspd=(rand()%(8+1)-4);
				}
				while(asteroids[i].xspd==0);
				do{
					asteroids[i].yspd=((rand()+55)%(8+1)-4);
				}
				while(asteroids[i].yspd==0);
			}

			//bullet initialize
			for(i=0;i<20;i++){
				bullets[i].dead=1;
			}



			//clear screen to title
			drawImage3(0,0,240,160,title);
			state++;

		}else		
			if(state == 1)// title screen do nothing state
			{
				if( key_hit(BUTTON_START) || key_hit(BUTTON_A) ){
					state++;
					//clear screen to black
					drawClr(BLACK);
				}
				//set up a ton of stuff (reset)


				//DO STUFF DURING STATE 0 //liek draw the image
				//actually dont do anything here unless i want to update part of the screen
				//drawImage3(5,5,2,2,&graystatic[0]);

			}else
				if(state == 2)// game is running add tons of code here
				{
					//check other stuff with buttons to unpause etc.
					if( key_hit(BUTTON_START) ){
						paused=!paused;
						if (!paused)//not paused anymore
						{
							//cover/refresh paused screen with bg or black
							drawRect(60,40,120,80,BLACK);
						}
					}	

					//do stuff when not paused
					if(!paused){
						//drawImage3(15,15,2,2,&graystatic[0]);
						//drawAsteroid(55,30,20,&graystatic[0]);

						//game should be handled here



						//check if movement
						//record old cpord
						player[0].prevx=player[0].x;
						player[0].prevy=player[0].y;
						//player move
						if(KEY_DOWN_NOW(BUTTON_LEFT))
						{
							player[0].x-=player[0].spd;
						}
						if(KEY_DOWN_NOW(BUTTON_RIGHT))
						{
							player[0].x+=player[0].spd;
						}
						if(KEY_DOWN_NOW(BUTTON_UP))
						{
							player[0].y-=player[0].spd;
						}
						if(KEY_DOWN_NOW(BUTTON_DOWN))
						{
							player[0].y+=player[0].spd;
						}
						//player wrap
						if(player[0].x < 0)	{player[0].x+= WIDTH;	}
						if(player[0].x > WIDTH)	{player[0].x+=-WIDTH;	}
						if(player[0].y < -10)	{player[0].y+= HEIGHT+10;	}
						if(player[0].y > HEIGHT){player[0].y+=-HEIGHT-10;}

						// player wep cool down
						player[0].cool++;



						//player rotate
						if(KEY_DOWN_NOW(BUTTON_L))
						{
							player[0].rot+=player[0].rotspd;
						}
						if(KEY_DOWN_NOW(BUTTON_R))
						{
							if(player[0].rot!=0){
								player[0].rot-=player[0].rotspd;
							}else{
								player[0].rot+=360-player[0].rotspd;

							}
						}
						//player fireeeeee
						if(KEY_DOWN_NOW(BUTTON_A)&&player[0].cool>8)
						{
							i=0;
							player[0].cool=0;
							while( (i<20)&&(bullets[i].dead==0) ){i++;}//loop till it finds bullet that is dead or reaches max
							if(i!=20){//as long as i isn't at maxed (meaning we have space for a new bullet
								bullets[i].dead=0;
								//if((player[0].rot/45)

								//do stuff depending on player sprite
								if((player[0].rot/45)%8==0)//if right
								{
									bullets[i].x=player[0].x+5   	+5;
									bullets[i].y=player[0].y 		+5;
									bullets[i].xspd=2;
									bullets[i].yspd=0;
								}else
									if((player[0].rot/45)%8==1){
										bullets[i].x=player[0].x+5 	+5;
										bullets[i].y=player[0].y-5 	+5;
										bullets[i].xspd=2;
										bullets[i].yspd=-2;
									}else
										if((player[0].rot/45)%8==2){
											bullets[i].x=player[0].x   		+5;
											bullets[i].y=player[0].y-5 		+5;
											bullets[i].xspd=0;
											bullets[i].yspd=-2;
										}else
											if((player[0].rot/45)%8==3){
												bullets[i].x=player[0].x-5 	+5;
												bullets[i].y=player[0].y-5 	+5;
												bullets[i].xspd=-2;
												bullets[i].yspd=-2;
											}else
												if((player[0].rot/45)%8==4){
													bullets[i].x=player[0].x-5 		+5;
													bullets[i].y=player[0].y 		+5;
													bullets[i].xspd=-2;
													bullets[i].yspd=0;
												}else
													if((player[0].rot/45)%8==5){
														bullets[i].x=player[0].x-5 	+5;
														bullets[i].y=player[0].y+5 	+5;
														bullets[i].xspd=-2;
														bullets[i].yspd=2;
													}else
														if((player[0].rot/45)%8==6){
															bullets[i].x=player[0].x   		+5;
															bullets[i].y=player[0].y+5 		+5;
															bullets[i].xspd=0;
															bullets[i].yspd=2;
														}else
															if((player[0].rot/45)%8==7){
																bullets[i].x=player[0].x+5 	+5;
																bullets[i].y=player[0].y+5 	+5;
																bullets[i].xspd=2;
																bullets[i].yspd=2;
															}


							}
						}




					
						//asteroid move and wrap
						for(i=0;i<32;i++){
							
							if(asteroids[i].dead==0){
								//asmove
								asteroids[i].prevx=asteroids[i].x;
								asteroids[i].prevy=asteroids[i].y;


								asteroids[i].x+=asteroids[i].xspd;
								asteroids[i].y+=asteroids[i].yspd;
								
								//aswrap
								if(asteroids[i].x < 0)	{asteroids[i].x+= WIDTH*10;	}
								if(asteroids[i].x > WIDTH*10 )	{asteroids[i].x+=-WIDTH*10;	}
								if(asteroids[i].y < -asteroids[i].size*10	  )	{asteroids[i].y+= HEIGHT*10+asteroids[i].size*10;}
								if(asteroids[i].y > HEIGHT*10)	{asteroids[i].y+=-HEIGHT*10-asteroids[i].size*10;}
							
								//coll with asteroid (player)
								if(
										(player[0].x+10 -2	> 	asteroids[i].x/10)&& //if left player x is greater that asteroid left x
										(player[0].x +2 	< 		asteroids[i].x/10+asteroids[i].size)&&
										(player[0].y+10 -2	> 		asteroids[i].y/10)&&
										(player[0].y +2	< 		asteroids[i].y/10+asteroids[i].size)

										)	{

									//die
									state++;
								}
							
							}
						}

						//bullet move

						for(i=0;i<20;i++){
							if(bullets[i].dead==0){//if not dead
								bullets[i].prevx=bullets[i].x;
								bullets[i].prevy=bullets[i].y;


								bullets[i].x+=bullets[i].xspd;
								bullets[i].y+=bullets[i].yspd;
							}
						}

						//perform steps




						//clear behind objects (or update with bg)
						drawRect(player[0].prevx,player[0].prevy,10,10,BLACK);
						//clear asteroidz
						for(i=0;i<32;i++){
							if(asteroids[i].dead==0){

								drawRect(asteroids[i].prevx/10,asteroids[i].prevy/10,asteroids[i].size,asteroids[i].size,BLACK);
							}
						}

						//clear bullets and check bound
						for(i=0;i<20;i++){
							if(bullets[i].dead==0){
								//clear spot
								drawRect(bullets[i].prevx,bullets[i].prevy,2,2,BLACK);
								
								if(bullets[i].x < 0)						{bullets[i].dead=1;}
								if(bullets[i].x > WIDTH )					{bullets[i].dead=1;}
								if(bullets[i].y < -5	  )					{bullets[i].dead=1;}
								if(bullets[i].y > HEIGHT+5)					{bullets[i].dead=1;}
							}
						}
						
						
						//bullet asteroid collision check
						for(i=0;i<20;i++){
							if(bullets[i].dead==0){
								//so for each not dead bullet cycle through asteroids
								for(j=0;j<32;j++){
									if(asteroids[j].dead==0){
										if(
												(bullets[i].x+1 	> 	asteroids[j].x/10)&& //if left player x is greater that asteroid left x
												(bullets[i].x 		< 	asteroids[j].x/10+asteroids[j].size)&&
												(bullets[i].y+1		> 	asteroids[j].y/10)&&
												(bullets[i].y		< 	asteroids[j].y/10+asteroids[j].size)

												)	{


											//hit
											//so if hit then do stuff for the asteroids and bullets
											bullets[i].dead=1; //bullet should no longer be done
											//now handle asteroid
											//I made it so that the number on there (5) will make it impossible to get
											//past the array bound if it always splits into two
											//change original one to smaller
											//this is if it's not size 5
											if(asteroids[j].size!=5){
												asteroids[j].x+=(asteroids[j].size/4)*10;
												asteroids[j].y+=(asteroids[j].size/4)*10;
												asteroids[j].size/=2;//make it smaller
												//change direction
												asteroids[j].xspd=(rand()%(8+1)-4)*(20/asteroids[j].size);
												asteroids[j].yspd=((rand()+55)%(8+1)-4)*(20/asteroids[j].size);
												//
												//now create other split off asteroid (add later)

												k=0;
												while( (k<20)&&(asteroids[k].dead==0) ){k++;}//loop till it finds bullet that is dead or reaches max
												if(k!=20){//as long as i isn't at maxed (meaning we have space for a new bullet
													asteroids[k].dead=0;
													asteroids[k].size=asteroids[j].size;
													asteroids[k].x=asteroids[j].x;
													asteroids[k].y=asteroids[j].y;
													asteroids[k].xspd=(rand()%(8+1)-4)*(20/asteroids[k].size);
													asteroids[k].yspd=((rand()+55)%(8+1)-4)*(20/asteroids[k].size);
													//since hit no need to check this bullet on anything else
												}
											}

											else
											{
												asteroids[j].dead=1;
											}


											break;

										}
									}
								}


							}
						}

						//see if all asteroids died
						i=0;
						while( (i<32)&&(asteroids[i].dead==1) ){i++;}
						
						if(i==32){//means all the asteroids are dead
							state=5;
						}
						
						//draw all the objects

						//draw asteroids
						//draw rectangle in back
						for(i=0;i<32;i++){
							if(asteroids[i].dead==0){
								drawRectHollow(asteroids[i].x/10, asteroids[i].y/10, asteroids[i].size,asteroids[i].size,WHITE);
							}

						}
						//draw bullet
						for(i=0;i<20;i++){
							if(bullets[i].dead==0){
								drawRect(bullets[i].x, bullets[i].y, 2,2,WHITE);
							}

						}


						//draw fill static (this'll make a neat effect
						for(i=0;i<32;i++){
							if(asteroids[i].dead==0){								
								drawAsteroid(asteroids[i].x/10+1, asteroids[i].y/10+1, asteroids[i].size-2, &graystatic[0]);
							}

						}


						//temp draw player (as asteroid)
						//drawAsteroid(player[0].x,player[0].y,10,&graystatic[0]);
						drawAni(player[0].x,player[0].y,player[0].rot/45,8,10,10,playershipsprite,80,RED);


						
						//draw hud
						drawImage3(0,0,72,12,livesimage);
						
						
						
					}else{//do stuff while paused
						//drawImage3(25,25,2,2,&graystatic[0]);

						drawImage3(40,60,120,80,pausescreen);


					}
				}else
					if(state == 3)//end screen (if you win or lose) refresh
					{
						ani=0;
						//refresh screen with end screen and score etc.
						//drawClr(BLACK);
						state++;
					}
					else if(state == 4)
						//last state is 4
					{


						drawAniScreen(ani/3,6,gamefinish,1440);
						if(ani<15+1){
							ani++;}

						if( key_hit(BUTTON_START) || key_hit(BUTTON_A) ){
							state=0;
							//clear screen to black
							drawClr(BLACK);
						}
						//check for it to restart or make neat animations or somethin'
					}else//last state is 5 or winning state
					
					{
						drawAniScreen(ani/5,10,winscreen,2400);
						
							ani++;

						if( key_hit(BUTTON_START) || key_hit(BUTTON_A) ){
							state=0;
							//clear screen to black
							drawClr(BLACK);
						}
					}


		//DRAW STUFF HERE
		waitForVblank();//do this once before drawing everywhere else		

	}
}


