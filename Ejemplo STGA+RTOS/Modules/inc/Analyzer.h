

#ifndef INC_ANALYZER_H_
#define INC_ANALYZER_H_

#define HUM_MAX_1 80
#define HUM_MAX_2 85
#define HUM_MAX_3 90

#define TEMP_MAX_1 40
#define TEMP_MAX_2 45
#define TEMP_MAX_3 50

#define WSPEED_MAX_1 10
#define WSPEED_MAX_2 15
#define WSPEED_MAX_3 20

#define FLOW_MAX_1 300
#define FLOW_MAX_2 375
#define FLOW_MAX_3 450

#define LAT_MAX 34
#define LAT_MIN 30
#define LON_MAX 62
#define LON_MIN 58

void vMonitoringTask( void *pvParameters );

#endif /* INC_ANALYZER_H_ */
