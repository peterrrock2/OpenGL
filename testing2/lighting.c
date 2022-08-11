#include "final_head.h"


/*
 *  Draw vertex in polar coordinates with normal
 *  Taken from ex13
 */
void Vertex(double th,double ph)
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
void ball(double x,double y,double z,double r)
{
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  Yellow ball
   glColor3f(1,1,1);
   glutSolidSphere(1.0,16,16);
   //  Undo transofrmations
   glPopMatrix();
}



/*
 *  Draw sky box
 */
void Sky(double D, int* sky)
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
