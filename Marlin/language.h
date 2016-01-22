#ifndef LANGUAGE_H
#define LANGUAGE_H


#define PROTOCOL_VERSION "1.0"

#define MACHINE_NAME "printrbot"
#define FIRMWARE_URL "printrbot.com"

#define MSG_POWERUP "PowerUp"
#define MSG_EXTERNAL_RESET "External Reset"
#define MSG_BROWNOUT_RESET "Brown out Reset"
#define MSG_WATCHDOG_RESET "Watchdog Reset"
#define MSG_SOFTWARE_RESET "Software Reset"
#define MSG_SETTINGS_STORED "{\"m\":\"m500\", \"d\":1}"
#define MSG_AUTOTUNE_START "{\"m\":\"info\", \"d\":\"PID Autotune start\"}"

#define MSG_OK "ok"

#define ERR_STEPPER_TOO_HIGH "Steprate too high"
#define ERR_ZPROBE_OUT "Z Probe outside bed"
#define ERR_POSITION_UNKNOWN "Home X/Y before Z"
#define ERR_KILLED "Printer halted. kill() called!"
#define ERR_STOPPED "Printer stopped due to errors"
#define ERR_UNKNOWN_COMMAND "{\"m\":\"err\", \"d\": \"Unknown command\"}"
#define ERR_COLD_EXTRUDE_STOP "cold extrusion prevented"
#define ERR_LONG_EXTRUDE_STOP "long extrusion prevented"
#define ERR_AUTOTUNE_FAILED "{\"m\":\"err\", \"d\":\"PID Autotune failed. Bad extruder number\"}"

#endif // ifndef LANGUAGE_H
