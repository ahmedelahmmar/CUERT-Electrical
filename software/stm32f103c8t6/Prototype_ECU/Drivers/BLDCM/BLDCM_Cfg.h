#ifndef _BLDCM_CFG_H_
#define _BLDCM_CFG_H_


/**************************************/
/* Configurable Macros ****************/
/**************************************/

#define BLDCM_SPWM_LUT_SIZE								125

#define BLDCM_POLES										24

#define BLDCM_MAX_SPEED_KMPH							50

#define BLDCM_DIAMETER_INCH								20

#define BLDCM_IDLE_STATE_THRESHOLD_PERCENTAGE			5

#define BLDCM_TRANSITION_STATE_THRESHOLD_PERCENTAGE		20

/**************************************/
/* Calculated Macros  [DO NOT MODIFY] */
/**************************************/

#define BLDCM_RADIUS_INCH					(BLDCM_DIAMETER_INCH / 2)
#define BLDCM_RADIUS_METER					(BLDCM_RADIUS_INCH * 0.0254)
#define BLDCM_MAX_SPEED_RPM					((25 / (3 * 3.14 * BLDCM_RADIUS_METER)) * BLDCM_MAX_SPEED_KMPH)
#define BLDCM_POLE_PAIRS					(BLDCM_POLES / 2)

#endif /* _BLDCM_CFG_H_ */
