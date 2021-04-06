#ifndef _CELESTIAL_H_
#define _CELESTIAL_H_

// Referencia: en.wikipedia.org/wiki/List_of_natural_satellites
enum orbes{mercury = 1, venus, earth, mars, jupiter, saturn, uranus, nepturne};
enum naturalSatellites{moon = 1, phobos, deimos}; // Apenas da Terra e Marte

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
                     int radius, float trans, char* caminho); // define valores dos corpos celestiais
void solidSphere(int radius, int stacks, int columns); // desenha esferas passiveis de serem texturizadas
void drawCelestials(); // desenha os corpos celestiais nas suas respectivas posicoes e com suas determinadas iluminacoes
void drawOrbits(); // desenha as orbitas dos astros
void moveCelestials(); // rotaciona o sol e todos os astros, translada os astros em torno do sol e os satelites em torno de seus respectivos planetas

#endif // _CELESTIAL_H_
