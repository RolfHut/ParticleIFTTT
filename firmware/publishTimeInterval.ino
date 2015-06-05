/*
this code, intended to run on a Particle Core (www.particle.io), publishes a stream of measurements to 
the Particle Cloud. 

The measurement is a count of the times that the value of pin D0 becomes high in the interval 
"intervalOnline". For example: when a hall sensor is connected to D0, it counts how often a magnet
has been close to the sensor, usefull for counting revelations of a wheel.

Using IFTTT (ifttt.com) the measurements are automatically uploaded to a google spreadsheet. See 
https://ifttt.com/recipes/246322-save-data-from-spark-core-in-google-spreadsheet for the recipe.
*/

//the topic that will be published to the Particle Cloud. maximum of 60 characters.
//it is best practice to "nest" the topic, ie. "measurement/EXPERIMENTNAME"
//Note that these are public posts: anyone listening to the Particle Cloud can
//view them.
const String topic = "measurement/EXPERIMENTNAME";

//this enables Serial communication. When TRUE the Particle will send the measurement value over
//the USB serial every "intervalSerial" seconds. Only change to FALSE when no further debugging
//is needed
#define DEBUG_SERIAL TRUE 

//Sensor is connected to this pin.
const int inPin = D0; 

//number of seconds between uploads to the Particle Cloud. This may never be lower than 60
//because IFTTT does not allow for more triggers than 1 per minute.
const int intervalOnline = 120; 

//number of seconds between uploads over the USB serial
const int intervalSerial = 1; 

//variable to hold the measurement
volatile int counter;

void setup(){
    
    //start Serial communication
    if (DEBUG_SERIAL) Serial.begin(9600);

    //attach an interrupt to inPin. Everytime inPin changes from 
    //LOW to HIGH, the function counting is called.
    attachInterrupt(inPin, counting, RISING);
}

void loop(){
    
    //get timestamp
    int t=millis();

    //every intervalSerial seconds, send measurement over Serial USB
    if (DEBUG_SERIAL){
        if (t%(intervalSerial*1000)<5){
            Serial.println(counter);
            delay(5);
        }
    }
    
    //every intervalOnline seconds, publish measurement to Spark Cloud
    if (t%(intervalOnline*1000)<10){
        String counterText = String(counter, DEC);
        Particle.publish(topic,counterText);
	counter = 0;
        delay(10);
    }

}

void counting(){
    counter = counter + 1;
}
