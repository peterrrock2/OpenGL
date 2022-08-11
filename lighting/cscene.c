#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int axes=0;       //  Display axes
int mode=0;       //  Projection mode
int fPerson=0;    //  First Person decision
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=5.0;   //  Size of world
int scale = 1;


// Parameters for neighborhood
double xnbhd = 0.0;
double ynbhd = 0.0;
double znbhd = 0.0;
int rings = 4;


double xperson = 0.0;
double yperson = 0.0;
double zperson = 1.3;
int pth=0;         //  Azimuth of view angle of person
int pph=0;         //  Elevation of view angle of person

//  Macro for sin & cos in degrees
#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))
#define PI 3.1415926535


/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 *  Taken from ex9 in class
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}



/*
 *  Set projection
 */
 static void Project()
 {
    //  Tell OpenGL we want to manipulate the projection matrix
    glMatrixMode(GL_PROJECTION);
    //  Undo previous transformations
    glLoadIdentity();
    if (fPerson)
       gluPerspective(fov,asp,dim/4,10*dim);
    //  Perspective transformation
    else if (mode)
       gluPerspective(fov,asp,dim/4,4*dim);
    //  Orthogonal projection
    else
       glOrtho(-asp*dim,+asp*dim, -dim,+dim, -4*dim,+4*dim);
    //  Switch to manipulating the model matrix
    glMatrixMode(GL_MODELVIEW);
    //  Undo previous transformations
    glLoadIdentity();
 }


/*
 *  Draw a house
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the z axis
 *  Modified from ex13 in class
 */
static void house(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float black[] = {0,0,0,1};
   glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,0,1);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
   //  Front
   glColor3f(1,0,0);
   glNormal3f(0, 0, 1);
   glVertex3f(-1.2,-1.2, 1);
   glVertex3f(+1.2,-1.2, 1);
   glVertex3f(+1.2,+1.2, 1);
   glVertex3f(-1.2,+1.2, 1);


   //  Back
   glColor3f(0,0,1);
   glNormal3f( 0, 0,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glColor3f(1,1,0);
   glNormal3f(+1, 0, 0);
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left
   glColor3f(0,1,0);
   glNormal3f(-1, 0, 0);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glColor3f(0,1,1);
   glNormal3f( 0,+1, 0);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   glColor3f(1,0,1);
   glNormal3f( 0,-one, 0);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();


   glBegin(GL_TRIANGLES);

   // Make Roof of House
   glColor3f(0,0,1);
   glVertex3f(-1.2,-1.2, 1);
   glVertex3f(0,0, 1.8);
   glVertex3f(-1.2,+1.2, 1);

   glVertex3f(-1.2,-1.2, 1);
   glVertex3f(0,0, 1.8);
   glVertex3f(+1.2,-1.2, 1);

   glVertex3f(+1.2,+1.2, 1);
   glVertex3f(0,0, 1.8);
   glVertex3f(-1.2,+1.2, 1);

   glVertex3f(+1.2,+1.2, 1);
   glVertex3f(0,0, 1.8);
   glVertex3f(+1.2,-1.2, 1);

   glEnd();
   //  Undo transofrmations
   glPopMatrix();
}



static void plane(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
   //  Save transformation
   glPushMatrix();
   //  Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Cube
   glBegin(GL_QUADS);
   //  Back
   glColor3f(1, 1, 1);

   glVertex3f(+2,+2,0);
   glVertex3f(-2,+2,0);
   glVertex3f(-2,-2,0);
   glVertex3f(+2,-2,0);
   //  End
   glEnd();
   //  Undo transofrmations
   glPopMatrix();
}



/* Builds a torus in the xy - plane according to the standard
 * parameterization given by
 *        x = (R + r * cos(theta)) * cos(phi)
 *        y = (R + r * cos(theta)) * cos(phi)
 *        z = r * sin(theta)
 * The variables have be switched so taht u = theta and v = phi
 * to avoid varable conflicts.
 */

static void torus(double x,double y,double z,
                  double th, double ph,
                  float R, float r)
{
  glPushMatrix();

  glTranslated(x,y,z);
  glRotated(ph, 0, 0, 1);
  glRotated(th, 0, 1, 0);


   double du = (2 * PI) / 200;
   double dv = (2 * PI) / 200;

   double u = 0.0;
   double v = 0.0;

   //places each of the circles of the torus
   while(u < 2 * PI + du)
   {
     v = 0.0;
     glBegin(GL_TRIANGLE_STRIP);

       while(v < 2 * PI + dv)
       {
         glVertex3d((R + r * cos(u)) * cos(v),
                    (R + r * cos(u)) * sin(v),
                     r * sin(u));

         glVertex3d((R + r * cos(u + du)) * cos(v + dv),
                    (R + r * cos(u + du)) * sin(v + dv),
                     r * sin(u + du));

         v += dv;
       }
     glEnd();

     u += du;
   }

  glPopMatrix();
}

