

#include "CSCIx229.h"
#include "final_head.h"

int axes=1;       //  Display axes
int move=1;       //  Move light
int fPerson=0;    //  First Person decision
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=4;     //  Aspect ratio
double dim = 5.0;   //  Size of world
int scale = 1;
// int shader = 0;


int side=0;       //  Two sided mode
int ntex=1;       //  Texture mode
int Th=0,Ph=30;   //  Light angles
float sco=180;    //  Spot cutoff angle
float Exp=0;      //  Spot exponent
int at0=100;      //  Constant  attenuation %
int at1=0;        //  Linear    attenuation %
int at2=0;        //  Quadratic attenuation %
// Light values
int num       =   1;  // Number of quads
int inf       =   0;  // Infinite distance light


int emission  =   0;  // Emission intensity (%)
float shiny   =   1;  // Shininess (value)
float XX       = 0;    // Light X position
float YY       = 0;    // Light Y position
float ZZ       = 1;    // Light Z position




// Light values
int one       =   1;  // Unit value
int distance  =   40;  // Light distance

int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model

int ambient   =   30;  // Ambient intensity (%)
int diffuse   = 100;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)

int zh        =  90;  // Light azimuth
float ylight  =  14;  // Elevation of light


double xperson = 5.0;
double yperson = 2.0;
double zperson = -5.0;
int pth= 0;         //  Azimuth of view angle of person
int pph= 0;         //  Elevation of view angle of person

//  Macro for sin & cos in degrees
#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))
#define PI 3.1415926535


int trees[5];  //trees list for my different trees
int sky[6];   //  Sky textures

unsigned int wood, leaff, flr, cliff;




static void compiletrees()
{
  //  Load textures
  wood = LoadTexBMP("bark.bmp");
  leaff = LoadTexBMP("leaff.bmp");


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
  tree(6,11,6,wood,leaff);
  glEndList();

  trees[3] = glGenLists(1);
  glNewList(trees[3], GL_COMPILE);
  tree(6,11,9,wood,leaff);
  glEndList();

  trees[4] = glGenLists(1);
  glNewList(trees[4], GL_COMPILE);
  tree(6,12,8,wood,leaff);
  glEndList();
}


static void quad(double center[3], double dx, double dz, double theta, double phi, double psi, unsigned int texture_name, int repetitions)
{
  glEnable(GL_TEXTURE_2D);

  glPushMatrix();
    glTranslated(center[0], center[1], center[2]);
    glRotated(psi, 0, 0, 1);
    glRotated(phi, 0, 1, 0);
    glRotated(theta, 1, 0, 0);
    glScaled(dx, 0, dz);

    glBindTexture(GL_TEXTURE_2D, texture_name);

    for(int j = 0; j < repetitions; j++)
    {
      for(int i = 0; i < repetitions; i++)
      {
        // if((i+j) % 2 == 0)
        {
          glBegin(GL_QUADS);
            glNormal3f(0,+1, 0);

            glTexCoord2f(0,0); glVertex3f(-1 + (2.0/repetitions)*i, 0, 1 - (2.0/repetitions)*j);
            glTexCoord2f(1,0); glVertex3f(-1 + (2.0/repetitions)*(i + 1), 0, 1 - (2.0/repetitions)*j);
            glTexCoord2f(1,1); glVertex3f(-1 + (2.0/repetitions)*(i + 1), 0, 1 - (2.0/repetitions)*(j + 1));
            glTexCoord2f(0,1); glVertex3f(-1 + (2.0/repetitions)*i, 0, 1 - (2.0/repetitions)*(j + 1));

          glEnd();
        }
      }
    }

  glPopMatrix();

  glDisable(GL_TEXTURE_2D);
}


static void fan(double center[3], float pieces, unsigned int texture_name)
{
  glEnable(GL_TEXTURE_2D);

  glPushMatrix();
    glTranslated(center[0], center[1], center[2]);

    glBindTexture(GL_TEXTURE_2D, texture_name);

    float jmax = 4;
    float iinc = 90/pieces;

    for(int j = 0; j < jmax; j++)
    {
      for(int i = 0; i < 90; i += iinc)
      {
        // if(i % 20 == 0 && j % 2 == 0)
        {
        glBegin(GL_QUADS);
          glNormal3f(0, +1, 0);

          glTexCoord2f(2* Cos(i) * (2/jmax) * j, 2* Sin(i) * (2/jmax) * j);                       glVertex3f(-10 + (10/jmax) * j * Cos(i), 5, -(10/jmax) * j * Sin(i));
          glTexCoord2f(2* Cos(i) * (2/jmax) * (j + 1), 2* Sin(i) * (2/jmax) * (j + 1));           glVertex3f(-10 + (10/jmax) * (j + 1) * Cos(i), 5, -(10/jmax) * (j + 1) * Sin(i));
          glTexCoord2f(2* Cos(i + iinc) * (2/jmax) * (j + 1), 2* Sin(i + iinc) * (2/jmax) * (j + 1)); glVertex3f(-10 + (10/jmax) * (j + 1) * Cos(i + iinc), 5, -(10/jmax) * (j + 1) * Sin(i + iinc));
          glTexCoord2f(2* Cos(i + iinc) * (2/jmax) * j, 2* Sin(i + iinc) * (2/jmax) * j);             glVertex3f(-10 + (10/jmax) * j * Cos(i + iinc), 5, -(10/jmax) * j * Sin(i + iinc));
        glEnd();
        }
      }
    }
  glPopMatrix();

  glDisable(GL_TEXTURE_2D);
}




