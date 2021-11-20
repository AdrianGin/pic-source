/*
 * XmasTwinkle.h
 *
 *  Created on: 20/11/2021
 *      Author: Adrian
 */

#ifndef XMASTWINKLE_H_
#define XMASTWINKLE_H_

#include "Pattern.h"

class XmasTwinkle : public Pattern {

public:

	XmasTwinkle(int16_t position, uint16_t length, uint32_t colour, uint16_t prescaler, uint16_t endPhase, uint16_t startPhase = 0) noexcept;
   virtual ~XmasTwinkle();

   //Updates the colours in the colour buffer based off the parameters
   void update() override;
   void reset(void) override;
   bool isFinished(void) override;

private:
   bool IsFadeIn;
   uint16_t phase;
   uint16_t m_endPhase;

};

#endif /* XMASTWINKLE_H_ */
