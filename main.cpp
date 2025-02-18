#include <iostream>
#include <thread>
#include <latch>
#include <chrono>
#include <vector>
#include <mutex>

constexpr int num_corredores = 3;

std::mutex cout_mutex;

//esperar corredores
std::latch readyLatch(num_corredores);

std::latch startLatch(1);

void corredor(int id) {
    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "Corredor " << id << " se esta preparando...\n";
    }

    std::this_thread::sleep_for(std::chrono::seconds(id));

    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "Corredor " << id << " esta listo.\n";
    }

    readyLatch.count_down();

    startLatch.wait();

    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "Corredor " << id << " inicia la carrera\n";
    }
}

int main() {
    std::vector<std::thread> corredores;

    for (int i = 1; i <= num_corredores; ++i) {
        corredores.emplace_back(corredor, i);
    }

    readyLatch.wait();

    {
        std::lock_guard<std::mutex> lock(cout_mutex);
        std::cout << "Todos los corredores estan listos! Empieza la carrera!\n";
    }

    startLatch.count_down();

    for (auto& corredorThread : corredores) {
        corredorThread.join();
    }

    return 0;
}
