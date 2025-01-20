/*SIM800L<  ------ >ARDUINO Board
Vcc < ------ > 5V
Gnd < ----- > Gnd
Rx < ------- > Pin no. 10 (SoftwareSerialTx)
Tx < ------- > Pin no. 9 (SoftwareSerialRx)
*/
#include <DS3231.h>
#include <EEPROM.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  //0x27 is the i2c address, while 16 = columns, and 2 = rows. 
SoftwareSerial mySerial(9,10);

DS3231 rtc(SDA, SCL);
Time t;

#define bt_clock  A0
#define bt_up     A1
#define bt_down   A2 
#define bt_timer  A3

#define Light 7
#define Fan   8 
#define TV    11
const int OnHour = 12;
const int OnMin = 24;
const int OffHour = 12;
const int OffMin = 25;

#define buzzer 13

int hh = 0, mm = 0, ss = 0, dd = 0, bb = 0, set_day;
int yy = 0;
String Day = "  ";

int StartHH  = 0, StartMM  = 0, FinishHH  = 0, FinishMM  = 0, setMode = 0, setAlarm = 0, alarmMode=1;
int Start1HH, Start1MM, Finish1HH, Finish1MM;
int Start2HH, Start2MM, Finish2HH, Finish2MM;
int Start3HH, Start3MM, Finish3HH, Finish3MM;
int Start4HH, Start4MM, Finish4HH, Finish4MM;

int timer1, timer2, timer3, timer4;
int stop =0, mode=0, flag=0;


char incomingByte; 
String incomingData;
bool atCommand = true;

int index = 0;
String number = "";
String message = "";