/*  Makes a fun neighborhood of tori with
 *  little houses in them
 */
static void neighborhood(double x0,double y0,double z0,
                    double th, double ph,
                    float R, float r, int numrings)
{
  glPushMatrix();

  glTranslated(x0,y0,z0);
  glRotated(ph, 0, 0, 1);
  glRotated(th, 0, 1, 0);

  glColor3f(1,0,0);
  torus(0, 0, 0, 0, 0, R*2, r); // makes a big outer red torus to contain neighborhood

  float RR = R / 2;
  float rr = r / 3;
  double x = x0 + R;
  double y = y0;
  double z = 0;

  for(int i = 0; i < numrings; i++)
  {
    x = R * cos(2 * PI * i / numrings);
    y = R * sin(2 * PI * i / numrings);
    glColor3f(rings % 3, (rings + 1) %3, (rings + 2) % 3);
    torus(x, y, z, 90, (360 / numrings) * i, RR, rr);  // make the tori
    house(x,y,z , 0.2,0.2,0.2 , (360 / numrings) * i); // make the houses
  }


  glPopMatrix();
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

   if(fPerson)
   {
     gluLookAt(xperson, yperson, zperson, xperson + Sin(pth),yperson + Cos(pth),zperson, 0,0,1);
   }

   else if (mode)
   {
      double Ex = -2*dim*Sin(th)*Cos(ph);
      double Ey = +2*dim        *Sin(ph);
      double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   //  Orthogonal - set world orientation
   else
   {
      glRotatef(ph,1,0,0);
      glRotatef(th,0,1,0);
   }

   glScaled(scale,scale,scale);


   glPolygonOffset(1,1);
   plane(0,0,-0.21, 2,2,2, 0);



   //  Draw neighborhood
   neighborhood(xnbhd, ynbhd, znbhd,0,0,2,0.5,rings);


   //  Draw axes
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
   Print("Angle=%d,%d Projection=%s  Rings=%d",th,ph, mode?"Perpective":"Orthogonal", rings);
   //  Render the scene and make it visible
   glFlush();
   glutSwapBuffers();
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
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}


/*
 *  GLUT calls this routine when a key is pressed
 *  Modified from ex9 in class
 */
void key(unsigned char ch,int x,int y)
{
  //  Exit on ESC
  if (ch == 27)
    exit(0);
  //  Reset view angle
  else if (ch == '0')
  {
    dim = 5.0;
    th = ph = 0;
  }
  //  Toggle axes
  else if (ch == '1')
    axes = 1-axes;
  //  Switch display mode
  else if (ch == 'm')
    mode = 1-mode;

  else if (ch == 'p')
  {
    fPerson = 1 - fPerson;
    mode = fPerson ? 1 : mode;
    scale = fPerson ? 10 : 1;
  }

  // Move the person around using asdw
  else if (ch == 'd')
  {
    xperson += 0.5*Cos(pth);
    yperson -= 0.5*Sin(pth);
  }
  else if (ch == 'a')
  {
    xperson -= 0.5*Cos(pth);
    yperson += 0.5*Sin(pth);
  }
  else if (ch == 's')
  {
    yperson -= 0.5*Cos(pth);
    xperson -= 0.5*Sin(pth);
  }
  else if (ch == 'w')
  {
    yperson += 0.5*Cos(pth);
    xperson += 0.5*Sin(pth);
  }


  // Move person's head left and right
  else if (ch == 'q')
    pth -= 3;
  else if (ch == 'e')
    pth += 3;

  // Adjust the number of rings
  else if (ch == 'r')
  {
    rings += 1;
    rings = rings > 7 ? 7 : rings;
  }
  else if (ch == 'f')
  {
    rings -= 1;
    rings = rings < 1 ? 1 : rings;
  }


  //  Reproject
  Project();
  //  Tell GLUT to redisplay the scene
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
   Project();
}

/*
 *  Start up GLUT and tell it what to do
 *  Modified from ex9 in class
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("Rock Hw4 - Projections");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   return 0;
}
