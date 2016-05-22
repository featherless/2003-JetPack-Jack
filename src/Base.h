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

class baseset
{
public:	
	
	baseset()
	{}
	
	GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
	{
		if (fullscreen)										// Are We In Fullscreen Mode?
		{
			ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
			ShowCursor(TRUE);								// Show Mouse Pointer
		}
		
		if (hRC)											// Do We Have A Rendering Context?
		{
			if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
			{
				MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			}
			
			if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
			{
				MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			}
			hRC=NULL;										// Set RC To NULL
		}
		
		if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
		{
			MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			hDC=NULL;										// Set DC To NULL
		}
		
		if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
		{
			MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			hWnd=NULL;										// Set hWnd To NULL
		}
		
		if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
		{
			MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			hInstance=NULL;									// Set hInstance To NULL
		}
	}


	BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
	{
		GLuint		PixelFormat;			// Holds The Results After Searching For A Match
		WNDCLASS	wc;						// Windows Class Structure
		DWORD		dwExStyle;				// Window Extended Style
		DWORD		dwStyle;				// Window Style
		RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
		WindowRect.left=(long)0;			// Set Left Value To 0
		WindowRect.right=(long)width;		// Set Right Value To Requested Width
		WindowRect.top=(long)0;				// Set Top Value To 0
		WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height
	
		fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag
	
		hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
		wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
		wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
		wc.cbClsExtra		= 0;									// No Extra Window Data
		wc.cbWndExtra		= 0;									// No Extra Window Data
		wc.hInstance		= hInstance;							// Set The Instance
		wc.hIcon			= (HCURSOR)LoadImage(hInstance,MAKEINTRESOURCE(IDI_ICON1),IMAGE_ICON,
		                                    0,0,LR_DEFAULTSIZE);;			// Load The Default Icon
		wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
		wc.hbrBackground	= NULL;									// No Background Required For GL
		wc.lpszMenuName		= NULL;									// We Don't Want A Menu
		wc.lpszClassName	= "OpenGL";								// Set The Class Name

		if (!RegisterClass(&wc))									// Attempt To Register The Window Class
		{
			MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;											// Return FALSE
		}
	
		if (fullscreen)												// Attempt Fullscreen Mode?
		{
			DEVMODE dmScreenSettings;								// Device Mode
			memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
			dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
			dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
			dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
			dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
			dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
	
			// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
			if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
			{
				// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
				if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","Jet Pack Jack",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
				{
					fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
				}
				else
				{
					// Pop Up A Message Box Letting User Know The Program Is Closing.
					MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
					return FALSE;									// Return FALSE
				}
			}
		}
	
		ShowCursor(FALSE);										// Hide Mouse Pointer

		if (fullscreen)												// Are We Still In Fullscreen Mode?
		{
			dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
			dwStyle=WS_POPUP;										// Windows Style
		}
		else
		{
			dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
			dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
		}

		AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size
	
		// Create The Window
		if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
									"OpenGL",							// Class Name
									title,								// Window Title
									dwStyle |							// Defined Window Style
									WS_CLIPSIBLINGS |					// Required Window Style
									WS_CLIPCHILDREN,					// Required Window Style
									0, 0,								// Window Position
									WindowRect.right-WindowRect.left,	// Calculate Window Width
									WindowRect.bottom-WindowRect.top,	// Calculate Window Height
									NULL,								// No Parent Window
									NULL,								// No Menu
									hInstance,							// Instance
									NULL)))								// Dont Pass Anything To WM_CREATE
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
		{
			sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
			1,											// Version Number
			PFD_DRAW_TO_WINDOW |						// Format Must Support Window
			PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,							// Must Support Double Buffering
			PFD_TYPE_RGBA,								// Request An RGBA Format
			bits,										// Select Our Color Depth
			0, 0, 0, 0, 0, 0,							// Color Bits Ignored
			0,											// No Alpha Buffer
			0,											// Shift Bit Ignored
			0,											// No Accumulation Buffer
			0, 0, 0, 0,									// Accumulation Bits Ignored
			16,											// 16Bit Z-Buffer (Depth Buffer)  
			0,											// No Stencil Buffer
			0,											// No Auxiliary Buffer
			PFD_MAIN_PLANE,								// Main Drawing Layer
			0,											// Reserved
			0, 0, 0										// Layer Masks Ignored
		};
		
		if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}
	
		if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}
	
		if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		ShowWindow(hWnd,SW_SHOW);						// Show The Window
		SetForegroundWindow(hWnd);						// Slightly Higher Priority
		SetFocus(hWnd);									// Sets Keyboard Focus To The Window
		ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen	
	
		if (!InitGL())									// Initialize Our Newly Created GL Window
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		return TRUE;									// Success
	}


	int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
	{
		
		BuildFont();
		
		if (!LoadTGA(&textures[JACK],"character.tga")				||
			!LoadTGA(&textures[JACK_JUMPING],"characterjump.tga")	||
			!LoadTGA(&textures[GROUND],"ground.tga")				||
			!LoadTGA(&textures[SKY],"sky.tga")						||
			!LoadTGA(&textures[SUN],"sun.tga")						||
			!LoadTGA(&textures[CRATES],"crate.tga")					||
			!LoadTGA(&textures[MOUNTAINS],"mountains.tga")			||
			!LoadTGA(&textures[BULLET_1],"bullet1.tga")				||
			!LoadTGA(&textures[PLATFORM],"platform.tga")			||
			!LoadTGA(&textures[PONO_ANI_1],"pono1.tga")				||
			!LoadTGA(&textures[PONO_ANI_2],"pono2.tga")				||
			!LoadTGA(&textures[POW],"POW.tga")						||
			!LoadTGA(&textures[HOPPER_STAND],"hopper.tga")			||
			!LoadTGA(&textures[HOPPER_JUMP],"hopperjump.tga")		||
			!LoadTGA(&textures[BAR],"bar.tga")						||
			!LoadTGA(&textures[TITLE],"title.tga")					||
			!LoadTGA(&textures[PONO_COIL],"ponocoil.tga")			||
			!LoadTGA(&textures[PONO_SPRING],"ponospring.tga")
			)				// Load The Textures
			return false;	// If Loading Failed, Return False
		
		glClearColor(1.0f,1.0f,1.0f,0.5f);					// White Background
		glClearDepth(1.0f);									// Depth Buffer Setup
		glEnable(GL_DEPTH_TEST);							// Enable Depth Testing
		glEnable(GL_BLEND);									// Enable Blending
		glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);	// Really Nice Perspective Calculations
		glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);				// Really Nice Point Smoothing
		glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// Enable Alpha Blending (disable alpha testing)
		glAlphaFunc(GL_GREATER,0.1f);						// Set Alpha Testing     (disable blending)
		glEnable(GL_ALPHA_TEST);							// Enable Alpha Testing  (disable blending)
		
		return TRUE;										// Initialization Went OK
	}

	GLvoid BuildFont(GLvoid)								// Build Our Bitmap Font
	{
		HFONT	font;										// Windows Font ID
		HFONT	oldfont;									// Used For Good House Keeping
		
		base = glGenLists(255);								// Storage For 255 Characters
		
		font = CreateFont(	-24,								// Height Of Font
			10,								// Width Of Font
			0,								// Angle Of Escapement
			0,								// Orientation Angle
			FW_BOLD,						// Font Weight
			FALSE,							// Italic
			FALSE,							// Underline
			FALSE,							// Strikeout
			ANSI_CHARSET,					// Character Set Identifier
			OUT_TT_PRECIS,					// Output Precision
			CLIP_DEFAULT_PRECIS,			// Clipping Precision
			ANTIALIASED_QUALITY,			// Output Quality
			FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
			"Comic Sans");					// Font Name
		
		oldfont = (HFONT)SelectObject(hDC, font);           // Selects The Font We Want
		wglUseFontBitmaps(hDC, 0, 255, base);				// Builds 255 Characters Starting At Character 0
		SelectObject(hDC, oldfont);							// Selects The Font We Want
		DeleteObject(font);									// Delete The Font
	}
	
	GLvoid KillFont(GLvoid)									// Delete The Font List
	{
		glDeleteLists(base, 255);							// Delete All 255 Characters
	}
	
	GLvoid glPrint(const char *fmt, ...)					// Custom GL "Print" Routine
	{
		char		text[256];								// Holds Our String
		va_list		ap;										// Pointer To List Of Arguments
		
		if (fmt == NULL)									// If There's No Text
			return;											// Do Nothing
		
		va_start(ap, fmt);									// Parses The String For Variables
		vsprintf(text, fmt, ap);						// And Converts Symbols To Actual Numbers
		va_end(ap);											// Results Are Stored In Text
		
		glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
		glListBase(base);									// Sets The Base Character to 0
		glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
		glPopAttrib();										// Pops The Display List Bits
	}
	
	bool LoadTGA(TextureImage *texture, char *filename)		// Loads A TGA File Into Memory
	{    
		GLubyte		TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};// Uncompressed TGA Header
		GLubyte		TGAcompare[12];							// Used To Compare TGA Header
		GLubyte		header[6];								// First 6 Useful Bytes From The Header
		GLuint		bytesPerPixel;							// Holds Number Of Bytes Per Pixel Used In The TGA File
		GLuint		imageSize;								// Used To Store The Image Size When Setting Aside Ram
		GLuint		temp;									// Temporary Variable
		GLuint		type=GL_RGBA;							// Set The Default GL Mode To RBGA (32 BPP)
		
		FILE *file = fopen(filename, "rb");					// Open The TGA File
		
		if(	file==NULL ||									// Does File Even Exist?
			fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) ||	// Are There 12 Bytes To Read?
			memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0		||	// Does The Header Match What We Want?
			fread(header,1,sizeof(header),file)!=sizeof(header))// If So Read Next 6 Header Bytes
		{
			if (file == NULL)								// Did The File Even Exist? *Added Jim Strong*
				return false;								// Return False
			else
			{
				fclose(file);								// If Anything Failed, Close The File
				return false;								// Return False
			}
		}
		
		texture->width  = header[1] * 256 + header[0];		// Determine The TGA Width	(highbyte*256+lowbyte)
		texture->height = header[3] * 256 + header[2];		// Determine The TGA Height	(highbyte*256+lowbyte)
		
		if(	texture->width	<=0	||							// Is The Width Less Than Or Equal To Zero
			texture->height	<=0	||							// Is The Height Less Than Or Equal To Zero
			(header[4]!=24 && header[4]!=32))				// Is The TGA 24 or 32 Bit?
		{
			fclose(file);									// If Anything Failed, Close The File
			return false;									// Return False
		}
		
		texture->bpp	= header[4];						// Grab The TGA's Bits Per Pixel (24 or 32)
		bytesPerPixel	= texture->bpp/8;					// Divide By 8 To Get The Bytes Per Pixel
		imageSize	= texture->width*texture->height*bytesPerPixel;			// Calculate The Memory Required For The TGA Data
		
		texture->imageData=(GLubyte *)malloc(imageSize);	// Reserve Memory To Hold The TGA Data
		
		if(	texture->imageData==NULL ||						// Does The Storage Memory Exist?
			fread(texture->imageData, 1, imageSize, file)!=imageSize)		// Does The Image Size Match The Memory Reserved?
		{
			if(texture->imageData!=NULL)					// Was Image Data Loaded
				free(texture->imageData);					// If So, Release The Image Data
			
			fclose(file);									// Close The File
			return false;									// Return False
		}
		for(GLuint i=0; i<int(imageSize); i+=bytesPerPixel)				// Loop Through The Image Data
		{													// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
			temp=texture->imageData[i];						// Temporarily Store The Value At Image Data 'i'
			texture->imageData[i] = texture->imageData[i + 2];			// Set The 1st Byte To The Value Of The 3rd Byte
			texture->imageData[i + 2] = temp;				// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
		}
		
		fclose (file);										// Close The File
		
		// Build A Texture From The Data
		glGenTextures(1, &texture[0].texID);				// Generate OpenGL texture IDs
		
		glBindTexture(GL_TEXTURE_2D, texture[0].texID);		// Bind Our Texture
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		// Linear Filtered
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		// Linear Filtered
		
		if (texture[0].bpp==24)								// Was The TGA 24 Bits
		{
			type=GL_RGB;									// If So Set The 'type' To GL_RGB
		}
		
		glTexImage2D(GL_TEXTURE_2D, 0, type, texture[0].width, texture[0].height, 0, type, GL_UNSIGNED_BYTE, texture[0].imageData);
		
		return true;										// Texture Building Went Ok, Return True
	}
	
	GLvoid ReSizeGLScene(GLsizei width, GLsizei height)				// Resize And Initialize The GL Window
	{
		if (height==0)								// Prevent A Divide By Zero By
			height=1;								// Making Height Equal One
		
		glViewport(0,0,width,height);						// Reset The Current Viewport
		
		glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
		glLoadIdentity();							// Reset The Projection Matrix
		
		glOrtho(0.0f,1024,768,0.0f,-10.0f,2.0f);
		
		glMatrixMode(GL_MODELVIEW);						// Select The Modelview Matrix
		glLoadIdentity();							// Reset The Modelview Matrix
	}
	
};