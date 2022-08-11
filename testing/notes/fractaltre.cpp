/**
 * File:   glFractal_Tree.cpp - Implementation of glObjectRendering.h
 *         ----------------------------------------------------------
 *
 * Author: Peter Dalton
 * Date:   August 23, 1999
 *
 * Last Modification: August 23, 1999
 *
 * Copyright (C) 1999 by Peter Dalton.
 *     ALL RIGHTS RESERVED.
 *
 *     Permission to use, copy, modify, and distribute this software
 *     for any purpose and without fee is hereby granted, provided that
 *     the above copyright notice appear in all copies and that both
 *     copyright notice and this notice appear in supporting documentation.
 *
 *     THIS SOFTWARE AND ANY ASSOCIATED MATERIALS IS PROVIDED TO YOU
 *     "AS-IS" AND WITHOUT REPRESENTATION OR WARRANTY OF ANY KIND,
 *     EXPRESS, IMPLIED OR OTHERWISE.  I DO NOT WARRANT THAT USE OF THE
 *     SOFTWARE WILL BE UNINTERRUPTED OR ERROR-FREE, OR THAT DEFECTS IN
 *     THE SOFTWARE WILL BE CORRECTED.
 *
 * OS: Generic -> Contains OpenGL Rendering Routines.
 *
 * Description:
			Creates the display list to render a Fractal Tree : dList_FRACT_TREE.
 */

#include <GL\glut.h>           // Contains OpenGL rendering routines
#include <time.h>              // Used to seed the random number generator
#include <limits.h>            // Required for predefined constants
#include <stdlib.h>            // Includes a random number generator

#include "glObjectRendering.h"

// *** Macros
#ifdef WIN32              // Windows 95/98 or NT
	#define drand48()  (((float)rand())/((float)RAND_MAX))
	#define srand48(x) (srand (x))
#endif

// *** Global Variables
GLuint STEM, LEAF, LEAF_MAT, TREE_MAT, STEMANDLEAVES;    // Display Lists
int   dList_FRACT_TREE = -1;        // Display List first defined in GObject.h
float tree_seed;                    // Random seed for the fractal_trees.

// *** Function Prototypes
void SetupMaterials (void);                       // Material Properties.
void CreateTreeLists (void);                      // Creates the STEMANDLEAVES display list.
void CreateTree (int fractal_level);              // Creates the main dList_FRACT_TREE dList.
void FractalTree (int level, int fractal_level);  // Recursive generates the fractal tree.


/**
 * CreateTree_DisplayList ():
 *
 *	@param - int fractal_level: The desired fractal_level of the fractal tree.
 */
void CreateTree_DisplayList (int fractal_level)
{
	tree_seed = time (NULL);          // Random number generator

	SetupMaterials ();                // Set up materials
	CreateTreeLists ();               // Set up intermediate display lists
	CreateTree (fractal_level);       // Generate the main display list: dList_FRACT_TREE
}

/**
 * SetupMaterials ():
 *	Responsible for creating the TREE_MAT and LEAF_MAT display lists.  These material
 *	display list hold the material properties of the components of the tree.
 */
void SetupMaterials (void)
{
	GLfloat tree_ambuse[] = {0.4f, 0.25f, 0.1f, 1.0f};
	GLfloat tree_specular[] = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat tree_shininess[] = {0.0f};

	GLfloat leaf_ambuse[] = {0.0f, 0.8f, 0.0f, 1.0f};
	GLfloat leaf_specular[] = {0.0f, 0.8f, 0.0f, 1.0f};
	GLfloat leaf_shininess[] = {10.0f};

	TREE_MAT = glGenLists (1);             // Tree material: Brown bark

	glNewList (TREE_MAT, GL_COMPILE);
		glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, tree_ambuse);
		glMaterialfv (GL_FRONT, GL_SPECULAR, tree_specular);
		glMaterialfv (GL_FRONT, GL_SHININESS, tree_shininess);
	glEndList ();

	LEAF_MAT = glGenLists (1);             // Leaf material: Green leafs

	glNewList (LEAF_MAT, GL_COMPILE);
		glMaterialfv (GL_FRONT, GL_AMBIENT_AND_DIFFUSE, leaf_ambuse);
		glMaterialfv (GL_FRONT, GL_SPECULAR, leaf_specular);
		glMaterialfv (GL_FRONT, GL_SHININESS, leaf_shininess);
	glEndList ();
}

/**
 * CreateTreeLists ():
 *	Responsible for creating the STEM, LEAF, and STEMANDLEAVES display lists.
 *	These display lists are used later to increase the rendering speed.
 */
