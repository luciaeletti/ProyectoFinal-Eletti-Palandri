/** @file Conditions.c
 * @version		1.0
 * @date		15/04/2024
 * @author		
 * @note
 * 
 *  Facultad de Ingenieria - UNER
 *  Laboratorio de prototipado Electronico &3D
 */
/*==================[inclusions]=============================================*/
#include "conditions.h"

/*==================[macros]=================================================*/
CONDIC_FUNC_T Current;
/*==================[internal functions declaration]==========================*/

/*==================[external functions declaration]==========================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions declaration]==========================*/

void SetConditions(CONDIC_FUNC_T* pCondition){
	Current=*pCondition;
}

void GetConditions(CONDIC_FUNC_T* pCondition){
	*pCondition=Current;
}
