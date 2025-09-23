#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>

// Prototipos de las funciones 
int contarL(const std::string& RutArch);
void leerArch(const std::string& RutArch,
              std::string nombres[],
              std::string paises[],
              std::string disciplinas[],
              std::string generos[],
              int medallas[]);
void buscarAtleta(const std::string nombres[],
                  const std::string paises[],
                  const std::string disciplinas[],
                  const std::string generos[],
                  const int medallas[],
                  int numRegistros);
void calcularMedallasPorPais(const std::string paises[], const int medallas[], int numRegistros);
void encontrarAtletaMasMedallas(const std::string nombres[], const int medallas[], int numRegistros);

int main(int argc, char* argv[]) {
    std::string RutArch;

    if (argc > 1) {
        RutArch = argv[1];
    } else {
        std::cout << "Por favor, introduce la ruta al archivo de datos de atletas: ";
        std::cin >> RutArch;
    }

    int numRegistros = contarL(RutArch);

    if (numRegistros <= 0) {
        std::cerr << "El archivo esta vacio o no se pudo abrir." << std::endl;
        return 1;
    } 

    std::cout << "Archivo abierto, tiene " << numRegistros - 1 << " registros de atletas." << std::endl;
    
    std::string nombres[numRegistros];
    std::string paises[numRegistros];
    std::string disciplinas[numRegistros];
    std::string generos[numRegistros];
    int medallas[numRegistros];
    
    leerArch(RutArch, nombres, paises, disciplinas, generos, medallas);
    
    int opcion = 0;
    
    do {
        std::cout << "\nmenu de Atletas\n";
        std::cout << "1. Buscar informacion de un atleta\n";
        std::cout << "2. Calcular total de medallas por pais\n";
        std::cout << "3. Encontrar al atleta con mas medallas\n";
        std::cout << "4. Salir\n";
        std::cout << "Elige una opcion: ";
        std::cin >> opcion;
        std::cin.ignore();

        switch (opcion) {
            case 1:
                buscarAtleta(nombres, paises, disciplinas, generos, medallas, numRegistros);
                break;
            case 2:
                calcularMedallasPorPais(paises, medallas, numRegistros);
                break;
            case 3:
                encontrarAtletaMasMedallas(nombres, medallas, numRegistros);
                break;
            case 4:
                std::cout << "Hasta la proximaa\n";
                break;
            default:
                std::cout << "Opcion no valida\n";
                break;
        }
    } while (opcion != 4);

    return 0;
}

int contarL(const std::string& RutArch) {
    std::ifstream archivo(RutArch);
    std::string linea;
    int contador = 0;
    if (archivo.is_open()) {
        while (getline(archivo, linea)) {
            contador++;
        }
        archivo.close();
    } else {
        return -1;
    }
    return contador;
}

void leerArch(const std::string& RutArch,
              std::string nombres[],
              std::string paises[],
              std::string disciplinas[],
              std::string generos[],
              int medallas[]) {
    std::ifstream archivo(RutArch);
    std::string linea;
    int i = 0;
    if (archivo.is_open()) {
        std::getline(archivo, linea); // Ignora el encabezado del archivo
        while (std::getline(archivo, linea)) {
            std::stringstream ss(linea);
            std::string dato;
            std::string datosLinea[5];
            int j = 0;
            while (std::getline(ss, dato, ',')) {
                // Eliminar espacios en blanco del inicio y final
                size_t inicio = dato.find_first_not_of(" \t\r\n");
                if (inicio != std::string::npos) {
                    size_t fin = dato.find_last_not_of(" \t\r\n");
                    datosLinea[j] = dato.substr(inicio, fin - inicio + 1);
                } else {
                    datosLinea[j] = "";
                }
                j++;
            }
            // Validacion para ver si la linea tiene 5 campos
            if (j == 5) {
                nombres[i] = datosLinea[0];
                paises[i] = datosLinea[1];
                disciplinas[i] = datosLinea[2];
                generos[i] = datosLinea[3];
                try {
                    medallas[i] = std::stoi(datosLinea[4]);
                } catch (const std::exception& e) {
                    medallas[i] = 0;
                }
                i++;
            } else {
                std::cout << "Advertencia: Se ignoro una linea con formato incorrecto. \n";
            }
        }
        archivo.close();
    }
}

void buscarAtleta(const std::string nombres[],
                  const std::string paises[],
                  const std::string disciplinas[],
                  const std::string generos[],
                  const int medallas[],
                  int numRegistros) {
    std::string nombreBusc;
    std::cout << "Cual nombre quieres buscar: ";
    std::cin.ignore();
    std::getline(std::cin, nombreBusc);

    bool encontrado = false;
    for (int i = 0; i < numRegistros; i++){
        if (nombres[i] == nombreBusc){
            std::cout << "\ninfo de atleta" << std::endl;
            std::cout << "Nombre: " << nombres[i] << std::endl;
            std::cout << "Pais: " << paises[i] << std::endl;
            std::cout << "Disciplina: " << disciplinas[i] << std::endl;
            std::cout << "Genero: " << generos[i] << std::endl;
            std::cout << "Medallas de oro: " << medallas[i] << std::endl;
            encontrado = true;
            break;
        }
    }
    if (!encontrado){
        std::cout << "Atleta no encontrado." << std::endl;
    }
}

void calcularMedallasPorPais(const std::string paises[], const int medallas[], int numRegistros) {
    std::map<std::string, int> totalesPorPais;

    for (int i = 0; i < numRegistros; ++i) {
        totalesPorPais[paises[i]] += medallas[i];
    }

    std::cout << "\nTotal de medallas por Pais:\n";
    for (const auto& par : totalesPorPais) {
        std::cout << "Pais: " << par.first << " - Medallas: " << par.second << std::endl;
    }
}

void encontrarAtletaMasMedallas(const std::string nombres[], const int medallas[], int numRegistros) {
    int maxMedallas = -1;
    int indiceGanador = -1;

    for (int i = 0; i < numRegistros; ++i) {
        if (medallas[i] > maxMedallas) {
            maxMedallas = medallas[i];
            indiceGanador = i;
        }
    }

    if (indiceGanador != -1) {
        std::cout << "\nAtleta con mas medallas de oro:\n";
        std::cout << "Nombre: " << nombres[indiceGanador] << std::endl;
        std::cout << "Medallas de oro: " << maxMedallas << std::endl;
    } else {
        std::cout << "No se encontro ningun atleta.\n";
    }
}
//disculpeme que le deje esto esque asi corre nadamas y no me aprendo aun el como corre
//vasti@vasti-Aspire-A514-54:~/Música/act1/act1$ output/main data/data1.csv
