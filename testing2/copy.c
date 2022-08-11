

#include "CSCIx229.h"

int axes=1;       //  Display axes
int mode=1;       //  Projection mode
int move=1;       //  Move light
int fPerson=0;    //  First Person decision
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=1;     //  Aspect ratio
double dim=5.0;   //  Size of world
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


double xperson = 0.0;
double yperson = 0.0;
double zperson = 1.3;
int pth=0;         //  Azimuth of view angle of person
int pph=0;         //  Elevation of view angle of person

//  Macro for sin & cos in degrees
#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))
#define PI 3.1415926535


unsigned int wall, wood;



/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the z axis
 *  Taken from ex13 in class
 */
 static void cube(double x,double y,double z,
                  double dx,double dy,double dz,
                  double th)
 {
    glEnable(GL_TEXTURE_2D);

    //  Set specular color to white
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
    //  Save transformation
    glPushMatrix();
    //  Offset, scale and rotate
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    //  Cube

    //  Front
    glBindTexture(GL_TEXTURE_2D, wall);
    glBegin(GL_QUADS);
    glNormal3f( 0, 0, 1);
    glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
    glTexCoord2f(1,0); glVertex3f(+1,-1, 1);
    glTexCoord2f(1,1); glVertex3f(+1,+1, 1);
    glTexCoord2f(0,1); glVertex3f(-1,+1, 1);
    glEnd();
    //  Back
    // glBindTexture(GL_TEXTURE_2D, wall);
    glBegin(GL_QUADS);
    glNormal3f( 0, 0,-1);
    glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
    glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
    glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
    glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
    glEnd();
    //  Right
    glBindTexture(GL_TEXTURE_2D, wall);
    glBegin(GL_QUADS);
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
    glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
    glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
    glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
    glEnd();
    //  Left
    glBindTexture(GL_TEXTURE_2D, wall);
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
    glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
    glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
    glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
    glEnd();
    //  Top
    glBindTexture(GL_TEXTURE_2D, wall);
    glBegin(GL_QUADS);
    glNormal3f( 0,+1, 0);
    glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
    glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
    glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
    glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
    glEnd();
    //  Bottom
    glBindTexture(GL_TEXTURE_2D, wall);
    glBegin(GL_QUADS);
    glNormal3f( 0,-1, 0);
    glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
    glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
    glTexCoord2f(1,1); glVertex3f(+1,-1,+1);
    glTexCoord2f(0,1); glVertex3f(-1,-1,+1);
    glEnd();
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

  double divu = 6.0;
  double divv = 10.0;

  double du = (2 * PI) / divu;
  double dv = (2 * PI) / divv;

  double u = 0.0;
  double v = 0.0;

  double cx = 0.0;
  double cy = 0.0;

  //places each of the circles of the torus
  while(u < 2 * PI + du)
  {
     v = 0.0;
     glBindTexture(GL_TEXTURE_2D, wood);
     glBegin(GL_TRIANGLE_STRIP);

       while(v < 2 * PI + dv)
       {
         glNormal3d(-(-r * cos(u) * (R + r * cos(u)) * cos(v) * pow(r * r * pow(R + r * cos(u), 0.2e1), -0.1e1 / 0.2e1)),
                    -(-r * cos(u) * (R + r * cos(u)) * sin(v) * pow(r * r * pow(R + r * cos(u), 0.2e1), -0.1e1 / 0.2e1)),
                    -((-r * sin(u) * pow(cos(v), 0.2e1) * (R + r * cos(u)) - r * sin(u) * pow(sin(v), 0.2e1) * (R + r * cos(u))) * pow(r * r * pow(R + r * cos(u), 0.2e1), -0.1e1 / 0.2e1)));

         glTexCoord2f(cx,cy);
         glVertex3d((R + r * cos(u)) * cos(v),
                    (R + r * cos(u)) * sin(v),
                     r * sin(u));


         glNormal3d(-(-r * cos(u + du) * (R + r * cos(u + du)) * cos(v + dv) * pow(r * r * pow(R + r * cos(u + du), 0.2e1), -0.1e1 / 0.2e1)),
                    -(-r * cos(u + du) * (R + r * cos(u + du)) * sin(v + dv) * pow(r * r * pow(R + r * cos(u + du), 0.2e1), -0.1e1 / 0.2e1)),
                    -((-r * sin(u + du) * pow(cos(v + dv), 0.2e1) * (R + r * cos(u + du)) - r * sin(u + du) * pow(sin(v + dv), 0.2e1) * (R + r * cos(u + du))) * pow(r * r * pow(R + r * cos(u + du), 0.2e1), -0.1e1 / 0.2e1)));

         cx += 1/divv;
         glTexCoord2f(cx,cy);
         glVertex3d((R + r * cos(u + du)) * cos(v + dv),
                    (R + r * cos(u + du)) * sin(v + dv),
                     r * sin(u + du));

         v += dv;
         cx += 1/divv;
       }
    glEnd();

    u += du;
    cy += 1/divu;
  }

  glPopMatrix();
}




/*  Makes a fun neighborhood of tori with
 *  little cubes in them
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

  // float RR = R / 2;
  // float rr = r / 3;
  double x = x0 + R;
  double y = y0;
  double z = 0;

  for(int i = 0; i < numrings; i++)
  {
    x = R * cos(2 * PI * i / numrings);
    y = R * sin(2 * PI * i / numrings);
    glColor3f(rings % 3, (rings + 1) %3, (rings + 2) % 3);
    // torus(x, y, z, 90, (360 / numrings) * i, RR, rr);  // make the tori
    cube(x,y,z , 0.2,0.2,0.2 , (360 / numrings) * i); // make the cubes
  }


  glPopMatrix();
}





/*
 *  Draw vertex in polar coordinates with normal
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
         Vertex(th,ph);
         Vertex(th,ph+inc);
      }
      glEnd();
   }
   //  Undo transofrmations
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


   if (mode)
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


   // glShadeModel(GL_SMOOTH);
   glShadeModel(GL_FLAT);

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

   //  Draw neighborhood
   neighborhood(xnbhd, ynbhd, znbhd,90,90,2,0.5,rings);


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
   //  Ambient level
   else if (ch=='a' && ambient>0)
      ambient -= 5;
   else if (ch=='A' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='d' && diffuse>0)
      diffuse -= 5;
   else if (ch=='D' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='s' && specular>0)
      specular -= 5;
   else if (ch=='S' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='e' && emission>0)
      emission -= 5;
   else if (ch=='E' && emission<100)
      emission += 5;
  // Adjust the number of rings
  else if (ch == 'R')
  {
    rings += 1;
    rings = rings > 7 ? 7 : rings;
  }
  else if (ch == 'r')
  {
    rings -= 1;
    rings = rings < 1 ? 1 : rings;
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
 *  Modified from ex9 in class
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("Rock Hw6 - Textures");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   //  Load textures
   wood = LoadTexBMP("mwood.bmp");
   wall = LoadTexBMP("wall.bmp");
   //  Pass control to GLUT so it can interact with the user


   // ErrCheck("init");
   glutMainLoop();
   return 0;
}
