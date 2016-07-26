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
const int kNetworkDelay = 10;

FileLogger Logger;

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

ApplicationClass::ApplicationClass():
		_server(80)
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

	Serial.begin(115200);
	SPIFFS.begin();

	connectWiFi();

	SPIFFS.remove("/log.txt");

	_server.begin();

	LOG("HUHU");

/*	digitalWrite(13,HIGH);
	pinMode(13,INPUT_PULLUP);*/

	//wiFiManager.autoConnect("Printrbot");
}

void ApplicationClass::updateServer()
{
	// Check if a client has connected
	WiFiClient client = _server.available();
	if (!client) {
		return;
	}

	// Read the first line of the request
	String req = client.readStringUntil('\r');
	Serial.println(req);
	client.flush();

	// Match the request
	int val = -1; // We'll use 'val' to keep track of both the
	// request type (read/set) and value if set.
	if (req.indexOf("/log/display") != -1)
		val = 0; // Will write LED low
	else if (req.indexOf("/log/clear") != -1)
		val = 1; // Will write LED high
	// Otherwise request will be invalid. We'll say as much in HTML

	client.flush();

	// Prepare the response. Start with the common header:
	String s = "HTTP/1.1 200 OK\r\n";
	s += "Content-Type: text/html\r\n\r\n";
	s += "<!DOCTYPE HTML>\r\n<html>\r\n";
	// If we're setting the LED, print out a message saying we did
	if (val >= 0)
	{
		if (val == 0)
		{
			client.print(s);

			File logFile = SPIFFS.open("/log.txt", "r");

			char buffer[1024];
			memset(buffer,0,1024);
			int bufferIndex = 0;
			for (int i=0;i<logFile.size();i=i+1)
			{
				uint8_t byte = logFile.read();
				if (byte == '\n')
				{
					client.print(buffer);
					memset(buffer,0,1024);
					bufferIndex = 0;

					client.print("<br/>\n");
				}
				else
				{
					buffer[bufferIndex] = byte;
					bufferIndex++;

					if (bufferIndex >= 1024)
					{
						client.print(buffer);
						memset(buffer,0,1024);
						bufferIndex = 0;
					}
				}
			}
			logFile.close();

			client.print("</html>\n");
		}
		else if (val == 1)
		{
			SPIFFS.remove("/log.txt");
			client.print("Log file removed");
		}
	}
	else
	{
		s += "Invalid Request.<br> Try /led/1, /led/0, or /read.";
		s += "</html>\n";

		client.print(s);
	}
}

void ApplicationClass::loop()
{
	//Check MK20 communication by updating the comm stack. This may result in a call to
	//the runTask member function which in turn most likely sets a new current mode controller
	_mk20->process();

	updateServer();

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

bool ApplicationClass::runTask(CommHeader &header, const uint8_t *data, size_t dataSize, uint8_t *responseData, uint16_t *responseDataSize, bool* sendResponse, bool* success)
{
	LOG_VALUE("Running Task with ID",header.getCurrentTask());
	LOG_VALUE("Comm-Type",header.commType);

	if (_currentMode->handlesTask(header.getCurrentTask()))
	{
		LOG_VALUE("Current mode handles Task with ID",header.getCurrentTask());
		return _currentMode->runTask(header,data,dataSize,responseData,responseDataSize,sendResponse,success);
	}

	if (header.getCurrentTask() == GetTimeAndDate)
	{
		if (header.commType == Request)
		{
			LOG("Date and time written to stream");
			char datetime[] = "2016-07-13 14:50:00 CEST";
			memcpy(responseData,datetime,strlen(datetime));
			*responseDataSize = strlen(datetime);
			*sendResponse = true;
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

		LOG_VALUE("Received GetJobWithID Request with file name",jobID);

		//Send the response later when we know how large the file is
		*sendResponse = false;

		//Initiate mode for file download
		DownloadFileToSDCard* mode = new DownloadFileToSDCard(jobID);
		pushMode(mode);
	}
	else
	{
		*responseDataSize = 0;
		*sendResponse = false;
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