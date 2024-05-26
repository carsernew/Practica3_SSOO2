#ifndef DEFINITIONS_H
#define DEFINITIONS_H


#include <string>
#include <vector>

#define DEFAULT_USERS 50
#define DEFAULT_BALANCE 2500
#define SEARCHER_QUEUE_NAME "/searcherQueue"
#define PAYMENT_QUEUE_NAME "/paymentQueue"
#define PAYMENT_QUEUE_NAME2 "/paymentQueue2"

#define FREE_USER_SEARCH_LIMIT 100
#define NUM_THREADS_FREE_SEARCH 4



struct rechargeRequest_t {
    int user_id;
};

enum class UserType {
    FREE,
    PREMIUM,
    PREMIUM_UNLIMITED
};

struct searchRequest_t {
    int id;                           // Identificador de la solicitud.
    UserType type;                    // Tipo de usuario que realiza la solicitud.
    char keyword[15];              // Palabra clave para la búsqueda.
    char sharedMemoryName[15]; // Nombre de la memoria compartida
    int numThreads;                 // Número de hilos a utilizar en la búsqueda.


};

#endif // DEFINITIONS_H