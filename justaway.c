/*****************************************************************************
File: opengl-anim1.c

Informatique Graphique IG1
Master d'informatique
Christian Jacquemin, Université Paris 11

Copyright (C) 2004 University Paris 11
This file is provided without support, instruction, or implied
warranty of any kind.  University Paris 11 makes no guarantee of its
fitness for a particular purpose and is not liable under any
circumstances for any damages or loss whatsoever arising from the use
or inability to use this file or items derived from it.
******************************************************************************/
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH  600
#define HEIGHT 600

#define RED   0
#define GREEN 0
#define BLUE  0
#define ALPHA 1

#define KEY_ESC 27
#define superEllipse 1
#define superTriangle 5
#define superCylindre 6
#define superSphere 7
#define superCercle 8
#define superCylindre2 9

#define PI 3.1415926535898

static GLfloat light_position0[] = { 15.0 , 15.0 , 15.0 , 0.0 };
static GLfloat light_position1[] = { 15.0 , 15.0 , -15.0 , 0.0 };

static GLfloat ambient_light0[] = { 0.0 , 0.0 , 0.0 , 0.0 };
static GLfloat diffuse_light0[] = { 0.7 , 0.7 , 0.7 , 1.0 };
static GLfloat specular_light0[] = { 0.1 , 0.1 , 0.1 , 0.1 };

static GLfloat ambient_light1[] = { 0.50 , 0.50 , 0.50 , 1.0 };
static GLfloat diffuse_light1[] = { 0.5 , 1.0 , 1.0 , 1.0 };
static GLfloat specular_light1[] = { 0.5 , 1.0 , 1.0 , 1.0 };

int angle = 0;
float dx = 0;
float dy = 0;

void init_scene();
void render_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);
void make_triangle();
void make_cylindre();
void make_cylindre2();
void make_sphere();
void make_cercle();
void make_ellipse();

void charger_texture();
GLvoid special_key();

int main(int argc, char **argv)
{
  // initialisation  des paramètres de GLUT en fonction
  // des arguments sur la ligne de commande
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

  // définition et création de la fenêtre graphique
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Tracé en volume");

  // initialisation de OpenGL et de la scène
  initGL();
  init_scene();
  //charger_texture();
  // choix des procédures de callback pour
  // le tracé graphique
  glutDisplayFunc(&window_display);
  // le redimensionnement de la fenêtre
  glutReshapeFunc(&window_reshape);
  // la gestion des événements clavier
  glutKeyboardFunc(&window_key);

  glutSpecialFunc(&special_key);

  // la boucle prinicipale de gestion des événements utilisateur
  glutMainLoop();

  return 1;
}

// initialisation du fond de la fenêtre graphique : noir opaque
GLvoid initGL()
{
  glClearColor(RED, GREEN, BLUE, ALPHA);
  glEnable(GL_DEPTH_TEST);

  // initialisation de lŽéclairement

  // définition de deux source lumineuses
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light0);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light0);
  glLightfv(GL_LIGHT1, GL_AMBIENT, ambient_light1);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse_light1);
  glLightfv(GL_LIGHT1, GL_SPECULAR, specular_light1);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
  glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

  // activation de lŽéclairement
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);

  glShadeModel( GL_SMOOTH );
  glEnable(GL_COLOR_MATERIAL);

   // mode de calcul de la texture en changement d'échelle
  // GL_LINEAR: interpolation plus fine mais plus lente
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		  GL_NEAREST);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		  GL_NEAREST);

  // mode par modulation avec couleur et luminosité de l'objet
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

}

void init_scene()
{
  // initialise des display lists du carré et du cube
  make_triangle();
  make_cylindre();
  make_cylindre2();
  make_sphere();
  make_cercle();
  make_ellipse();
}

// fonction de call-back pour lŽaffichage dans la fenêtre

GLvoid window_display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  render_scene();

  // trace la scène grapnique qui vient juste d'être définie
  glFlush();
}

// fonction de call-back pour le redimensionnement de la fenêtre

GLvoid window_reshape(GLsizei width, GLsizei height)
{
  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.5, 1.5, -1.5, 1.5, -1.5, 1.5);

  // toutes les transformations suivantes sŽappliquent au modèle de vue
  glMatrixMode(GL_MODELVIEW);
}

// fonction de call-back pour la gestion des événements clavier

GLvoid window_key(unsigned char key, int x, int y)
{
  switch (key) {
  case KEY_ESC:
    exit(1);
    break;

  case '+':
    angle++;
    glutPostRedisplay();
    break;

  case '-':
    angle--;
    glutPostRedisplay();
    break;

  default:
    printf ("La touche %d nŽest pas active.\n", key);
    break;
  }
}

