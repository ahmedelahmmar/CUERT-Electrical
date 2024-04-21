#ifndef _BLDCM_CFG_H_
#define _BLDCM_CFG_H_


/**************************************/
/* Configurable Macros ****************/
/**************************************/

#define mBLDCM_LUT_SIZE									64

#define mBLDCM_POLES									24

#define mBLDCM_MAX_SPEED_KMPH							50

#define mBLDCM_DIAMETER_INCH							20

#define mBLDCM_IDLE_STATE_THRESHOLD_PERCENTAGE			5

#define mBLDCM_TRANSITION_STATE_THRESHOLD_PERCENTAGE	25

/**************************************/
/* Calculated Macros  [DO NOT MODIFY] */
/**************************************/

#define mBLDCM_RADIUS_INCH					(mBLDCM_DIAMETER_INCH / 2)
#define mBLDCM_RADIUS_METER					(mBLDCM_RADIUS_INCH * 0.0254)
//#define mBLDCM_MAX_SPEED_RPM				(uint32_t)((25 / (float)(3 * 3.14 * mBLDCM_RADIUS_METER)) * mBLDCM_MAX_SPEED_KMPH)
#define mBLDCM_MAX_SPEED_RPM				260
#define mBLDCM_POLE_PAIRS					(mBLDCM_POLES / 2)

#endif /* _BLDCM_CFG_H_ */
