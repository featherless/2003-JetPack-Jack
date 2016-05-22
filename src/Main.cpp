/*
 Copyright 2003, 2016 Jeff Verkoeyen. All Rights Reserved.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

/*******************************************************\
*														*
*	JetPack Jack v0.8a									*
*		Team:											*
*			Programmer:	Jeff Verkoeyen					*
*			Artist:		Josh Rodasti					*
*		Info:											*
*			JetPack Jack is a platform style			*
*			game made with OpenGL						*
*			Every picture in the game is				*
*			hand drawn, with pen and paper,				*
*			then it is scanned in to the				*
*			computer and put in to TGA format			*
*		Version:										*
*			Version 0.8:								*
*				Released:								*
*					Sunday, January 12, 2003			*
*				New things added:						*
*					JetPack Jack has made a great leap	*
*					from .7 to .8, first off, there is	*
*					a title screen now, with sounds,	*
*					pictures and also many other things	*
*					Also created our first permanent	*
*					level (we might change it, but very	*
*					unlikely), some new graphix here	*
*					and there, and the game is just		*
*					plain better!						*
*				Estimated Progress:						*
*					53%									*
*				Estimated Release Date:					*
*					Early March of 2003					*
*			Version 0.7:								*
*				Released:								*
*					Thursday, January 09, 2003			*
*				New things added:						*
*					Added new enemies, the Hoppers, it	*
*					took a lot of work, but I finally	*
*					got their engine down and working	*
*					they run decent enough, in the		*
*					future I might make their jumps		*
*					curved instead of straight.  Also	*
*					added a new stat bar, INIT.DAT file	*
*					and many other misc. things.		*
*				Estimated Progress:						*
*					31%									*
*				Estimated Release Date:					*
*					March of 2003						*
*			Version 0.6:								*
*				Released:								*
*					Thursday, December 17, 2002			*
*				New things added:						*
*					Added a better AI to the Ponos		*
*					they now have what I call Insane	*
*					mode, where, if you get close to	*
*					one of them, they will start		*
*					you, but leave their designated		*
*					area.  Also added the ability to	*
*					kill the pono's, a life meter, and	*
*					also collision detection with the	*
*					bullets and boxes, so you can't		*
*					just shoot through boxes.			*
*				Estimated Progress:						*
*					22%									*
*				Estimated Release Date:					*
*					February of 2003					*
*			Version 0.5e:								*
*				Released:								*
*					Thursday, December 17, 2002			*
*				New things added:						*
*					Just added some commentary,			*
*					cleaned up the code a bit,			*
*					too, so it theoretically runs		*
*					about 1% faster (hey, every bit		*
*					counts!)							*
*				Estimated Progress:						*
*					21%									*
*				Estimated Release Date:					*
*					February of 2003					*
*		Contact:										*
*			Jeff Verkoeyen:								*
*				ragingflame@msn.com						*
*				ragingflame.150m.com					*
*														*
*	JetPack Jack is completely programmed by			*
*	Jeff Verkoeyen, using only some code from			*
*	NeHe.gamedev.net to make the window/load the		*
*	TGA's, etc.											*
*	You are NOT permitted to use this code in your		*
*	own program, without consulting me first.			*
*														*
*			©2003 A production of IVGDA					*
*														*
\*******************************************************/

#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <string.h>
#include "CFmod.h"
#include "resource.h"
#include "variables.h"
#include "base.h"
#include "enemies.h"
#include "movement.h"
#include "weapons.h"
#include "saving.h"

weapon w1;
movement m1;
enemies e1;
baseset b1;
saving s1;

bool intro=true;

void initialize();
int DrawGLScene(GLvoid);

float titlesize=0.0f,titlevel=0.003f;
int titlestage=0,substage;
int textcount=0;
int ponoy=0;
int ponox,jpjy;
int shotx, ms,bounce=-1;

