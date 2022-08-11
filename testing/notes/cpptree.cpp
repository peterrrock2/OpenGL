//=========================================================
//	Fractal Tree
//  Created by Viet Trinh on 1/31/13.
//  Copyright (c) 2013 Viet Trinh. All rights reserved.
//=========================================================

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#define PI 3.14159265359
using namespace std;

/*----- CLASSES DECLARATION ----------*/
//=== class Vector ===
class Vector{
public:
    float x;
    float y;
    float z;

    Vector(){x = 0.0;y=0.0;z=0.0;}
};

//=== class Tree ===
class Tree{
    float x;
    float y;
    float z;
    float radius;
    float height;
    float scale;
    int level;
public:
    Tree(){x=0;y=0;z=0;radius=0;height=0;scale=0;level=0;}
    Tree(float xp,float yp,float zp,float rp, float hp,float sp, float lp){
        x=xp;y=yp;z=zp;radius=rp;height=hp;scale=sp;level=lp;}
    float getX(){return x;}
    float getY(){return y;}
    float getZ(){return z;}
    float getR(){return radius;}
    float getH(){return height;}
    float getScale(){return scale;}
    int getLevel(){return level;}
};

/*----- GLOBAL VARIABLES -------------*/
int button, state = 1;
float gx, gy;
float x_win = 600.0;
float z_win = 600.0;
float obj_material_amb[4];
float obj_material_diff[4];
float obj_material_spec[4];
bool isDataCreated = false;
float VIEWER[3]={50.0,50.0,270.0};
float LOOKAT[3]={static_cast<float>(x_win/2.0),0.0,static_cast<float>(z_win/2.0)};
Vector direction;           // direction vector from viewer to look-at-point
Tree* trees[5];             // Arrays of trees

/*----- FUNCTIONS DECLARATION --------*/
void screenSetUp();
void displayScreen();
void mouseClicks(int but,int sta,int x,int y);
void mouseMoves(int x, int y);
void keyPresses(unsigned char c, int x, int y);
void RotateLeftRight(float angle);
void MoveUpDown(float coef);

void setTree();
void DrawForest();
void drawTree(float r, float h, int level);

//===== main function ==================================
int main(int argc, char**argv){

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	// Create a window
	glutCreateWindow("Fractal Tree");
	glutPositionWindow(0, 0);
	glutReshapeWindow(x_win, z_win);

	// Program start here...
	glutDisplayFunc(displayScreen);
	glutMouseFunc(mouseClicks);
	glutMotionFunc(mouseMoves);
    glutKeyboardFunc(keyPresses);
	glutMainLoop();
	return 0;
}

//===== screenSetUp ====================================
void screenSetUp(){

    float light_position[4] = { 0.0, 50.0, 300.0 , 0.0 };   // x, y, z, w
    // set up object color
    float obj_light_ambient[4] = { 0.2, 0.2, 0.2, 1.0 };     // r, g, b, a
    float obj_light_diffuse[4] = { 0.8, 0.3, 0.1, 1.0 };     // r, g, b, a
    float obj_light_specular[4] = { 0.8, 0.3, 0.1, 1.0 };    // r, g, b, a

    // set up background/terrain color
    float terrain_material_amb_diff[4] = { 0.5, 0.5, 0.5, 1.0 }; // r, g, b, a
    float terrain_material_specular[4] = { 0.65, 0.16, 0.16, 1.0 }; // r, g, b, a

    glClearColor(0.0,0.0,0.0,1.0);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0,x_win/z_win,5.0,5000.0);
    glViewport(0,0,x_win,z_win);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    glEnable(GL_POINT_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT, obj_light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, obj_light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, obj_light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHT0);

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, terrain_material_amb_diff);	// set up color for background/terrain
    glMaterialfv(GL_FRONT, GL_SPECULAR, terrain_material_specular);             // set up color for background/terrain

	// viewer is looking towards the center of the terrain
	gluLookAt(VIEWER[0],VIEWER[1],VIEWER[2],LOOKAT[0],LOOKAT[1],LOOKAT[2],0,1,0);

    // draw the terrain
     // a x_win x z_win rectangle with lower left corner (0,0,0)
     // up direction is y
     glBegin(GL_QUADS);
     glNormal3f(0.0,1.0,0.0);
     glVertex3f(300.0,0.0,0.0);
     glVertex3f(x_win,0.0,0.0);
     glVertex3f(x_win,0.0,z_win);
     glVertex3f(300.0,0.0,z_win);
     glEnd();

}

//===== displayScreen ===================================
void displayScreen(){

    screenSetUp();

    /*--- if all trees are created ---*/
    if (!isDataCreated) {
        setTree();
        isDataCreated = true;
    }

    DrawForest();
    glutSwapBuffers();
}

//===== mouseClicks ====================================
void mouseClicks(int but,int sta,int x,int y){
    button = but;
    state = sta;


    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){

        gx = float(x)/x_win;
        gy = float(z_win-y)/z_win;

        // code here...
    }

    glutPostRedisplay();

}

//===== mouseMoves ====================================
void mouseMoves(int x, int y){

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		gx = float(x)/x_win;
        gy = float(z_win-y)/z_win;

        // code here..

    }

    glutPostRedisplay();

}

//===== keyPresses ====================================
void keyPresses(unsigned char c, int x, int y){

    if      (c =='w'){ MoveUpDown(10.0);}
    else if (c =='x'){ MoveUpDown(-10.0);}
    else if (c =='a'){ RotateLeftRight(10*PI/180.0);}
    else if (c =='d'){ RotateLeftRight(-10*PI/180.0);}
    else {;}

    glutPostRedisplay();
}

