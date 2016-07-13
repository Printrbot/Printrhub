/*
 * Little Helper -- your best friend in your lab
 * http://www.appfruits.com/littlehelper
 *
 * This software is part of the firmware running Little Helper. You may use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies of the Software
 * but you must include this copyright notice and this permission in all copies or
 * substantial portions of the Software.
 *
 * Appfruits invests time and resources to make this open source. Please support us
 * and the open-source hardware initiative.
 *
 * Copyright 2014-2015 Phillip Schuster (@appfruits)
 * http://www.appfruits.com
 *
 * MIT-License
 */

#include "Application.h"
#include "Mode.h"
#include <FS.h>
#include "../MK20FirmwareUpdate.h"
#include "../DownloadFileToSDCard.h"
#include <HttpClient.h>
#include "PubNub.h"

//WiFi Setup
const char WIFI_SSID[] = "Apple Network 123";
const char WIFI_PSK[] = "2287143359371763";

// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30*1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 1000;

PubNubLogger Logger;

void pubnub_callback(char* message)
{

}

void logToPubNub(const char* message)
{
	pubnub_publish("ESP8622",message,pubnub_callback);
}

void logValueToPubNub(const char* message, int value)
{
	pubnub_publish("ESP8622",message,pubnub_callback);
	//String msg = String(message) + ":" + String(value);
	//pubnub_publish("ESP8622",msg.c_str(),pubnub_callback);
}

void logValueToPubNub(const char* message, float value)
{
	pubnub_publish("ESP8622",message,pubnub_callback);
	//String msg = String(message) + ":" + String(value);
	//pubnub_publish("ESP8622",msg.c_str(),pubnub_callback);
}

void logValueToPubNub(const char* message, String value)
{
	pubnub_publish("ESP8622",message,pubnub_callback);
	//String msg = String(message) + ":" + value;
	//pubnub_publish("ESP8622",msg.c_str(),pubnub_callback);
}

void logValueToPubNub(const char* message, size_t value)
{
	pubnub_publish("ESP8622",message,pubnub_callback);
	//String msg = String(message) + ":" + String((int)value);
	//pubnub_publish("ESP8622",msg.c_str(),pubnub_callback);
}

ApplicationClass Application;

ApplicationClass::ApplicationClass()
{
	_firstModeLoop = true;
	_nextMode = NULL;
	_currentMode = NULL;
	_lastTime = 0;
	_deltaTime = 0;
	_buttonPressedTime = 0;
	_mk20 = new CommStack(&Serial,this);
}

ApplicationClass::~ApplicationClass()
{
	delete _mk20;
	_mk20 = NULL;
}

void ApplicationClass::connectWiFi()
{
	byte led_status = 0;

	// Set WiFi mode to station (client)
	WiFi.mode(WIFI_STA);

	// Initiate connection with SSID and PSK
	WiFi.begin(WIFI_SSID, WIFI_PSK);

	// Blink LED while we wait for WiFi connection
	while ( WiFi.status() != WL_CONNECTED ) {
		digitalWrite(LED_PIN, led_status);
		led_status ^= 0x01;
		delay(100);
	}

	// Turn LED on when we are connected
	digitalWrite(LED_PIN, HIGH);
}

void ApplicationClass::setup()
{
/*
	const int CTSPin = 13; // GPIO13 for CTS input
	const int RTSPin = 15;
	pinMode(CTSPin, FUNCTION_4); // make pin U0CTS
	pinMode(RTSPin, FUNCTION_4); // make pin U0RTS
	U0C0 |= UCTXHFE; //add this sentense to add a tx flow control via MTCK( CTS ) - See more at: http://www.esp8266.com/viewtopic.php?f=27&t=8560#sthash.Os3nf7qX.dpuf
	U0C1 |= UCRXHFE; //attach RTS*/

	//Turn off MK20
/*	pinMode(13, OUTPUT);
	digitalWrite(13,LOW);*/

	Serial.begin(230400);
	SPIFFS.begin();

	connectWiFi();

	pubnub_init("pub-c-920d363e-723c-4fe8-bf82-127dbf5e2456", "sub-c-b7ae78f0-48ff-11e6-8b3b-02ee2ddab7fe");
	pubnub_connect();
	pubnub_publish("ESP8622","Setup finished",pubnub_callback);

	LOG("HUHU");

/*	digitalWrite(13,HIGH);
	pinMode(13,INPUT_PULLUP);*/

	//wiFiManager.autoConnect("Printrbot");
}

