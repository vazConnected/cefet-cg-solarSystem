#include "headers/celestial.h"

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <stdio.h>
#include <math.h>
#include <time.h>

void buildCelestial(Celestial *cel, int x, int y, int z, int rx, int rz,
                     int posx, int posy, int posz,
                     int radius, float rot, float trans ){

    cel->orbitcx = x;
    cel->orbitcy = y;
    cel->orbitcz = z;
    cel->orbitrx = rx;
    cel->orbitrz = rz;
    cel->posx = posx;
    cel->posy = posy;
    cel->posz = posz;
    cel->radius = radius;
    cel->velRotation = rot;
    cel->velTranslation = trans;
    cel->restartT = 0.0;

}

void drawCelestials(){
    // draw the planets
    for(int i = 0; i < nepturne; i++){
        glPushMatrix();
         glColor3f(i/(i+1.5), i/(i+0.9), 1.0f);
         if( i == nepturne - 1) glRotatef(10, 0, 0, 1); // nepturne's move is inclined
         glTranslatef(planets[i].posx, planets[i].posy, planets[i].posz);
         glRotatef(planets[i].velRotation, 0, 1, 0);
         glutWireSphere(planets[i].radius, 30, 30);
        glPopMatrix();
    }

    // draw the satellites
    for(int i = 0; i < deimos; i++){
        glPushMatrix();
         glColor3f(0.3*i, 1.0f, 1.0f);
         glTranslatef(satellites[i].posx, satellites[i].posy, satellites[i].posz);
         glRotatef(satellites[i].velRotation, 0, 1, 0);
         glutWireSphere(satellites[i].radius, 30, 30);
        glPopMatrix();
    }

    // draw the sun
    glColor3f(0.9f, 0.9f, 0.0f);
    glPushMatrix();
     glTranslated(sun.posx, sun.posy, sun.posz);
     glRotatef(sun.velRotation, 0, 1, 0);
     glutWireSphere(sun.radius, 30, 30);
    glPopMatrix();

}

void drawOrbits(){
    // planets
    glPushMatrix();
    for(int i = 0; i < nepturne; i++){
        float angulo;
        glColor3f(0.15*i, 0.0f, 1.0f);
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
        glColor3f(0.15*i, 0.0f, 1.0f);
        // indicates which planet corresponds to the satellite
        int k = 2;
        if(i > moon - 1) k = 3;
        // make an ellipse or a circle
        glBegin(GL_LINE_LOOP);
            for (int j = 0; j <= MAX_LADOS; ++j) {
                angulo = 2 * M_PI * j / MAX_LADOS;
                glVertex3f(planets[k].posx + cos(angulo)*(planets[k].radius+12*k),
                           satellites[i].orbitcy,
                           planets[k].posz + sin(angulo)*(planets[k].radius+12*k));
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
        if(planets[i].velRotation > 360) planets[i].velRotation = 0;
    }

    // satellites are translated and rotated
    for(int i = 0; i < deimos; i++){
        // indicates which planet corresponds to the satellite
        int k = 2;
        if(i > moon - 1) k = 3;
        // calculate the moviment
        float angulo;
        angulo = 2 * 2 * M_PI * (satellites[i].restartT) / MAX_LADOS;
        satellites[i].posx = planets[k].posx + cos(angulo)*(planets[k].radius+12*k);
        satellites[i].posz = planets[k].posz + sin(angulo)*(planets[k].radius+12*k);
        satellites[i].restartT += satellites[i].velTranslation;
        if(satellites[i].restartT > MAX_LADOS){
            satellites[i].restartT = 0;
        }
        satellites[i].velRotation += 5;
        if(satellites[i].velRotation > 360) satellites[i].velRotation = 0;
    }

    // the sun is only rotated
    sun.velRotation += 5;
    if(sun.velRotation > 360) sun.velRotation = 0;
}