void runintro()
{
	char temp[60];

	int a;
	switch(titlestage)
	{
	case 0:
		songintro.Init("voice.mp3");
		songintro.PlaySong();
		titlestage++;
	break;
	case 1:
		if(textcount<255)
			textcount++;
		
		glDisable(GL_TEXTURE_2D);
		
		glLoadIdentity();
		glTranslated(0,0,8);

		glClearColor((float)textcount/255,(float)textcount/255,(float)textcount/255,0.5f);
		glColor3f(255-(float)textcount/255,255-(float)textcount/255,255-(float)textcount/255);
				
		sprintf(temp, "First, there was nothing");
		glRasterPos2d(490-(strlen(temp)*10)/2,374);
		b1.glPrint(temp);

		a=songintro.GetPosition();

		if(a>=235000)
		{
			glClearColor(1.0f,1.0f,1.0f,0.5f);
			titlestage++;
		}

		glEnable(GL_TEXTURE_2D);

	break;
	case 2:
		glBindTexture(GL_TEXTURE_2D, textures[PONO_ANI_1].texID);
		
		glLoadIdentity();
		glTranslated(512,ponoy,8);
		if(ponoy<500)
			ponoy+=5;
		else
			titlestage++;

		glRotatef(730-(float)ponoy+50,0.0f,0.0f,1.0f);

		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.99f); glVertex3d(-64,-64, 0);
			glTexCoord2f(0.99f,0.99f); glVertex3d( 64,-64, 0);
			glTexCoord2f(0.99f,0.0f); glVertex3d( 64, 64, 0);
			glTexCoord2f(0.0f,0.0f); glVertex3d(-64, 64, 0);
		glEnd();
	break;
	case 3:
		glBindTexture(GL_TEXTURE_2D, textures[PONO_ANI_1].texID);
		
		glLoadIdentity();
		glTranslated(1000-ponoy,500,8);
		if(ponoy<780)
			ponoy+=5;
		else
		{
			titlestage++;
			substage=0;
			jpjy=950;
			ponox=1000-ponoy;
		}

		glRotatef(730-(float)ponoy+50,0.0f,0.0f,1.0f);

		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.99f); glVertex3d(-64,-64, 0);
			glTexCoord2f(0.99f,0.99f); glVertex3d( 64,-64, 0);
			glTexCoord2f(0.99f,0.0f); glVertex3d( 64, 64, 0);
			glTexCoord2f(0.0f,0.0f); glVertex3d(-64, 64, 0);
		glEnd();
	break;
	case 4:
		switch(substage)
		{
		case 0:
			if(jpjy>460)
				jpjy-=3;
			else
			{
				substage++;
				shot.LoadSong("shot.mp3");
				shot.PlaySong();
				shotx=850;
				ponoy=0;
			}
			glBindTexture(GL_TEXTURE_2D, textures[PONO_ANI_1].texID);
			
			glLoadIdentity();
			glTranslated(ponox,500,8);
			
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f,0.99f); glVertex3d(-64,-64, 0);
				glTexCoord2f(0.99f,0.99f); glVertex3d( 64,-64, 0);
				glTexCoord2f(0.99f,0.0f); glVertex3d( 64, 64, 0);
				glTexCoord2f(0.0f,0.0f); glVertex3d(-64, 64, 0);
			glEnd();

			glBindTexture(GL_TEXTURE_2D, textures[JACK_JUMPING].texID);
			
			glLoadIdentity();
			glTranslated(850,jpjy,8);
			
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f,0.99f); glVertex3d( 100,-100, 0);
				glTexCoord2f(0.99f,0.99f); glVertex3d(-100,-100, 0);
				glTexCoord2f(0.99f,0.0f); glVertex3d(-100, 100, 0);
				glTexCoord2f(0.0f,0.0f); glVertex3d( 100, 100, 0);
			glEnd();
		break;
		case 1:

			if(shotx>430)
			{
				ponox+=2;
				shotx-=5;
				if(ponox%3==0)
					ponoy=1;
				else
					ponoy=0;
			}
			else
			{
				substage++;
				shot.LoadSong("ponohit.mp3");
				shot.PlaySong();
			}

			glBindTexture(GL_TEXTURE_2D, textures[14+(ponoy)].texID);
			
			glLoadIdentity();
			glTranslated(ponox,500,8);
			
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f,0.99f); glVertex3d( 64,-64, 0);
				glTexCoord2f(0.99f,0.99f); glVertex3d(-64,-64, 0);
				glTexCoord2f(0.99f,0.0f); glVertex3d(-64, 64, 0);
				glTexCoord2f(0.0f,0.0f); glVertex3d( 64, 64, 0);
			glEnd();

			glBindTexture(GL_TEXTURE_2D, textures[JACK].texID);
			
			glLoadIdentity();
			glTranslated(850,460,8);
			
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f,0.99f); glVertex3d( 100,-100, 0);
				glTexCoord2f(0.99f,0.99f); glVertex3d(-100,-100, 0);
				glTexCoord2f(0.99f,0.0f); glVertex3d(-100, 100, 0);
				glTexCoord2f(0.0f,0.0f); glVertex3d( 100, 100, 0);
			glEnd();

			glBindTexture(GL_TEXTURE_2D, textures[BULLET_1].texID);
			
			glLoadIdentity();
			glTranslated(shotx,460,8);
			
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f,0.99f); glVertex3d( 30,-15, 0);
				glTexCoord2f(0.99f,0.99f); glVertex3d(-30,-15, 0);
				glTexCoord2f(0.99f,0.0f); glVertex3d(-30, 15, 0);
				glTexCoord2f(0.0f,0.0f); glVertex3d( 30, 15, 0);
			glEnd();
		break;
		case 2:
			if(jpjy>-100)
				jpjy-=4;
			else
			{
				titlestage++;
				songintro.LoadSong("trill.mp3");
				substage=0;
			}

			if(jpjy>360)
			{
				glBindTexture(GL_TEXTURE_2D, textures[POW].texID);
				
				glLoadIdentity();
				glTranslated(ponox,500,8);
				
				glBegin(GL_QUADS);
					glTexCoord2f(0.0f,0.99f); glVertex3d(-64,-64, 0);
					glTexCoord2f(0.99f,0.99f); glVertex3d( 64,-64, 0);
					glTexCoord2f(0.99f,0.0f); glVertex3d( 64, 64, 0);
					glTexCoord2f(0.0f,0.0f); glVertex3d(-64, 64, 0);
				glEnd();
			}

			glBindTexture(GL_TEXTURE_2D, textures[JACK_JUMPING].texID);
			
			glLoadIdentity();
			glTranslated(850,jpjy,8);
			
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f,0.99f); glVertex3d( 100,-100, 0);
				glTexCoord2f(0.99f,0.99f); glVertex3d(-100,-100, 0);
				glTexCoord2f(0.99f,0.0f); glVertex3d(-100, 100, 0);
				glTexCoord2f(0.0f,0.0f); glVertex3d( 100, 100, 0);
			glEnd();
		break;

		}
	break;
	case 5:
		if(titlesize<1000)
		{
			titlesize+=titlevel;
			titlevel+=0.01f;
		}
		else
		{
			titlestage++;
			BGMusic.Init("theme.mp3");
			BGMusic.PlaySong();
			BGMusic.Repeat(2);
			shot.LoadSong("beep.mp3");
			ms=0;

			xpos=xpos2=xpos3=xpos1=0;
			character.xtile=0;
		}

		if(titlesize>=10 && titlesize<=13)
			songintro.PlaySong();
		
		glBindTexture(GL_TEXTURE_2D, textures[TITLE].texID);
		
		glLoadIdentity();									// Reset The Current Modelview Matrix
		
		glTranslated(512,384,2);
		
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,1.0f); glVertex3d(-titlesize,-titlesize*0.75f, 0);
			glTexCoord2f(1.0f,1.0f); glVertex3d( titlesize,-titlesize*0.75f, 0);
			glTexCoord2f(1.0f,0.0f); glVertex3d( titlesize, titlesize*0.75f, 0);
			glTexCoord2f(0.0f,0.0f); glVertex3d(-titlesize, titlesize*0.75f, 0);
		glEnd();
	break;
	case 6:

		m1.testmovement();
		DrawGLScene();

		xpos+=bounce*3;
		xpos1+=bounce*3;
		xpos2+=bounce*2;
		xpos3+=bounce;
		
		if(xpos1<-512)
		{
			character.xtile++;
			xpos1=512;
		}
		if(xpos1>512)
		{
			character.xtile--;
			xpos1=-512;
		}
		
		if(character.xtile<0)
			character.xtile=0;
		
		if(xpos3<-512)
			xpos3=512;
		if(xpos3>512)
			xpos3=-512;
		
		if(xpos2<-512)
			xpos2=512;
		if(xpos2>512)
			xpos2=-512;

		if(character.xtile>=11 && bounce==-1)
			bounce=1;

		if(character.xtile<0 && bounce==1)
			bounce=-1;

		glDisable(GL_TEXTURE_2D);

		glLoadIdentity();
		glTranslated(0,0,8);
		
		glColor3f(1.0,0.0f,0.0f);
		
		sprintf(temp, "JetPack Jack");
		glRasterPos2d(490-(strlen(temp)*10)/2,20);
		b1.glPrint(temp);

		sprintf(temp, "©2003 IVGDA");
		glRasterPos2d(490-(strlen(temp)*10)/2,700);
		b1.glPrint(temp);

		sprintf(temp, "JetPack Jack was created by Jeff Verkoeyen and Josh Rodasti");
		glRasterPos2d(490-(strlen(temp)*10)/2,730);
		b1.glPrint(temp);

		if(substage==0)
		{
			if(ms==0)
				glColor3f(0.0f,1.0f,0.0f);
			
			sprintf(temp, "Start Playing");
			glRasterPos2d(490-(strlen(temp)*10)/2,200);
			b1.glPrint(temp);
			
			glColor3f(1.0,0.0f,0.0f);
			
			if(ms==1)
				glColor3f(0.0f,1.0f,0.0f);
			
			sprintf(temp, "Options");
			glRasterPos2d(490-(strlen(temp)*10)/2,230);
			b1.glPrint(temp);
			
			glColor3f(1.0,0.0f,0.0f);
			
			if(ms==2)
				glColor3f(0.0f,1.0f,0.0f);
			
			sprintf(temp, "Mutagens");
			glRasterPos2d(490-(strlen(temp)*10)/2,260);
			b1.glPrint(temp);
			
			glColor3f(1.0,0.0f,0.0f);	
			
			if(ms==3)
				glColor3f(0.0f,1.0f,0.0f);
			
			sprintf(temp, "About");
			glRasterPos2d(490-(strlen(temp)*10)/2,290);
			b1.glPrint(temp);
			
			glColor3f(1.0,0.0f,0.0f);
			
			if(ms==4)
				glColor3f(0.0f,1.0f,0.0f);
			
			sprintf(temp, "Quit");
			glRasterPos2d(490-(strlen(temp)*10)/2,320);
			b1.glPrint(temp);
			
			glColor3f(1.0,0.0f,0.0f);
			
			if(keys[VK_UP])
			{
				ms--;
				keys[VK_UP]=false;
				shot.PlaySong();
			}
			if(keys[VK_DOWN])
			{
				ms++;
				keys[VK_DOWN]=false;
				shot.PlaySong();
			}
			if(ms<0)
				ms=4;
			if(ms>4)
				ms=0;
			
			if(keys[VK_RETURN] && ms==0)
			{
				intro=false;
				initialize();
				glColor3f(1.0f,1.0f,1.0f);
				glClearColor(1.0f,1.0f,1.0f,0.5f);
				xpos=xpos1=xpos2=xpos3=0;
				character.xtile=0;
			}

			if(keys[VK_RETURN] && ms==3)
			{
				substage=1;
				keys[VK_RETURN]=false;
			}
			
			if(keys[VK_RETURN] && ms==4)
				done=true;
			
		}

		if(substage==1)
		{
			glRasterPos2d(10,50);
			b1.glPrint("Jeff: Lead Programmer, has been programming C++ for 6 months, and OpenGL for 3");
			glRasterPos2d(10,80);
			b1.glPrint("months. He has made many games for the computer, in both DOS and OpenGL.  He");
			glRasterPos2d(10,110);
			b1.glPrint("also spent 8th through 9th grade making games on the calculator, and still");
			glRasterPos2d(10,140);
			b1.glPrint("does make an occasional math program.");


			glRasterPos2d(10,180);
			b1.glPrint("Josh: Lead Artist/Musician, enjoys spending all allocated time in his room on");

			glRasterPos2d(10,210);
			b1.glPrint("his computer either making music or playing games. He also frequently gets");

			glRasterPos2d(10,240);
			b1.glPrint("caught in class doodling on his assignments, (JetPack Jack's birth) and");

			glRasterPos2d(10,270);
			b1.glPrint("listening to his own cds.");


			glRasterPos2d(10,310);
			b1.glPrint("IVGDA: at www.ragingflame.150M.com, you can find links and special sneak previews");

			glRasterPos2d(10,340);
			b1.glPrint("to our new ideas, games, and etc. Check out the bios of the IVGDA team members,");

			glRasterPos2d(10,370);
			b1.glPrint("and download tutorials, games, demos, and also find career info.");

			if(keys[VK_RETURN] && ms==3)
			{
				substage=0;
				keys[VK_RETURN]=false;
			}
		}

		glEnable(GL_TEXTURE_2D);
	break;
	}
}

