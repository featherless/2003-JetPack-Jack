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

class enemies
{
public:	

	enemies()
	{}
	
	void testenemies()
	{
		for(int	a=0;a<30;a++)
		{
			if(hopper.comeback[a]>0)
			{
				hopper.comeback[a]--;
				if(hopper.comeback[a]<=0)
				{
					hopper.on[a]=true;
					hopper.life[a]=4;
					hopper.hurtcounter[a]=0;
				}
			}

			if(pono.comeback[a]>0)
			{
				pono.comeback[a]--;
				if(pono.comeback[a]<=0)
				{
					pono.on[a]=true;
					pono.life[a]=3;
					pono.hurtcounter[a]=0;
				}
			}

			if(ponojump.comeback[a]>0)
			{
				ponojump.comeback[a]--;
				if(ponojump.comeback[a]<=0)
				{
					ponojump.on[a]=true;
					ponojump.life[a]=3;
					ponojump.hurtcounter[a]=0;
				}
			}

			if(hopper.on[a] && hopper.xpos[a]+xpos>-300 && hopper.xpos[a]+xpos<1324)
			{
				for(int b=0;b<6;b++)
				{
					if(weapons.on[b])
					{
						if(hopper.hurtcounter[a]<=0 && weapons.xpos[b]>hopper.xpos[a]+xpos-50 && weapons.xpos[b]<hopper.xpos[a]+xpos+50 && weapons.ypos[b]>hopper.ypos[a]-100 && weapons.ypos[b]<hopper.ypos[a]+50)
						{
							weapons.on[b]=FALSE;
							hopper.life[a]--;
							if(hopper.hurtcounter[a]!=90)
							{
								songintro.LoadSong("hopperhit.mp3");
								songintro.PlaySong();
							}
							hopper.hurtcounter[a]=90;
							character.hits++;
						}
					}
				}

				if(hopper.hurtcounter[a]>0)
					hopper.hurtcounter[a]--;
				
				if(hopper.life[a]<=0)
				{
					hopper.on[a]=FALSE;
					hopper.comeback[a]=1500;
				}

				hopper.count[a]++;

				glBindTexture(GL_TEXTURE_2D, textures[HOPPER_STAND].texID);

				if(hopper.count[a]>=50 && hopper.count[a]<100)
				{
					hopper.xpos[a]+=hopper.xvel[a];
					if(hopper.count[a]<75)
						hopper.ypos[a]-=hopper.yvel[a];
					else
						hopper.ypos[a]+=hopper.yvel[a];
					glBindTexture(GL_TEXTURE_2D, textures[HOPPER_JUMP].texID);
				}
				if(hopper.count[a]>=120 && hopper.count[a]<170)
				{
					glBindTexture(GL_TEXTURE_2D, textures[HOPPER_JUMP].texID);
					if(hopper.count[a]<145)
						hopper.ypos[a]-=3;
					else
					{
						hopper.face[a]=-1;
						hopper.ypos[a]+=3;
					}
				}
				if(hopper.count[a]>=190 && hopper.count[a]<240)
				{
					hopper.xpos[a]-=hopper.xvel[a];
					if(hopper.count[a]<215)
						hopper.ypos[a]-=hopper.yvel[a];
					else
						hopper.ypos[a]+=hopper.yvel[a];
					glBindTexture(GL_TEXTURE_2D, textures[HOPPER_JUMP].texID);
				}
				if(hopper.count[a]>=260 && hopper.count[a]<310)
				{
					glBindTexture(GL_TEXTURE_2D, textures[HOPPER_JUMP].texID);
					if(hopper.count[a]<285)
						hopper.ypos[a]-=3;
					else
					{
						hopper.face[a]=1;
						hopper.ypos[a]+=3;
					}
				}
				if(hopper.count[a]>=310)
					hopper.count[a]=20;

				if(hopper.hurtcounter[a]==0 || hopper.hurtcounter[a]%2==0)
				{
					
					glLoadIdentity();
					glTranslated(hopper.xpos[a]+xpos,hopper.ypos[a]-60,8);
					
					glBegin(GL_QUADS);
					glTexCoord2f(0.0f,1.0f); glVertex3d( hopper.face[a]*64,-128, 0);
					glTexCoord2f(1.0f,1.0f); glVertex3d(-hopper.face[a]*64,-128, 0);
					glTexCoord2f(1.0f,0.0f); glVertex3d(-hopper.face[a]*64, 128, 0);
					glTexCoord2f(0.0f,0.0f); glVertex3d( hopper.face[a]*64, 128, 0);
					glEnd();
				}

				if(character.hurtcounter<=0 && hopper.xpos[a]+xpos>=435 && hopper.xpos[a]+xpos<600 && hopper.ypos[a]>character.ypos-50 && hopper.ypos[a]<character.ypos+150)
				{
					character.life-=13;
					character.hurtcounter=80;
				}

				if(hopper.hurtcounter[a]>=76)	//this shows the POW pic for a couple frames
				{
					glBindTexture(GL_TEXTURE_2D, textures[16].texID);
					
					glLoadIdentity();
					glTranslated(hopper.xpos[a]+xpos,hopper.ypos[a]-20,9);
					
					glBegin(GL_QUADS);
					glTexCoord2f(0.0f,1.0f); glVertex3d(-64,-64, 0);
					glTexCoord2f(1.0f,1.0f); glVertex3d( 64,-64, 0);
					glTexCoord2f(1.0f,0.0f); glVertex3d( 64, 64, 0);
					glTexCoord2f(0.0f,0.0f); glVertex3d(-64, 64, 0);
					glEnd();
				}
			}

			if(ponojump.on[a] && ponojump.xpos[a]+xpos>-100 && ponojump.xpos[a]+xpos<1124)
			{
				for(int b=0;b<6;b++)
				{
					if(weapons.on[b])
					{
						if(ponojump.hurtcounter[a]<=0 && weapons.xpos[b]>ponojump.xpos[a]+xpos-50 && weapons.xpos[b]<ponojump.xpos[a]+xpos+50 && weapons.ypos[b]>ponojump.ypos[a]-70 && weapons.ypos[b]<ponojump.ypos[a]+50)
						{
							weapons.on[b]=FALSE;
							ponojump.life[a]--;
							if(ponojump.hurtcounter[a]!=90)
							{
								songintro.LoadSong("hoppohit.mp3");
								songintro.PlaySong();
							}
							ponojump.hurtcounter[a]=90;
							character.hits++;
						}
					}
				}
				
				if(ponojump.hurtcounter[a]>0)
					ponojump.hurtcounter[a]--;
				
				if(ponojump.life[a]<=0)
				{
					ponojump.on[a]=FALSE;
					ponojump.comeback[a]=1500;
				}
				
				if(character.hurtcounter<=0 && ponojump.xpos[a]+xpos>=435 && ponojump.xpos[a]+xpos<600 && ponojump.ypos[a]>character.ypos-50 && ponojump.ypos[a]<character.ypos+50)
				{
					character.life-=13;
					character.hurtcounter=80;
				}
				
				ponojump.count[a]++;

				glBindTexture(GL_TEXTURE_2D, textures[PONO_COIL].texID);

				if(ponojump.count[a]>=50 && ponojump.count[a]<100)
				{
					glBindTexture(GL_TEXTURE_2D, textures[PONO_SPRING].texID);
					ponojump.ypos[a]-=ponojump.vel[a];
				}

				if(ponojump.count[a]>=100 && ponojump.count[a]<150)
					ponojump.ypos[a]+=ponojump.vel[a];

				if(ponojump.count[a]>=150)
					ponojump.count[a]=0;
				
				if(ponojump.hurtcounter[a]==0 || ponojump.hurtcounter[a]%2==0)
				{
					
					glLoadIdentity();
					glTranslated(ponojump.xpos[a]+xpos,ponojump.ypos[a]+20,8);
					
					glBegin(GL_QUADS);
					glTexCoord2f(0.0f,1.0f); glVertex3d(-64,-80, 0);
					glTexCoord2f(1.0f,1.0f); glVertex3d( 64,-80, 0);
					glTexCoord2f(1.0f,0.0f); glVertex3d( 64, 80, 0);
					glTexCoord2f(0.0f,0.0f); glVertex3d(-64, 80, 0);
					glEnd();
				}
				
				if(ponojump.hurtcounter[a]>=76)	//this shows the POW pic for a couple frames
				{
					glBindTexture(GL_TEXTURE_2D, textures[POW].texID);
					
					glLoadIdentity();
					glTranslated(ponojump.xpos[a]+xpos,ponojump.ypos[a]-20,9);
					
					glBegin(GL_QUADS);
					glTexCoord2f(0.0f,1.0f); glVertex3d(-64,-64, 0);
					glTexCoord2f(1.0f,1.0f); glVertex3d( 64,-64, 0);
					glTexCoord2f(1.0f,0.0f); glVertex3d( 64, 64, 0);
					glTexCoord2f(0.0f,0.0f); glVertex3d(-64, 64, 0);
					glEnd();
				}
			}

			if(pono.on[a] && pono.xpos[a]+xpos>-100 && pono.xpos[a]+xpos<1124)
			{
				for(int b=0;b<6;b++)
				{
					if(weapons.on[b])
					{
						if(pono.hurtcounter[a]<=0 && weapons.xpos[b]>pono.xpos[a]+xpos-50 && weapons.xpos[b]<pono.xpos[a]+xpos+50 && weapons.ypos[b]>pono.ypos[a]-50 && weapons.ypos[b]<pono.ypos[a]+50)
						{
							weapons.on[b]=FALSE;
							pono.life[a]--;
							if(pono.hurtcounter[a]!=90)
							{
								songintro.LoadSong("ponohit.mp3");
								songintro.PlaySong();
							}
							pono.hurtcounter[a]=90;
							character.hits++;
						}
					}
				}
				
				if(pono.hurtcounter[a]>0)
					pono.hurtcounter[a]--;
				
				if(pono.life[a]<=0)
				{
					pono.on[a]=FALSE;
					pono.comeback[a]=1500;
				}
				
				if(pono.xpos[a]+xpos>0 && pono.xpos[a]+xpos<1024 && pono.ypos[a]>character.ypos-100 && pono.ypos[a]<character.ypos+100)
					pono.insane[a]=TRUE;
				else
					pono.insane[a]=FALSE;
				
				if(character.hurtcounter<=0 && pono.xpos[a]+xpos>=435 && pono.xpos[a]+xpos<600 && pono.ypos[a]>character.ypos-50 && pono.ypos[a]<character.ypos+50)
				{
					character.life-=18;
					character.hurtcounter=80;
				}
				
				pono.count[a]++;		//increase the animation counter

				if(pono.count[a]>15)	//speed that they walk
				{
					pono.image[a]++;
					pono.count[a]=0;
				}
				if(pono.image[a]>1)
					pono.image[a]=0;
				
				if(!pono.insane[a])
				{
					if(pono.face[a]==1 && (float)pono.xpos[a]/128>=pono.maxright[a])
						pono.face[a]=-1;
					
					if(pono.face[a]==-1 && (float)pono.xpos[a]/128<=pono.maxleft[a])
						pono.face[a]=1;
					
					pono.xpos[a]+=pono.face[a];
				}
				else
				{

					pono.count[a]+=2; //technically, if a pono is in insane mode, it will come after you very fast
				
					if(pono.xpos[a]+xpos<=512)
						pono.face[a]=1;
					else
						pono.face[a]=-1;
					
					if(!(pono.face[a]==1 && (float)pono.xpos[a]/128>=pono.maxright[a]) && !(pono.face[a]==-1 && (float)pono.xpos[a]/128<=pono.maxleft[a]))
						pono.xpos[a]+=pono.face[a]*3;
				}
				
				if(pono.hurtcounter[a]==0 || pono.hurtcounter[a]%2==0)
				{
					glBindTexture(GL_TEXTURE_2D, textures[14+pono.image[a]].texID);
					
					glLoadIdentity();
					glTranslated(pono.xpos[a]+xpos,pono.ypos[a],8);
					
					glBegin(GL_QUADS);
					glTexCoord2f(0.0f,1.0f); glVertex3d( pono.face[a]*64,-64, 0);
					glTexCoord2f(1.0f,1.0f); glVertex3d(-pono.face[a]*64,-64, 0);
					glTexCoord2f(1.0f,0.0f); glVertex3d(-pono.face[a]*64, 64, 0);
					glTexCoord2f(0.0f,0.0f); glVertex3d( pono.face[a]*64, 64, 0);
					glEnd();
				}
				
				if(pono.hurtcounter[a]>=76)	//this shows the POW pic for a couple frames
				{
					glBindTexture(GL_TEXTURE_2D, textures[16].texID);
					
					glLoadIdentity();
					glTranslated(pono.xpos[a]+xpos,pono.ypos[a]-20,9);
					
					glBegin(GL_QUADS);
					glTexCoord2f(0.0f,1.0f); glVertex3d(-64,-64, 0);
					glTexCoord2f(1.0f,1.0f); glVertex3d( 64,-64, 0);
					glTexCoord2f(1.0f,0.0f); glVertex3d( 64, 64, 0);
					glTexCoord2f(0.0f,0.0f); glVertex3d(-64, 64, 0);
					glEnd();
				}
			}
		}
	}
};