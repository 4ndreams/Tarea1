#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <cstdlib>

// Variables globales
std::mutex mtx;  
int posicion = 1; 

// Definición de los colores
const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string MAGENTA = "\033[35m";
const std::string CYAN = "\033[36m";

// Ancho de la pantalla (esto se puede cambiar para la visibilidad de la carrera)
const int WIDTH = 100;

void carrera_auto(int id, int M) {
    int distancia_recorrida = 0;
    std::string color;

    // Asignar un color a cada auto
    switch (id % 6) {
        case 0: color = RED; break;
        case 1: color = GREEN; break;
        case 2: color = YELLOW; break;
        case 3: color = BLUE; break;
        case 4: color = MAGENTA; break;
        case 5: color = CYAN; break;
    }

    // Avance de los autos
    while (distancia_recorrida < M) {
        int avance = rand() % 10 + 1;
        distancia_recorrida += avance;
        if (distancia_recorrida > M) {
            distancia_recorrida = M;
        }

        // Imprimir por consola la posición de los autos
        {
            std::lock_guard<std::mutex> guard(mtx);
            for (int i = 0; i <= id; ++i) std::cout << std::endl;
            for (int i = 0; i < distancia_recorrida % WIDTH; ++i) std::cout << " ";
            std::cout << color << "Auto " << id +1 << " 🚗 " << distancia_recorrida << "[mt]"<< RESET << std::endl;
        }

        // Esperar un tiempo aleatorio
        std::this_thread::sleep_for(std::chrono::milliseconds(100 + rand() % 401));
    }

        // Imprimir por consola la posición de los autos
    {
        std::lock_guard<std::mutex> guard(mtx);
        std::cout << color << "Auto " << id +1<< " termina la carrera en el lugar " << posicion++ << "!" << RESET << std::endl;
    }
}


int main(int argc, char* argv[]) {
    
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <distancia_total> <numero_autos>" << std::endl;
        return 1;
    }

    // Inicializar la carrera
    int M = std::stoi(argv[1]);
    int N = std::stoi(argv[2]);
    std::vector<std::thread> hilos;

    for (int i = 0; i < N; ++i) {
        hilos.push_back(std::thread(carrera_auto, i, M));
    }

    for (auto& hilo : hilos) {
        hilo.join();
    }

    // Imprimir por consola el término de la carrera
    std::cout << "La carrera ha terminado!" << std::endl;
    return 0;
}
