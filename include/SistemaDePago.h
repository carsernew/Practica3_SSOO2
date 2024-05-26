
/*******************************************************************************************
 *
 * PROJECT          :       Practica 3 SSOO2
 *
 * PROGRAM NAME     :       SistemaDePago.h
 *
 * AUTHOR           :       Carlos y Pedro
 *
 * DATE CREATE      :       26/05/23
 *
 * PURPOSE          :       Implementation of a Singleton called SisitemaDePago
 *
 ********************************************************************************************/

#ifndef SISTEMA_DE_PAGO_H
#define SISTEMA_DE_PAGO_H

class SistemaDePago {
public:
    // Method to get the Singleton instance
    static SistemaDePago* getInstance();

    // We remove the copy constructor and the assignment operator
    SistemaDePago(const SistemaDePago&) = delete;
    void operator=(const SistemaDePago&) = delete;
    int procesarPago();

private:
    // Private constructor
    SistemaDePago() {}

    // Static instance of the Singleton
    static SistemaDePago* instance;
};

#endif // SISTEMA_DE_PAGO_H