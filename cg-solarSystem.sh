function deleteFiles {
    echo -e "\nExcluindo arquivos desnecessarios..."

    echo -e "Feito!"
}

clear
echo -e "\nCarregando o Sistema Solar..."

echo -e "\n\tCompilando src/main.c"
gcc -o src/cg-solarSystem.out src/main.c -lGL -lGLEW -lglut -lGLU
if [ $? -ne 0 ]; then
    deleteFiles
    exit 1
fi

echo -e "\nCompilacao finalizada!"
deleteFiles

echo -e "\nIniciando a simulacao..."
./src/cg-solarSystem.out

echo -e "\nObrigado(a) por simular conosco\n"
rm -f $(pwd)/src/cg-solarSystem.out

exit 0