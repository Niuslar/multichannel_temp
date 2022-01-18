/*
 * @file pid_control.h
 *
 */

/*  Created on: 11 Jan 2022
 *      Author: niuslar
 */

#ifndef INC_HEATERS_H_
#define INC_HEATERS_H_

#include "main.h"

#define DEFAULT_SET_POINT	 	2500
#define MIN_VRANGE   (0.569/ADC_VDDA*(ADC_RES))
#define MAX_VRANGE	 (2.672/ADC_VDDA*(ADC_RES))

/* Exported Functions Prototypes */
void PIDInit();
void PIDControl(float input);

#endif /* INC_HEATERS_H_ */
