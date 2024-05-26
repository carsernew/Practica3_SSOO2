
/*******************************************************************************************
 *
 * PROJECT          :       Practica 3 SSOO2
 *
 * PROGRAM NAME     :       User.h
 *
 * AUTHOR           :       Carlos y Pedro
 *
 * DATE CREATE      :       26/05/23
 *
 * PURPOSE          :       Definition of the User class with its attributes
 *
 ********************************************************************************************/

#ifndef USER_H
#define USER_H
#include <string>
#include "definitions.h"

class User {
public:

    int id;
    int balance;
    UserType type;
    User(int id, int balance, UserType type);
    User();

    int getId();
    void setId(int id);
    int getBalance();
    void setBalance(int balance);
    UserType getType();
    void setType(UserType type);
    void decreaseBalance();
};

#endif // USER_H