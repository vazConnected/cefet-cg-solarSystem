function deleteFiles {
    echo -e "\nExcluindo arquivos desnecessarios..."

    rm -f $(pwd)/src/libs/celestial.o
    rm -f $(pwd)/src/libs/textures.o

    echo -e "Feito!"
}

clear
echo -e "\nCarregando o Sistema Solar..."

echo -e "\n\tCompilando src/libs/celestial.c"
gcc -c -o src/libs/celestial.o src/libs/celestial.c -lGL -lGLEW -lglut -lSOIL -lm
if [ $? -ne 0 ]; then
    deleteFiles
    exit 1
fi

echo -e "\n\tCompilando src/libs/textures.c"
gcc -c -o src/libs/textures.o src/libs/textures.c -lGL -lGLEW -lglut -lSOIL
if [ $? -ne 0 ]; then
    deleteFiles
    exit 1
fi


echo -e "\n\tCompilando src/main.c"
gcc -o src/cg-solarSystem.out src/main.c src/libs/celestial.o src/libs/textures.o -lGL -lGLEW -lglut -lGLU -lSOIL -lm
if [ $? -ne 0 ]; then
    deleteFiles
    exit 1
fi

echo -e "\nCompilacao finalizada!"
deleteFiles

echo -e "\nIniciando a simulacao..."
./src/cg-solarSystem.out

echo -e "\nObrigado(a) por simular conosco!\n"
rm -f $(pwd)/src/cg-solarSystem.out

exit 0
