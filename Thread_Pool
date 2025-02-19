#include <boost/asio.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>

std::mutex cout_mutex;

void preparar_pedido(int pedido_id) {
    std::this_thread::sleep_for(std::chrono::seconds(2)); 

    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << "Cocinero " << std::this_thread::get_id() 
              << " preparo el pedido " << pedido_id << std::endl;
}

int validar(const std::string& mensaje) {
    int numero;
    while (true) {
        std::cout << mensaje;
        std::cin >> numero;

        if (std::cin.fail() || numero <= 0) {
            std::cin.clear(); 
            std::cin.ignore(10000, '\n'); 
            std::cout << "Entrada invalida\n";
        } else {
            std::cin.ignore(10000, '\n');
            break; 
        }
    }
    return numero;
}

int main() {
    int num_cocineros = validar("Ingrese la cantidad de cocineros: ");
    int num_pedidos = validar("Ingrese la cantidad de pedidos: ");

    boost::asio::thread_pool pool(num_cocineros);

    std::cout << "\n Restaurante abierto con " << num_cocineros 
              << " cocineros. Procesando " << num_pedidos << " pedidos...\n\n";

    for (int i = 1; i <= num_pedidos; i++) {
        boost::asio::post(pool, [i]() { preparar_pedido(i); });
    }
    pool.join(); 

    std::cout << "\nTodos los pedidos han sido preparados.\n";

    return 0;
}
