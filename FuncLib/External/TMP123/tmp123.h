/** \file tmp123.h
	 \brief Texas Instruments TMP123 Drivers
*/

/**	 

	\ingroup avr_external
 	\defgroup tmp123 Texas Instruments TMP123 Drivers
 	\code #include "tmp123.h" \endcode

 	 
	 \par Description
   Interfaces with the TMP123 over the SPI bus.
*/
//@{

#ifndef	_TMP123_ROUTINES
#define	_TMP123_ROUTINES

#define TMP123_PORT	PORTA
#define TMP123_DDR	DDRA
#define TMP123_PIN	PINA
#define TMP123_SO		(5)
#define TMP123_nCS	(4)
#define TMP123_SCK	(3)

#define TMP123_RESOULTION 	(0.0625)

void TMP123_Init(void);
int16_t TMP123_GetTemp(void);
float	TMP123_GetTempFP(int16_t inWord);

#endif
