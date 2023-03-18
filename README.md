# PULSE-OXIMETER-MAX30105-LM35-1.3OLED-CON-




THIS IS A IOT BASED PULSE OXIMETER , in this project we will use LM35 SENSOR to measure temperature / max30105 to measure BPM and HR and we will show the output on 1.3OLED display to show the output ,, and if you have internet then you can show the output  on (http://consentiuminc.online).but currently SPO2 part was not working but we will work on it .


---------------+----------------
CUNNECTION 

-     MAX30105      OLED                LM35            BUZAAR         ESP8266


-     SCL -         SCL-                -                 -            D1(SCL)


-     SDA -         SDA-                -                 -             D2(SDA)


-     -             -                   DATA pin          -            A0


-     -              -                  -                 DATA          D7 {GPIO(13)}


-     VCC (3.5V)      5V                5V                5V            5V
