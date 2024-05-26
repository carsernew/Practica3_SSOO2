
/*******************************************************************************************
 *
 * PROJECT          :       Practica 3 SSOO2
 *
 * PROGRAM NAME     :       Search.h
 *
 * AUTHOR           :       Carlos y Pedro
 *
 * DATE CREATE      :       26/05/23
 *
 * PURPOSE          :       Definition of the Search class
 *
 ********************************************************************************************/

#ifndef SEARCH_H
#define SEARCH_H
#include "definitions.h"
#include <queue>
#include <mutex>
#include <mqueue.h>
#include "User.h"
#include <semaphore.h>

class Search {
   
public:

    char keyword[15];
    User *user;
    std::vector<std::string> files;
    std::mutex mtx;
    sem_t sem;
    static mqd_t paymentQueue; // Declaración de la cola de mensajes de pago
    static mqd_t paymentQueue2; // Declaración de la cola de mensajes de pago
    
    Search();
    Search(searchRequest_t request);
    void FreeUserSearch(int numThreads);
    void FreeSearchInFile(const std::string& file, const std::string& keyword);
    int getTotalOccurrences();
    void unlimitedUserSearch();
    void UnlimitedSearchInFile(const std::string& file, const std::string& keyword);
    void PremiumUserSearch(int numThreads);
    void PremiumSearchInFile(const std::string& file, const std::string& keywords);
    
};


#endif // SEARCH_H