void setup() 
{
    lcd.init();                 //Init the LCD
    lcd.backlight();            //Activate backlight 
    rtc.begin();
    Serial.begin(9600);
    mySerial.begin(9600);
    pinMode(bt_clock, INPUT_PULLUP);
    pinMode(bt_up, INPUT_PULLUP);
    pinMode(bt_down, INPUT_PULLUP);
    pinMode(bt_timer, INPUT_PULLUP);
    pinMode(Light, OUTPUT);
    pinMode(TV, OUTPUT);
    pinMode(Fan, OUTPUT);
    pinMode(buzzer, OUTPUT); 
    lcd.setCursor(0,0);
    lcd.print("GSM Based");
    lcd.setCursor(0,1);
    lcd.print("Home Automation");
    delay(2000);
    lcd.clear();
    // Check if you're currently connected to SIM800L 
    while(!mySerial.available()){
      mySerial.println("AT");
      delay(1000); 
      Serial.println("connecting....");
    }
    Serial.println("Connected..");
    mySerial.println("AT+CMGF=1");  //Set SMS Text Mode 
    delay(1000);  
    mySerial.println("AT+CNMI=1,2,0,0,0");  //procedure, how to receive messages from the network
    delay(1000);
    mySerial.println("AT+CMGL=\"REC UNREAD\""); // Read unread messages
    Serial.println("Ready to received Commands..");  
    lcd.print("System Ready!");  
    delay(1000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Fan  TV   Light");
    lcd.setCursor(0,1);
    lcd.print("OFF  OFF   OFF");
    delay(2000);
    lcd.clear();
    stop=EEPROM.read(50);  
    if(stop==0){  
    }else{
    mode=1;WriteEeprom ();delay (20);
    mode=2;WriteEeprom ();delay (20);
    mode=3;WriteEeprom ();delay (20);
    mode=4;WriteEeprom ();delay (20);
    mode=0;
  }
EEPROM.write(50,0); 

ReadEeprom();
}

void loop()
{   
  lcd.setCursor(0,0);
  lcd.print("Fan  TV   Light");
  if (mySerial.available()){
      delay(100);
      
      // Serial buffer
      while(mySerial.available()){
        incomingByte = mySerial.read();
        incomingData += incomingByte; 
       }
        
        delay(10); 
        if(atCommand == false){
          receivedMessage(incomingData);
        }else{
          atCommand = false;
        }
        
        //delete messages to save memory
        if (incomingData.indexOf("OK") == -1){
          mySerial.println("AT+CMGDA=\"DEL ALL\"");
          delay(1000);
          atCommand = true;
        }
        
        incomingData = "";
  }
  
  t = rtc.getTime();
  Day = rtc.getDOWStr(1);
  if (setMode == 0){
  hh = t.hour,DEC;
  mm = t.min,DEC;
  ss = t.sec,DEC;
  dd = t.date,DEC;
  bb = t.mon,DEC;
  yy = t.year,DEC;
  //if(t.hour>12){hh=t.hour-12;}// for removing 24 hours
  //else{hh=t.hour;}
  }  
  if(setAlarm==0){
  lcd.setCursor(0,2); 
  lcd.print((hh/10)%10);
  lcd.print(hh % 10); 
  lcd.print(":");
  lcd.print((mm/10)%10);
  lcd.print(mm % 10);
  lcd.print(":");
  lcd.print((ss/10)%10);
  lcd.print(ss % 10); 
  lcd.print("  T:"); 
  lcd.print(rtc.getTemp(),0);
  lcd.write(223); 
  lcd.print("C");
  lcd.print("  "); 

  lcd.setCursor(1,3);
  lcd.print(Day);
  lcd.print(" ");
  lcd.print((dd/10)%10);
  lcd.print(dd % 10); 
  lcd.print("/");
  lcd.print((bb/10)%10);
  lcd.print(bb % 10);
  lcd.print("/"); 
  lcd.print((yy/1000)%10);
  lcd.print((yy/100)%10);
  lcd.print((yy/10)%10);
  lcd.print(yy % 10);
  }
  setupClock();
  setTimer();
  delay (100);
  blinking(); 

  //Timer1 ON
  if (timer1==1 && alarmMode==1 && hh==Start1HH && mm==Start1MM)  {
    digitalWrite(Light, HIGH);
    lcd.setCursor(11,1);
    lcd.print("ON");
    }
  //Timer1 OFF
  if (timer1==1 && alarmMode==1 && hh==Finish1HH && mm==Finish1MM){
    digitalWrite(Light, LOW);
    lcd.setCursor(11,1);
    lcd.print("OFF");
    lcd.clear();
    }

  //Timer2 ON
  if (timer2==1 && alarmMode==1 && hh==Start2HH && mm==Start2MM)  {
    digitalWrite(Fan, HIGH);
    lcd.setCursor(0,1);
    lcd.print("ON");
    
    }
  //Timer2 OFF
  if (timer2==1 && alarmMode==1 && hh==Finish2HH && mm==Finish2MM){
    digitalWrite(Fan, LOW);
    lcd.setCursor(0,1);
    lcd.print("OFF");
    lcd.clear();
    }

  //Timer3 ON
  if (timer3==1 && alarmMode==1 && hh==Start3HH && mm==Start3MM)  {
    digitalWrite(TV, HIGH);
    lcd.setCursor(6,1);
    lcd.print("ON");
    
    }
  //Timer3 OFF
  if (timer3==1 && alarmMode==1 && hh==Finish3HH && mm==Finish3MM){
    digitalWrite(TV, LOW);
    lcd.setCursor(6,1);
    lcd.print("OFF");
    lcd.clear();
    }
    if (timer4==1 && alarmMode==1 && hh==Start4HH && mm==Start4MM)  {
      digitalWrite(Light, HIGH);
      digitalWrite(Fan, HIGH);
      digitalWrite(TV, HIGH);
      lcd.setCursor(0,1);
      lcd.print("ON    ON     ON");
      }
  //Timer4 OFF
    if (timer4==1 && alarmMode==1 && hh==Finish4HH && mm==Finish4MM){
      digitalWrite(Light, HIGH);
      digitalWrite(Fan, HIGH);
      digitalWrite(TV, HIGH);
      lcd.setCursor(0,1);
      lcd.print("OFF    OFF     OFF");
      lcd.clear();
      }

  delay (100);
  digitalWrite(buzzer, LOW);
}

void receivedMessage(String inputString){
  
  //Get the number of the sender
  index = inputString.indexOf('"')+1;
  inputString = inputString.substring(index);
  index = inputString.indexOf('"');
  number = inputString.substring(0,index);
  Serial.println("Number: " + number);

  //Get the message of the sender
  index = inputString.indexOf("\n")+1;
  message = inputString.substring(index);
  message.trim();
  Serial.println("Message: " + message);
        
  message.toUpperCase(); //Uppercase the message received

  //Turn LED ON or OFF
  if (message.indexOf("ON1") > -1){
      digitalWrite(Light, HIGH);
      lcd.setCursor(13,1);
      lcd.print("ON     ");
      delay(200);
      Serial.println("Command: Light Turned On.");
      mySerial.println("AT+CMGF=1");
      delay(1000);
      mySerial.println("AT+CMGS=\"+639761656784\"\r");
      delay(1000);
      mySerial.println("The Light is On!!");
      delay(100);
      mySerial.println((char)26);
      delay(1000);
   }
          
  if (message.indexOf("OFF1") > -1){
        digitalWrite(Light, LOW);
        lcd.setCursor(13,1);
        lcd.print("OFF     ");
        delay(200);
        Serial.println("Command: Light Turned Off.");
        mySerial.println("AT+CMGF=1");
        delay(1000);
        mySerial.println("AT+CMGS=\"+639761656784\"\r");
        delay(1000);
       mySerial.println("The Light is OFF!!");
        delay(100);
        mySerial.println((char)26);
        delay(1000);
   }
          
   delay(50);
   if (message.indexOf("ON2") > -1){
      digitalWrite(Fan, HIGH);
      lcd.setCursor(0,1);
      lcd.print("ON     ");
      delay(200);
      Serial.println("Command: Fan Turn On.");
      mySerial.println("AT+CMGF=1");
      delay(1000);
      mySerial.println("AT+CMGS=\"+639761656784\"\r");
      delay(1000);
      mySerial.println("The Fan is On!!");
      delay(100);
      mySerial.println((char)26);
      delay(1000);
   }
          
  if (message.indexOf("OFF2") > -1){
        digitalWrite(Fan, LOW);
        lcd.setCursor(0,1);
        lcd.print("OFF     ");
         delay(200);
        Serial.println("Command: Fan Turned Off.");
        mySerial.println("AT+CMGF=1");
      delay(1000);
      mySerial.println("AT+CMGS=\"+639761656784\"\r");
      delay(1000);
      mySerial.println("The Fan is Off!!");
      delay(100);
      mySerial.println((char)26);
      delay(1000);
   }
          
   delay(50);
   if (message.indexOf("ON3") > -1){
      digitalWrite(TV, HIGH);
      lcd.setCursor(7,1);
      lcd.print("ON     ");
      delay(200);
      Serial.println("Command: TV Turned On.");
      mySerial.println("AT+CMGF=1");
      delay(1000);
      mySerial.println("AT+CMGS=\"+639761656784\"\r");
      delay(1000);
      mySerial.println("The TV is On!!");
      delay(100);
      mySerial.println((char)26);
      delay(1000);
   }
          
  if (message.indexOf("OFF3") > -1){
        digitalWrite(TV, LOW);
        lcd.setCursor(7,1);
         lcd.print("OFF     ");
        delay(200);
        Serial.println("Command: TV Turned Off.");
        mySerial.println("AT+CMGF=1");
      delay(1000);
      mySerial.println("AT+CMGS=\"+639761656784\"\r");
      delay(1000);
      mySerial.println("The TV is OFF!!");
      delay(100);
      mySerial.println((char)26);
      delay(1000);
   }
          
   delay(50);
   if (message.indexOf("ON4") > -1){
      digitalWrite(TV, HIGH);
      digitalWrite(Fan, HIGH);
      digitalWrite(Light, HIGH);
      lcd.setCursor(0,1);
      lcd.print("ON     ON    ON");
      delay(200);
      Serial.println("Command: Turned On All.");
      mySerial.println("AT+CMGF=1");
      delay(1000);
      mySerial.println("AT+CMGS=\"+639761656784\"\r");
      delay(1000);
      mySerial.println("All Appliances are On!!");
      delay(100);
      mySerial.println((char)26);
      delay(1000);
   }
          
  if (message.indexOf("OFF4") > -1){
        digitalWrite(TV, LOW);
        digitalWrite(Fan, LOW);
        digitalWrite(Light, LOW);
        lcd.setCursor(0,1);
        lcd.print("OFF     OFF    OFF");
        delay(200);
        Serial.println("Command: Turned Off All.");
         mySerial.println("AT+CMGF=1");
        delay(1000);
        mySerial.println("AT+CMGS=\"+639761656784\"\r");
        delay(1000);
        mySerial.println("All Appliances are OFF!!");
        delay(100);
        mySerial.println((char)26);
        delay(1000);
   }
          
   delay(50);
   
  }

void blinking (){     //BLINKING SCREEN

//Set Clock
if (setAlarm <2 && setMode == 1){lcd.setCursor(0,2);  lcd.print("  ");}  //0,1
if (setAlarm <2 && setMode == 2){lcd.setCursor(3,2);  lcd.print("  ");}   //3,0
if (setAlarm <2 && setMode == 3){lcd.setCursor(6,2);  lcd.print("  ");}   //6,0
if (setAlarm <2 && setMode == 4){lcd.setCursor(1,3);  lcd.print("   ");}    //1,1
if (setAlarm <2 && setMode == 5){lcd.setCursor(5,3);  lcd.print("  ");}   //5,1
if (setAlarm <2 && setMode == 6){lcd.setCursor(8,3);  lcd.print("  ");}   //8,1
if (setAlarm <2 && setMode == 7){lcd.setCursor(11,3); lcd.print("    "); }//11,1

//Set Timer

if (setMode == 0 && setAlarm == 1 && mode==0){lcd.setCursor(2,2); lcd.print("  "); }
if (setMode == 0 && setAlarm == 2 && mode==0){lcd.setCursor(6,2); lcd.print("  "); }
if (setMode == 0 && setAlarm == 3 && mode==0){lcd.setCursor(10,2); lcd.print("  "); }
if (setMode == 0 && setAlarm == 4 && mode==0){lcd.setCursor(13,2); lcd.print("  "); }

if (setMode == 0 && setAlarm == 1 && mode>0){lcd.setCursor(11,2); lcd.print("  "); }
if (setMode == 0 && setAlarm == 2 && mode>0){lcd.setCursor(14,2); lcd.print("  "); }
if (setMode == 0 && setAlarm == 3 && mode>0){lcd.setCursor(11,3); lcd.print("  "); }
if (setMode == 0 && setAlarm == 4 && mode>0){lcd.setCursor(14,3); lcd.print("  "); }
}

void setupClock (void) {
   
    if (setMode == 8){
    lcd.setCursor (0,2);
    lcd.print ("Set Time Finish ");
    lcd.setCursor (0,3);
    lcd.print ("Set Date Finish ");
    delay (1000);
    rtc.setTime (hh, mm, ss);
    rtc.setDate (dd, bb, yy);  
    lcd.clear();
    setMode = 0;
    }
    
 if (setAlarm >0){alarmMode=0;}
    
 if(digitalRead (bt_clock) == 0 && flag==0) {flag=1;
 if(setAlarm>0){WriteEeprom(); setAlarm=1; mode =5;}
 else{setMode = setMode+1;}
 digitalWrite(buzzer, HIGH);
 }
  
 if(digitalRead (bt_timer) == 0 && flag==0){flag=1;
 if(setMode>0){setMode=8;}
  else{
  setAlarm = setAlarm+1;
  if(setAlarm>4){setAlarm=1; 
  WriteEeprom ();
  mode=mode+1;
  ReadEeprom();
  }
  } 
  lcd.clear();
  digitalWrite(buzzer, HIGH);
  } 

    if(setAlarm == 1 && mode==5){
    lcd.setCursor (0,2);
    lcd.print ("Set Timer Finish");
    lcd.setCursor (0,3);
    lcd.print ("-EEPROM Updated-");
    delay (2000); 
    lcd.clear();
    setAlarm=0;
    mode =0;
    alarmMode=1;
    }

if(digitalRead (bt_clock) == 1 && digitalRead (bt_timer) == 1){flag=0;}
  
 if(digitalRead (bt_up) == 0){                          
            if (setAlarm<2 && setMode==1)hh=hh+1; 
            if (setAlarm<2 && setMode==2)mm=mm+1;
            if (setAlarm<2 && setMode==3)ss=ss+1;
            if (setAlarm<2 && setMode==4)set_day=set_day+1;
            if (setAlarm<2 && setMode==5)dd=dd+1;
            if (setAlarm<2 && setMode==6)bb=bb+1;
            if (setAlarm<2 && setMode==7)yy=yy+1;
            //Timer
            if (mode==0 && setMode==0 && setAlarm==1)timer1=1;
            if (mode==0 && setMode==0 && setAlarm==2)timer2=1;
            if (mode==0 && setMode==0 && setAlarm==3)timer3=1;
            if (mode==0 && setMode==0 && setAlarm==4)timer4=1;
             
            if (mode>0 && setMode==0 && setAlarm==1)StartHH=StartHH+1;
            if (mode>0 && setMode==0 && setAlarm==2)StartMM=StartMM+1;
            if (mode>0 && setMode==0 && setAlarm==3)FinishHH=FinishHH+1;
            if (mode>0 && setMode==0 && setAlarm==4)FinishMM=FinishMM+1;

if(hh>23)hh=0;
if(mm>59)mm=0;
if(ss>59)ss=0;
if(set_day>7)set_day=0;
if(dd>31)dd=0;
if(bb>12)bb=0;
if(yy>2030)yy=2000;
if(StartHH>23)StartHH=0;
if(StartMM>59)StartMM=0;
if(FinishHH>23)FinishHH=0;
if(FinishMM>59)FinishMM=0;
rtc.setDOW(set_day);
digitalWrite(buzzer, HIGH);
 }        

if(digitalRead (bt_down) == 0){                                      
            if (setAlarm<2 && setMode==1)hh=hh-1; 
            if (setAlarm<2 && setMode==2)mm=mm-1;
            if (setAlarm<2 && setMode==3)ss=ss-1;
            if (setAlarm<2 && setMode==4)set_day=set_day-1;
            if (setAlarm<2 && setMode==5)dd=dd-1;
            if (setAlarm<2 && setMode==6)bb=bb-1;
            if (setAlarm<2 && setMode==7)yy=yy-1;
            //Timer
            if (mode==0 && setMode==0 && setAlarm==1)timer1=0;
            if (mode==0 && setMode==0 && setAlarm==2)timer2=0;
            if (mode==0 && setMode==0 && setAlarm==3)timer3=0;
            if (mode==0 && setMode==0 && setAlarm==4)timer4=0;
            
            if (mode>0 && setMode==0 && setAlarm==1)StartHH=StartHH-1;
            if (mode>0 && setMode==0 && setAlarm==2)StartMM=StartMM-1;
            if (mode>0 && setMode==0 && setAlarm==3)FinishHH=FinishHH-1;
            if (mode>0 && setMode==0 && setAlarm==4)FinishMM=FinishMM-1;

if(hh<0)hh=23;
if(mm<0)mm=59;
if(ss<0)ss=59;
if(set_day<0)set_day=7;
if(dd<0)dd=31;
if(bb<0)bb=12;
if(yy<0)yy=2030;
if(StartHH<0)StartHH=23;
if(StartMM<0)StartMM=59;
if(FinishHH<0)FinishHH=23;
if(FinishMM<0)FinishMM=59;
rtc.setDOW(set_day);
digitalWrite(buzzer, HIGH); 
 }
 
}
void setTimer (){
//Timer
if (setMode == 0 && setAlarm >0 && mode>0){
  lcd.setCursor (0,2);
  lcd.print("Load ");
  lcd.print(mode);
  lcd.print(" On :");
  lcd.setCursor (11,2);
  lcd.print((StartHH/10)%10);
  lcd.print(StartHH % 10);
  lcd.print(":");
  lcd.print((StartMM/10)%10);
  lcd.print(StartMM % 10);

  lcd.setCursor (0,3);
  lcd.print("Load ");
  lcd.print(mode);
  lcd.print(" Off:");
  lcd.setCursor (11,3);
  lcd.print((FinishHH/10)%10);
  lcd.print(FinishHH % 10);
  lcd.print(":");
  lcd.print((FinishMM/10)%10);
  lcd.print(FinishMM % 10);
 }

if (setMode == 0 && setAlarm >0 && mode==0){
  lcd.setCursor (0,2);
  lcd.print(" L1  L2  L3  L4 ");
  lcd.setCursor (0,3);
    if(timer1==1){lcd.print("  A");}
  else{lcd.print("  D");}
   
    if(timer2==1){lcd.print("   A");}
  else{lcd.print("   D");}
  
    if(timer3==1){lcd.print("   A");}
  else{lcd.print("   D");}

  if(timer4==1){lcd.print("   A");}
  else{lcd.print("   D");}

}
}


void ReadEeprom() {
Start1HH=EEPROM.read(11);Start1MM=EEPROM.read(12);Finish1HH=EEPROM.read(13);Finish1MM=EEPROM.read(14);
Start2HH=EEPROM.read(21);Start2MM=EEPROM.read(22);Finish2HH=EEPROM.read(23);Finish2MM=EEPROM.read(24);
Start3HH=EEPROM.read(31);Start3MM=EEPROM.read(32);Finish3HH=EEPROM.read(33);Finish3MM=EEPROM.read(34);
Start4HH=EEPROM.read(41);Start4MM=EEPROM.read(42);Finish4HH=EEPROM.read(43);Finish4MM=EEPROM.read(44);

if(mode==1){StartHH=Start1HH, StartMM=Start1MM, FinishHH=Finish1HH,FinishMM=Finish1MM;}
if(mode==2){StartHH=Start2HH, StartMM=Start2MM, FinishHH=Finish2HH,FinishMM=Finish2MM;}
if(mode==3){StartHH=Start3HH, StartMM=Start3MM, FinishHH=Finish3HH,FinishMM=Finish3MM;}
if(mode==4){StartHH=Start4HH, StartMM=Start4MM, FinishHH=Finish4HH,FinishMM=Finish4MM;}

timer1=EEPROM.read(1);
timer2=EEPROM.read(2);
timer3=EEPROM.read(3);
timer4=EEPROM.read(4);
}


void WriteEeprom() {
if(mode==1){EEPROM.write(11,StartHH);EEPROM.write(12,StartMM);EEPROM.write(13,FinishHH);EEPROM.write(14,FinishMM);}
if(mode==2){EEPROM.write(21,StartHH);EEPROM.write(22,StartMM);EEPROM.write(23,FinishHH);EEPROM.write(24,FinishMM);}
if(mode==3){EEPROM.write(31,StartHH);EEPROM.write(32,StartMM);EEPROM.write(33,FinishHH);EEPROM.write(34,FinishMM);}
if(mode==4){EEPROM.write(41,StartHH);EEPROM.write(42,StartMM);EEPROM.write(43,FinishHH);EEPROM.write(44,FinishMM);}

EEPROM.write(1,timer1);
EEPROM.write(2,timer2);
EEPROM.write(3,timer3);
EEPROM.write(4,timer4);
}