void CreateTreeLists (void)
{
	GLUquadricObj* cylquad = gluNewQuadric ();

	STEM = glGenLists (1);

	glNewList (STEM, GL_COMPILE);               // Generate the STEM display list.  A cylinder
	glPushMatrix ();                            //  wider at the bottom and narrower towards the
		glRotatef (-90, 1, 0, 0);                 //  top.
		gluCylinder (cylquad, 0.1f, 0.08f, 1, 10, 2);
	glPopMatrix ();
	glEndList ();

	LEAF = glGenLists (1);

	glNewList (LEAF, GL_COMPILE);               // Generate the LEAF display list.  Made up of
		glBegin (GL_TRIANGLES);                   //  two simple triangles.
			glNormal3f (-0.3714f, 0.0, 0.9285f);
			glVertex3f (0, 0, 0);
			glVertex3f (0.25f, 0.25f, 0.1f);
			glVertex3f (0, 0.5f, 0);

			glNormal3f (0.3714f, 0, 0.9285f);
			glVertex3f (0, 0, 0);
			glVertex3f (0, 0.5, 0);
			glVertex3f (-0.25f, 0.25f, 0.1f);
		glEnd ();
	glEndList ();

	STEMANDLEAVES = glGenLists (1);

	glNewList (STEMANDLEAVES, GL_COMPILE);      // Now use the STEM and LEAF display lists to
	glPushMatrix ();                            //  generate a STEMANDLEAVES display list.
	glPushAttrib (GL_LIGHTING_BIT);             //  Each stem has three leaves associated with
		glCallList (STEM);                        //  it
		glCallList (LEAF_MAT);
		for (int i = 0; i < 3; i++) {
			glTranslatef (0, 0.333f, 0);
			glRotatef (90, 0, 1, 0);
			glPushMatrix ();
				glRotatef (0, 0, 1, 0);
				glRotatef (50, 1, 0, 0);
				glCallList (LEAF);
			glPopMatrix ();
			glPushMatrix ();
				glRotatef (180, 0, 1, 0);
				glRotatef (60, 1, 0, 0);
				glCallList (LEAF);
			glPopMatrix ();
		}
	glPopAttrib ();
	glPopMatrix ();
	glEndList ();
}

/**
 * CreateTree ():
 *	Responsible for creating the TREE display list by calling the recursive fractal
 *	function.
 *
 *	@param - int fractal_level: The desired fractal level for the fractal trees.
 */
void CreateTree (int fractal_level)
{
	srand48 (tree_seed);

	dList_FRACT_TREE = glGenLists (1);

	glNewList (dList_FRACT_TREE, GL_COMPILE); // Create the dList_FRACT_TREE display list.
		glPushMatrix ();
		glPushAttrib (GL_LIGHTING_BIT);
		glCallList (TREE_MAT);                  // Setup the tree material.
		glTranslatef (0, -1, 0);
		FractalTree (0, fractal_level);         // Recursively generate the tree.
		glPopAttrib ();
		glPopMatrix ();
	glEndList ();
}

/**
 * FractalTree ():
 *	Recursively renders the fractal tree given a desired depth.  Uses the previously
 *	created display lists to speedup the process.
 *
 *	@param - int level: The current level of the fractal tree, usually called with zero.
 *  @param - int fractal_level: The final desired fractal level.
 */
void FractalTree (int level, int fractal_level)
{
	long savedseed;                         // Need to save seed while building tree too.

	if (level == fractal_level) {           // Base Case: current depth == desired depth
		glPushMatrix ();
			glRotatef (drand48()*180, 0, 1, 0);
			glCallList (STEMANDLEAVES);
		glPopMatrix ();
	}
	else {
		glCallList (STEM);                    // Draw a stem and call FractalTree 3 times
		glPushMatrix ();                      //  recursively do obtain 3-way branching
			glRotatef (drand48 ()*180, 0, 1, 0);
			glTranslatef (0, 1, 0);
			glScalef (0.7f, 0.7f, 0.7f);

			savedseed = (long) drand48 () * ULONG_MAX;   // First branch
			glPushMatrix ();
				glRotatef (110 + drand48 ()*40, 0, 1, 0);
				glRotatef (30 + drand48 () * 20, 0, 0, 1);
				FractalTree (level + 1, fractal_level);
			glPopMatrix ();

			srand48 (savedseed);
			savedseed = (long) drand48 () * ULONG_MAX;   // Second branch
			glPushMatrix ();
				glRotatef (-130 + drand48 ()*40, 0, 1, 0);
				glRotatef (30 + drand48 () * 20, 0, 0, 1);
				FractalTree (level + 1, fractal_level);
			glPopMatrix ();

			srand48 (savedseed);                         // Third branch
			glPushMatrix ();
				glRotatef (-20 + drand48 ()*40, 0, 1, 0);
				glRotatef (30 + drand48 () * 20, 0, 0, 1);
				FractalTree (level + 1, fractal_level);
			glPopMatrix ();

		glPopMatrix ();
	}
}
