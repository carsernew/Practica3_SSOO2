
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
 * PURPOSE          :       Functions to create and obtain information from shared memory
 *
 ********************************************************************************************/

#ifndef UTILS_H
#define UTILS_H

#include <mqueue.h>
#include <string>
#include "User.h"
#include "definitions.h"

void create_shared_memory_user(User* user, const char *shared_memory_name);
void get_shared_memory_user(User *user, const char *shared_memory_name);

#endif // UTILS_H