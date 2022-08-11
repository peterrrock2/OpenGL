#include "final_head.h"

void quad(double center[3], double dx, double dz, double theta, double phi, double psi, unsigned int texture_name, int repetitions)
{
  glEnable(GL_TEXTURE_2D);

  glPushMatrix();
    glTranslated(center[0], center[1], center[2]);
    glRotated(theta, 1, 0, 0);
    glRotated(psi, 0, 0, 1);
    glRotated(phi, 0, 1, 0);


    glScaled(dx, 0, dz);

    glBindTexture(GL_TEXTURE_2D, texture_name);

    for(int j = 0; j < repetitions; j++)
    {
      for(int i = 0; i < repetitions; i++)
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

  glPopMatrix();

  glDisable(GL_TEXTURE_2D);
}




void fan(double center[3], double tex_center[2], float r, int start, int jjmax, float pieces, float max_angle, unsigned int texture_name)
{
  glEnable(GL_TEXTURE_2D);

  glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture_name);

    float jmax = jjmax;
    float iinc = max_angle/pieces;

    for(int j = start; j < jmax; j++)
    {
      for(int i = 0; i < max_angle; i += iinc)
      {
        glBegin(GL_QUADS);
          glNormal3f(0, +1, 0);

          glTexCoord2f(2* Cos(i) * (2/jmax) * j, 2* Sin(i) * (2/jmax) * j);                       glVertex3f(center[0] + (r/jmax) * j * Cos(i), center[1], center[2] - (r/jmax) * j * Sin(i));
          glTexCoord2f(2* Cos(i) * (2/jmax) * (j + 1), 2* Sin(i) * (2/jmax) * (j + 1));           glVertex3f(center[0] + (r/jmax) * (j + 1) * Cos(i), center[1], center[2] - (r/jmax) * (j + 1) * Sin(i));
          glTexCoord2f(2* Cos(i + iinc) * (2/jmax) * (j + 1), 2* Sin(i + iinc) * (2/jmax) * (j + 1)); glVertex3f(center[0] + (r/jmax) * (j + 1) * Cos(i + iinc), center[1], center[2] - (r/jmax) * (j + 1) * Sin(i + iinc));
          glTexCoord2f(2* Cos(i + iinc) * (2/jmax) * j, 2* Sin(i + iinc) * (2/jmax) * j);             glVertex3f(center[0] + (r/jmax) * j * Cos(i + iinc), center[1], center[2] - (r/jmax) * j * Sin(i + iinc));
        glEnd();
      }
    }
  glPopMatrix();

  glDisable(GL_TEXTURE_2D);
}


void drawgeometry(unsigned int flr, unsigned int cliff, unsigned int ent_out, unsigned int ent_edge)
{
  double quad01[3] = {5, 0, -15};
  quad(quad01, 35, 15, 0, 0, 0, flr, 10);

  double quad02[3] = {25, 0, 7.5};
  quad(quad02, 15, 7.5, 0, 0, 0, flr, 5);

  double quad03[3] = {5, -2, 10};
  quad(quad03, 5, 5, 0, 0, 0, flr, 2);

  double quad04[3] = {5, -1, 2.5};
  quad(quad04, 5, 2.6926, 21.8, 0, 0, flr, 2);

  double quad05[3] = {-20, 5, 2.5};
  quad(quad05, 10, 12.5, 0, 0, 0, flr, 6);

  double quad06[3] = {-5, 5, 7.5};
  quad(quad06, 5, 7.5, 0, 0, 0, flr, 3);

  double quad07[3] = {-10, 2, -35};
  quad(quad07, 20, 5, 0, 0, 0, flr, 3);

  double quad08[3] = {-20, 1.5, -14};
  quad(quad08, 10, 5.315, -41.19, 0, 0, flr, 6);

  double quad09[3] = {25, 5, -37.5};
  quad(quad09, 15, 7.5, 0, 0, 0, flr, 6);

  double quad10[3] = {-10, 7, -40};
  quad(quad10, 20, 5, 0, 0, 0, flr, 3);





  double cliff01[3] = {0, 0, 7.5};
  quad(cliff01, 7.5, 5, 0, 90, 90, cliff, 3);

  double cliff02[3] = {10, -1, 7.5};
  quad(cliff02, 7.5, 1, 0, 90, 90, cliff, 2);

  double cliff03[3] = {-30, 7.5, -15};
  quad(cliff03, 30, 7.5, 0, 90, 90, cliff, 3);

  double cliff04[3] = {40, 7.5, -15};
  quad(cliff04, 30, 7.5, 0, 90, 90, cliff, 3);

  double cliff05[3] = {5, 5, 15};
  quad(cliff05, 35, 10, -90, 0, 0, cliff, 3);

  double cliff06[3] = {-10, 0, -11};
  quad(cliff06, 4.046, 3.1042, -41.19, 90, 90, cliff, 2);

  double cliff07[3] = {-10, 0, -30};
  quad(cliff07, 20, 2, 90, 0, 0, cliff, 2);

  double cliff08[3] = {-10, 2, -35};
  quad(cliff08, 20, 5, 90, 0, 0, cliff, 3);

  double cliff09[3] = {25, 2.5, -30};
  quad(cliff09, 15, 2.5, 90, 0, 0, cliff, 2);

  double cliff10[3] = {10, 3.5, -32.5};
  quad(cliff10, 2.5, 1.5, 90, 0, 90, cliff, 1);

  double cliff11[3] = {5, 5, -45};
  quad(cliff11, 35, 10, -90, 0, 0, cliff, 3);

  double cliff12[3] = {10, 5, -40};
  quad(cliff12, 5, 2, 90, 0, 90, cliff, 1);


  int pieces = 5;

  double fann[3] = {-10, 5, 0};
  double fann_tex[2] = {0,0};
  fan(fann, fann_tex, 10, 0, 4, pieces, 90, flr);

  double cliffer[3] = {0,2.5,0};
  double origin[3] = {0,0,0};

  for(int i = 0; i < pieces; i ++)
  {
    float angle = (45/pieces) + (90/pieces) * i;
    float dist = 10 * Cos(45 / pieces);
    cliffer[0] = -10 + dist * Cos(angle);
    cliffer[2] = -dist * Sin(angle);
    glPushMatrix();
      glTranslated(cliffer[0], cliffer[1], cliffer[2]);
      glRotated(angle, 0, 1, 0);
      quad(origin, 10 * Sin(45/pieces), 2.5, 90, 0, 90, cliff, 1);
    glPopMatrix();
  }

  glPushMatrix();
    glTranslated(-15, 6, 12);
    glRotated(90, 1, 0, 0);
    entrance(3, ent_out, ent_edge);
  glPopMatrix();

  glPushMatrix();
    glTranslated(-27, 1.5, -22);
    glRotated(-90, 0, 1, 0);
    glRotated(90, 1, 0, 0);
    entrance(3, ent_out, ent_edge);
  glPopMatrix();

  glPushMatrix();
    glTranslated(37, 1.5, -5);
    glRotated(90, 0, 1, 0);
    glRotated(90, 1, 0, 0);
    entrance(3, ent_out, ent_edge);
  glPopMatrix();

}