struct			 											// Create A Structure For The Timer Information
{
  __int64       frequency;									// Timer Frequency
  float         resolution;									// Timer Resolution
  unsigned long mm_timer_start;								// Multimedia Timer Start Value
  unsigned long mm_timer_elapsed;							// Multimedia Timer Elapsed Time
  bool			performance_timer;							// Using The Performance Timer?
  __int64       performance_timer_start;					// Performance Timer Start Value
  __int64       performance_timer_elapsed;					// Performance Timer Elapsed Time
} timer;

int		steps[6]={ 1, 2, 4, 5, 10, 20 };				// Stepping Values For Slow Video Adjustment

void menu();
float TimerGetTime();

void TimerInit(void)										// Initialize Our Timer (Get It Ready)
{
	memset(&timer, 0, sizeof(timer));						// Clear Our Timer Structure

	// Check To See If A Performance Counter Is Available
	// If One Is Available The Timer Frequency Will Be Updated
	if (!QueryPerformanceFrequency((LARGE_INTEGER *) &timer.frequency))
	{
		// No Performace Counter Available
		timer.performance_timer	= FALSE;					// Set Performance Timer To FALSE
		timer.mm_timer_start	= (unsigned long)TimerGetTime();			// Use timeGetTime() To Get Current Time
		timer.resolution		= 1.0f/1000.0f;				// Set Our Timer Resolution To .001f
		timer.frequency			= 1000;						// Set Our Timer Frequency To 1000
		timer.mm_timer_elapsed	= timer.mm_timer_start;		// Set The Elapsed Time To The Current Time
	}
	else
	{
		// Performance Counter Is Available, Use It Instead Of The Multimedia Timer
		// Get The Current Time And Store It In performance_timer_start
		QueryPerformanceCounter((LARGE_INTEGER *) &timer.performance_timer_start);
		timer.performance_timer			= TRUE;				// Set Performance Timer To TRUE
		// Calculate The Timer Resolution Using The Timer Frequency
		timer.resolution				= (float) (((double)1.0f)/((double)timer.frequency));
		// Set The Elapsed Time To The Current Time
		timer.performance_timer_elapsed	= timer.performance_timer_start;
	}
}

