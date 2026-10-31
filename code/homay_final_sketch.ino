#include <Homay_Voice_Command_inferencing.h>
#include "driver/i2s_std.h"
#include <Wire.h>

// ---------- Pin Definitions ----------
const int MOTOR_IN1 = 4;
const int MOTOR_IN2 = 5;
const int I2S_SCK = 3;
const int I2S_WS  = 2;
const int I2S_SD  = 1;
const uint8_t INA_ADDR = 0x40;
const float SHUNT_OHM = 0.1;

// ---------- Voice Settings ----------
float GRIP_CONF   = 0.95;       // Minimum confidence required for "grip"
float LOOSE_CONF  = 0.95;       // Minimum confidence required for "loose"
const int COOLDOWN_MS = 1500;   // Wait time to ignore audio after mechanical action

// ---------- Gripper Mechanics ----------
float stallThreshold_mA = 210;  // Current limit to detect a physical stall
const int OPEN_MS          = 600;
const int DUTY             = 200;
const int BLANKING_MS      = 150;
const int STALL_HOLD_MS    = 30;
const int CLOSE_TIMEOUT_MS = 3000;

// ---------- Audio Config ----------
const int GAIN_SHIFT = 11;
const int CH = 0;               // Use left channel from I2S
const int PWM_FREQ = 20000;
const int PWM_BITS = 8;

// ---------- INA219 Current Sensor ----------
uint16_t readReg(uint8_t reg) {
  Wire.beginTransmission(INA_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom((int)INA_ADDR, 2);
  return (Wire.read() << 8) | Wire.read();
}

float readCurrent_mA() {
  int16_t raw = (int16_t)readReg(0x01);
  return raw * 0.01 / SHUNT_OHM;
}

// ---------- Motor Control ----------
void motorStop() {            
  ledcWrite(MOTOR_IN1, 0);
  ledcWrite(MOTOR_IN2, 0);
}

void openGrip() {
  Serial.printf(">>> OPEN %d ms\n", OPEN_MS);
  ledcWrite(MOTOR_IN1, 0);
  ledcWrite(MOTOR_IN2, DUTY);
  delay(OPEN_MS);
  motorStop();
}

void closeGrip() {
  Serial.printf(">>> CLOSE until > %.0f mA\n", stallThreshold_mA);
  ledcWrite(MOTOR_IN2, 0);
  ledcWrite(MOTOR_IN1, DUTY);

  unsigned long t0 = millis(), aboveSince = 0;
  bool stalled = false;
  
  while (millis() - t0 < CLOSE_TIMEOUT_MS) {
    // Average 3 rapid current readings to filter out motor noise
    float mA = (readCurrent_mA() + readCurrent_mA() + readCurrent_mA()) / 3.0;
    unsigned long t = millis() - t0;
    
    // Check for stall after the initial motor inrush spike clears
    if (t > BLANKING_MS && mA > stallThreshold_mA) {
      if (aboveSince == 0) aboveSince = millis();
      if (millis() - aboveSince >= STALL_HOLD_MS) { 
        stalled = true; 
        break; 
      }
    } else {
      aboveSince = 0;
    }
    delay(10);
  }
  motorStop();
  Serial.println(stalled ? "<<< GRIP DETECTED" : "<<< TIMEOUT, no grip detected");
}

// ---------- Microphone & I2S ----------
i2s_chan_handle_t rx;

const int WIN        = EI_CLASSIFIER_RAW_SAMPLE_COUNT;  // 16000 samples = 1 s window
const int CHUNK      = 800;                             // 50 ms slices
const int PRE_CHUNKS = 8;                               // 400 ms pre-roll to catch soft consonants like 'G'
float ONSET_RMS      = 600;                             // Energy threshold to trigger classification

static int16_t win[WIN];
static int16_t pre[PRE_CHUNKS * CHUNK];

void i2sInit() {
  i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
  i2s_new_channel(&chan_cfg, NULL, &rx);
  i2s_std_config_t cfg = {
    .clk_cfg  = I2S_STD_CLK_DEFAULT_CONFIG(16000),
    .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_32BIT, I2S_SLOT_MODE_STEREO),
    .gpio_cfg = {
      .mclk = I2S_GPIO_UNUSED,
      .bclk = (gpio_num_t)I2S_SCK,
      .ws   = (gpio_num_t)I2S_WS,
      .dout = I2S_GPIO_UNUSED,
      .din  = (gpio_num_t)I2S_SD,
      .invert_flags = { .mclk_inv = false, .bclk_inv = false, .ws_inv = false },
    },
  };
  i2s_channel_init_std_mode(rx, &cfg);
  i2s_channel_enable(rx);
}