void ApplicationClass::loop()
{
	//Check MK20 communication by updating the comm stack. This may result in a call to
	//the runTask member function which in turn most likely sets a new current mode controller
	_mk20->process();

	//Setup new controller and delete the old one if a new controller is pushed
	if (_nextMode != NULL)
	{
		if (_currentMode != NULL)
		{
			delete _currentMode;
		}

		_currentMode = _nextMode;
		_nextMode = NULL;
		_firstModeLoop = true;
	}

	//Run current controller
	if (_currentMode != NULL)
	{
		//LOG("Run Controller");
		Mode* mode = _currentMode;

		//Call onWillAppear event handler if this is the first time the loop function is called by the scene
		//The default implementation will clear the display!
		if (_firstModeLoop)
		{
			LOG("First loop");

			LOG_VALUE("Appearing mode", mode->getName());
			mode->onWillStart();
			LOG("Mode started");
		}

		//Calculate Delta Time
		unsigned long currentTime = millis();
		if (currentTime < _lastTime)
		{
			//millis overflowed, just set delta to 0
			_deltaTime = 0;
		}
		else
		{
			_deltaTime = (float)(currentTime - _lastTime)/1000.0f;
		}

		//Run the scenes loop function
		mode->loop();
		_lastTime = millis();

		_firstModeLoop = false;
	}

	//Special mode - initiate firmware update by button pressed
	if (digitalRead(0) == LOW)
	{
		if (_buttonPressedTime == 0)
		{
			_buttonPressedTime = millis();
		}
		else
		{
			if ((millis() - _buttonPressedTime) > 2000)
			{
				if (digitalRead(0) == LOW)
				{
					//Button pressed for two seconds
					_buttonPressedTime = 0;

					Mode* mode = new MK20FirmwareUpdate();
					Application.pushMode(mode);
					return;
				}
			}
		}
	}
	else if (_buttonPressedTime > 0)
	{
		_buttonPressedTime = 0;
	}
}

void ApplicationClass::pushMode(Mode *mode)
{
	LOG_VALUE("Pushing mode",mode->getName());

	_nextMode = mode;
}

float ApplicationClass::getDeltaTime()
{
	return _deltaTime;
}

bool ApplicationClass::runTask(CommHeader &header, const uint8_t *data, uint8_t *responseData, uint16_t *responseDataSize)
{
	LOG_VALUE("Running Task with ID",header.getCurrentTask());
	LOG_VALUE("Comm-Type",header.commType);
	if (header.getCurrentTask() == GetTimeAndDate)
	{
		if (header.commType == Request)
		{
			LOG("Date and Time written zu Stream");
			char datetime[] = "2016-07-13 14:50:00 CEST";
			memcpy(responseData,datetime,strlen(datetime));
			*responseDataSize = strlen(datetime);
		}
		else
		{
			LOG("Don't know what to do with a GetTimeAndDate Response");
		}
	}
	else if (header.getCurrentTask() == GetJobWithID)
	{
		char jobID[header.contentLength+1];
		memset(jobID,0,header.contentLength+1);
		memcpy(jobID,data,header.contentLength);

		//Response contains the same jobID
		memcpy(responseData,data,header.contentLength);
		*responseDataSize = header.contentLength;

		//Initiate mode for file download
		//DownloadFileToSDCard* mode = new DownloadFileToSDCard(jobID);
		//pushMode(mode);
	}
	else
	{
		*responseDataSize = 0;
		return true;
	}
/*	else if (header.getCurrentTask() == -1000)//GetJobWithID)	//Disabled
	{
		if (header.commType == Request)
		{
			//Read the job id
			String jobID = stream->readStringUntil('\n');

			//Write job ID back
			stream->println(jobID);

			int err = 0;
			WiFiClient client;
			HttpClient httpClient(client);
			if (httpClient.get("www.appfruits.com", 80, jobID.c_str()) == 0)
			{
				err = httpClient.responseStatusCode();
				if (err >= 0)
				{
					err = httpClient.skipResponseHeaders();
					if (err >= 0)
					{
						int32_t bodyLen = httpClient.contentLength();

						//Write the number of bytes to the stream
						stream->write((char*)&bodyLen,sizeof(int32_t));

						// Now we've got to the body, so we can print it out
						unsigned long timeoutStart = millis();
						char c;

						while ((httpClient.connected() || httpClient.available()) &&
						       ((millis() - timeoutStart) < kNetworkTimeout))
						{
							ESP.wdtFeed();

							if (httpClient.available())
							{
								c = httpClient.read();
								// Print out this character

								stream->write(c);

								bodyLen--;
								// We read something, reset the timeout counter
								timeoutStart = millis();
							}
							else
							{
								// We haven't got any data, so let's pause to allow some to
								// arrive
								delay(kNetworkDelay);
							}
						}

						//All data sent
						return true;
					}
				}
			}

			//If we are here something went wrong
		}
	}
	else if (header.getCurrentTask() == GetProjectItemWithID)
	{
		if (header.commType == Request)
		{
			ESP.wdtFeed();

			String jobID = stream->readStringUntil('\n');
			LOG_VALUE("GetProjectItemWithID received with JobID",jobID);

			//Write job ID back
			stream->println(jobID);

			int32_t numberOfBytes = 300000;
			int8_t byte = 12;
			int32_t currentByteIndex = 0;

			//Write the number of bytes to the stream
			stream->write((char*)&numberOfBytes,sizeof(int32_t));

			while (currentByteIndex < numberOfBytes)
			{
				ESP.wdtFeed();

				while(stream->write(byte) <= 0)
				{
					ESP.wdtFeed();
					delay(10);
				}
				currentByteIndex++;
				//byte++;
			}

			return true;
		}
	}*/

	return true;
}