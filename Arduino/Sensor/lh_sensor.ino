// Constants
typedef unsigned long ulong;

const int DEBUG_READ = 0;

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
  int ltime;
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
  Serial.begin(9600);
}

// Buffered information
int vbuffer[20] = {};
int vbufferIndex = 0;
ulong vaverage = 0;
const int vbufferSize = 20;

// Detection logic
int state = 0;
// Allowable error
int verror = 1;
long terror = 10000;
// Last voltage (for transitions)
int lv = 0;
// Saved voltage
int rv = 0;
long rvtime = 0;
// Max Detected Voltage
int maxv = -1;
long maxvts = 0;
long maxvte = 0;

void loop() {
  // Get the current time
  ulong ct = micros();
  
  int voltage = analogRead(SensorPin);

  // Take average
  ulong av = vaverage;
  av /= vbufferSize;
  // Every 20 loops
  if (vbufferIndex % 20 == 0) {
    // Display
    if (DEBUG_READ) {
      Serial.println(av);
    }

    // Run detection logic
    int diff;
    long ldiff;
    if (state == 0) {
      // Detect a header (rising edge)
      diff = av - lv;
      if (diff > verror) {
        state = 1;
        rv = av;
        rvtime = ct;
      }
    }
    else if (state == 1) {
      // Detect a header (falling edge)
      diff = av - rv;
      if (diff > verror) {
        // New rising edge
        rv = av;
        rvtime = ct;
      }
      else if (diff < -verror) {
        // Falling edge
        ldiff = ct - rvtime;
        ldiff = ldiff - HeaderTime;
        ldiff = abs(ldiff);
        if (ldiff < terror) {
          // Found header of proper time length
          state = 2;
          maxv = -1;
          rvtime = ct;
          lv = av;
        }
      }
    }
    else if (state == 4) {
      // Idle until next cycle
      if (ct - rvtime > IdleTime - HeaderTime) {
        state = 0;
      }
    }
    lv = av;
  }

  // State 2 and 3
  if (state == 2) {
    // While within SweepTime, detect peaks
    if (ct - rvtime < SweepTimeAll) {
      // Detect a new max
      if (av > maxv + verror) {
        // New peak, set max record
        maxv = av;
        maxvts = ct - rvtime;
        maxvte = ct - rvtime;
      }
      else if (av > maxv - verror) {
        // Old peak, update end time
        maxvte = ct - rvtime;
      }
    }
    else {
      // Peak detection time is over, move on to analysis
      state = 3;
      rvtime = ct;
    }
  }
  else if (state == 3) {
    // Check if peak was detected
    if (maxv > 0) {
      long ptime = (maxvte + maxvts) / 2;
      // Compute angle fraction
      long sts = SweepTime / 1000;
      long angle = (ptime - SweepInterp) / sts;
      if (!DEBUG_READ) {
        Serial.print(maxv);
        Serial.print(" ");
        Serial.print(angle);
        Serial.print(" ");
        Serial.print(ptime);
        Serial.print("\n");
      }
    }
    state = 4;
  }

  // Add to the buffer
  vaverage -= vbuffer[vbufferIndex];
  vbuffer[vbufferIndex] = voltage;
  vaverage += vbuffer[vbufferIndex];
  vbufferIndex = (vbufferIndex + 1) % vbufferSize;
}
