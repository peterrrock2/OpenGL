#include "final_head.h"

int shader = 0;
float X=0,Y=0,Z=1; //  Mandelbrot X,Y,Z

/*
 *  Read text file
 */
char* ReadText(char *file)
{
   int   n;
   char* buffer;
   //  Open file
   FILE* f = fopen(file,"rt");
   if (!f) Fatal("Cannot open text file %s\n",file);
   //  Seek to end to determine size, then rewind
   fseek(f,0,SEEK_END);
   n = ftell(f);
   rewind(f);
   //  Allocate memory for the whole file
   buffer = (char*)malloc(n+1);
   if (!buffer) Fatal("Cannot allocate %d bytes for text file %s\n",n+1,file);
   //  Snarf the file
   if (fread(buffer,n,1,f)!=1) Fatal("Cannot read %d bytes for text file %s\n",n,file);
   buffer[n] = 0;
   //  Close and return
   fclose(f);
   return buffer;
}



/*
 *  Print Shader Log
 */
void PrintShaderLog(int obj,char* file)
{
   int len=0;
   glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) Fatal("Cannot allocate %d bytes of text for shader log\n",len);
      glGetShaderInfoLog(obj,len,&n,buffer);
      // fprintf(stderr,"%s:\n%s\n",file,buffer);
      free(buffer);
   }
   glGetShaderiv(obj,GL_COMPILE_STATUS,&len);
   if (!len) Fatal("Error compiling %s\n",file);
}

/*
 *  Print Program Log
 */
void PrintProgramLog(int obj)
{
   int len=0;
   glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&len);
   if (len>1)
   {
      int n=0;
      char* buffer = (char *)malloc(len);
      if (!buffer) Fatal("Cannot allocate %d bytes of text for program log\n",len);
      glGetProgramInfoLog(obj,len,&n,buffer);
      // fprintf(stderr,"%s\n",buffer);
   }
   glGetProgramiv(obj,GL_LINK_STATUS,&len);
   if (!len) Fatal("Error linking program\n");
}




/*
 *  Create Shader
 */
int CreateShader(GLenum type,char* file)
{
   //  Create the shader
   int shader = glCreateShader(type);
   //  Load source code from file
   char* source = ReadText(file);
   glShaderSource(shader,1,(const char**)&source,NULL);
   free(source);
   //  Compile the shader
   // fprintf(stderr,"Compile %s\n",file);
   glCompileShader(shader);
   //  Check for errors
   PrintShaderLog(shader,file);
   //  Return name
   return shader;
}

/*
 *  Create Shader Program
 */
int CreateShaderProg(char* VertFile, char* FragFile)
{
   //  Create program
   int prog = glCreateProgram();
   //  Create and compile vertex shader
   if(VertFile){
     int vert = CreateShader(GL_VERTEX_SHADER  ,VertFile);
     glAttachShader(prog,vert);
   }
   //  Create and compile fragment shader
   if(FragFile)
   {
     int frag = CreateShader(GL_FRAGMENT_SHADER,FragFile);
     glAttachShader(prog,frag);
   }
   //  Link program
   glLinkProgram(prog);
   //  Check for errors
   PrintProgramLog(prog);
   //  Return name
   return prog;
}


// Makes the limbs for the tree
void cone(double rb, double height,
					unsigned int bark_texture)
{
	glEnable(GL_TEXTURE_2D);

	int divisions = 8;

	double rt = 0.4 * rb ;


	glBindTexture(GL_TEXTURE_2D, bark_texture);
	glBegin(GL_QUAD_STRIP);
  for (int i = 0; i <= divisions; i++)
  {
      float angle = i*((1.0/divisions) * (2*PI));
      glNormal3d( cos(angle), (rb - rt) / height, sin(angle) );
			glTexCoord2f( i * (1.0/divisions), 1);
      glVertex3d( rb*cos(angle), 0, rb*sin(angle) );


			glNormal3d( cos(angle), (rb - rt) / height, sin(angle) );
			glTexCoord2f( i * (1.0/divisions), 0);
      glVertex3d( rt*cos(angle), height, rt*sin(angle) );

  }
  glEnd();

	glDisable(GL_TEXTURE_2D);
}

/* Draws a happy little leaf to go on
 * the happy little tree
*/
void leaf(float scale, unsigned int leaf_texture)
{
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, leaf_texture);

	glNormal3f(0.0, 0.0, -1.0);


	glBegin(GL_QUADS);
		glTexCoord2f(0,0); glVertex3f(0,0,0);
		glTexCoord2f(0,1); glVertex3f(0,1,0);
		glTexCoord2f(1,1); glVertex3f(1,1,0);
		glTexCoord2f(1,0); glVertex3f(1,0,0);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}




