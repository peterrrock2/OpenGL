#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int numPts = 50000;
double theta = 0;  //angle of rotation
float arrPts[50000][3];
static GLfloat view_rotx = 20.0, view_roty = 30.0, view_rotz = 0.0;
// static GLfloat view_posz = 0.0;

/*  Lorenz Parameters  */
double s  = 10;
double b  = 2.6666;
double r  = 28;

int iteration = 0;
int incIteration = 10;

double zoom = -60.0;


#define LEN 8192  // Maximum length of text string
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
 *  Function to print any errors encountered
 */
void ErrCheck(char* where)
{
   int err = glGetError();
   if (err) fprintf(stderr,"ERROR: %s [%s]\n",gluErrorString(err),where);
}


void lorenz(void)
{
   int i;
   /*  Coordinates  */
   double x = 1;
   double y = 1;
   double z = 1;
   /*  Time step  */
   double dt = 0.001;

   /*
    *  Integrate 50,000 steps (50 time units with dt = 0.001)
    *  Explicit Euler integration
    */
   for (i = 0; i < numPts; i++)
   {
      double dx = s*(y-x);
      double dy = x*(r-z)-y;
      double dz = x*y - b*z;
      x += dt*dx;
      y += dt*dy;
      z += dt*dz;

      arrPts[i + 1][0] = x;
      arrPts[i + 1][1] = y;
      arrPts[i + 1][2] = z;
   }
}




static void display(void)
{
  lorenz();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



  glLoadIdentity();
    //set the view of the user
    glTranslatef(0.0, 0.0, zoom);
    glRotatef(view_rotx, 1.0, 0.0, 0.0);
    glRotatef(view_roty, 0.0, 1.0, 0.0);
    glRotatef(view_rotz, 0.0, 0.0, 1.0);


    //create axes labels
    glColor3f(1,1,1);
    glBegin(GL_LINES);
    glVertex3d(0,0,0);
    glVertex3d(40,0,0);
    glVertex3d(0,0,0);
    glVertex3d(0,40,0);
    glVertex3d(0,0,0);
    glVertex3d(0,0,40);
    glEnd();
    //  Label axes
    glRasterPos3d(45,0,0);
    Print("X");
    glRasterPos3d(0,45,0);
    Print("Y");
    glRasterPos3d(0,0,45);
    Print("Z");

    glWindowPos2i(5,5);
    Print("s = %lf, b = %lf, r = %lf",s,b,r);


    glBegin(GL_LINE_STRIP); //GL_LINE_STRIP will connect the dots sequentially
      // for(int i = 0; i < numPts; i++)
      // {
      //   glColor3fv(arrPts[i]);  //colors the image
      //   glVertex3fv(arrPts[i++]);
      // }
      int i = 0;

      while(i < iteration && i < numPts)
      {
        glColor3fv(arrPts[i]);  //colors the image
        glVertex3fv(arrPts[i++]);
      }

      if(iteration < numPts)  //moves attractor itteration to the next point
      {
        if(iteration + incIteration > numPts) iteration = numPts;
        else iteration += incIteration;
      }

    glEnd();

  ErrCheck("display");  //Will display any errors that appear
  glFlush();
  glutSwapBuffers();
}


//Resize the window so that the picture doesn't get messed up
static void reshape(int width, int height)
{
  GLfloat h = (GLfloat) height/ (GLfloat) width;

  glViewport(0, 0, (GLint) width, (GLint) height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-2.0, 2.0, -h*2, h*2, 1.0, 200.0); //multiply the current matrix by a perspective matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}



//key bindings to rotate the scene when arrows are pressed
void special(int k, int x, int y)
{
  switch (k)
  {
    case GLUT_KEY_UP:
			view_rotx += 5.0;
			break;
		case GLUT_KEY_DOWN:
			view_rotx -= 5.0;
			break;
		case GLUT_KEY_LEFT:
			view_roty += 5.0;
			break;
		case GLUT_KEY_RIGHT:
			view_roty -= 5.0;
			break;
    case GLUT_KEY_PAGE_UP:
      zoom += 5.0;
      break;
    case GLUT_KEY_PAGE_DOWN:
      zoom -= 5.0;
      break;
		default:
			return;
  }

  glutPostRedisplay();
}



//special key bindings to change the scene around without arrow keys
static void key (unsigned char key, int x, int y)
{
  switch (key)
  {
    case 27:
      exit(0);
      break;
    case 's':
      s += 1.0;
      break;
    case 'x':
      s -= 1.0;
      break;
    case 'd':
      b += 0.3333;
      break;
    case 'c':
      b -= 0.3333;
      break;
    case 'f':
      r += 1;
      break;
    case 'v':
      r -= 1;
      break;
    case 'r':         //resets the Lorenz animation
      iteration = 0;
      break;
    case 't':         //finishes the Lorenz animation
      iteration = numPts;
      break;
    case '0':
      s  = 10;
      b  = 2.6666;
      r  = 28;
      view_rotx = 20.0;
      view_roty = 30.0;
      view_rotz = 0.0;
      break;
    default:
      return;
  }

  glutPostRedisplay();
}



static void idle(void)
{
  iteration += incIteration;
  glutPostRedisplay();
}

static void visible(int vis)
{
  if(vis == GLUT_VISIBLE)
    glutIdleFunc(idle);
  else glutIdleFunc(NULL);
}




int main(int argc, char* argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);

  glutInitWindowPosition(300, 300);
  glutInitWindowSize(800, 800);
  glutCreateWindow("Lorenz Attractor");

  // init(argc, argv);

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(key);
  glutSpecialFunc(special);
  glutVisibilityFunc(visible);

  glutMainLoop();

  return 0;
}























//
