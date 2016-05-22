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

#include <fstream>
#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>
using namespace std;

class saving
{
private:
	int x,y,bit;
	bool fullscreen;
public:	

	saving()
	{}
	
	void updateInitFile()
	{
		
		ofstream initfile;
		
		initfile.open("INIT.DAT");
		
		initfile << "INIT FILE FOR JETPACK JACK\n\n";
		initfile << "CURRENT_VERSION " << winname << endl;
		initfile << "\nJetPack Jack was created by Jeff Verkoeyen and Josh Rodasti\n";
		initfile << "Only edit this file where it says you are allowed to\n";
		initfile << "\nSCREENSIZEX " << resset[0] [res];
		initfile << "\nSCREENSIZEY " << resset[1] [res];
		initfile << "\nSCREENBITS " << resset[2] [res];
		initfile << "\n\nYou may change the above numbers, but not the text, the allowed\n";
		initfile << "screen sizes are 1024x768 16, 1024x768 32, 800x600 16, 800x600 32, 640x480 16, 640x480 32\n";
		if(!fullres)
			initfile << "\nFULLSCREEN true\n";
		else
			initfile << "\nFULLSCREEN false\n";
		initfile << "Fullscreen value is true or false\n";
		initfile << "\nGAMESPEED " << adjust << endl;
		initfile << "Game speed can be from 0-5, 5 being slowest, 0 being fastest" << endl;
		initfile << "\nJetPack Jack is a production of the IVGDA.\n";
		
		initfile.close();
		
	}

	int readInitFile()
	{
		
		ofstream initfile;
		ifstream initfile2;
		
		initfile.open("INIT.DAT");
		
		bool cont=false;
		
		if(initfile)
		{
			initfile << "INIT FILE FOR JETPACK JACK\n\n";
			initfile << "CURRENT_VERSION " << winname << endl;
			initfile << "\nJetPack Jack was created by Jeff Verkoeyen and Josh Rodasti\n";
			initfile << "Only edit this file where it says you are allowed to\n";
			initfile << "\nSCREENSIZEX 1024";
			initfile << "\nSCREENSIZEY 768";
			initfile << "\nSCREENBITS 16\n";
			initfile << "\nYou may change the above numbers, but not the text, the allowed\n";
			initfile << "screen sizes are 1024x768 16, 1024x768 32, 800x600 16, 800x600 32, 640x480 16, 640x480 32\n";
			initfile << "\nFULLSCREEN true\n";
			initfile << "Fullscreen value is true or false\n";
			initfile << "\nGAMESPEED " << 3 << endl;
			initfile << "Game speed can be from 0-5, 5 being slowest, 0 being fastest" << endl;
			initfile << "\nFullscreen value is true or false\n";
			initfile << "\nJetPack Jack is a production of the IVGDA.\n";
			
			cont=true;
			
			initfile.close();
		}
		else
		{
			if(checkValidicity())
				cont=true;
			else
				cont=false;
		}
		
		if(cont)
		{
			initfile2.open("INIT.DAT",ios::ate);
			
			char buff[30]=" ";
			
			initfile2.seekg(ios::beg);
			
			while(strcmp(buff,"SCREENSIZEX ") && !initfile2.eof())
			{
				initfile2.get(buff,13);
				if(strcmp(buff,"SCREENSIZEX "))
					initfile2.ignore(100,'\n');
			}
			
			initfile2.get(buff,10,'\n');
			
			x=atoi(buff);
			
			
			initfile2.seekg(ios::beg);
			
			while(strcmp(buff,"SCREENSIZEY ") && !initfile2.eof())
			{
				initfile2.get(buff,13);
				if(strcmp(buff,"SCREENSIZEY "))
					initfile2.ignore(100,'\n');
			}
			
			initfile2.get(buff,10,'\n');
			
			y=atoi(buff);
			
			
			initfile2.seekg(ios::beg);
			
			while(strcmp(buff,"SCREENBITS ") && !initfile2.eof())
			{
				initfile2.get(buff,12);
				if(strcmp(buff,"SCREENBITS "))
					initfile2.ignore(100,'\n');
			}
			
			initfile2.get(buff,10,'\n');
			
			bit=atoi(buff);
		
			
			initfile2.seekg(ios::beg);
			
			while(strcmp(buff,"FULLSCREEN ") && !initfile2.eof())
			{
				initfile2.get(buff,12);
				if(strcmp(buff,"FULLSCREEN "))
					initfile2.ignore(100,'\n');
			}
			
			initfile2.get(buff,10,'\n');
			
			if(!strcmp(buff,"true"))
				fullscreen=true;
			if(!strcmp(buff,"false"))
				fullscreen=false;


			initfile2.seekg(ios::beg);
			
			while(strcmp(buff,"GAMESPEED ") && !initfile2.eof())
			{
				initfile2.get(buff,11);
				if(strcmp(buff,"GAMESPEED "))
					initfile2.ignore(100,'\n');
			}
			
			initfile2.get(buff,10,'\n');
			
			adjust=atoi(buff);

			
			initfile2.close();
			
			bool flag=false;
			int a;
			for(a=0;a<6;a++)
			{
				if(x==resset[0] [a] && y==resset[1] [a] && bit==resset[2] [a])
				{
					flag=true;
					break;
				}
			}
			
			if(!flag)
			{
				system("del INIT.DAT");
				readInitFile();
			}
			else
			{
				res=a;
				fullres=(int)!fullscreen;
				char temp[100];
				sprintf(temp, "%dx%d %d setting %d",x,y,bit,res);
			//	MessageBox(NULL,temp,"NOTICE",MB_OK | MB_ICONINFORMATION);
				return 1;
			}
			
		}
		else
		{
			system("del INIT.DAT");
			readInitFile();
		}
		return 1;
	}
	
	int checkValidicity()
	{
		
		ifstream initfile;
		
		initfile.open("INIT.DAT",ios::ate);
		
		char buff[100];
		
		initfile.seekg(ios::beg);
		
		while(strcmp(buff,"CURRENT_VERSION ") && !initfile.eof())
		{
			initfile.get(buff,17);
			if(strcmp(buff,"CURRENT_VERSION "))
				initfile.ignore(100,'\n');
		}
		
		initfile.get(buff,30,'\n');
		
		if(strcmp(buff,winname))
			return 0;
		else
			return 1;
	}
	
	
};

/*
ios::app       -- Opens the file, and allows additions at the end
ios::ate       -- Opens the file, but allows additions anywhere
ios::trunc     -- Deletes everything in the file
ios::nocreate  -- Does not open if the file must be created
ios::noreplace -- Does not open if the file already exists
*/