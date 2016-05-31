#ifndef SENSOR_H
#define SENSOR_H


#define SENSOR_OUTPUT	(INCH_A1)

/* Variable Output */
#define SENSOR_OUTPUT2	(INCH_A0)
#define POT_INPUT			(INCH_A3)

#define MAX_CROSSTALK	(1000)
#define DEFAULT_CROSSTALK (12)
#define MIN_CROSSTALK	(0)

#define CH0        (0)
#define CH1        (1)
#define CH2        (2)
#define CH3        (3)
#define CH4        (4)
#define CH5        (5)
#define CH6        (6)
#define CH7        (7)
#define CH8        (8)
#define CH9        (9)
#define CH10       (10)
#define CH11       (11)
#define CH12       (12)
#define CH13       (13)
#define CH14       (14)
#define CH15       (15)




typedef struct {

   uint16_t	SensorInputPort;
	int16_t CrosstalkDelay;
} SensorSettings_t;

extern SensorSettings_t* SensorSettings;

void SensorInit(void);
/* Changes the sampling channel to channel, CH0 -> CH15 */
void SensorChannel(uint8_t channel);


/* New Port must be within INCH_A0 -> INCH_Ax */
void SensorInputSelect(uint8_t newPort);
uint8_t GetSensorInput(void);

/* Crosstalk controls the delay between changing the channel */
int16_t GetCrossTalkDelay(void);
void SetCrossTalkDelay(int16_t newCrosstalk);

uint16_t SensorPotValue(void);




#endif
