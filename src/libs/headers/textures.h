#ifndef _TEXTURES_H_
#define _TEXTURES_H_

unsigned int carregaTextura(char* caminho); // armazena o id da textura
void desenhaCena(GLuint textura, GLint raio, GLint divisoes); // desenha esferas e adiciona a textura as embrulhando

#endif // _TEXTURES_H_