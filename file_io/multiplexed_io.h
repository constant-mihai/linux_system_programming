/****************************************** 

* File Name : multiplexed_io.h

* Creation Date : 14-09-2018

* Last Modified :

* Created By : Mihai Constantin constant.mihai@googlemail.com

* License : 

******************************************/

/**
 * Purpose
 */

#ifndef __MULTIPLEXED_IO_H_
#define __MULTIPLEXED_IO_H_

#include <sys/select.h>
#include <sys/time.h>

/**
 * ******************************************************
 * @brief 
 * 1. Tell me when any of these fds become ready for i/o
 * 2. Sleep until one or more fds are ready
 * 3. Woken up! What is ready?
 * 4. Handle all fds ready for i/o without blocking
 * 5. go back to 1.
 * ******************************************************
**/
int wrapper_select();

int wrapper_poll();

#endif
