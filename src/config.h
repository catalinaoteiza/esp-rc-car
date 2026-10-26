const char* ssid_base = "ROV-"; //Enter SSID
const char* password = "1234567890"; //Enter Password

/* Pin definitions */ 


/* Pin definitions */

// ============================================================
// ESP8266 NodeMCU
// ============================================================
#if defined(ESP8266)

// Front Left
const int pwm_FL = D1;  // GPIO5
const int dir_FL = D3;  // GPIO0

// Front Right
const int pwm_FR = D2;  // GPIO4
const int dir_FR = D4;  // GPIO2

// Rear Left
const int pwm_RL = D5;  // GPIO14
const int dir_RL = D6;  // GPIO12

// Rear Right
const int pwm_RR = D7;  // GPIO13
const int dir_RR = D8;  // GPIO15


// ============================================================
// Auxiliary DC motor pins
// Each motor uses two H-bridge inputs for bidirectional control
// ============================================================

// Three auxiliary DC motors: two inputs per H-bridge
const int pwm_M1 = D0;
const int dir_M1 = D8;  // Example only: conflicts with traction!
const int pwm_M2 = D5;  // Example only: conflicts with traction!
const int dir_M2 = D6;  // Example only: conflicts with traction!
const int pwm_M3 = D7;  // Example only: conflicts with traction!
const int dir_M3 = D4;  // Example only: conflicts with traction!

// ============================================================
// Auxiliary servo configuration
// ============================================================

const int servoPin = D2; // Example only: conflicts with traction!

// ============================================================
// ESP32 DevKit V1 / ESP32-WROOM-32
// ============================================================
#elif defined(ESP32)

// Front Left
const int pwm_FL = 25;
const int dir_FL = 26;

// Front Right
const int pwm_FR = 27;
const int dir_FR = 32;

// Rear Left
const int pwm_RL = 33;
const int dir_RL = 13;

// Rear Right
const int pwm_RR = 14;
const int dir_RR = 4;

const int pwm_M1 = 16;
const int dir_M1 = 17;
const int pwm_M2 = 18;
const int dir_M2 = 19;
const int pwm_M3 = 21;
const int dir_M3 = 22;
const int servoPin = 23;


#else
#error "Unsupported microcontroller: use ESP8266 or ESP32"
#endif






