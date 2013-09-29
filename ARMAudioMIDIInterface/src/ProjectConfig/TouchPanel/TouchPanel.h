

#include "HY32D_LCD/TouchPanel/TouchPanel.h"


//Coordinate *Read_Ads7846(void);


#define GET_RAW_TOUCH_POINT() 			Read_Ads7846()
#define GET_NORMALISED_TOUCH_POINT(resultPtr, rawDataPtr)	TP_BudgetGetDisplayPoint(resultPtr, rawDataPtr);
