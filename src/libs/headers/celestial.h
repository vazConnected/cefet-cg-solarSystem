#ifndef _CELESTIAL_H_
#define _CELESTIAL_H_

// Source: en.wikipedia.org/wiki/List_of_natural_satellites
enum orbes{mercury = 1, venus, earth, mars, jupiter, saturn, uranus, nepturne};
enum naturalSatellites{moon = 1, phobos, deimos}; // Only Earth and Mars

#define MAX_LADOS 60

typedef struct{
    GLint orbitcx;
    GLint orbitcy;
    GLint orbitcz;
    GLint orbitrx;
    GLint orbitrz;
    GLint radius;
    GLfloat velTranslation;
    GLfloat velRotation;
    GLint posx;
    GLint posy;
    GLint posz;
    GLfloat restartT;
    GLuint textura;
} Celestial;

GLuint atmosphere;

Celestial planets[8];
Celestial satellites[3];
Celestial sun;

void buildCelestial(Celestial* cel, int x, int y, int z, int rx, int ry,
                     int posx, int posy, int posz,
                     int radius, float trans, char* caminho);
void solidSphere(int radius, int stacks, int columns);
void drawCelestials();
void drawOrbits();
void moveCelestials();

#endif // _CELESTIAL_H_
