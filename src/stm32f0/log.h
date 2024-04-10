/*
 * log.h
 *
 *  Created on: Jan 17, 2019
 *      Author: eug
 */

#ifndef CORE_INC_LOG_H_
#define CORE_INC_LOG_H_


void LogInit(void);

void lprint(char *msg);
void lnprint(char *msg, size_t len);


#endif /* CORE_INC_LOG_H_ */
