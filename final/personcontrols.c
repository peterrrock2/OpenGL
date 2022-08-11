

#include "CSCIx229.h"
#include "final_head.h"

int axes=1;       //  Display axes
int mode=1;       //  Projection mode
int move=1;       //  Move light
int fPerson=0;    //  First Person decision
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=1;     //  Aspect ratio
double dim = 5.0;   //  Size of world
int scale = 1;


// Light values
int one       =   1;  // Unit value
int distance  =   5;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  20;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   4;  // Elevation of light

// Parameters for neighborhood
double xnbhd = 0.0;
double ynbhd = 0.0;
double znbhd = 0.0;
int rings = 6;


double xperson = -1.0;
double yperson = 0.0;
double zperson = -1.0;
int pth=0;         //  Azimuth of view angle of person
int pph=0;         //  Elevation of view angle of person

//  Macro for sin & cos in degrees
#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))
#define PI 3.1415926535


int trees[5];  //trees list for my different trees
int sky[6];   //  Sky textures

unsigned int wood, leaff;






/*
 *  Draw vertex in polar coordinates with normal
 *  Taken from ex13
 */
static void Vertex(double th,double ph)
{
  double x = Sin(th)*Cos(ph);
  double y = Cos(th)*Cos(ph);
  double z =         Sin(ph);
  //  For a sphere at the origin, the position
  //  and normal vectors are the same
  glNormal3d(x,y,z);
  glVertex3d(x,y,z);
}



/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 *  Taken from ex13
 */
static void ball(double x,double y,double z,double r)
{
  int th,ph;
  float yellow[] = {1.0,1.0,0.0,1.0};
  float Emission[]  = {0.0,0.0,0.01*emission,1.0};
  //  Save transformation
  glPushMatrix();
  //  Offset, scale and rotate
  glTranslated(x,y,z);
  glScaled(r,r,r);
  //  White ball
  glColor3f(1,1,1);
  glMaterialf(GL_FRONT,GL_SHININESS,shiny);
  glMaterialfv(GL_FRONT,GL_SPECULAR,yellow);
  glMaterialfv(GL_FRONT,GL_EMISSION,Emission);
  //  Bands of latitude
  for (ph=-90;ph<90;ph+=inc)
  {
    glBegin(GL_QUAD_STRIP);
    for (th=0;th<=360;th+=2*inc)
    {
      Vertex(th,ph+inc);
      Vertex(th,ph);
    }
    glEnd();
  }
  //  Undo transofrmations
  glPopMatrix();
}





/*
 *  Draw sky box
 */
