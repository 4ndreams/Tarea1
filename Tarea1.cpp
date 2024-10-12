#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <cstdlib>

std::mutex mtx;  // Mutex para sincronizar la salida en la consola
int posicion = 1; // Posición para saber el orden de llegada de los autos

// Definiciones de colores ANSI
const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string MAGENTA = "\033[35m";
const std::string CYAN = "\033[36m";

// Ancho de la pantalla de la terminal (ajustar según sea necesario)
const int WIDTH = 70;

// Función que simula el avance de cada automóvil
void carrera_auto(int id, int M) {
    int distancia_recorrida = 0;
    std::string color;

    // Asignar un color diferente a cada auto
    switch (id % 6) {
        case 0: color = RED; break;
        case 1: color = GREEN; break;
        case 2: color = YELLOW; break;
        case 3: color = BLUE; break;
        case 4: color = MAGENTA; break;
        case 5: color = CYAN; break;
    }

    while (distancia_recorrida < M) {
        int avance = rand() % 10 + 1;
        distancia_recorrida += avance;
        if (distancia_recorrida > M) {
            distancia_recorrida = M;
        }

        {
            std::lock_guard<std::mutex> guard(mtx);
            //std::cout << "\033[H\033[J";  // Limpiar la pantalla
            for (int i = 0; i <= id; ++i) std::cout << std::endl;  // Mover la posición del cursor hacia abajo
            for (int i = 0; i < distancia_recorrida % WIDTH; ++i) std::cout << " ";
            std::cout << color << "Auto " << id +1 << " 🚗 " << distancia_recorrida << "[mt]"<< RESET << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100 + rand() % 401));
    }

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

    int M = std::stoi(argv[1]);
    int N = std::stoi(argv[2]);
    std::vector<std::thread> hilos;

    for (int i = 0; i < N; ++i) {
        hilos.push_back(std::thread(carrera_auto, i, M));
    }

    for (auto& hilo : hilos) {
        hilo.join();
    }

    std::cout << "La carrera ha terminado!" << std::endl;
    return 0;
}