float TimerGetTime()										// Get Time In Milliseconds
{
	__int64 time;											// time Will Hold A 64 Bit Integer

	if (timer.performance_timer)							// Are We Using The Performance Timer?
	{
		QueryPerformanceCounter((LARGE_INTEGER *) &time);	// Grab The Current Performance Time
		// Return The Current Time Minus The Start Time Multiplied By The Resolution And 1000 (To Get MS)
		return ( (float) ( time - timer.performance_timer_start) * timer.resolution)*1000.0f;
	}
	else
	{
		// Return The Current Time Minus The Start Time Multiplied By The Resolution And 1000 (To Get MS)
		return( (float) ( TimerGetTime() - timer.mm_timer_start) * timer.resolution)*1000.0f;
	}
}

void initialize()
{

	for(int a=0;a<30;a++)
	{			//Set all the default status
		//ponos
		pono.image[a]=pono.count[a]=pono.hurtcounter[a]=pono.comeback[a]=0;
		pono.face[a]=1;
		pono.insane[a]=FALSE;
		pono.on[a]=FALSE;
		pono.life[a]=3;

		//hoppers
		hopper.face[a]=1;
		hopper.count[a]=hopper.hurtcounter[a]=pono.comeback[a]=0;
		hopper.on[a]=FALSE;
		hopper.life[a]=4;

		//jumping ponos
		ponojump.count[a]=ponojump.comeback[a]=ponojump.hurtcounter[a]=0;
		ponojump.life[a]=3;
		ponojump.on[a]=FALSE;
	}

	hopper.xpos[0]=5*128;
	hopper.ypos[0]=2*128;
	hopper.peak[0]=-128;
	hopper.jumpstart[0]=(int)hopper.xpos[0];
	hopper.jumpend[0]=7*128;
	hopper.xvel[0]=(float)(hopper.jumpend[0]-hopper.jumpstart[0])/50;
	hopper.yvel[0]=(float)(hopper.ypos[0]-hopper.peak[0])/50;
	hopper.on[0]=true;

	hopper.xpos[1]=18*128;
	hopper.ypos[1]=2*128;
	hopper.peak[1]=-128;
	hopper.jumpstart[1]=(int)hopper.xpos[1];
	hopper.jumpend[1]=21*128;
	hopper.xvel[1]=(float)(hopper.jumpend[1]-hopper.jumpstart[1])/50;
	hopper.yvel[1]=(float)(hopper.ypos[1]-hopper.peak[1])/50;
	hopper.on[1]=true;

	pono.xpos[0]=14*128;		//set up pono 1
	pono.ypos[0]=4*128;
	pono.on[0]=TRUE;
	pono.maxleft[0]=13;
	pono.maxright[0]=16;

	ponojump.xpos[0]=34*128;
	ponojump.ypos[0]=3*128;
	ponojump.peak[0]=32;
	ponojump.vel[0]=(float)(ponojump.ypos[0]-ponojump.peak[0])/50;
	ponojump.on[0]=true;

	ponojump.xpos[1]=36*128;
	ponojump.ypos[1]=3*128;
	ponojump.peak[1]=32;
	ponojump.vel[1]=(float)(ponojump.ypos[1]-ponojump.peak[1])/50;
	ponojump.on[1]=true;

	character.look=1;		//Set up the character facing right
	character.ypos=0;	//Start him a bit above the screen
	character.xtile=0;		//First tile
	character.currentgun=0;	//Default gun
	character.maxshots=1;	//Default max shots
	character.life=100;		//Character's max life
	character.hits=0;
	character.total=0;
	character.termvel=12;
	character.grav=2;
	character.xvel=character.yvel=0;

	BGMusic.Init("JetPack Jack.mp3");
	shot.LoadSong("shot.mp3");
	BGMusic.PlaySong();
	BGMusic.Repeat(FSOUND_CD_PLAYLOOPED);
	
}

