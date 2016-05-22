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

class weapon
{
public:	

	weapon()
	{}
	
	void testweapons()
	{
		
		character.currentshots=character.maxshots;
		
		for(int a=0;a<character.maxshots;a++)
		{

			if(weapons.regen[a]>0)
				weapons.regen[a]--;
			
			if(weapons.on[a])
			{
				character.currentshots--;
				weapons.xpos[a]+=weapons.velocity[a];
				
				if(weapons.xpos[a]>1024 || weapons.xpos[a]<0)
					weapons.on[a]=FALSE;
				
				glBindTexture(GL_TEXTURE_2D, textures[character.currentgun+7].texID);
				
				glLoadIdentity();
				glTranslated(weapons.xpos[a],weapons.ypos[a]+13,9);
				
				glBegin(GL_QUADS);
				glTexCoord2f(0.0f,1.0f); glVertex3d(-weapons.face[a]*30,-15, 0);
				glTexCoord2f(1.0f,1.0f); glVertex3d( weapons.face[a]*30,-15, 0);
				glTexCoord2f(1.0f,0.0f); glVertex3d( weapons.face[a]*30, 15, 0);
				glTexCoord2f(0.0f,0.0f); glVertex3d(-weapons.face[a]*30, 15, 0);
				glEnd();
				
			}
			
		}
		
	}
	
	void shoot()
	{
		
		for(int a=0;a<character.maxshots;a++)
		{
			
			if(!weapons.on[a] && weapons.regen[a]<=0)
			{
				character.total++;
				
				shot.PlaySong();
				
				weapons.regen[a]=100;
				weapons.velocity[a]=character.look*20;
				weapons.on[a]=TRUE;
				weapons.xpos[a]=512;
				weapons.ypos[a]=(int)character.ypos;
				weapons.face[a]=character.look;
				
				break;
				
			}
			
		}
		
	}
	
};