
/*******************************************************************************************
 *
 * PROJECT          :       Practica 3 SSOO2
 *
 * PROGRAM NAME     :       definitions.h
 *
 * AUTHOR           :       Carlos y Pedro
 *
 * DATE CREATE      :       26/05/23
 *
 * PURPOSE          :       Definition of constants and structures.
 *
 ********************************************************************************************/

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
    int id;                             // Request identifier
    UserType type;                      // Type of user making the request.
    char keyword[15];                   // Keyword for the search.
    char sharedMemoryName[15];          // Shared memory name
    int numThreads;                     // Number of threads to use in the search.


};

#endif // DEFINITIONS_H