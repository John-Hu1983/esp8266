#ifndef _APP_WIFI_H_
#define _APP_WIFI_H_

typedef enum
{
  WIFI_AET_NONE = 0,
  WIFI_AET_SCAN,
  WIFI_AET_AP_ROLE,
  WIFI_AET_PROPERTY,
  WIFI_AET_MAX,
} Wifi_App_Event_t;

typedef enum
{
  WIFI_STEP_INIT = 0,
  WIFI_STEP_LOOP,

  WIFI_STEP_NETWORKS,
  WIFI_STEP_AP_ROLE,
  WIFI_STEP_PROPERTY,

  WIFI_STEP_QUIT,
} Wifi_Step_t;

int app_wifi_loop(osvar_t ms, btnval_t btn, encoder_t *encoder);

#endif
