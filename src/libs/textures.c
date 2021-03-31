#include <GL/glew.h>
#include <GL/freeglut.h>
#include <SOIL/SOIL.h>

#include <stdio.h>

#include "headers/celestial.h"
#include "headers/textures.h"

unsigned int carregaTextura(char* caminho){

    unsigned int idTextura;
    idTextura = SOIL_load_OGL_texture(
        caminho,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );

    if (idTextura == 0) {
        printf("Erro ao carregar textura com caminho '%s': '%s'\n", caminho, SOIL_last_result());
    }
    return idTextura;
}

void desenhaCena(GLuint textura, GLint raio, GLint divisoes){
    
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glScalef(1, 1, 1);

    glMatrixMode(GL_MODELVIEW);
    
    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textura);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glPolygonMode(GL_FRONT, GL_FILL);
        solidSphere(raio, divisoes, divisoes);
    glDisable(GL_TEXTURE_2D);
}