/*
 *  OpenGL (GLUT) calls this routine to display the scene
 *  Modified from ex9 in class
 */
void display()
{
  float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
  //  Erase the window and the depth buffer
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  //  Enable Z-buffering in OpenGL
  glEnable(GL_DEPTH_TEST);
  //  Undo previous transformations
  glLoadIdentity();
  //  Perspective - set eye position

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
     //  Spotlight color and direction
     float white[] = {1.0,1.0,1.0,1.0};
     float Direction[] = {Cos(Th)*Sin(Ph),Sin(Th)*Sin(Ph),-Cos(Ph),0};
     //  Draw light position as ball (still no lighting here)
     ball(Position[0],Position[1],Position[2] , 0.1);
     //  OpenGL should normalize normal vectors
     glEnable(GL_NORMALIZE);
     //  Enable lighting
     glEnable(GL_LIGHTING);
     //  Location of viewer for specular calculations
     glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
     //  Two sided mode
     glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,side);
     //  glColor sets ambient and diffuse color materials
     glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
     glEnable(GL_COLOR_MATERIAL);
     //  Set specular colors
     glMaterialfv(GL_FRONT,GL_SPECULAR,white);
     glMaterialf(GL_FRONT,GL_SHININESS,shiny);
     //  Enable light 0
     glEnable(GL_LIGHT0);
     //  Set ambient, diffuse, specular components and position of light 0
     glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
     glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
     glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
     glLightfv(GL_LIGHT0,GL_POSITION,Position);
     //  Set spotlight parameters
     glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,Direction);
     glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,sco);
     glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,Exp);
     //  Set attenuation
     glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION ,at0/100.0);
     glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION   ,at1/100.0);
     glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,at2/100.0);
  }
  else
    glDisable(GL_LIGHTING);








// THIS IS WHERE THE SCENE IS BUILT FOR NOW. WILL MOVE TO SEPARATE FILE
// AND PROBABLY PLACED IN A DISPLAY LIST TO LOWER THE RENDER TIME



  // double quad01[3] = {5, 0, -15};
  // quad(quad01, 35, 15, 0, 0, 0, flr, 10);
  //
  //
  // double quad02[3] = {25, 0, 7.5};
  // quad(quad02, 15, 7.5, 0, 0, 0, flr, 5);
  //
  // double quad03[3] = {5, -2, 10};
  // quad(quad03, 5, 5, 0, 0, 0, flr, 2);
  //
  // double quad04[3] = {5, -1, 2.5};
  // quad(quad04, 5, 2.6926, 21.8, 0, 0, flr, 2);
  //
  // double quad05[3] = {-20, 5, 2.5};
  // quad(quad05, 10, 12.5, 0, 0, 0, flr, 6);
  //
  // double quad06[3] = {-5, 5, 7.5};
  // quad(quad06, 5, 7.5, 0, 0, 0, flr, 3);
  //
  // double quad07[3] = {5, 2, -32.5};
  // quad(quad07, 35, 2.5, 0, 0, 0, flr, 3);
  //
  // double quad08[3] = {-20, 1.5, -14};
  // quad(quad08, 10, 5.315, -41.19, 0, 0, flr, 6);
  //
  //
  //
  //
  // double cliff01[3] = {0, 0, 7.5};
  // quad(cliff01, 7.5, 5, 90, 90, 0, cliff, 3);
  //
  // double cliff02[3] = {10, -1, 7.5};
  // quad(cliff02, 7.5, 1, -90, 90, 0, cliff, 2);
  //
  // double cliff03[3] = {-30, 7.5, -10};
  // quad(cliff03, 25, 7.5, 90, 90, 0, cliff, 5);
  //
  // double cliff04[3] = {40, 7.5, -10};
  // quad(cliff04, 25, 7.5, -90, 90, 0, cliff, 5);
  //
  // double cliff05[3] = {5, 5, 15};
  // quad(cliff05, 35, 10, -90, 0, 0, cliff, 8);
  //
  // double cliff06[3] = {-10, 0, -10};
  // quad(cliff06, 3, 3.792, 20, 0, 90, cliff, 2);
  //
  //
  //
  // int pieces = 5;
  //
  // double fann[3] = {0, 0, 0};
  // fan(fann, pieces, flr);
  //
  // double cliffer[3] = {0,2.5,0};
  //
  // for(int i = 0; i < pieces; i ++)
  // {
  //   float angle = (45/pieces) + (90/pieces) * i;
  //   float dist = 10 * Cos(45 / pieces);
  //   cliffer[0] = -10 + dist * Cos(angle);
  //   cliffer[2] = -dist * Sin(angle);
  //   quad(cliffer, 10 * Sin(45/pieces), 2.5, 90, 90 + angle, cliff, 1);
  // }


  glPushMatrix();
    glTranslated(5,-2,7);
    glScaled(0.5,0.6,0.5);
    glCallList(trees[0]);
  glPopMatrix();

  // glPushMatrix();
  //   glTranslated(30,0,5);
  //   glScaled(0.5,0.6,0.5);
  //   glCallList(trees[1]);
  // glPopMatrix();
  //
  // glPushMatrix();
  //   glTranslated(30,0,-20);
  //   glScaled(0.5,0.5,0.5);
  //   glCallList(trees[2]);
  // glPopMatrix();
  //
  // glPushMatrix();
  //   glTranslated(15,0,10);
  //   glScaled(0.5,0.5,0.5);
  //   glCallList(trees[3]);
  // glPopMatrix();
  //
  // glPushMatrix();
  //   glTranslated(-20,0,-25);
  //   glScaled(0.6,0.8,0.6);
  //   glCallList(trees[4]);
  // glPopMatrix();
  //
  // glPushMatrix();
  //   glTranslated(-20,5,0);
  //   glScaled(0.5,0.5,0.5);
  //   glCallList(trees[2]);
  // glPopMatrix();