/* Makes a happy little tree
 * The tree is drawnn using a fractal that continulally redraws
 * the tree at different locations. The branches are placed
 * randomly along the upper part of the previously generated
 * limb.
*/

void tree(double levels, double height, double rbase,
					unsigned int bark_texture, unsigned int leaf_texture)
{
	shader = CreateShaderProg(NULL, "leaf.frag");
  glEnable(GL_CULL_FACE);

	height *= 0.78;
	float curLen = height * ((rand() / (float) RAND_MAX) / 2 + 0.75); // vary the length of each branch
	rbase *= 0.4;


	cone(rbase, height, bark_texture);

  glDisable(GL_CULL_FACE);
	int numLeaves = 20 - levels * levels;
	for(int i = 0; i < numLeaves; i++)
	{
		if(numLeaves > 15)
		{
			double position = curLen *(rand() / (float) RAND_MAX); //randomize base position of leaf
			double angle = 360 * (rand() / (float) RAND_MAX);

			glPushMatrix();
				glRotated(angle, 0, 1, 0);
				glTranslatef(0, position, rbase);
				glRotated(60, 1, 0, 0);



				glUseProgram(shader);
			  int id;
			  float time = 1 ? 0.001*glutGet(GLUT_ELAPSED_TIME) : 0;
			  id = glGetUniformLocation(shader,"Xcenter");
			  if (id>=0) glUniform1f(id,X);
			  id = glGetUniformLocation(shader,"Ycenter");
			  if (id>=0) glUniform1f(id,Y);
			  id = glGetUniformLocation(shader,"Zoom");
			  if (id>=0) glUniform1f(id,Z);
			  id = glGetUniformLocation(shader,"time");
			  if (id>=0) glUniform1f(id,time);


				leaf(0.17, leaf_texture);

				glUseProgram(0);

			glPopMatrix();
		}
	}

  glEnable(GL_CULL_FACE);
	if (levels != 0)
	{
		glPushMatrix();
			glRotatef(120 * (rand() / (float)RAND_MAX + 0.5), 0.0, 1.0, 0.0);

			// draw one limb at the end of the previous limb
			glPushMatrix();
				glTranslated(0.0, 0.93 * height, 0.0);
				glRotated(30, 0.0, 0.0, 1.0);
				tree(levels - 1, height, rbase, bark_texture, leaf_texture);
			glPopMatrix();

			// draw three other limbs
			for (int i=0; i < 3; i++)
			{
				glRotatef((360 / 3) * (rand() / (float)RAND_MAX + 0.5), 0.0, 1.0, 0.0);
				glPushMatrix();
				  //randomize base position for the new limb near the top portion of the previous limb
					glTranslated(0.0, ((rand() / (float)RAND_MAX) * 0.78 + 0.22) * curLen ,0.0);
					glRotated(30, 0.0, 0.0, 1.0);
					//recursive call to generate more of the tree
					tree(levels - 1, height, rbase, bark_texture, leaf_texture);
				glPopMatrix();
			}
		glPopMatrix();

	}
  glDisable(GL_CULL_FACE);
}


void cylinder(double rb, double height, int divisions,
					    unsigned int bark_texture)
{
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, bark_texture);
	glBegin(GL_QUAD_STRIP);
  for (int i = 0; i <= divisions; i++)
  {
      float angle = i*((1.0/divisions) * (2*PI));
      glNormal3d( cos(angle), rb / height, sin(angle) );
			glTexCoord2f( i * (1.0/divisions), 1);
      glVertex3d( rb*cos(angle), 0, rb*sin(angle) );


			glNormal3d( cos(angle), rb / height, sin(angle) );
			glTexCoord2f( i * (1.0/divisions), 0);
      glVertex3d( rb*cos(angle), height, rb*sin(angle) );

  }
  glEnd();

	glDisable(GL_TEXTURE_2D);
}

void entrance(double rb, unsigned int outside_tex, unsigned int edge_tex)
{
  cylinder(rb, 6, 12, outside_tex);
  cylinder(rb * 0.9, 6, 12,edge_tex);
  double center[3] = {0,0,0};
  double tex_center[2] = {0,0};
  fan(center, tex_center, rb, 9, 10, 12, 360, edge_tex);

  double center2[3] = {0,1,0};
  glColor3f(0.12,0.12,0.12);
  fan(center2, tex_center, rb * 0.95, 0, 10, 12, 360, 0);
  glColor3f(1,1,1);
}
