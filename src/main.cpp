#include <iostream>
#include <thread>
#include <unistd.h>
//#include "SistemaDePago.h"
#include "Searcher.h"
#include "utils.h"
#include "User.h"
#include "definitions.h"
#include <cstring> // Include the necessary header file for the strcpy function
#include <csignal> // Include the necessary header file for the signal function
#include <mqueue.h> // Include the necessary header file for the mq_open function
#include <fcntl.h> // Include the necessary header file for the O_CREAT flag
// Include for shm functions
#include <sys/mman.h>
#include <sys/stat.h>
#include "SistemaDePago.h"

void handle_sigint(int sig) 
{
    std::cout << "Caught signal " << sig << std::endl;
    mq_unlink(SEARCHER_QUEUE_NAME);
    mq_unlink(PAYMENT_QUEUE_NAME);
    mq_unlink(PAYMENT_QUEUE_NAME2);

    shm_unlink("/user_prueba");
    exit(0);
}


int main() {
    signal(SIGINT, handle_sigint);

    mqd_t payment_mq;
    mqd_t payment_mq2;

    struct mq_attr payment_attr;
    payment_attr.mq_flags = 0;
    payment_attr.mq_maxmsg = 10;
    payment_attr.mq_msgsize = 128;
    payment_attr.mq_curmsgs = 0;

    payment_mq = mq_open(PAYMENT_QUEUE_NAME, O_CREAT | O_WRONLY, 0644, &payment_attr);
    if(payment_mq == -1) {
        perror("mq_open");
        return -1;
    }

    payment_mq2 = mq_open(PAYMENT_QUEUE_NAME2, O_CREAT | O_WRONLY, 0644, &payment_attr);
    if(payment_mq2 == -1) {
        perror("mq_open");
        return -1;
    }

    // Create a process to handle the payment requests using the static instance of the SistemaDePago class
    pid_t paymentPid = fork();

    if (paymentPid == -1) {
        std::cerr << "Failed to fork()" << std::endl;
        return -1;
    } else if (paymentPid > 0) {
        // Parent process

    } else { // Child process (payment)
        ssize_t bytes_read_payment;
        char paymentBuffer[128];
        SistemaDePago* sistemaDePago = SistemaDePago::getInstance();
        // Loop waiting requests for the queue Payment to process them
        while (true) {
            bytes_read_payment = mq_receive(payment_mq, (char *)&paymentBuffer, 128, NULL);
            rechargeRequest_t* payRequest = reinterpret_cast<rechargeRequest_t*>(paymentBuffer);
            if (bytes_read_payment > 0) {
                int payment = sistemaDePago->procesarPago();
                // Print the payment success message with the user id
                printf("Payment processed for user %d\n", payRequest->user_id);
                // Send the payment confirmation to the Searcher (the confirmation is just to send int 1
                mq_send(payment_mq2, (char *)&payment, sizeof(int), 0);

            }
        }
           
        
    }
    
    mqd_t mq;
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 128;
    attr.mq_curmsgs = 0;

    mq = mq_open(SEARCHER_QUEUE_NAME, O_CREAT | O_WRONLY, 0644, &attr);
    if(mq == -1) {
        perror("mq_open");
        return -1;
    }


    // Create an user and share him in memory shared (do not use the shared_memory_user function)
    int shm_fd = shm_open("/user_prueba", O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open_user_create");
        return -1;
    }

    // Configure the size of the shared memory
    ftruncate(shm_fd, sizeof(User));

    // Map the shared memory in the address space of the process
    User* user_shared = (User*)mmap(0, sizeof(User), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    // Initialize the user use getters
    user_shared->setId(12);
    user_shared->setType(UserType::FREE);
    user_shared->setBalance(30);

    searchRequest_t testRequest = {12, UserType::FREE, "ahora", "/user_prueba", 6};
    mq_send(mq, (char *)&testRequest, sizeof(searchRequest_t), 0);
    printf("Request sent\n");
    
    sleep(5);
    // Create another user and share him in memory shared (do not use the shared_memory_user function) also put random values and send it to the searcher
    int shm_fd2 = shm_open("/user_prueba2", O_CREAT | O_RDWR, 0666);
    if (shm_fd2 == -1) {
        perror("shm_open_user_create");
        return -1;
    }

    // Configure the size of the shared memory
    ftruncate(shm_fd2, sizeof(User));

    // Map the shared memory in the address space of the process
    User* user_shared2 = (User*)mmap(0, sizeof(User), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd2, 0);

    // Initialize the user use getters
    user_shared2->setId(13);
    user_shared2->setType(UserType::PREMIUM);
    user_shared2->setBalance(30);

    searchRequest_t testRequest2 = {13, UserType::PREMIUM_UNLIMITED, "antes", "/user_prueba2", 6};
    mq_send(mq, (char *)&testRequest2, sizeof(searchRequest_t), 0);
       
    pid_t searcherPid = fork();
    if (searcherPid == -1) {
        std::cerr << "Failed to fork()" << std::endl;
        return -1;
    } else if (searcherPid > 0) {
        
    } else { // Child process (searcher)
        Searcher* searcher = Searcher::getInstance();
        printf("[SEARCHER] Searcher process started\n");
        std::thread searcherThread([&searcher]() { searcher->receiveRequests(); });
        // Thread with the searcher processRequest method
        std::thread processRequestThread([&searcher]() { searcher->processRequests(); });

        searcherThread.join();
        processRequestThread.join();

        exit(0);
    }
    
    while (true) {};

    std::cout << "Simulación completa. Todos los usuarios han terminado sus actividades." << std::endl;
    
    return 0;
}
