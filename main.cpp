#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

// Prototipos de las funciones 
int contarL(const std::string& RutArch);
void leerArch(const std::string& RutArch,
              std::string nombres[],
              std::string paises[],
              std::string disciplinas[],
              std::string generos[],
              int medallas[]);
void buscarAt(std::string nombreAt[], 
              std::string paises[], 
              std::string disciplina[],
              std::string genero[],
              const int medallas[], 
              const int registros);
void calcularMedallasPorPais(const std::string paises[], const int medallas[], int numRegistros);
void encontrarAtletaMasMedallas(const std::string nombres[], const int medallas[], int numRegistros);

int main(int argc, char* argv[]) {
    std::string RutArch;

    if (argc > 1) {
        RutArch = argv[1];
    } else {
        std::cout << "Por favor, introduce la ruta al archivo del registro: ";
        std::cin >> RutArch;
    }

    int numRegistros = contarL(RutArch);

    if (numRegistros <= 0) {
        std::cerr << "El archivo esta vacio o no se pudo abrir" << std::endl;
        return 1;
    } 

    std::cout << "Archivo abierto " << numRegistros - 1 << " registros de atletas." << std::endl;
    
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
                buscarAt(nombres, paises, disciplinas, generos, medallas, numRegistros - 1);
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
                std::cout << "formato incorrecto compañere \n";
            }
        }
        archivo.close();
    }
}

void buscarAt(std::string nombreAt[], 
              std::string paises[], 
              std::string disciplina[],
              std::string genero[],
              const int medallas[], 
              const int registros) {
    std::string busqueda;
    std::cout << "Ingrese el nombre del atleta: ";
    std::getline(std::cin, busqueda);
    
    for (int i = 0; i < registros; i++) {
        if(nombreAt[i] == busqueda){
            std::cout << "Nombre: " << nombreAt[i] << std::endl;
            std::cout << "Pais: " << paises[i] << std::endl;
            std::cout << "Disciplina: " << disciplina[i] << std::endl;
            std::cout << "Genero: " << genero[i] << std::endl;
            std::cout << "Medallas: " << medallas[i] << std::endl;
            return; // Sale de la función cuando encuentra el atleta
        }
    }
    // Si llega aquí no encontró el atleta
    std::cout << "no estaba" << std::endl;
}

void calcularMedallasPorPais(const std::string paises[], const int medallas[], int numRegistros) {
    const int MAX_PAISES_UNICOS = 100; 
    std::string paisesUnicos[MAX_PAISES_UNICOS];
    int medTotales[MAX_PAISES_UNICOS] = {0};
    int cont = 0;
    
    for (int i = 0; i < numRegistros; i++) {
        bool encontrado = false;
        
        for (int j = 0; j < cont; j++) {
            if (paises[i] == paisesUnicos[j]) {
                medTotales[j] += medallas[i];
                encontrado = true;
                break;
            }
        }
        
        if (!encontrado && cont < MAX_PAISES_UNICOS) {
            paisesUnicos[cont] = paises[i];
            medTotales[cont] = medallas[i];
            cont++;
        }
    }
    
    std::cout << "\nMedallas por país:\n";
    for (int i = 0; i < cont; i++) {
        std::cout << "País: " << paisesUnicos[i] << " - Medallas: " << medTotales[i] << std::endl;
    }
    return;
}


void calcularMedallasPorPais(const std::string paises[], const int medallas[], int numRegistros) {
    const int tamañomax = 100; 
    std::string paisesQueYaConte[tamañomax];
    int medallasPorPais[tamañomax] = {0};
    int contadorDePaisesUnicos = 0;
    
    for (int i = 0; i < numRegistros; i++) {
        bool paisEncontrado = false;
        
        for (int j = 0; j < contadorDePaisesUnicos; j++) {
            if (paises[i] == paisesQueYaConte[j]) {
                medallasPorPais[j] += medallas[i];
                paisEncontrado = true;
                break;
            }
        }
        
        if (!paisEncontrado && contadorDePaisesUnicos < tamañomax) {
            paisesQueYaConte[contadorDePaisesUnicos] = paises[i];
            medallasPorPais[contadorDePaisesUnicos] = medallas[i];
            contadorDePaisesUnicos++;
        }
    }
    
    std::cout << "\nMedallas por país:\n";
    for (int i = 0; i < contadorDePaisesUnicos; i++) {
        std::cout << "País: " << paisesQueYaConte[i] << " - Medallas: " << medallasPorPais[i] << std::endl;
    }
    return;
}
//comando para correrlo
//cd /home/vasti/Música/act1/act1
//output/main data/data1.csv