static void Sky(double D)
{
   glColor3f(1,1,1);
   glEnable(GL_TEXTURE_2D);

   //  Sides
   glBindTexture(GL_TEXTURE_2D,sky[0]);
   glBegin(GL_QUADS);
   glTexCoord2f(0.0,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(1.0,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(1.0,1); glVertex3f(+D,+D,-D);
   glTexCoord2f(0.0,1); glVertex3f(-D,+D,-D);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,sky[1]);
   glTexCoord2f(0.0,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(1.0,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(1.0,1); glVertex3f(+D,+D,+D);
   glTexCoord2f(0.0,1); glVertex3f(+D,+D,-D);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,sky[2]);
   glTexCoord2f(0.0,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(1.0,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(1.0,1); glVertex3f(-D,+D,+D);
   glTexCoord2f(0.0,1); glVertex3f(+D,+D,+D);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,sky[3]);
   glTexCoord2f(0.0,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(1.0,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(1.0,1); glVertex3f(-D,+D,-D);
   glTexCoord2f(0.0,1); glVertex3f(-D,+D,+D);
   glEnd();

   //  Top and bottom
   glBindTexture(GL_TEXTURE_2D,sky[4]);
   glBegin(GL_QUADS);
   glTexCoord2f(0.0,0); glVertex3f(+D,+D,-D);
   glTexCoord2f(1,0); glVertex3f(+D,+D,+D);
   glTexCoord2f(1,1); glVertex3f(-D,+D,+D);
   glTexCoord2f(0.0,1); glVertex3f(-D,+D,-D);
   glEnd();

   glBindTexture(GL_TEXTURE_2D,sky[5]);
   glTexCoord2f(1.0,1); glVertex3f(-D,-D,+D);
   glTexCoord2f(0.0,1); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.0,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(1.0,0); glVertex3f(-D,-D,-D);
   glEnd();

   glDisable(GL_TEXTURE_2D);
}









/*
 *  OpenGL (GLUT) calls this routine to display the scene
 *  Modified from ex9 in class
 */
void display()
{
  const double len=1.5;  //  Length of axes
  //  Erase the window and the depth buffer
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  //  Enable Z-buffering in OpenGL
  glEnable(GL_DEPTH_TEST);
  //  Undo previous transformations
  glLoadIdentity();
  //  Perspective - set eye position

  Sky(3.5 * dim);

  gluLookAt(xperson, yperson, zperson, xperson + Cos(pth), yperson + Sin(pph), zperson + Sin(pth), 0, 1, 0);

  glShadeModel(GL_SMOOTH);

  //  Light switch
  if (light)
  {
    //  Translate intensity to color vectors
    float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
    float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
    float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
    //  Light position
    float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
    //  Draw light position as ball (still no lighting here)
    glColor3f(1,1,1);
    ball(Position[0],Position[1],Position[2] , 0.1);
    //  OpenGL should normalize normal vectors
    glEnable(GL_NORMALIZE);
    //  Enable lighting
    glEnable(GL_LIGHTING);
    //  Location of viewer for specular calculations
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
    //  glColor sets ambient and diffuse color materials
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    //  Enable light 0
    glEnable(GL_LIGHT0);
    //  Set ambient, diffuse, specular components and position of light 0
    glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
    glLightfv(GL_LIGHT0,GL_POSITION,Position);
  }
  else
    glDisable(GL_LIGHTING);

  cube(0,0,0,0.5,0.5,0.5,0, wood);


  //  Draw axes - no lighting from here on
  glDisable(GL_LIGHTING);
  glColor3f(1,1,1);
  if (axes)
  {
    glBegin(GL_LINES);
    glVertex3d(0.0,0.0,0.0);
    glVertex3d(len,0.0,0.0);
    glVertex3d(0.0,0.0,0.0);
    glVertex3d(0.0,len,0.0);
    glVertex3d(0.0,0.0,0.0);
    glVertex3d(0.0,0.0,len);
    glEnd();
    //  Label axes
    glRasterPos3d(len,0.0,0.0);
    Print("X");
    glRasterPos3d(0.0,len,0.0);
    Print("Y");
    glRasterPos3d(0.0,0.0,len);
    Print("Z");
  }

  //  Display parameters
  glWindowPos2i(5,5);
  Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s Light=%s",
  th,ph,dim,fov,mode?"Perpective":"Orthogonal",light?"On":"Off");
  if (light)
  {
    glWindowPos2i(5,45);
    Print("Model=%s LocalViewer=%s Distance=%d Elevation=%.1f",smooth?"Smooth":"Flat",local?"On":"Off",distance,ylight);
    glWindowPos2i(5,25);
    Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shiny);
  }

  //  Render the scene and make it visible
  glFlush();
  glutSwapBuffers();
}



/*
 *  GLUT calls this routine when the window is resized
 *  Taken from ex13
 */
void idle()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}



/*
 *  GLUT calls this routine when an arrow key is pressed
 *  Modified from ex9 in class
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - decrease dim
   else if (key == GLUT_KEY_PAGE_UP)
      dim -= 0.1;
   //  PageDown key - increase dim
   else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
      dim += 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(fov, asp, dim);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}


/*
 *  GLUT calls this routine when a key is pressed
 *  Modified from ex13
 */
void key(unsigned char ch,int x,int y)
{
  //  Exit on ESC
  if (ch == 27)
    exit(0);
  //  Reset view angle
  else if (ch == '0')
    th = ph = 0;
  //  Toggle axes
  else if (ch == 'x' || ch == 'X')
    axes = 1-axes;
  //  Toggle lighting
  else if (ch == 'l' || ch == 'L')
    light = 1-light;
  //  Switch projection mode
  else if (ch == 'p' || ch == 'P')
    mode = 1-mode;
  //  Toggle light movement
  else if (ch == 'm' || ch == 'M')
    move = 1-move;
  //  Move light
  else if (ch == '<')
    zh += 1;
  else if (ch == '>')
    zh -= 1;
  //  Change field of view angle
  else if (ch == '-' && ch>1)
    fov--;
  else if (ch == '+' && ch<179)
    fov++;
  //  Light elevation
  else if (ch=='[')
    ylight -= 0.1;
  else if (ch==']')
    ylight += 0.1;




    // Move the person around using asdw
  else if (ch == 'd')
  {
    xperson -= 0.1*Sin(pth);
    zperson += 0.1*Cos(pth);
  }
  else if (ch == 'a')
  {
    xperson += 0.1*Sin(pth);
    zperson -= 0.1*Cos(pth);
  }
  else if (ch == 's')
  {
    zperson -= 0.1*Sin(pth);
    xperson -= 0.1*Cos(pth);
  }
  else if (ch == 'w')
  {
    zperson += 0.1*Sin(pth);
    xperson += 0.1*Cos(pth);
  }
  else if (ch == 'z')
  {
    yperson -= 0.1;
  }
  else if (ch == 'c')
  {
    yperson += 0.1;
  }


  // Move person's head left and right
  else if (ch == 'q')
    pth -= 3;
  else if (ch == 'e')
    pth += 3;
  else if (ch == 'r')
  {
    pph += 3;
    if (pph >= 90)
      pph = 89.9;
  }
  else if (ch == 'f')
  {
    pph -= 3;
    if (pph <= -90)
      pph = -89.9;
  }


  //  Reproject
  Project(mode?fov:0,asp,dim);
  //  Animate if requested
  glutIdleFunc(move?idle:NULL);
  //  Tell GLUT it is necessary to redisplay the scene
  glutPostRedisplay();
}




/*
 *  GLUT calls this routine when the window is resized
 *  Taken from ex9 in class
 */
void reshape(int width,int height)
{
  //  Ratio of the width to the height of the window
  asp = (height>0) ? (double)width/height : 1;
  //  Set the viewport to the entire window
  glViewport(0,0, width,height);
  //  Set projection
  Project(fov, asp, dim);
}

/*
 *  Start up GLUT and tell it what to do
 *  Modified from ex15 in class
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("Rock Final - Kokiri Forest");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);

   //  Load skybox texture
   sky[0] = LoadTexBMP("slf.bmp");
   sky[1] = LoadTexBMP("sft.bmp");
   sky[2] = LoadTexBMP("srt.bmp");
   sky[3] = LoadTexBMP("sbk.bmp");
   sky[4] = LoadTexBMP("sup.bmp");
   sky[5] = LoadTexBMP("sdn.bmp");

   wood = LoadTexBMP("wall.bmp");

   //  Pass control to GLUT so it can interact with the user


   ErrCheck("init");
   glutMainLoop();
   return 0;
}
