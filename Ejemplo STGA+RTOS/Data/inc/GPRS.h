

#ifndef INC_GPRS_H_
#define INC_GPRS_H_


#include "lpc_types.h"



/**
 * @brief Signal definitions
 */
typedef enum{
MARGINAL,
POOR,
GOOD,
EXCELLENT
}GPRS_SIGNAL_T;

Status GPRS_SetSignal(uint8_t CSQ);
Status GPRS_GetSignal(GPRS_SIGNAL_T* Signal);
Status GPRS_GetFileSent(char* FileName);
Status GPRS_SetFileSent(char* FileName);


#endif /* INC_GPRS_H_ */