void checkstuff()
{

	if(xpos1<-512)			//when the ground loops, add or subtract from the character's tile
	{
		character.xtile++;
		xpos1=512;
	}
	if(xpos1>512)
	{
		character.xtile--;
		xpos1=-512;
	}

	if(character.xtile<0)
		character.xtile=0;

	if(xpos3<-512)
		xpos3=512;
	if(xpos3>512)
		xpos3=-512;

	if(xpos2<-512)
		xpos2=512;
	if(xpos2>512)
		xpos2=-512;


	//		CHARACTER START

	glBindTexture(GL_TEXTURE_2D, textures[JACK+keys[VK_UP]].texID);	//maps the normal texture and adds one if you are pressing up,
					//changing the current pic to the jumping one

	glLoadIdentity();									// Reset The Current Modelview Matrix

	glTranslated(512,character.ypos,8);

	if(keys[VK_UP] && (keys[VK_LEFT] || keys[VK_RIGHT]))
		glRotatef((float)character.look*20,0.0f,0.0f,1.0f);		//Rotate the character 20 degrees
													//if he is moving left or right, while jet-
													//packing, looks a bit more realistic
	if(character.hurtcounter==0 || character.hurtcounter%2==0)
	{
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.99f); glVertex3d(-character.look*100,-100, 0);
			glTexCoord2f(0.99f,0.99f); glVertex3d( character.look*100,-100, 0);
			glTexCoord2f(0.99f,0.0f); glVertex3d( character.look*100, 100, 0);
			glTexCoord2f(0.0f,0.0f); glVertex3d(-character.look*100, 100, 0);
		glEnd();
	}

	//		CHARACTER END

}

