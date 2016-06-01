
//Code for the temp read function
char tempC = 0;
char buffer[8] = "       ";

//Settings
int target_ON = 0;
int tempTarget = 22;
String Heater_State = "Controller just turned on";
int relay = D7;


void setup() 
{
    //Code for the temp read function
    Particle.subscribe("tempDHT:", myHandler, MY_DEVICES);
    Serial.begin(9600);
    //Let's control the onboard RBG LED to let us see what is going on.
    RGB.control(true);
    RGB.color(0, 255, 0);
    //Present our options to the Cloud services for us to call via IFTTT
    Particle.function("on", heaterOn);
    Particle.function("SeekTemp", heaterTargetSeekOn);
    Particle.function("off", heaterOff);
    //Present a variable which we can view over the cloud to check the state of the controller
    Particle.variable("HeaterStatus", Heater_State);
    //The relay is in a default mode of 'the circuit being complete 'NC' normally connected.
    //The circuit is set to if the relay has no input signal - the circuit will be active.
    //Therefore - when the device is powered, ensure the relay is set to 'HIGH' to close the circuit.
    //What if the power goes off? There is still a manual controller inline that will prevent the heater from going on forever, it will stop at a temp of 20.5c.
    pinMode(relay, OUTPUT);
    digitalWrite(relay, HIGH);
}

void loop() {
//Check to see if the Particle device has a connection to the Web, if not - then don't enter the loop to check the DHT sensor temp from the other particle device
if (Particle.connected())
{
    //Check if the temperature is less than zero, if so - chances are the DHT sensor returned an error. The temp will not reach zero deg inside.
    if (tempC < 0)
    {
        digitalWrite(relay, HIGH);
        Particle.publish("ERROR", "DHT has returned odd data", 60, PRIVATE);
    }
    else
    {
        //Code to check if the mode is set to 'heat until target temperature is reached'
        if (target_ON == 1)
            {
                if (tempC < tempTarget)
                {
                    //set the PIN out to ON
                    digitalWrite(relay, LOW);
                }
                else
                {
                     digitalWrite(relay, HIGH);
                }
            }
            else
            {
                //Leave at current state by doing nothing
            }
    }   
    
}


//Delay the loop for 1 minute - this way we protect the heater from turning on/off if the temperature is on the 'edge' of the limit.
delay(60000);

}


//Reads the temperature from the other device
void myHandler(const char *event, const char *data)
{
    if (data)
    {
        strcpy_P(buffer, data);
        tempC = atof(buffer);
            //do the temp checking now and turn on if temp < target
            if (tempC < tempTarget)
            {
                    Particle.publish("tempAbove22:", "No", 60, PRIVATE);
                  delay(2000);
            }
            else
            {
                    Particle.publish("tempAbove22:", "Yes", 60, PRIVATE);
                    delay(2000);
            }
    }
     delay(2000);
}




//HEATER SET TO ON - USE THE TEMP LIMIT SET ON THE WALL PLATE
int heaterOn(String command)
{
    digitalWrite(relay, LOW);
    //Flash the LED on so we can see that a message has been recieved 
    RGB.color(0, 0, 255);
    //Remember to add the delay for 2 seconds, otherwise the LED will just flash for a period of time too small for us to see
    delay(2000);
    //Set target temp setting to be off
    target_ON = 0;
    //Set Heater State
    Heater_State = "On-Wall-Plate-Temp-Mode";
    //We return something to signify the end of the function - doesn't really matter what it is
    return 0;
}


//HEATER SET TO ON - USE THE TEMP LIMIT SET IN THE CODE & DETECTED BY THE 2nd Photon
int heaterTargetSeekOn(String command)
{
    RGB.color(0, 0, 255);
    //Remember to add the delay for 2 seconds, otherwise the LED will just flash for a period of time too small for us to see
    delay(2000);
    //Set Heater State
    Heater_State = "On-Target-Seeking";
    //Set target temp setting to be off
    target_ON = 1;
    //We return something to signify the end of the function - doesn't really matter what it is
    return 0;
}


//HEATER SET TO OFF  
int heaterOff(String command)
{
    
    digitalWrite(relay, HIGH);
    //Flash the LED on so we can see that a message has been recieved - just because we can
    RGB.color(0, 0, 255);
    //Remember to add the delay for 2 seconds, otherwise the LED will just flash for a period of time too small for us to see
    delay(2000);
    //Now set the LED back to off
    RGB.color(0, 0, 0);
    //Set Heater State
    Heater_State = "All Off";
    //Set target temp setting to be off
    target_ON = 0;
    //We return something to signify the end of the function - doesn't really matter what it is
    return 0;
}
