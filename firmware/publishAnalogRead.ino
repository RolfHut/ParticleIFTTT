/*
this code, intended to run on a Spark Core (www.spark.io), publishes a stream of measurements to 
the Spark Cloud. This is part of the "no Code Internet of Things" course material. 

Students are supplied with a Spark Core that runs this firmware. They build an analog sensor that they
connect to pin A0. The Spark Core will publish their measurement to the Spark Cloud every 120 seconds
(which can be changed below). 

Using IFTTT (ifttt.com) the students build a recipe, or use an existing one, to, for example, upload
their measurement to a google spreadsheet, or tweet about their measurement.

For course material see: https://github.com/RolfHut/noCodeIoT

*/

//the topic that will be published to the Spark Cloud. maximum of 60 characters.
//it is best practice to "nest" the topic, ie. "noCodeIoT/TEACHERNAME/MEASUREMENT"
//Note that these are public measurements: anyone listening to the Spark Cloud can
//view them.
const String topic = "noCodeIoT/TEACHERNAME/MEASUREMENT";

//this enables Serial communication. When TRUE the Spark will send the measurement value over
//the USB serial every "intervalSerial" seconds. Only change to FALSE when no further debugging
//is needed
#define DEBUG_SERIAL TRUE 

//Sensor is connected to this pin.
const int inPin = A0; 

//number of seconds between uploads to the Spark Cloud. This may never be lower than 60
//because IFTTT does not allow for more triggers than 1 per minute.
const int intervalOnline = 120; 

//number of seconds between uploads over the USB serial
const int intervalSerial = 1; 

//number of milliseconds between measurements. This could be as low as needed, but higher values
//results in a more stable behaviour of the ADC. 
const int intervalMeasurement = 100;

int measurement;

void setup(){
    
    //start Serial communication
    if (DEBUG_SERIAL) Serial.begin(9600);

    //set the inPin to INPUT
    pinMode(inPin,INPUT);
}

void loop(){
    
    //get timestamp
    int t=millis();

    //every intervalMeasurement milliseconds, do a measurement 
    if (t%(intervalMeasurement)<1){
        measurement = analogRead(inPin);
        delay(1);
    }

    //every intervalSerial seconds, send measurement over Serial USB
    if (DEBUG_SERIAL){
        if (t%(intervalSerial*1000)<5){
            Serial.println(measurement);
            delay(5);
        }
    }
    
    //every intervalOnline seconds, publish measurement to Spark Cloud
    if (t%(intervalOnline*1000)<10){
        String measurementText = String(measurement, DEC);
        Spark.publish(topic,measurementText);
        delay(10);
    }

}
