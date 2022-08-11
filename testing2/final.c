

#include "CSCIx229.h"
#include "final_head.h"

int axes=1;       //  Display axes
int move= 0;       //  Move light
int fPerson=0;    //  First Person decision
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=4;     //  Aspect ratio
double dim = 5.0;   //  Size of world
int scale = 1;

// Light values
int one       =   1;  // Unit value
int distance  =   60;  // Light distance

int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model

int ambient   =   20;  // Ambient intensity (%)
int diffuse   =   80;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)

int zh        =  90;  // Light azimuth
float ylight  =  30;  // Elevation of light

int fog = 0;          // Fog on/off


double xperson = 5.0;
double yperson = 2.0;
double zperson = -10.0;
int pth= 90;         //  Azimuth of view angle of person
int pph= 0;         //  Elevation of view angle of person

//  Macro for sin & cos in degrees
#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))
#define PI 3.1415926535


int trees[5];  //trees list for my different trees
int sky[6];   //  Sky textures
int geo;

// Texture names
unsigned int wood, leaff, flr, cliff, ent_out, ent_edge;




static void compilescene()
{
  //  Load textures
  wood = LoadTexBMP("bark.bmp");
  leaff = LoadTexBMP("leaff.bmp");
  flr = LoadTexBMP("Forest_Floor.bmp");
  cliff = LoadTexBMP("cliff.bmp");
  ent_out = LoadTexBMP("entrance_bark.bmp");
  ent_edge = LoadTexBMP("entrance_edge.bmp");


  trees[0] = glGenLists(1);
  glNewList(trees[0], GL_COMPILE);
  tree(5,12,8,wood,leaff);
  glEndList();

  trees[1] = glGenLists(1);
  glNewList(trees[1], GL_COMPILE);
  tree(5,15,10,wood,leaff);
  glEndList();


  trees[2] = glGenLists(1);
  glNewList(trees[2], GL_COMPILE);
  tree(5,11,6,wood,leaff);
  glEndList();

  trees[3] = glGenLists(1);
  glNewList(trees[3], GL_COMPILE);
  tree(5,11,9,wood,leaff);
  glEndList();

  trees[4] = glGenLists(1);
  glNewList(trees[4], GL_COMPILE);
  tree(5,12,8,wood,leaff);
  glEndList();

  geo = glGenLists(1);
  glNewList(geo, GL_COMPILE);
  drawgeometry(flr, cliff, ent_out, ent_edge);
  glEndList();
}




/*
 *  OpenGL (GLUT) calls this routine to display the scene
 *  Modified from ex9 in class
 */
void display()
{

  //  Erase the window and the depth buffer
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  //  Enable Z-buffering in OpenGL
  glEnable(GL_DEPTH_TEST);
  //  Undo previous transformations
  glLoadIdentity();
  //  Perspective - set eye position

  if(fog)
  {
    glEnable(GL_FOG);
    float FogCol[3]={0.6f,0.8f,0.6f}; // Define a nice light grey
    glFogfv(GL_FOG_COLOR,FogCol);     // Set the fog color
    glFogi(GL_FOG_MODE, GL_LINEAR); // Note the 'i' after glFog - the GL_LINEAR constant is an integer.
    glFogf(GL_FOG_START, 10.f);
    glFogf(GL_FOG_END, 40.f);
    glFogf(GL_FOG_DENSITY, 0.05);
  }
  else glDisable(GL_FOG);


  Sky(64 * dim, sky);

  gluLookAt(xperson, yperson, zperson, xperson + Cos(pth), yperson + Sin(pph), zperson + Sin(pth), 0, 1, 0);

  glShadeModel(GL_SMOOTH);
  glEnable(GL_NORMALIZE);

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
    ball(Position[0],Position[1],Position[2] , 0.6);
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


    // START drawing the actual scene

  glCallList(geo);

  glPushMatrix();
    glTranslated(5,-2,7);
    glScaled(0.5,0.6,0.5);
    glCallList(trees[0]);
  glPopMatrix();

  glPushMatrix();
    glTranslated(30,0,5);
    glScaled(0.5,0.6,0.5);
    glCallList(trees[1]);
  glPopMatrix();

  glPushMatrix();
    glTranslated(30,0,-20);
    glScaled(0.5,0.5,0.5);
    glCallList(trees[2]);
  glPopMatrix();

  glPushMatrix();
    glTranslated(15,0,10);
    glScaled(0.5,0.5,0.5);
    glCallList(trees[3]);
  glPopMatrix();

  glPushMatrix();
    glTranslated(-20,0,-25);
    glScaled(0.6,0.8,0.6);
    glCallList(trees[4]);
  glPopMatrix();

  glPushMatrix();
    glTranslated(-20,5,0);
    glScaled(0.5,0.5,0.5);
    glCallList(trees[2]);
  glPopMatrix();


  // END OF THE DRAWING OF THE SCENE

  glDisable(GL_FOG);
  glDisable(GL_LIGHTING);

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
   //  Move right
   if (key == GLUT_KEY_RIGHT)
   {
     xperson -= 0.5*Sin(pth);
     zperson += 0.5*Cos(pth);
   }
   //  Move left
   else if (key == GLUT_KEY_LEFT)
   {
     xperson += 0.5*Sin(pth);
     zperson -= 0.5*Cos(pth);
   }
   //  Move foreward
   else if (key == GLUT_KEY_UP)
   {
     zperson += 0.5*Sin(pth);
     xperson += 0.5*Cos(pth);
   }
   //  Move back
   else if (key == GLUT_KEY_DOWN)
   {
     zperson -= 0.5*Sin(pth);
     xperson -= 0.5*Cos(pth);
   }
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
    pth = pph = 0;
  //  Toggle lighting
  else if (ch == 'l' || ch == 'L')
    light = 1-light;
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
  else if (ch == 'v')
  {
    fog = 1 - fog;
    if(fog) diffuse = 20;
    else diffuse = 80;
  }




  // Move the person's head
  else if (ch == 'd')
  {
    pth += 3;
    pth %= 360;
  }
  else if (ch == 'a')
  {
    pth -= 3;
    pth %= 360;
  }
  else if (ch == 's')
  {
    pph -= 3;
    if (pph <= -90)
      pph = -89.9;
  }
  else if (ch == 'w')
  {
    pph += 3;
    if (pph >= 90)
      pph = 89.9;
  }
  else if (ch == 'r')
  {
    yperson += 0.1;
  }
  else if (ch == 'f')
  {
    yperson -= 0.1;
  }

  //  Reproject
  Project(fov,asp,dim);
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
   glutInitWindowSize(800,800);
   glutCreateWindow("Rock Final - Kokiri Forest");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(move?idle:NULL);

   compilescene();

   //  Load skybox texture
   sky[0] = LoadTexBMP("slf.bmp");
   sky[1] = LoadTexBMP("sft.bmp");
   sky[2] = LoadTexBMP("srt.bmp");
   sky[3] = LoadTexBMP("sbk.bmp");
   sky[4] = LoadTexBMP("sup.bmp");
   sky[5] = LoadTexBMP("sdn.bmp");


   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
