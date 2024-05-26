#include "Searcher.h"
#include "definitions.h"
#include <queue>
#include <mutex>
#include <mqueue.h>
#include "utils.h"
#include "Search.h"
#include <unistd.h>
#include <thread> // Include the necessary header file for using std::this_thread
#include <chrono> // Include the necessary header file for using std::chrono
#include <fcntl.h> // Include the necessary header file for using the O_RDONLY flag
#include <cstring>

Searcher* Searcher::instance = nullptr;
std::mutex Searcher::bufferMutex;

Searcher::Searcher() : premiumWeight(4), freeWeight(1)
{
    premiumCount = 0;
    freeCount = 0;
    running = true;
}

Searcher *Searcher::getInstance()
{
    std::lock_guard<std::mutex> lock(bufferMutex);
        if (instance == nullptr) {
           instance = new Searcher();
        }
    return instance;
}

void Searcher::receiveRequests() {
    mqd_t mq;
    char buffer[128];

    mq = mq_open(SEARCHER_QUEUE_NAME, O_RDONLY);
    if(mq == -1) {
        perror("mq_open");
        return;
    }
    printf("[SEARCHER-RECEIVER] Receiving service started\n");
    ssize_t bytes_read; 
        while (running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            bytes_read = mq_receive(mq, buffer, 128, NULL);
            searchRequest_t* searchRequestR = reinterpret_cast<searchRequest_t*>(buffer);
            printf("[SEARCHER-RECEIVER] Received request\n");
            printf("Keyword: %s\n", searchRequestR->keyword);
            printf("SharedMemoryName: %s\n", searchRequestR->sharedMemoryName);
            printf("Id: %d\n", searchRequestR->id);
            printf("Type: %d\n", searchRequestR->type);
                    
            if (bytes_read > 0) {
                std::lock_guard<std::mutex> lock(bufferMutex);
                switch (searchRequestR->type)
                {
                case UserType::PREMIUM:
                    instance->PremiumRequestBuffer.push(*searchRequestR);
                    break;

                case UserType::PREMIUM_UNLIMITED:
                    instance->PremiumRequestBuffer.push(*searchRequestR);
                    break;

                case UserType::FREE:
                    instance->FreeRequestBuffer.push(*searchRequestR);
                    break;

                default:
                    fprintf(stderr, "[SEARCHER-ERR]Tipo de usuario no válido\n");
                    break;
                }

            } else {
                perror("mq_receive_searcher");
                running = false;  // Stop if there's an error
            }
        }
}

void Searcher::processRequests() {
    printf("[SEARCHER-PROCESSER] Processing service started\n");
        while (running) {
            std::lock_guard<std::mutex> lock(bufferMutex);
            if (!PremiumRequestBuffer.empty() && (premiumCount < premiumWeight || FreeRequestBuffer.empty())) {
                // Process premium request
                searchRequest_t req = PremiumRequestBuffer.front();
                PremiumRequestBuffer.pop();
                pid_t pid = fork();
                if (pid == -1) {
                    perror("fork");
                } else if (pid == 0) {
                    // Child process
                    printf("[SEARCHER-PROCESSER] Processing PREMIUM request\n");
                    // Now check if the user is PremiumUserIlimited or PremiumUser
                    if (req.type == UserType::PREMIUM) {
                        Search* search = new Search(req);

                        search->PremiumUserSearch(req.numThreads);
                        printf("[SEARCHER-PROCESSER] Premium search finished\n");
                        printf("[SEARCHER-PROCESSER] Total occurrences: %d\n", search->getTotalOccurrences());
                    } else {
                        Search* search = new Search(req);
                        search->unlimitedUserSearch();
                        printf("[SEARCHER-PROCESSER] Premium unlimited search finished\n");
                        printf("[SEARCHER-PROCESSER] Total occurrences: %d\n", search->getTotalOccurrences());
                    }
                   
                    exit(0);
                }
                premiumCount++;
                freeCount = 0;  // Reiniciar contador de gratuitos si premium se procesa
            } else if (!FreeRequestBuffer.empty() && (freeCount < freeWeight || PremiumRequestBuffer.empty())) {
                // Process free request
                searchRequest_t req = FreeRequestBuffer.front();
                FreeRequestBuffer.pop();
                pid_t pid = fork();
                if (pid == -1) {
                    perror("fork");
                } else if (pid == 0) {
                    // Child process
                    // Imprime el contenido de req
                    printf("[SEARCHER-PROCESSER] Processing free request\n");
                    Search* search = new Search(req);
                    search->FreeUserSearch(NUM_THREADS_FREE_SEARCH);
                    printf("[SEARCHER-PROCESSER] Free search finished\n");
                    printf("[SEARCHER-PROCESSER] Total occurrences: %d\n", search->getTotalOccurrences());
                                                                       
                }
                freeCount++;
                premiumCount = 0;  // Reiniciar contador de premium si gratuito se procesa
            }
        }
}