GLvoid special_key(int key, int x, int y){
	switch(key){
		case GLUT_KEY_LEFT :
			dx -= 0.3;
			glutPostRedisplay();
			break;

		case GLUT_KEY_RIGHT :
			dx += 0.3;
			glutPostRedisplay();
			break;

		case GLUT_KEY_DOWN :
			dy -= 0.3;
			glutPostRedisplay();
			break;

		case GLUT_KEY_UP :
			dy += 0.3;
			glutPostRedisplay();
			break;

		default:
			printf ("La touche %d nŽest pas active.\n", key);
			break;
	}

}

// le tracé graphique cercles et ellipses


void make_triangle(){
	glNewList(superTriangle, GL_COMPILE);
		glBegin(GL_TRIANGLES);
			// Sommet A
			glVertex3f(1, -1, 0);
			// Sommet B
			glVertex3f(1, 1, 0);
			// Sommet S
			glVertex3f(0, 0, 1);
		glEnd();
	glEndList();
}

void make_cercle(){
  float rayon = 0.3;
  glNewList(superCercle, GL_COMPILE);
    glBegin( GL_TRIANGLE_FAN );
        for( float i = 0; i <= 2 * PI + 0.1; i += 0.1 ){
            glVertex2f(sin( i ) * rayon, cos( i ) * rayon );
        }
    glEnd();
  glEndList();
}


void make_cylindre(){
  GLUquadric *quad = gluNewQuadric();
  glNewList(superCylindre, GL_COMPILE);
    gluCylinder(quad, 0.3, 0.3, 1.3, 100, 100);
  glEndList();
}

void make_cylindre2(){
  GLUquadric *quad = gluNewQuadric();
  glNewList(superCylindre2, GL_COMPILE);
    gluCylinder(quad, 0.008, 0.008, 0.7, 100, 100);
  glEndList();
}

void make_sphere(){
  glNewList(superSphere, GL_COMPILE);
    glutSolidSphere(0.28, 100, 1000);
  glEndList();
}

void make_ellipse(){
  glNewList(superEllipse, GL_COMPILE);
    glBegin( GL_TRIANGLE_FAN );
        for( float i = 0; i <= 2 * PI + 0.1; i += 0.1 ){
            glVertex2f(sin( i ) * 0.18, cos( i ) * 0.18 );
        }
    glEnd();
  glEndList();
}

void charger_texture(){
  FILE * file = fopen("justaway_face.bmp", "rb");
  if(!file){
    printf("Pas pu load bmp\n");
    exit(EXIT_FAILURE);
  }

  unsigned char data[480*480*3];
  fread(data, 1, 256*256*3, file);
  fclose(file);

  GLuint textureID = 1;
  glGenTextures(1, &textureID);

  glBindTexture(GL_TEXTURE_2D, textureID);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void render_scene()
{
  glRotatef(angle, 1, 0, 0);
  glRotatef(angle, 0, 0, 0);
  glTranslatef(dx, dy, 0);

  //LE TRONC
  glPushMatrix();
  glColor3f(0.976, 0.275, 0.165); //couleur un peu rouge
  glRotatef(90, 1, 0, 0);
  glCallList(superCylindre);
  glPopMatrix();

  //LA TETE
  glPushMatrix();
  glColor3f(1, 0.878, 0.749); //couleur peau
  glCallList(superSphere);
  glPopMatrix();

  //LA BASE - haut
  glPushMatrix();
  glColor3f(0.976, 0.275, 0.165); //couleur un peu rouge
  glRotatef(90, 1, 0, 0);
  glCallList(superCercle);
  glPopMatrix();

  //LA BASE - bas
  glPushMatrix();
  glColor3f(0.459, 0.439, 0.42);//couleur grise
  glRotatef(90, 1, 0, 0);
  glTranslatef(0, 0, 1.3);
  glCallList(superCercle);
  glPopMatrix();

  //BRAS GAUCHE
  glPushMatrix();
  glColor3f(1, 0.878, 0.749); //couleur peau
  glRotatef(90, 1, 0, 0); //mettre à la verticale
  glTranslatef(0.29, 0, 0.25);
  glRotatef(30, 0, 1, 0);
  glCallList(superCylindre2);
  glPopMatrix();

  //BRAS DROIT
  glPushMatrix();
  glColor3f(1, 0.878, 0.749); //couleur peau
  glRotatef(90, 1, 0, 0); //mettre à la verticale
  glTranslatef(-0.29, 0, 0.25);
  glRotatef(-30, 0, 1, 0);
  glCallList(superCylindre2);
  glPopMatrix();

  //OEIL GAUCHE
  glPushMatrix();
  glColor3f(1, 1, 1);
  glRotatef(90, 0, 0, 1);
  glTranslatef(-0.1, 0, 0.2);
  glCallList(superEllipse);
  glPopMatrix();

  glutSwapBuffers();
}