//=================================================================
void MoveUpDown(float coef){

    float distance;

    // normalize direction vector
    direction.x = LOOKAT[0] - VIEWER[0];
    direction.y = LOOKAT[1] - VIEWER[1];
    direction.z = LOOKAT[2] - VIEWER[2];

    distance = sqrt(direction.x*direction.x + direction.y*direction.y + direction.z*direction.z);

    direction.x = direction.x/distance;
    direction.y = direction.y/distance;
    direction.z = direction.z/distance;

    // translate viewer and look-at-point positions
    VIEWER[0] += coef*direction.x;
    VIEWER[2] += coef*direction.z;
    LOOKAT[0] += (coef/2.0)*direction.x;
    LOOKAT[2] += (coef/2.0)*direction.z;

}

//=================================================================
void RotateLeftRight(float angle){
    float x1,y1,z1,x2,y2,z2;

    // translate to origin
    x1 = LOOKAT[0] - VIEWER[0];
    y1 = LOOKAT[1] - VIEWER[1];
    z1 = LOOKAT[2] - VIEWER[2];

    // rotate around a pivot
    x2 = x1*cos(angle) + z1*sin(angle);
    y2 = y1;
    z2 = -x1*sin(angle) + z1*cos(angle);

    // translate back
    LOOKAT[0] = x2 + VIEWER[0];
    LOOKAT[1] = y2 + VIEWER[1];
    LOOKAT[2] = z2 + VIEWER[2];
}

//=================================================================
void setTree(){
    trees[0] = new Tree(400.0,0.0,150.0,2,45,0.3,4);
    trees[1] = new Tree(400.0,0.0,200.0,2,55,0.3,5);
    trees[2] = new Tree(400.0,0.0,270.0,3,75,0.3,6);
    trees[3] = new Tree(400.0,0.0,350.0,3,75,0.4,6);
    trees[4] = new Tree(400.0,0.0,450.0,4,75,0.5,7);
}

//=================================================================
void drawTree(float r, float h, int level){
    GLUquadricObj *tree;

    if(level<3){    // set up material for leaves
        glEnable(GL_BLEND);                 // enable GL_BLEND because GL_BLEND is disable in previous iterations
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        obj_material_amb[0] = 34.0/255.0 ;
        obj_material_amb[1] = 139.0/255.0 ;
        obj_material_amb[2] = 34.0/255.0;
        obj_material_amb[3] = 1.0;
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, obj_material_amb);

    }
    else{           // set up material for tree
        glDisable(GL_BLEND);    // disable GL_BLEND because of showing the body of the tree
        obj_material_amb[0] = 105.0/255.0;
        obj_material_amb[1] = 105.0/255.0;
        obj_material_amb[2] = 105.0/255.0;
        obj_material_amb[3] = 1.0;
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, obj_material_amb);
    }

    // draw tree body
    tree = gluNewQuadric();
    gluQuadricDrawStyle(tree, GLU_FILL);
    glPushMatrix();
    glRotatef(-90.0, 1.0, 0.0, 0.0);
    gluCylinder(tree, r, r, h, 10, 1);
    glPopMatrix();

    if(level>0){
        // 1st branch
        glPushMatrix();
        glTranslatef(0.0, h, 0.0);
        glRotatef(60, 1.0, 0.0, 0.0);
        glRotatef(-60, 0.0, 1.0, 0.0);
        drawTree(0.6*r, 0.6*h, level-1);
        glPopMatrix();

        // 2nd branch
        glPushMatrix();
        glTranslatef(0.0, h, 0.0);
        glRotatef(20, 1.0, 0.0, 0.0);
        glRotatef(-20, 0.0, 0.0, 1.0);
        drawTree(0.6*r, 0.8*h, level-1);
        glPopMatrix();

        // 3rd branch
        glPushMatrix();
        glTranslatef(0.0, h, 0.0);
        glRotatef(-20, 1.0, 0.0, 0.0);
        glRotatef(20, 0.0, 0.0, 1.0);
        drawTree(0.6*r, 0.8*h, level-1);
        glPopMatrix();

        // 4th branch
        glPushMatrix();
        glTranslatef(0.0, h, 0.0);
        glRotatef(-50, 1.0, 0.0, 0.0);
        glRotatef(50, 0.0, 1.0, 0.0);
        drawTree(0.6*r, 0.6*h, level-1);
        glPopMatrix();
    }
}

//=================================================================
void DrawForest(){

    obj_material_amb[0] = 0.1;
    obj_material_amb[1] = 0.1;
    obj_material_amb[2] = 0.1;
    obj_material_amb[3] = 1.0;

    obj_material_diff[0] = 0.9;
    obj_material_diff[1] = 0.9;
    obj_material_diff[2] = 0.9;
    obj_material_diff[3] = 1.0;

    obj_material_spec[0] = 0.0;
    obj_material_spec[1] = 0.0;
    obj_material_spec[2] = 0.0;
    obj_material_spec[3] = 1.0;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, obj_material_amb);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, obj_material_diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, obj_material_spec);

    for (int i=0;i<5 ; i++) {
        glPushMatrix();
        glTranslatef(trees[i]->getX(), trees[i]->getY(), trees[i]->getZ());
        glScalef(trees[i]->getScale(), trees[i]->getScale(), trees[i]->getScale());
        drawTree(trees[i]->getR(), trees[i]->getH(), trees[i]->getLevel());
        glPopMatrix();
    }
}
