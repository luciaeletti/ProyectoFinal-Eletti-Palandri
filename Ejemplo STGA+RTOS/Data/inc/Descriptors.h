
#ifndef STGA_RTOS_DATA_INC_DESCRIPTORS_H_
#define STGA_RTOS_DATA_INC_DESCRIPTORS_H_

#include "lpc_types.h"

#define  MAX_NUM_SNS 4 /*the maximum number of connected sensors*/


/**
 * @brief Sensor message identifier definitions
 */
enum {
	SEND_CFG=0,
	RECEIVE_DATA,
	RECEIVE_CFG
};

/**
 *@brief SNS frequency type definition.
 */
typedef enum {
	FREQ_0 	= 0,	/*operation frequency 0*/
	FREQ_1,	 		/*operation frequency 1*/
	FREQ_2, 		/*operation frequency 2*/
	FREQ_3			/*operation frequency 3*/
} DSC_FREQ_T;

/**
 *@brief SNS resolution type definition.
 */
typedef enum {
	RESOL_0 = 0,/*operation resolution 0*/
	RESOL_1,	/*operation resolution 1*/
	RESOL_2,	/*operation resolution 2*/
	RESOL_3		/*operation resolution 3*/
} DSC_RESOL_T;

/**
 *@brief SNS transmission modes type definition.
 */
typedef enum {
	MODE_SINGLE= 0,	/*single transmission mode*/
	MODE_ONDEMAND,	/*on demand transmission mode*/
	MODE_AUTOMATIC,	/*automatic transmission mode**/
	MODE_MONITOR
} DSC_MODE_T;


/**
 * @brief Sensor message identifier definitions
 */
typedef enum {
	ID_CPU=1,
	ID_GPS,
	ID_ANEMOMETER,
	ID_THERMOHYGROMETER,
	ID_FLOWMETER,
	/*ID_VELOCIMETER*/

}DSC_IDENTIFIER_T;

/**
 *@brief Sensor descriptor definitions
 */
typedef struct{
DSC_IDENTIFIER_T id;            /*SNS id*/
FunctionalState  state;  /*SNS functional state*/
DSC_FREQ_T		 frequency;
DSC_MODE_T		 mode;
DSC_RESOL_T	     resolution;

}DSC_DESCRIPTOR_T;

Status DSC_AddSensorToList(DSC_DESCRIPTOR_T* pDescriptor);
Status DSC_RemoveSensorFromList(DSC_DESCRIPTOR_T* pDescriptor);
Status DSC_GetSensorFromList(DSC_DESCRIPTOR_T* pDescriptor);
Status DSC_RestoreList(short value);
short DSC_GetSensorListLength(void);
Status DSC_ResetSensorsList(void);

#endif /* STGA_RTOS_DATA_INC_DESCRIPTORS_H_ */
