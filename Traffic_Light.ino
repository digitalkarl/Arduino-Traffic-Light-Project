
// An auto timer option is enabled if pin10 is HIGH but if grounded then no auto timer
// Pedestrian Request Button is connected to Pin 2 via an interupt
// A Pedestrian acknowledgement LED is connected to Pin 12
// Speaker is connected to Pin 9

const int TrafficRed = 3;
const int TrafficAmber = 4;
const int TrafficGreen = 5;
const int pedestrianRed = 7;
const int pedestrianGreen = 8;
const int speakerPin = 9;
const int buttonPin = 2; // pushbutton doesn't need hardware debouncing
const int pedestrianAcknowledgeLED_Red = 12;
const int pedestrianAcknowledgeLED_Green = 11;
const int autoSwitchPin = 10;

volatile int pinState = LOW; // toggling purposes
int counter = 0; // creates a loop of set length which drives time for On State
long lastTime = 0; // counter for Auto Timer
long autoTimerDelay = 60000; // delay period for auto timer

//******************** These values create the audio beep tone
int duration = 10;
int onTime = 500;
int offTime = 2000;
//********************

void setup() {
  for (int i = 3; i < 13; i++) // pins 3-12 as OUTPUT's
  {
    pinMode (i, OUTPUT); 
  }

  pinMode (buttonPin, INPUT); // digital pin2 is set to be an INPUT (interupt 0)
  pinMode (autoSwitchPin, INPUT); // digital pin10 is set to be an INPUT to detect auto switch mode
  
  attachInterrupt(0, pressDetector, RISING); // interrupt captures any button presses using DigitalPin2
  
  lastTime = millis(); // record the millis
  digitalWrite (pedestrianAcknowledgeLED_Green, HIGH);
}

void pressDetector() // This is the ISR Routine and simply sets pinState HIGH if a press is detected
{
  pinState = HIGH;
  digitalWrite (pedestrianAcknowledgeLED_Red, HIGH);
  digitalWrite (pedestrianAcknowledgeLED_Green, LOW);
}

void trafficStop() // Manages all the lamps needed to correctly show RED for Traffic & GREEN for pedestreans
{
  digitalWrite (TrafficRed, HIGH);
  digitalWrite (TrafficAmber, LOW);
  digitalWrite (TrafficGreen, LOW);
  digitalWrite (pedestrianGreen, HIGH);
  digitalWrite (pedestrianRed, LOW);
}

void trafficAmber() //Manages all the lamps needed to correctly show AMBER for Traffic & RED for pedestreans
{
  digitalWrite (TrafficRed, LOW);
  digitalWrite (TrafficAmber, HIGH);
  digitalWrite (TrafficGreen, LOW);
  digitalWrite (pedestrianGreen, LOW);
  digitalWrite (pedestrianRed, HIGH);
}

void trafficGreen() // Manages all the lamps needed to correctly show GREEN for Traffic & RED for pedestreans
{
  digitalWrite (TrafficRed, LOW);
  digitalWrite (TrafficAmber, LOW);
  digitalWrite (TrafficGreen, HIGH);
  digitalWrite (pedestrianGreen, LOW);
  digitalWrite (pedestrianRed, HIGH);
}

void loop() {

  int autoSwitchState = digitalRead(autoSwitchPin); // Read if Auto Timer Switch is active

  // ************ Auto Timer activetaded if option switch is set HIGH

  if (autoSwitchState == HIGH) {
    if ((millis() - lastTime) > autoTimerDelay) {
      digitalWrite (pedestrianAcknowledgeLED_Green, LOW); // Change the ped acknowledge LEd's
      digitalWrite (pedestrianAcknowledgeLED_Red, HIGH);
      for (int x = 15; x < 500; (x = x + 1) / 0.25) // squeal
      {
        digitalWrite(speakerPin, HIGH);
        delayMicroseconds(x);
        digitalWrite(speakerPin, LOW);
        delayMicroseconds(x);
      }
      trafficAmber(); // call amber
      delay(1000);
      trafficStop(); // call stop
      while ( counter < 100) // this counter serves to set walktime for pedestrians
      {
        for (int i = 0; i < duration; i++)
        {
          digitalWrite(speakerPin, HIGH);
          delayMicroseconds(onTime); // changing this duration will efffect its frequency & thus tone
          // longer delay will be lower note, shorter delay will be higher note
          digitalWrite(speakerPin, LOW);
          delayMicroseconds(offTime);
        }
        delay(100); // gap between notes
        counter++;
      }
      trafficAmber(); // now that we are done walking, call amber
      digitalWrite (pedestrianAcknowledgeLED_Red, LOW);
      digitalWrite (pedestrianAcknowledgeLED_Green, HIGH);
      counter = 0;
      pinState = LOW; // need to reset pinState to LOW
      lastTime = millis();
      delay(1000);
    }
  }
    // ******************************** end auto timer if selected!
    
    if (pinState == HIGH) // if the button has been pressed then do this
    {
      delay(1000);
      for (int x = 15; x < 500; (x = x + 1) / 0.25) // squeal
      {
        digitalWrite(speakerPin, HIGH);
        delayMicroseconds(x);
        digitalWrite(speakerPin, LOW);
        delayMicroseconds(x);
      }
      trafficAmber(); // call amber
      delay(1000);
      trafficStop(); // call stop
      lastTime = millis(); // reset the auto timer
      while ( counter < 100) // this counter serves to set walktime for pedestreans
      {
        for (int i = 0; i < duration; i++)
        {
          digitalWrite(speakerPin, HIGH);
          delayMicroseconds(onTime); // changing this duration will efffect its frequency & thus tone
          // longer delay will be lower note, shorter delay will be higher note
          digitalWrite(speakerPin, LOW);
          delayMicroseconds(offTime);
        }
        delay(100); // gap between notes
        counter++;
      }
      trafficAmber(); // now that we are done walking, call amber
      digitalWrite (pedestrianAcknowledgeLED_Red, LOW);
      digitalWrite (pedestrianAcknowledgeLED_Green, HIGH);
      counter = 0;
      pinState = LOW; // need to reset pinState to LOW
      delay(1000);
    }
    trafficGreen(); // call green
    for (int i = 0; i < duration; i++) // sound for No Walking
    {
      digitalWrite(speakerPin, HIGH);
      delayMicroseconds(onTime);
      digitalWrite(speakerPin, LOW);
      delayMicroseconds(offTime);
    }
  delay(1000); // slow beeps for pedestrians
}
