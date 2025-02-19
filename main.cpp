#include <iostream>
#include <future>
#include <thread>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <mutex>

std::mutex cout_mutex;

int procesar_pedido(int id) {
    // Generar una semilla única para cada hilo
    std::srand(std::time(0) + id + std::hash<std::thread::id>{}(std::this_thread::get_id()));

    // tiempo aleatorio
    int tiempo = 500 + (std::rand() % 1501);
    std::this_thread::sleep_for(std::chrono::milliseconds(tiempo));

    //  precio aleatorio entre $5 y $100
    int precio = 5 + (std::rand() % 96);

    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "Pedido #" << id << " procesado en " << tiempo << "ms ----> Precio: $" << precio << std::endl;
    }
    return precio;
}

void pedido_tienda_online(int num_pedidos) {
    std::cout << "Realizando " << num_pedidos << " pedidos...\n";

    std::vector<std::pair<int, std::future<int>>> futuros_pedidos;
    // Crear pedidos asincrónicos y asociarlos con su ID real
    for (int i = 1; i <= num_pedidos; i++) {
        futuros_pedidos.emplace_back(i, std::async(std::launch::async, procesar_pedido, i));
    }

    std::cout << "Pedidos realizados, puedes seguir con otras tareas mientras se procesan...\n";
    int total_precio = 0;

    // Obtener los resultados de cada pedido asegurando que el ID sea el correcto
    for (auto& pedido : futuros_pedidos) {
        int id = pedido.first;      // ID real del pedido
        int precio = pedido.second.get(); // Obtener el precio calculado

        {
            std::lock_guard<std::mutex> lock(cout_mutex);
            std::cout << "Pedido #" << id << " entregado. Precio: $" << precio << std::endl;
        }

        total_precio += precio;
    }

    std::cout << "Total de la compra: $" << total_precio << std::endl;
}

int main() {
    std::srand(std::time(0)); // Inicializar la semilla para std::rand()

    std::cout << "----- FUTURE (Pedido en Tienda Online) -----\n";

    int num_pedidos;
    while (true) {
        std::cout << "Ingrese el número de pedidos a realizar: ";
        if (std::cin >> num_pedidos && num_pedidos > 0) {
            break;
        } else {
            std::cout << "Entrada inválida. Por favor, ingrese un número entero positivo.\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
    }

    pedido_tienda_online(num_pedidos);

    return 0;
}
