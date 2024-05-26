#include <string>
#include <mutex>
#include <thread>
#include <vector>
#include <fstream>
#include <sstream>
#include "User.h"
#include "definitions.h"
#include "utils.h"
#include <atomic> // Include the <atomic> header
#include "Search.h"
#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <sys/mman.h> // Include the <sys/mman.h> header
#include <semaphore.h>



std::atomic<int> totalOccurrences(0); // Variable global para contar las ocurrencias totales;
std::atomic<int> premiumBalance(0);
mqd_t Search::paymentQueue = mq_open(PAYMENT_QUEUE_NAME, O_WRONLY); // Inicialización de la cola de mensajes
mqd_t Search::paymentQueue2 = mq_open(PAYMENT_QUEUE_NAME2, O_WRONLY); // Inicialización de la cola de mensajes
static std::mutex mtx; // Mutex para proteger la sección crítica

Search::Search(searchRequest_t request) {
    // Get the keyword and the shared memory name from the request
    std::strncpy(keyword, request.keyword, sizeof(keyword));
    keyword[sizeof(keyword) - 1] = '\0'; // asegura que la cadena esté terminada con un carácter nulo
    
    printf("[SEARCHER] Getting user shared memory\n");
    int shm_fd = shm_open(request.sharedMemoryName, O_RDONLY, 0666);
    if (shm_fd == -1) {
        // Maneja el error
        perror("shm_open");
        return;
        
    }
    // Obtén el tamaño del objeto User
    size_t size = sizeof(User);

    // Mapea el objeto de memoria compartida en este proceso
    void* ptr = mmap(0, size, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
    // Maneja el error
        perror("mmap");
        return;
    }
    user = static_cast<User*>(ptr);

    // Get the files to search. You have to fill your vector files with the routes of the files to search that are in the file paths.txt (1 path per lane)
    std::ifstream pathsFile("assets/paths.txt");
    std::string path;
    while (std::getline(pathsFile, path)) {
        files.push_back(path);
    }
    
}

void Search::FreeUserSearch(int numThreads) {
    printf("[SEARCHER] Free user search started\n");

    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads.push_back(std::thread(&Search::FreeSearchInFile, this, files[i], keyword));
    }
    for (auto& th : threads) {
        th.join();
    }
}

void Search::FreeSearchInFile(const std::string& file, const std::string& keyword) {
    // Print msg that says the thread ID and the file where the thread is searching in
    printf("[SEARCHER] Thread (%d) searching in file %s\n", std::this_thread::get_id(), file.c_str());
    std::ifstream inFile(file);
    std::string line;
    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        std::string word;
        while (iss >> word) {
            if (word == keyword) {
                std::lock_guard<std::mutex> lock(mtx);
                if (totalOccurrences >= FREE_USER_SEARCH_LIMIT) {
                    printf("[SEARCHER] Free user search limit reached\n");
                    return; // Si se ha alcanzado el límite, termina la ejecución del hilo
                }
                ++totalOccurrences;
            }
        }
    }
}

int Search::getTotalOccurrences() {
    return totalOccurrences.load();
}

void Search::unlimitedUserSearch() {
    printf("[SEARCHER] Unlimited user search started\n");
    for (const auto& file : files) {
        UnlimitedSearchInFile(file, keyword);
    }
}

void Search::UnlimitedSearchInFile(const std::string& file, const std::string& keyword) {
    // Print msg that says the thread ID and the file where the thread is searching in
    printf("[SEARCHER] Thread (%d) searching in file %s\n", std::this_thread::get_id(), file.c_str());
    std::ifstream inFile(file);
    std::string line;
    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        std::string word;
        while (iss >> word) {
            if (word == keyword) {
                std::lock_guard<std::mutex> lock(mtx);
                ++totalOccurrences;
            }
        }
    }
}

void Search::PremiumSearchInFile(const std::string& file, const std::string& keywords) {
    printf("[SEARCHER] Thread (%d) searching in file %s\n", std::this_thread::get_id(), file.c_str());
    std::ifstream inFile(file);
    std::string line;
    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        std::string word;
        while (iss >> word) {
            if (word == keyword) {
                #include <mqueue.h> // Add the missing header for message queues

                sem_wait(&sem); // Wait for the semaphore
                printf("[SEARCHER] Premium user balance: %d\n", premiumBalance.load());

                std::lock_guard<std::mutex> lock(mtx); // Bloquea el mutex para proteger la sección crítica
                if (premiumBalance <= 0) {
                    sem_post(&sem); // Release the semaphore
                    printf("[SEARCHER] Premium user balance is 0\n");
                    // Send a rechargeRequest message to the paymentQueue
                    rechargeRequest_t rechargeRequest;
                    rechargeRequest.user_id = user->getId();
                    mq_send(paymentQueue, (char*)&rechargeRequest, sizeof(rechargeRequest), 0);
                    char bufferRece[128];
                    mq_receive(paymentQueue2, bufferRece, sizeof(bufferRece), NULL);
                    // Receiving a int from the paymentQueue2, printing the message with the rechargeAmount, adding it to the variable premiumBalance and increasing the totalOccurrences
                    int rechargeAmount = *reinterpret_cast<int*>(bufferRece);
                    printf("[SEARCHER] Recharge amount: %d\n", rechargeAmount);
                    premiumBalance += rechargeAmount;
                    sem_wait(&sem); // Wait for the semaphore      
                    
                }
                premiumBalance--; // Decrease the balance
                ++totalOccurrences;
                sem_post(&sem); // Release the semaphore
            }
        }
    }
}

void Search::PremiumUserSearch(int numThreads) {
    printf("[SEARCHER] Premium user search started\n");
    premiumBalance = user->getBalance();
    sem_init(&sem, 0, 1); // Initialize the semaphore
    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads.push_back(std::thread(&Search::PremiumSearchInFile, this, files[i], keyword));
    }
    for (auto& th : threads) {
        th.join();
    }
    sem_destroy(&sem); // Destroy the semaphore when done
}