int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{

	/*

	**********************************************************
	
	  table of who gets what in JetPack Jack
	  this just maps out which objects are viewed first, etc.

	  CHARACTER			8
	  AMMO METER		7
	  FOREGROUND		6
	  MIDDLEGROUND		4
	  BACKGROUND		2
	  SUN				1

	**********************************************************

	*/

	//		SUN

	glBindTexture(GL_TEXTURE_2D, textures[SUN].texID);

	glLoadIdentity();
		
	glTranslated(900,128,1);

	glRotatef(sunrot,0.0f,0.0f,1.0f);

	sunrot+=0.2f;
		
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,1.0f); glVertex3d(-64,-64, 0);
		glTexCoord2f(1.0f,1.0f); glVertex3d( 64,-64, 0);
		glTexCoord2f(1.0f,0.0f); glVertex3d( 64, 64, 0);
		glTexCoord2f(0.0f,0.0f); glVertex3d(-64, 64, 0);
	glEnd();

	//		END SUN


	//		ALL GROUNDS START

	//		BACKGROUND

	glBindTexture(GL_TEXTURE_2D, textures[SKY].texID);

	for(int a=(int)xpos3-2048;a<xpos3+2048;a+=1024)
	{
		
		glLoadIdentity();
		
		glTranslated(a,128,2);
		
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,1.0f); glVertex3d(-512,-128, 0);
			glTexCoord2f(1.0f,1.0f); glVertex3d( 512,-128, 0);
			glTexCoord2f(1.0f,0.0f); glVertex3d( 512, 128, 0);
			glTexCoord2f(0.0f,0.0f); glVertex3d(-512, 128, 0);
		glEnd();
		
	}


	//		MIDDLEGROUND

	glBindTexture(GL_TEXTURE_2D, textures[MOUNTAINS].texID);

	for(int a=(int)xpos2-2048;a<xpos2+2048;a+=1024)
	{
		
		glLoadIdentity();
		
		glTranslated(a,475,4);
		
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,1.0f); glVertex3d(-512,-200, 0);
			glTexCoord2f(1.0f,1.0f); glVertex3d( 512,-200, 0);
			glTexCoord2f(1.0f,0.0f); glVertex3d( 512, 200, 0);
			glTexCoord2f(0.0f,0.0f); glVertex3d(-512, 200, 0);
		glEnd();
		
	}


	//		FOREGROUND

	for(int a=(int)xpos1-2048;a<xpos1+2048;a+=1024)
	{

		glBindTexture(GL_TEXTURE_2D, textures[GROUND].texID);

		glLoadIdentity();
		
		glTranslated(a,704,6);
		
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,1.0f); glVertex3d(-512,-64, 0);
			glTexCoord2f(1.0f,1.0f); glVertex3d( 512,-64, 0);
			glTexCoord2f(1.0f,0.0f); glVertex3d( 512, 64, 0);
			glTexCoord2f(0.0f,0.0f); glVertex3d(-512, 64, 0);
		glEnd();
		
	}

	//		ALL GROUNDS END


	//		AMMO METER

	glBindTexture(GL_TEXTURE_2D, textures[character.currentgun+7].texID);

	glLoadIdentity();
	
	glTranslated(10,30,7);

	for(int a=0;a<character.currentshots;a++)
	{
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,1.0f); glVertex3d(0,-15, 0);
			glTexCoord2f((100-(float)weapons.regen[a])/100,1.0f); glVertex3d( ((100-(float)weapons.regen[a])/100)*40,-15, 0);
			glTexCoord2f((100-(float)weapons.regen[a])/100,0.0f); glVertex3d( ((100-(float)weapons.regen[a])/100)*40, 15, 0);
			glTexCoord2f(0.0f,0.0f); glVertex3d(0, 15, 0);
		glEnd();

		glTranslated(40,0,0);
		
	}

	//		END AMMO METER


	//		LIFE METER

	glDisable(GL_TEXTURE_2D);

	glLoadIdentity();
	
	glTranslated(10,60,7);

	glBegin(GL_QUADS);
		glVertex3d( 1,  -15, 0);
		glVertex3d( 101,-15, 0);
		glVertex3d( 101, 15, 0);
		glVertex3d( 1,   15, 0);
	glEnd();

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, textures[BAR].texID);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,1.0f); glVertex3d( 1,					-15, 1);
		glTexCoord2f(1.0f,1.0f); glVertex3d( character.life+1,	-15, 1);
		glTexCoord2f(1.0f,0.0f); glVertex3d( character.life+1,	 15, 1);
		glTexCoord2f(0.0f,0.0f); glVertex3d( 1,					 15, 1);
	glEnd();

	glPolygonMode(GL_BACK,GL_LINE);
	glPolygonMode(GL_FRONT,GL_LINE);

	glBegin(GL_QUADS);
		glColor3f(0.0f,0.0f,0.0f);
		glVertex3d( 0,  -16, 0);
		glVertex3d( 102,-16, 0);
		glVertex3d( 102, 16, 0);
		glVertex3d( 0,   16, 0);
	glEnd();

	glPolygonMode(GL_BACK,GL_FILL);
	glPolygonMode(GL_FRONT,GL_FILL);

	glColor3f(1.0f,1.0f,1.0f);

/*
	glDisable(GL_TEXTURE_2D);

	glLoadIdentity();
	glTranslated(0,0,8);
	glColor3f(1.0,0.0f,0.0f);
	glRasterPos2d(0,30);
	b1.glPrint("rightpress %d",rightpress);
	glColor3f(1.0,1.0f,1.0f);

	glEnable(GL_TEXTURE_2D);
*/
	return TRUE;										// Keep Going
}

