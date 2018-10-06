typedef enum{
  Tune_p,
  Tune_i,
  Tune_d,
  Tune_m, //Menu to choose p, i, or d. It's the "no state selected state"
} TuneState;

typedef struct {
  int sensor_left, sensor_right, diff, error;
} Sensors;

typedef struct {
  double iGain, pGain, dGain;

  double dState, iState, iMin, iMax;
} PidObject;
