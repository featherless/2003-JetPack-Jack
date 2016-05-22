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

#include <stdio.h>
#include <windows.h>

char winname[]="JetPack Jack v0.8a";

int resset[3] [6]=
{
	{1024,	1024,	800,	800,	640,	640},
	{768,	768,	600,	600,	480,	480},
	{16,	32,		16,		32,		16,		32}
};

int		adjust=3;

char full[2] [11]=
{
	{"Fullscreen"},
	{"Windowed"}
};

int res,oldres;
int fullres,oldfullres;

#define VK_0	0x30
#define VK_1	0x31
#define VK_2	0x32
#define VK_3	0x33
#define VK_4	0x34
#define VK_5	0x35
#define VK_6	0x36
#define VK_7	0x37
#define VK_8	0x38
#define VK_9	0x39

#define VK_A	0x41
#define VK_B	0x42
#define VK_C	0x43
#define VK_D	0x44
#define VK_E	0x45
#define VK_F	0x46
#define VK_G	0x47
#define VK_H	0x48
#define VK_I	0x49
#define VK_J	0x4A
#define VK_K	0x4B
#define VK_L	0x4C
#define VK_M	0x4D
#define VK_N	0x4E
#define VK_O	0x4F
#define VK_P	0x50
#define VK_Q	0x51
#define VK_R	0x52
#define VK_S	0x53
#define VK_T	0x54
#define VK_U	0x55
#define VK_V	0x56
#define VK_W	0x57
#define VK_X	0x58
#define VK_Y	0x59
#define VK_Z	0x5A

#define JACK			0
#define JACK_JUMPING	1
#define GROUND			2
#define SKY				3
#define SUN				4
#define CRATES			5
#define MOUNTAINS		6
#define BULLET_1		7
#define PLATFORM		13
#define PONO_ANI_1		14
#define PONO_ANI_2		15
#define POW				16
#define HOPPER_STAND	17
#define HOPPER_JUMP		18
#define BAR				19
#define TITLE			21
#define PONO_COIL		22
#define PONO_SPRING		23

BOOL rpress=FALSE,rvar=FALSE;;

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int menuselection=1;

CFmod shot,BGMusic,songintro;

HDC			hDC=NULL;
HGLRC		hRC=NULL;
HWND		hWnd=NULL;
HINSTANCE	hInstance;

BOOL	done=FALSE;								// Bool Variable To Exit Loop

bool	active=TRUE;
bool	fullscreen=TRUE;

GLuint	base;			// Font variable

int level[6] [100]=
{
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{ 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 2, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 2, 0, 0, 2, 0, 0, 2, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{ 1, 0, 0, 0, 0, 1, 1, 1, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 2, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{ 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{ 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
}; // 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64 65 66 67 68 69 70 71 72 73 74 75 76 77 78 79 80 81 82 83 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99

//Current level ^^

typedef	struct				// Create A Structure
{
	GLubyte	*imageData;		// Image Data (Up To 32 Bits)
	GLuint	bpp;			// Image Color Depth In Bits Per Pixel
	GLuint	width;			// Image Width
	GLuint	height;			// Image Height
	GLuint	texID;			// Texture ID Used To Select A Texture
} TextureImage;				// Structure Name

TextureImage textures[50];

unsigned int mousex,mousey;
BOOL gleft=FALSE,gright=FALSE;

bool	leftpress=FALSE,rightpress=FALSE,uppress=FALSE;		//Optional keypress variables

bool	keys[256];

int menuset=0;

float xpos1=0,	//foreground (ground)
	  xpos2=0,	//middleground (mountains)
	  xpos3=0,	//background (clouds)
	  xpos;		//Xposition of everything
float sunrot=0;	//Self-explanatory, rotation of the sun

struct Pono
{				//Gotta love those ponos

	int xpos[30], ypos[30];	//xpos & ypos in pixels
	bool on[30];			//they even alive?
	int face[30];			//make sure we know which way they're facing
	int image[30];			//which animation to show
	int count[30];			//Counter between the animations
	int maxleft[30], maxright[30];	//For the PAI (Pseudo-Artificial-Intelligence)
											//they just make sure to not leave the left or right
											//constraining area
	bool insane[30];		//Insane mode, where they chase you
	int life[30];
	int hurtcounter[30];
	int comeback[30];

}pono;		// same as Pono pono, hehe

struct PonoJump
{
	float vel[30];
	float xpos[30], ypos[30];
	bool on[30];
	int count[30];
	int life[30];
	int hurtcounter[30];
	int comeback[30];
	int peak[30];

}ponojump;

struct Hopper
{

	int peak[30];
	float yvel[30], xvel[30];
	float xpos[30], ypos[30];
	bool on[30];
	int face[30];
	int count[30];
	int jumpstart[30],jumpend[30];
	int life[30];
	int hurtcounter[30];
	int comeback[30];

}hopper;

struct Weapons
{				//Just kinda assuming there's only going to be 6 bullets

	int xpos[6], ypos[6];	//xpos & ypos of the shots
	bool on[6];				//should we even both displaying it?
	int velocity[6];		//Velocity, really just pos/neg
	int face[6];			//Direction it's facing
	int regen[6];

}weapons;

struct Character
{						//All the stats needed for the character, so far

	int look;			//look...don't know why i have this in here any more
	float ypos;			//don't need the xpos, the character is always at 512
	int xtile;			//Current tile we are looking at, mainly
	int currentgun;		//Current selected gun
	int maxshots;		//Max number of shots at a time
	int currentshots;	//Number of shots fired at one point in time
	int life;			//How much life you have left
	int hurtcounter;	//Waits untill you can be hurt again
	int hits,total;		//For the accuracy
	float xvel,yvel,termvel,grav;

}character;

class variables
{
public:	

	variables()
	{}
};