void drawmenu()
{
	
	glDisable(GL_TEXTURE_2D);
	
	glLoadIdentity();
	glTranslated(0,0,8);
	
	glColor3f(1.0,0.0f,0.0f);

	char temp[50];

	sprintf(temp, "JetPack Jack");
	glRasterPos2d(490-(strlen(temp)*10)/2,100);
	b1.glPrint(temp);

	sprintf(temp, "By Jeff Verkoeyen and Josh Rodasti");
	glRasterPos2d(490-(strlen(temp)*10)/2,550);
	b1.glPrint(temp);

	sprintf(temp, "A Production of IVGDA");
	glRasterPos2d(490-(strlen(temp)*10)/2,590);
	b1.glPrint(temp);

	sprintf(temp, "Is the game running too slow or too fast?");
	glRasterPos2d(490-(strlen(temp)*10)/2,40);
	b1.glPrint(temp);

	sprintf(temp, "Remember to check out the INIT.DAT file to change the game settings");
	glRasterPos2d(490-(strlen(temp)*10)/2,63);
	b1.glPrint(temp);

	glRasterPos2d(10,35);
	b1.glPrint(winname);
	
	if(keys[VK_RETURN] && !rpress)
		rpress=rvar=TRUE;
	if(!keys[VK_RETURN])
		rpress=FALSE;

	if(keys[VK_UP] || keys[VK_DOWN] || rvar)
		shot.PlaySong();
	
	if(menuset==0)
	{
		
		if(keys[VK_UP])
		{
			menuselection--;
			keys[VK_UP]=FALSE;
		}
		if(keys[VK_DOWN])
		{
			menuselection++;
			keys[VK_DOWN]=FALSE;
		}

		if(menuselection==0)
			menuselection=3;

		if(menuselection==4)
			menuselection=1;
		
		glColor3f(0.0,0.0f,1.0f);

		if(menuselection==1)
		{
			glColor3f(0.0,1.0f,0.0f);
			if(rpress && rvar)
			{
				menuset=1;
				rvar=FALSE;
			}
		}
		
		sprintf(temp, "Stats");
		glRasterPos2d(490-(strlen(temp)*10)/2,200);
		b1.glPrint(temp);

		glColor3f(0.0,0.0f,1.0f);

		if(menuselection==2)
		{
			glColor3f(0.0,1.0f,0.0f);
			if(rpress && rvar)
			{
				menuset=2;
				rvar=FALSE;
				menuselection=1;
			}
		}
		
		sprintf(temp, "Options");
		glRasterPos2d(490-(strlen(temp)*10)/2,250);
		b1.glPrint(temp);

		glColor3f(0.0f,0.0f,1.0f);

		if(menuselection==3)
		{
			glColor3f(0.0,1.0f,0.0f);
			if(rpress && rvar)
				done=TRUE;
		}

		sprintf(temp, "Quit");
		glRasterPos2d(490-(strlen(temp)*10)/2,500);
		b1.glPrint(temp);

	}
	else if(menuset==1)
	{

		int accuracy;

		if(character.total!=0)
			accuracy=(int)(float(character.hits)/float(character.total)*100);
		else
			accuracy=0;

		glColor3f(0.0f,0.0f,1.0f);

		sprintf(temp, "Accuracy %d",accuracy);
		strcat(temp,"%%");
		glRasterPos2d(490-(strlen(temp)*10)/2,200);
		b1.glPrint(temp);

		sprintf(temp, "Health %d/100",character.life);
		glRasterPos2d(490-(strlen(temp)*10)/2,250);
		b1.glPrint(temp);

		sprintf(temp, "Max Ammo %d",character.maxshots);
		glRasterPos2d(477-(strlen(temp)*10)/2,300);
		b1.glPrint(temp);
		
		glColor3f(0.0,1.0f,0.0f);

		if(rpress && rvar)
		{
			menuset=0;
			rvar=FALSE;
		}

		sprintf(temp, "Back");
		glRasterPos2d(490-(strlen(temp)*10)/2,500);
		b1.glPrint(temp);
		
	}
	else if(menuset==2)
	{
		
		if(keys[VK_UP])
		{
			menuselection--;
			keys[VK_UP]=FALSE;
			if(res!=oldres || fullres!=oldfullres)
			{
				if(res!=oldres)
					oldres=res;
				if(fullres!=oldfullres)
					oldfullres=fullres;
				b1.KillGLWindow();
				b1.CreateGLWindow(winname,resset[0] [res],resset[1] [res],resset[2] [res],(bool)!fullres);
			}
		}
		if(keys[VK_DOWN])
		{
			menuselection++;
			keys[VK_DOWN]=FALSE;
			if(res!=oldres || fullres!=oldfullres)
			{
				if(res!=oldres)
					oldres=res;
				if(fullres!=oldfullres)
					oldfullres=fullres;
				b1.KillGLWindow();
				b1.CreateGLWindow(winname,resset[0] [res],resset[1] [res],resset[2] [res],(bool)!fullres);
			}
		}

		if(menuselection==0)
			menuselection=3;

		if(menuselection==4)
			menuselection=1;
		
		glColor3f(0.0,0.0f,1.0f);

		if(menuselection==1)
		{
			glColor3f(0.0,1.0f,0.0f);
			if(rpress && rvar && res!=oldres)
			{
				rvar=FALSE;
				oldres=res;
				b1.KillGLWindow();
				b1.CreateGLWindow(winname,resset[0] [res],resset[1] [res],resset[2] [res],(bool)!fullres);
			}
			if(keys[VK_RIGHT] && res<5)
			{
				shot.PlaySong();
				res++;
				keys[VK_RIGHT]=FALSE;
			}
			if(keys[VK_LEFT] && res>0)
			{
				shot.PlaySong();
				res--;
				keys[VK_LEFT]=FALSE;
			}
		}
		
		sprintf(temp, "Resolution: %dx%d %d",resset[0] [res],resset[1] [res],resset[2] [res]);
		glRasterPos2d(490-(strlen(temp)*10)/2,200);
		b1.glPrint(temp);
		
		glColor3f(0.0,0.0f,1.0f);

		if(menuselection==2)
		{
			glColor3f(0.0,1.0f,0.0f);
			if(rpress && rvar && fullres!=oldfullres)
			{
				rvar=FALSE;
				oldfullres=fullres;
				b1.KillGLWindow();
				b1.CreateGLWindow(winname,resset[0] [res],resset[1] [res],resset[2] [res],(bool)!fullres);
			}
			if(keys[VK_RIGHT] && fullres<1)
			{
				shot.PlaySong();
				fullres++;
				keys[VK_RIGHT]=FALSE;
			}
			if(keys[VK_LEFT] && fullres>0)
			{
				shot.PlaySong();
				fullres--;
				keys[VK_LEFT]=FALSE;
			}
		}
		
		sprintf(temp, "%s",full[fullres]);
		glRasterPos2d(490-(strlen(temp)*10)/2,250);
		b1.glPrint(temp);

		glColor3f(0.0f,0.0f,1.0f);

		if(menuselection==3)
		{
			glColor3f(0.0,1.0f,0.0f);
			if(rpress && rvar)
			{
				menuset=0;
				rvar=FALSE;
				res=oldres;
				fullres=oldfullres;
				menuselection=1;
			}
		}

		sprintf(temp, "Back");
		glRasterPos2d(490-(strlen(temp)*10)/2,500);
		b1.glPrint(temp);

	}

//	glPolygonMode(GL_BACK,GL_LINE);
//	glPolygonMode(GL_FRONT,GL_LINE);

	glColor3f(1.0,1.0f,1.0f);

	glEnable(GL_TEXTURE_2D);

}

LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_LBUTTONDOWN:
		{
			gleft=TRUE;
			return 0;
		}

		case WM_LBUTTONUP:
		{
			gleft=FALSE;
			return 0;
		}

		case WM_RBUTTONDOWN:
		{
			gright=TRUE;
			return 0;
		}

		case WM_RBUTTONUP:
		{
			gright=FALSE;
			return 0;
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			b1.ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}

		case WM_MOUSEMOVE:
		{
			mousex=LOWORD(lParam);
			mousey=HIWORD(lParam);
			return 0;
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure

	s1.readInitFile();

	// Create Our OpenGL Window
	if (!b1.CreateGLWindow(winname,resset[0] [res],resset[1] [res],resset[2] [res],(bool)!fullres))
		return 0;									// Quit If Window Was Not Created

	TimerInit();

	glClearColor(0.0f,0.0f,0.0f,0.5f);

	BOOL escpress=FALSE;
	BOOL inmenu=FALSE;
	bool vspace=FALSE;

	while(!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message==WM_QUIT)				// Have We Received A Quit Message?
			{
				done=TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
		
		if(intro)
		{
			runintro();
			SwapBuffers(hDC);
		}
		
		if(!intro)
		{
			
			float start=TimerGetTime();	
			
			if(keys[VK_ESCAPE] && !inmenu && !escpress)
			{
				inmenu=escpress=TRUE;
				shot.LoadSong("beep.mp3");
				oldres=res;
				oldfullres=fullres;
			}
			if(keys[VK_ESCAPE] && inmenu && !escpress)
			{
				inmenu=FALSE;
				shot.LoadSong("shot.mp3");
				escpress=TRUE;
			}
			if(!keys[VK_ESCAPE])
				escpress=FALSE;
			
			DrawGLScene();
			
			if(inmenu)
			{
				m1.testmovement();
				drawmenu();
			}
			
			if(!inmenu)
			{
				
				if(keys[VK_SPACE] && !vspace)
				{
					w1.shoot();
					vspace=TRUE;
				}
				if(!keys[VK_SPACE])
					vspace=FALSE;
				
				w1.testweapons();
				m1.testmovement();
				e1.testenemies();
				checkstuff();
				
				//remember to do all drawing BEFORE swap buffers!!!!!
				
				if(character.hurtcounter>0)
					character.hurtcounter--;
				
				if(character.life<0)
					character.life=0;
				
				if(keys[VK_DOWN])
					keys[VK_UP]=false;
				
				if(keys[VK_UP])
				{
					if(character.yvel<character.termvel)
						character.yvel+=.5;
					character.ypos-=character.yvel;
				}
				else
					character.yvel=0;
				
				if(!uppress && character.ypos<600)
				{
					character.ypos+=character.grav;
					character.grav+=.5;
				}
				else
					character.grav=2;
				
				if(character.ypos>600)
					character.ypos=600;
				if(character.ypos<98)
					character.ypos=98;
				
				if(leftpress && !rightpress)
				{
					xpos1+=3;
					xpos+=3;
					xpos3++;
					xpos2+=2;
					
					if(keys[VK_UP])
					{
						xpos1-=character.xvel;
						xpos-=character.xvel;
						xpos2-=character.xvel/2;
						if(character.xvel>-4)
							character.xvel-=.5;
					}
					else
						character.xvel=0;
					
					character.look=-1;
				}
				
				if(rightpress && !leftpress)
				{
					xpos-=3;
					xpos1-=3;
					xpos2-=2;
					xpos3--;
					
					if(keys[VK_UP])
					{
						xpos1-=character.xvel;
						xpos-=character.xvel;
						xpos2-=character.xvel/2;
						if(character.xvel<4)
							character.xvel+=.5;
					}
					else
						character.xvel=0;
					
					character.look=1;
				}			
			}
			SwapBuffers(hDC);					// Swap Buffers (Double Buffering)
			while(TimerGetTime()<start+float(steps[adjust]*2.0f)) {}
			
		}
		
	}
	
	// Shutdown
	s1.updateInitFile();
	b1.KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}
// By Jeff Verkoeyen and Josh Rodasti, 2003, January