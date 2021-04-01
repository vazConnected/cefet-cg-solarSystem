#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>

#include "headers/celestial.h"
#include "headers/textures.h"

#include <stdio.h>
#include <math.h>

void buildCelestial(Celestial* cel, int x, int y, int z, int rx, int rz,
                     int posx, int posy, int posz,
                     int radius, float trans, char* caminho ){

    cel->orbitcx = x;
    cel->orbitcy = y;
    cel->orbitcz = z;
    cel->orbitrx = rx;
    cel->orbitrz = rz;
    cel->posx = posx;
    cel->posy = posy;
    cel->posz = posz;
    cel->radius = radius;
    cel->velRotation = 0;
    cel->velTranslation = trans;
    cel->restartT = 0.0;
    cel->textura = carregaTextura(caminho);
}

void solidSphere(int radius, int stacks, int columns){

    // função retirada do exemplo "mapeamento"
    GLUquadric* quadObj = gluNewQuadric();
    gluQuadricDrawStyle(quadObj, GLU_FILL);
    gluQuadricNormals(quadObj, GLU_SMOOTH);
    gluQuadricTexture(quadObj, GL_TRUE);
    gluSphere(quadObj, radius, stacks, columns);
    gluDeleteQuadric(quadObj);
}

void drawCelestials(){

    GLfloat sunLight[4] = {0.7, 0.6, 0.0, 1.0};
    // draw the sun
    glPushMatrix();
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, sunLight);
        glTranslated(sun.posx, sun.posy, sun.posz);
        glRotatef(sun.velRotation, 0, 1, 0);
        desenhaCena(sun.textura, sun.radius, 30);
    glPopMatrix();

    GLfloat planetsLight[4] = {0.0, 0.0, 0.0, 0.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, planetsLight);

    // draw the planets
    for(int i = 0; i < nepturne; i++){
        glPushMatrix();
            if(i == nepturne - 1){
                glRotatef(10, 0, 0, 1); // nepturne's move is inclined
            }
            glTranslatef(planets[i].posx, planets[i].posy, planets[i].posz);
            glRotatef(planets[i].velRotation, 0, 1, 0);
            desenhaCena(planets[i].textura, planets[i].radius, 30);
        glPopMatrix();
    }

    // draw atmosphere
    glPushMatrix();
        glTranslated(planets[earth-1].posx, planets[earth-1].posy, planets[earth-1].posz);
        glRotatef(planets[earth-1].velRotation, 0, 1, 1);
        desenhaCena(atmosphere, planets[earth-1].radius+5, 30);
    glPopMatrix();

    // draw the satellites
    for(int i = 0; i < deimos; i++){
        glPushMatrix();
            glTranslatef(satellites[i].posx, satellites[i].posy, satellites[i].posz);
            glRotatef(satellites[i].velRotation, 0, 1, 0);
            desenhaCena(satellites[i].textura, satellites[i].radius, 30);
        glPopMatrix();
    }

    // draw the rings
    GLint especMaterial = 100;
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, especMaterial);
    for(int i = jupiter-1; i < nepturne; i++){
        glPushMatrix();
            if(i == nepturne - 1){
                glRotatef(10, 0, 0, 1); // nepturne's move is inclined
                glColor3f(1.0f, 1.0f, 1.0f);
            }else if(i == saturn-1 ){
                glColor3f(0.9f, 0.8f, 0.0f);
            }else if(i == uranus-1 ){
                glColor3f(0.0f, 0.8f, 0.9f);
            }else{
                glColor3f(0.9f, 0.5f, 0.0f);
            }
            glTranslatef(planets[i].posx,planets[i].posy, planets[i].posz-1);
            glRotatef(planets[i].velRotation, 0, 1, 0);
            glRotatef(90, 1, 0, 0);
            glutSolidTorus(5,(planets[i].radius)+20, 20, 40);
        glPopMatrix();
    }
}

void drawOrbits(){

    // planets
    glPushMatrix();
    for(int i = 0; i < nepturne; i++){
        float angulo;
        if( i == nepturne - 1) glRotatef(10, 0, 0, 1); // nepturne's move is inclined
        // make an ellipse or a circle
        glBegin(GL_LINE_LOOP);
            for (int k = 0; k <= MAX_LADOS; ++k) {
                angulo = 2 * M_PI * k / MAX_LADOS;
                glVertex3f(planets[i].orbitcx + cos(angulo)*planets[i].orbitrx,
                           planets[i].orbitcy,
                           planets[i].orbitcz + sin(angulo)*planets[i].orbitrz);
            }
        glEnd();
    }
    glPopMatrix();

    // satellites
    for(int i = 0; i < deimos; i++){
        float angulo;
        // indicates which planet corresponds to the satellite
        int k = 2;
        if(i > moon - 1){
            k = 3;
        }
        // make an ellipse or a circle
        glBegin(GL_LINE_LOOP);
            for (int j = 0; j <= MAX_LADOS; ++j) {
                angulo = 2 * M_PI * j / MAX_LADOS;
                glVertex3f(planets[k].posx + cos(angulo)*satellites[i].orbitrx,
                           satellites[i].posy,
                           planets[k].posz + sin(angulo)*satellites[i].orbitrz);
            }
        glEnd();
    }
}

void moveCelestials(){

    // planets are translated and rotated
    for(int i = 0; i < nepturne; i++){
        float angulo;
        angulo = 2 * M_PI * planets[i].restartT / MAX_LADOS;
        planets[i].posx = planets[i].orbitcx + cos(angulo)*planets[i].orbitrx;
        planets[i].posz = planets[i].orbitcz + sin(angulo)*planets[i].orbitrz;
        planets[i].restartT += planets[i].velTranslation;
        if(planets[i].restartT > MAX_LADOS){
            planets[i].restartT = 0;
        }
        planets[i].velRotation += 5;
        if(planets[i].velRotation > 360){
            planets[i].velRotation = 0;
        }
    }

    // satellites are translated and rotated
    for(int i = 0; i < deimos; i++){
        // indicates which planet corresponds to the satellite
        int k = 2;
        if(i > moon - 1){
            k = 3;
        }
        // calculate the movement
        float angulo;
        angulo = 2 * 2 * M_PI * (satellites[i].restartT) / MAX_LADOS;
        satellites[i].posx = planets[k].posx + cos(angulo)*satellites[i].orbitrx;
        satellites[i].posz = planets[k].posz + sin(angulo)*satellites[i].orbitrz;
        satellites[i].restartT += satellites[i].velTranslation;
        if(satellites[i].restartT > MAX_LADOS){
            satellites[i].restartT = 0;
        }
        satellites[i].velRotation += 5;
        if(satellites[i].velRotation > 360){
            satellites[i].velRotation = 0;
        }
    }

    // the sun is only rotated
    sun.velRotation += 5;
    if(sun.velRotation > 360) sun.velRotation = 0;
}
