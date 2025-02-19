#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

class Semaphore {
public:
    Semaphore(unsigned long initial_count) {
        count_ = initial_count;
    }

    void acquire() {
        std::unique_lock<std::mutex> lock(m_);
        while (!count_) {
            cv_.wait(lock);
        }
        count_--;
    }

    void release() {
        std::unique_lock<std::mutex> lock(m_);
        count_++;
        lock.unlock();
        cv_.notify_one();
    }

private:
    std::mutex m_;
    std::condition_variable cv_;
    unsigned long count_;
};

Semaphore mesas(5);
std::mutex print_mutex;

void cliente(int id) {
    {
        std::lock_guard<std::mutex> lock(print_mutex);
        std::cout << "Cliente " << id << " esta esperando una mesa..." << std::endl;
    }

    mesas.acquire();

    {
        std::lock_guard<std::mutex> lock(print_mutex);
        std::cout << "Cliente " << id << " consiguio una mesa y esta comiendo." << std::endl;
    }

    srand(id);
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 3000 + 1000));

    {
        std::lock_guard<std::mutex> lock(print_mutex);
        std::cout << "Cliente " << id << " termino de comer y se fue." << std::endl;
    }

    mesas.release();
}

int main() {
    std::thread clientes[11];

    for (int i = 1; i <= 10; i++) {
        clientes[i] = std::thread(cliente, i);
    }

    for (int i = 1; i <= 10; i++) {
        clientes[i].join();
    }

    return 0;
}