// END OF THE DRAWING OF THE SCENE


  glDisable(GL_LIGHTING);

  float len = 3;

  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1,1);

  glBegin(GL_LINES);
  glVertex3d(0,0,0);
  glVertex3d(0+len,0,0);
  glVertex3d(0,0,0);
  glVertex3d(0,0+len,0);
  glVertex3d(0,0,0);
  glVertex3d(0,0,0+len);
  glEnd();
  //  Label axes
  glRasterPos3d(0.1+len,0.1,0.1);
  Print("X");
  glRasterPos3d(0.1,0.1+len,0.1);
  Print("Y");
  glRasterPos3d(0.1,0.1,0.1+len);
  Print("Z");




  //  Display parameters
  glWindowPos2i(5,5);
  Print("Angle=%d,%d  Dim=%.1f FOV=%d Light=%s",
  pth,pph,dim,fov,light?"On":"Off");
  if (light)
  {
    glWindowPos2i(5,45);
    Print("Model=%s LocalViewer=%s Distance=%d Elevation=%.1f",smooth?"Smooth":"Flat",local?"On":"Off",distance,ylight);
    glWindowPos2i(5,25);
    Print("Ambient=%d  Diffuse=%d Specular=%d",ambient,diffuse,specular);
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
   //  Move right
   if (key == GLUT_KEY_RIGHT)
   {
     xperson -= 0.1*Sin(pth);
     zperson += 0.1*Cos(pth);
   }
   //  Move left
   else if (key == GLUT_KEY_LEFT)
   {
     xperson += 0.1*Sin(pth);
     zperson -= 0.1*Cos(pth);
   }
   //  Move foreward
   else if (key == GLUT_KEY_UP)
   {
     zperson += 0.1*Sin(pth);
     xperson += 0.1*Cos(pth);
   }
   //  Move back
   else if (key == GLUT_KEY_DOWN)
   {
     zperson -= 0.1*Sin(pth);
     xperson -= 0.1*Cos(pth);
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




  // Move the person's head
  else if (ch == 'd')
    pth += 3;
  else if (ch == 'a')
    pth -= 3;
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
   glutInitWindowSize(600,600);
   glutCreateWindow("Rock Final - Kokiri Forest");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);

   compiletrees();

   // shader = CreateShaderProg("leaf.vert","leaf.frag");

   //  Load skybox texture
   sky[0] = LoadTexBMP("slf.bmp");
   sky[1] = LoadTexBMP("sft.bmp");
   sky[2] = LoadTexBMP("srt.bmp");
   sky[3] = LoadTexBMP("sbk.bmp");
   sky[4] = LoadTexBMP("sup.bmp");
   sky[5] = LoadTexBMP("sdn.bmp");

   wood = LoadTexBMP("wall.bmp");
   flr = LoadTexBMP("Forest_Floor.bmp");
   cliff = LoadTexBMP("cliff.bmp");
   leaff = LoadTexBMP("leaf.bmp");

   //  Pass control to GLUT so it can interact with the user


   ErrCheck("init");
   glutMainLoop();
   return 0;
}
