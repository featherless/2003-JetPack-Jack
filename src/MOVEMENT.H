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

class movement
{
public:	

	movement()
	{}
	
	void testmovement()
	{
		
		if(!keys[VK_UP])
			uppress=FALSE;
		else
			uppress=TRUE;
		
		if(!keys[VK_LEFT])
			leftpress=FALSE;
		else
			leftpress=TRUE;
		
		if(!keys[VK_RIGHT])
			rightpress=FALSE;
		else
			rightpress=TRUE;
		
		int c=2,d=2,e=0,f;
		
		switch(character.xtile)
		{
			
		case 0:
			c=0;
			break;
		case 1:
			c=1;
			break;
		case 12:
			e=26;
			break;
		case 11:
			e=17;
			break;
		}
		
		glColor3f(1.0,1.0f,1.0f);
		
		// The following code not only draws all the boxes and platforms, but also checks for
		// any collision detection with the character
		
		for(int a=0;a<6;a++)
		{
			for(int b=(character.xtile-c)*9;b<(character.xtile+d)*9-e;b++)
			{			
				switch(level[a] [b])
				{
				case 1:
					glBindTexture(GL_TEXTURE_2D, textures[CRATES].texID);
					
					glLoadIdentity();
					glTranslated(b*128+xpos,a*128,7);
					
					glBegin(GL_QUADS);
					glTexCoord2f(0.0f,1.0f); glVertex3d(-64,-64, 0);
					glTexCoord2f(1.0f,1.0f); glVertex3d( 64,-64, 0);
					glTexCoord2f(1.0f,0.0f); glVertex3d( 64, 64, 0);
					glTexCoord2f(0.0f,0.0f); glVertex3d(-64, 64, 0);
					glEnd();
					
					if((b*128+xpos+64>=454 && b*128+xpos+64<=462) && (a*128>=character.ypos-128 && a*128<=character.ypos+128))
					{
						leftpress=FALSE;
						
						if(keys[VK_LEFT])
							character.look=-1;
					}
					
					if((b*128+xpos-64>=562 && b*128+xpos-64<=570) && (a*128>=character.ypos-128 && a*128<=character.ypos+128))
					{
						rightpress=FALSE;
						
						if(keys[VK_RIGHT])
							character.look=1;
						
					}
					
					if(!uppress && (b*128+xpos>=395 && b*128+xpos<=629) && (a*128>=character.ypos && a*128<=character.ypos+170))
						uppress=TRUE;
					
					if((b*128+xpos>=395 && b*128+xpos<=629) && (a*128<=character.ypos && a*128>=character.ypos-160))
						uppress=FALSE;
					
					for(f=0;f<6;f++)
					{
						if(weapons.on)
						{
							
							if(weapons.xpos[f]>b*128+xpos-75 && weapons.xpos[f]<b*128+xpos+75 && weapons.ypos[f]>a*128-75 && weapons.ypos[f]<a*128+75)
								weapons.on[f]=FALSE;
							
						}
					}
					
					break;
					
				case 2:
					glBindTexture(GL_TEXTURE_2D, textures[13].texID);
					
					glLoadIdentity();
					glTranslated(b*128+xpos,a*128,7);

					glRotatef(4,0.0f,0.0f,1.0f);
					
					glBegin(GL_QUADS);
						glTexCoord2f(0.0f,1.0f); glVertex3d(-192,-64, 0);
						glTexCoord2f(1.0f,1.0f); glVertex3d( 192,-64, 0);
						glTexCoord2f(1.0f,0.0f); glVertex3d( 192, 64, 0);
						glTexCoord2f(0.0f,0.0f); glVertex3d(-192, 64, 0);
					glEnd();
					
					if(!uppress && (b*128+xpos>=320 && b*128+xpos<=708) && (a*128>=character.ypos+100 && a*128<=character.ypos+115))
						uppress=TRUE;

					if(keys[VK_DOWN] && uppress)
						uppress=false;
					
					break;
				}
			}
		}	
	}
};