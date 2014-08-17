#include <Si4703_Breakout.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <IRremote.h>

enum RemoteKeys
{
	OnOff = 0xFD00FF,
	VolUp = 0xFD807F,
	FuncOrStop = 0xFD40BF,
	Back = 0xFD20DF,
	PlayPause = 0xFDA05F,
	Forward = 0xFD609F,
	Down = 0xFD10EF,
	VolDown = 0xFD906F,
	Up = 0xFD50AF,
	Zero = 0xFD30CF,
	EQ = 0xFDB04F,
	SingleOrRepeate = 0xFD708F,
	One = 0xFD08F7,
	Two = 0xFD8877,
	Three = 0xFD48B7,
	Four = 0xFD28D7,
	Five = 0xFDA857,
	Six = 0xFD6897,
	Seven = 0xFD18E7,
	Eight = 0xFD9867,
	Nine = 0xFD58A7
};

int resetPin = 2;
int SDIO = A4;
int SCLK = A5;

Si4703_Breakout radio(resetPin, SDIO, SCLK);

int RECV_PIN = 10;
decode_results irInput;
int lcdLedPin = 6;
int lcdLedIntesity = 125;
boolean isOn = true;
int last_freq = 0;
int volume = 10;
char rdsBuffer[10];
boolean readRDS = false;

IRrecv irrecv(RECV_PIN);
LiquidCrystal lcd(12, 11, 5, 4, 3, 7);

void setup() {  
	radio.powerOn();
	radio.setVolume(volume);
  
	lcd.begin(16, 2);
	irrecv.enableIRIn();

	analogWrite(lcdLedPin, lcdLedIntesity);  

	pinMode(13, OUTPUT);
	digitalWrite(13, 0); 

	last_freq = 880;
	radio.setChannel(last_freq);
	DisplayStation(last_freq);
}

void loop() {
	unsigned char buf[5];
	unsigned long current_millis = millis();
  
	if (irrecv.decode(&irInput)) 
	{
	        irrecv.resume();

		switch (irInput.value)
		{
		case Zero:
			last_freq = 900;
			radio.setChannel(last_freq);
			DisplayStation(last_freq);
			break;
		case One:
			last_freq = 1048;
			radio.setChannel(last_freq);
			DisplayStation(last_freq);
			break;
		case Two:
			last_freq = 950;
			radio.setChannel(last_freq);
			DisplayStation(last_freq);
			break;
		case Three:
			last_freq = 897;
			radio.setChannel(last_freq);
			DisplayStation(last_freq);
			break;
		case Four:
			last_freq = 1040;
			radio.setChannel(last_freq);
			DisplayStation(last_freq);
			break;
		case Five:
			last_freq = 1020;
			radio.setChannel(last_freq);
			DisplayStation(last_freq);
			break;
		case Six:
			last_freq = 1030;
			radio.setChannel(last_freq);
			DisplayStation(last_freq);
			break;
		case Seven:
			last_freq = 918;
			radio.setChannel(last_freq);
			DisplayStation(last_freq);
			break;
		case Eight:
			last_freq = 880;
			radio.setChannel(last_freq);
			DisplayStation(last_freq);
			break;
		case Nine:
			last_freq = 990;
			radio.setChannel(last_freq);
			DisplayStation(last_freq);
			break;
		case Forward:
			last_freq = radio.seekUp();
			DisplayStation(last_freq);
			break;
		case Back:
			last_freq = radio.seekDown();
			DisplayStation(last_freq);
			break;
		case Up:
            if (!isOn) 
                break;
                    
            lcdLedIntesity +=25;

            if (lcdLedIntesity > 255) lcdLedIntesity = 255;

			analogWrite(lcdLedPin, lcdLedIntesity);
			break;
		case Down:
            if (!isOn) 
                break;
                    
            lcdLedIntesity -=25;

            if (lcdLedIntesity < 0) lcdLedIntesity = 0;

			analogWrite(lcdLedPin, lcdLedIntesity);
			break;
		case OnOff:
            if (isOn)
            {
            	radio.setVolume(0);
                lcd.clear();
                analogWrite(lcdLedPin, 0);           
                isOn = false;
            }
            else
            {
            	radio.setVolume(volume);
                DisplayStation(last_freq);
                analogWrite(lcdLedPin, lcdLedIntesity);
                isOn = true;
            }

			break;
		case VolUp:
			volume ++;
		    if (volume == 16) volume = 15;
		    radio.setVolume(volume);
			break;
		case VolDown:
			volume --;
      		if (volume < 0) volume = 0;
      		radio.setVolume(volume);
			break;
		case EQ:
			lcd.clear();
    		lcd.setCursor(0, 0);
			lcd.print("RDS read...");
			readRDS = true;
		default:
			break;
		}
	}

	if (readRDS)
	{
		radio.readRDS(rdsBuffer, 500);

		if (rdsBuffer[0] != '\0')
		{
			readRDS = false;

        	lcd.clear();
    		lcd.setCursor(0, 0);
			lcd.print("RDS heard:");
        	lcd.setCursor(0, 1);
      		lcd.print(rdsBuffer);
      	}
	}
}

void DisplayStation(int station)
{
        lcd.clear();
        lcd.setCursor(0, 0);
        double displayStation = (double)station / 10;
        lcd.print(displayStation);
        lcd.print("FM");
        
        lcd.setCursor(0, 1);
        
        if (station == 880) lcd.print("Radio 88FM");
        else if (station == 1048) lcd.print("Reshet Alef");
        else if (station == 95 ) lcd.print("Reshet Bet");
        else if (station == 897) lcd.print("Reshet Gimel");
        else if (station == 972) lcd.print("Kol Hamusica");
        else if (station == 1020) lcd.print("Radio Tel Aviv");
        else if (station == 1030) lcd.print("Radio Lelo Hafsaqa");
        else if (station == 918) lcd.print("Galgalatz");
        else if (station == 990) lcd.print("ECO 99FM");
        else if (station == 1040) lcd.print("Glz");
        else if (station == 1012) lcd.print("REQA");
}
