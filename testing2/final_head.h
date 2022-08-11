#ifndef final_head
#define final_head

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "CSCIx229.h"

#ifdef USEGLEW
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define PI 3.1415926535

#ifdef __cplusplus
extern "C" {
#endif


void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, unsigned int texture);
//
char* ReadText(char *file);
//
void PrintShaderLog(int obj,char* file);
//
void PrintProgramLog(int obj);
//
int CreateShader(GLenum type,char* file);
//
int CreateShaderProg(char* VertFile, char* FragFile);
//
void cone(double rb, double height,
					unsigned int bark_texture);
//
void leaf(float scale, unsigned int leaf_texture);
//
void tree(double levels, double height, double rbase,
					unsigned int bark_texture, unsigned int leaf_texture);
//
void cylinder(double rb, double height, int divisions,
					    unsigned int bark_texture);
//
void entrance(double rb, unsigned int outside_tex, unsigned int edge_tex);
//
void Vertex(double th,double ph);
//
void ball(double x,double y,double z,double r);
//
void Sky(double D, int* sky);
//
void quad(double center[3], double dx, double dz, double theta, double phi, double psi, unsigned int texture_name, int repetitions);
//
void fan(double center[3], double tex_center[2], float r, int start, int jjmax,
         float pieces, float max_angle, unsigned int texture_name);
//
void drawgeometry(unsigned int flr, unsigned int cliff, unsigned int ent_out, unsigned int ent_edge);


#ifdef __cplusplus
}
#endif



#endif
