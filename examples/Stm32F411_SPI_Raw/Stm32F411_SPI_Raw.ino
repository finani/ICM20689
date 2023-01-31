#include "ICM20689.h"

static const uint8_t MOSI_PIN = PA7;
static const uint8_t MISO_PIN = PA6;
static const uint8_t SCLK_PIN = PA5;

static const uint8_t LED_PIN     = PC13;
static const uint8_t CS_PIN  = PA4;
static const uint8_t INT_PIN = PA1;

static SPIClass spi = SPIClass(MOSI_PIN, MISO_PIN, SCLK_PIN);

static ICM20689 IMU(spi, CS_PIN);

void setup() {
  // serial to display data
  Serial.begin(115200);
  while(!Serial) {}

  // start communication with IMU
  auto status = IMU.begin();
  if (status < 0) {
    Serial.println("IMU initialization unsuccessful");
    Serial.println("Check IMU wiring or try cycling power");
    Serial.print("Status: ");
    Serial.println(status);
    while(1) {}
  }
  // setting DLPF bandwidth to 20 Hz
  IMU.setDlpfBandwidth(ICM20689::DLPF_BANDWIDTH_21HZ);
  // setting SRD to 19 for a 50 Hz update rate
  IMU.setSrd(19);
  // enabling the data ready interrupt
  IMU.enableDataReadyInterrupt();
  // attaching the interrupt to microcontroller pin 1
  pinMode(1,INPUT);
  attachInterrupt(1,getIMU,RISING);
}

void loop() {}

void getIMU() {

  // read the sensor
  IMU.readSensor();
  
  // display the raw data
  Serial.print(IMU.getAccelX_count());
  Serial.print("\t");
  Serial.print(IMU.getAccelY_count());
  Serial.print("\t");
  Serial.print(IMU.getAccelZ_count());
  Serial.print("\t");
  Serial.print(IMU.getGyroX_count());
  Serial.print("\t");
  Serial.print(IMU.getGyroY_count());
  Serial.print("\t");
  Serial.println(IMU.getGyroZ_count());
}
