
/*******************************************************************************************
 *
 * PROJECT          :       Practica 3 SSOO2
 *
 * PROGRAM NAME     :       SistemaDePago.cpp
 *
 * AUTHOR           :       Carlos y Pedro
 *
 * DATE CREATE      :       26/05/23
 *
 * PURPOSE          :       Provide a method for obtaining a single instance and a method for processing payments.
 *
 ********************************************************************************************/

#include "SistemaDePago.h"
#include <thread>
#include <chrono>
#include <random> 

// Inicialización de la instancia del Singleton
SistemaDePago* SistemaDePago::instance = nullptr;

SistemaDePago* SistemaDePago::getInstance() {
    if (instance == nullptr) {
        instance = new SistemaDePago();
    }
    return instance;
}

int SistemaDePago::procesarPago() {
    printf("[PAYMENT-PROCESSOR] Processing payment...\n");
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(20, 100);

    int payment = distr(gen);

    // Simulate payment processing time
    std::this_thread::sleep_for(std::chrono::seconds(5));

    return payment;
}

