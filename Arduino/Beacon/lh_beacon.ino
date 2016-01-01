// Constants
typedef unsigned long ulong;

// Timing variables
// Time of an entire cycle
const long CycleTime = 500000;
// Header time when all LEDs are on
const long HeaderTime = 50000;
// Time that the LED sweeps
const long SweepInterp = 20000;

// Idle time is half of cycle time
const long IdleTime = CycleTime / 2;
// Sweep time
const long SweepTimeAll = CycleTime - IdleTime - HeaderTime;
const long SweepTime = SweepTimeAll - SweepInterp - SweepInterp;
const long SweepStart = HeaderTime + SweepInterp;
const long SweepEnd = SweepStart + SweepTime;

const int SensorPin = A0;
const int SweepCount = 7;

typedef struct {
  // Pin number
  int pin;
  // Lighting time (between 0 and 1)
  float lfrac;
  // Light time (in milliseconds after offset)
  ulong ltime;
} Sweep;

Sweep leds[] = {
  // Left
  { 11, 0.125, 0 },
  { 10, 0.250, 0 },
  {  9, 0.375, 0 },
  {  8, 0.500, 0 },
  {  7, 0.625, 0 },
  {  6, 0.750, 0 },
  {  5, 0.875, 0 }
  // Right
};

void setup() {
  // Setup led pins
  for (int i = 0; i < SweepCount; ++i) {
    leds[i].ltime = leds[i].lfrac * SweepTime;
  }
}

void loop() {
  ulong ct = micros();

  ulong tpart = ct % CycleTime;

  if (tpart < HeaderTime) {
    for (int i = 0; i < SweepCount; ++i) {
      analogWrite(leds[i].pin, 255);
    }
  }
  else if (tpart < IdleTime) {
    long diff;
  
    for (int i = 0; i < SweepCount; ++i) {
      diff = tpart - SweepStart - leds[i].ltime;
      if (-SweepInterp <= diff && diff <= SweepInterp) {
        diff = abs(diff);
        diff = SweepInterp - diff;
        float intensity = 255.f * diff / SweepInterp;
        analogWrite(leds[i].pin, (int) intensity);
      }
      else {
        analogWrite(leds[i].pin, 0);
      }
    }
  }
  else { // Zero
    for (int i = 0; i < SweepCount; ++i) {
      analogWrite(leds[i].pin, 0);
    }
  }
}