// Extracts mono audio by reading 32-bit L/R pairs to maintain strict channel alignment
bool readSamples(int16_t* dst, int count) {
  static int32_t buf[256];
  int n = 0;
  
  while (n < count) {
    size_t bytes = 0;
    size_t want = (size_t)(count - n) * 2 * sizeof(int32_t);
    if (want > sizeof(buf)) want = sizeof(buf);
    
    if (i2s_channel_read(rx, buf, want, &bytes, 1000) != ESP_OK || bytes == 0) return false;
    
    int words = bytes / sizeof(int32_t);
    for (int i = 0; i < words; i += 2) {
      if (n >= count) break;
      
      int32_t sample_raw = buf[i + CH]; 
      int32_t s = sample_raw >> GAIN_SHIFT;
      
      if (s > 32767) s = 32767;
      if (s < -32768) s = -32768;
      dst[n++] = (int16_t)s;
    }
  }
  return true;
}

static int getData(size_t offset, size_t length, float *out) {
  numpy::int16_to_float(&win[offset], out, length);
  return 0;
}

// Empties the I2S buffer to discard motor noise or echoes
void flushAudio(int ms) {              
  static int16_t tmp[CHUNK];
  unsigned long t0 = millis();
  while (millis() - t0 < (unsigned long)ms) {
    readSamples(tmp, CHUNK);
  }
}

// ---------- Setup ----------
void setup() {
  Serial.begin(115200);
  delay(2000);
  Wire.begin(8, 9);

  ledcAttach(MOTOR_IN1, PWM_FREQ, PWM_BITS);
  ledcAttach(MOTOR_IN2, PWM_FREQ, PWM_BITS);
  motorStop();

  i2sInit();
  Serial.println("\n=== Autonomous Voice Gripper Initiated ===");
  Serial.println("Listening for 'grip' or 'loose'...");
  flushAudio(500);

  run_classifier_init();
}

// ---------- Main Execution Loop ----------
void loop() {
  // Allow manual serial override for live demo/testing
  if (Serial.available()) {
    char ch = Serial.read();
    if (ch == 'O' || ch == 'o')      { openGrip();  flushAudio(COOLDOWN_MS); }
    else if (ch == 'C' || ch == 'c') { closeGrip(); flushAudio(COOLDOWN_MS); }
    else if (ch == 'T' || ch == 't') {
      float v = Serial.parseFloat();
      if (v > 0) { stallThreshold_mA = v; Serial.printf("New Threshold = %.0f mA\n", v); }
    }
  }

  // 1. Read 50ms chunk and slide it into the rolling pre-record buffer
  static int16_t c[CHUNK];
  if (!readSamples(c, CHUNK)) { 
    Serial.println("Mic read failed."); 
    return; 
  }
  
  memmove(pre, pre + CHUNK, (PRE_CHUNKS - 1) * CHUNK * sizeof(int16_t));
  memcpy(pre + (PRE_CHUNKS - 1) * CHUNK, c, CHUNK * sizeof(int16_t));

  // 2. Calculate RMS to detect voice onset
  double sum = 0;
  for (int i = 0; i < CHUNK; i++) sum += (double)c[i] * c[i];
  float rms = sqrt(sum / CHUNK);
  if (rms < ONSET_RMS) return;           

  // 3. Onset triggered: Construct 1s window (400ms pre-roll + 600ms new audio)
  memcpy(win, pre, PRE_CHUNKS * CHUNK * sizeof(int16_t));
  if (!readSamples(win + PRE_CHUNKS * CHUNK, WIN - PRE_CHUNKS * CHUNK)) return;

  // 4. Run Edge Impulse Neural Network
  signal_t signal;
  signal.total_length = WIN;
  signal.get_data = &getData;
  ei_impulse_result_t result = { 0 };
  if (run_classifier(&signal, &result, false) != EI_IMPULSE_OK) return;

  // 5. Parse Confidence Scores
  float grip = 0, loose = 0, unk = 0;
  for (size_t i = 0; i < EI_CLASSIFIER_LABEL_COUNT; i++) {
    const char* l = result.classification[i].label;
    float v = result.classification[i].value;
    if (strcmp(l, "grip") == 0) grip = v;
    else if (strcmp(l, "loose") == 0) loose = v;
    else unk = v;
  }
  
  Serial.printf("Triggered (RMS %.0f) -> Grip: %.2f | Loose: %.2f | Unknown: %.2f\n", rms, grip, loose, unk);

  // 6. Actuate Mechanism
  if (grip >= GRIP_CONF && grip > loose) {
    Serial.println("Command Recognized: GRIP");
    closeGrip();
    flushAudio(COOLDOWN_MS);
  } else if (loose >= LOOSE_CONF && loose > grip) {
    Serial.println("Command Recognized: LOOSE");
    openGrip();
    flushAudio(COOLDOWN_MS);
  }
}