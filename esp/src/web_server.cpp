#include "web_server.h"
#include "Hash.h"
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "config.h"
#include "event_logger.h"
#include "controllers/MK20FirmwareUpdate.h"
#include "controllers/ESPFirmwareUpdate.h"
#include "controllers/ManageWifi.h"
#include "controllers/DownloadFileToSPIFFs.h"
#include "controllers/PushFileToSDCard.h"
#include "core/Mode.h"
#include "controllers/DownloadFile.h"
#include "controllers/Idle.h"
#include "Application.h"
#include "AsyncJson.h"
#include <ArduinoJson.h>

AsyncWebServer server(80);
AsyncEventSource events("/events");

WebServer webserver;
extern Config config;

WebServer::WebServer() {

}

void doUpdateConfig(AsyncWebServerRequest *request) {

	if (request->hasParam("name", true)) {
		AsyncWebParameter* n = request->getParam("name", true);
		strcpy(config.data.name, n->value().c_str());
	}
	if (request->hasParam("ssid", true)) {
		AsyncWebParameter* ssid = request->getParam("ssid", true);
		strcpy(config.data.wifiSsid, ssid->value().c_str());
		AsyncWebParameter* wp = request->getParam("wifipassword", true);
		strcpy(config.data.wifiPassword, wp->value().c_str());
	}

	if (request->hasParam("locked", true) && request->hasParam("password", true)) {
		AsyncWebParameter* pp = request->getParam("password", true);
		strcpy(config.data.password, pp->value().c_str());
	}

	config.data.blank = false;
	config.save();
	EventLogger::log("Done updating config, restarting ESP");

	delay(1000);
	ESP.restart();
}

void WebServer::begin() {

	server.addHandler(&events);

	events.onConnect([](AsyncEventSourceClient *client) {
    client->send("Printrbot Event Monitor");
  });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
		char html[] = "<html>\
		  <head>\
		    <title>printrbot</title>\
				<style>body {font-family:Helvetica, Arial;} input {margin: 2px; padding: 4px 9px;border: 1px solid #ccc;font-size: smaller;background-color: #f7f7f7;color: black;}</style>\
		  </head>\
		  <body>\
				<form method=\"post\" action=\"/wifi\">\
					<div><input name=\"name\" type=\"text\" placeholder=\"name\" value=\"printrbot\"/></div>\
			    <div><input name=\"ssid\" type=\"text\" placeholder=\"wifi ssid\" value=\"\"/></div>\
					<div><input name=\"wifipassword\" type=\"password\" placeholder=\"wifi password\" value=\"\"/></div>\
					<div><input type=\"submit\" value=\"Save\"/></div>\
				</form>\
		  </body>\
		</html>";
		request->send(200, "text/html", html);
	});

	server.on("/update_esp", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncWebParameter* url = request->getParam("url");
		EventLogger::log(url->value().c_str());
		Mode* espFU = new ESPFirmwareUpdate(url->value().c_str());
		Application.pushMode(espFU);
		request->send(200, "text/plain", "\nupdate started, please wait...\n\n");
	});

	server.on("/info", HTTP_GET, [](AsyncWebServerRequest *request) {
	//	String info = brain.getInfo();
		AsyncJsonResponse * response = new AsyncJsonResponse();
		response->addHeader("Access-Control-Allow-Origin", "*");

		JsonObject& root = response->getRoot();

		root["name"] = config.data.name;
		root["locked"] = config.data.locked;
		root["ssid"] = config.data.wifiSsid;
		root["firmware"] = FIRMWARE_BUILDNR;
		root["mk20"] = Application.isMK20Available();
		root["ipaddress"] = WiFi.localIP().toString();
		root["softapip"] = WiFi.softAPIP().toString();

		if (Application.getFirmwareUpdateInfo() != NULL) {
			root["fw_update"] = true;
			root["fw_info_sent"] = Application.firmwareUpdateNotified();
			root["fw_cloud_buildnr"] = Application.getFirmwareUpdateInfo()->buildnr;
		}
		else
		{
			root["fw_update"] = false;
		}

		response->setLength();
		request->send(response);
	});

	server.on("/test", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncWebServerResponse *response = request->beginResponse(200, "text/json", "{'sup':'yo'}");
		EventLogger::log(config.data.name);
		response->addHeader("Access-Control-Allow-Origin", "*");
		request->send(response);
	});

	server.on("/updatefirmware", HTTP_GET, [](AsyncWebServerRequest *request) {
        FirmwareUpdateInfo* updateInfo = Application.getFirmwareUpdateInfo();
        AsyncWebServerResponse *response = NULL;
        if (updateInfo == NULL) {
            response = request->beginResponse(200, "text/json", "{'success':'false','error':'Firmware update infos not been loaded yet'}");
        } else {
            if (updateInfo->buildnr <= FIRMWARE_BUILDNR) {
                response = request->beginResponse(200, "text/json", "{'success':'false','error':'Latest firmware already installed'}");
            } else {
                //Notify MK20 that firmware has started (shows a screen without user interaction)
                Application.getMK20Stack()->showFirmwareInProgressNotification();
                Application.startFirmwareUpdate();

                response = request->beginResponse(200, "text/json", "{'success':'true'}");
            }
        }

        if (response != NULL) {
            EventLogger::log(config.data.name);
            response->addHeader("Access-Control-Allow-Origin", "*");
            request->send(response);
        }
	});

	server.on("/scanwifi", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncJsonResponse * response = new AsyncJsonResponse();
		response->addHeader("Access-Control-Allow-Origin", "*");
		JsonObject& root = response->getRoot();
		root["scanwifi"] = "ok";

		ManageWifi* mode = new ManageWifi();
		mode->setTask(TaskID::ScanWifi);
		Application.pushMode(mode);

		response->setLength();
		request->send(response);
	});

	server.on("/networks", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/net.json");
		response->addHeader("Access-Control-Allow-Origin", "*");
		request->send(response);
	});

	server.on("/fetch", HTTP_GET, [](AsyncWebServerRequest *request) {
	//	String info = brain.getInfo();
		//AsyncWebServerResponse *response = request->beginResponse(200, "text/json", "{'name':'printrbot'}");
		AsyncJsonResponse * response = new AsyncJsonResponse();
		response->addHeader("Access-Control-Allow-Origin", "*");

		JsonObject& root = response->getRoot();

		if (request->hasParam("url") && request->hasParam("id")) {
			AsyncWebParameter* url = request->getParam("url");
			AsyncWebParameter* id = request->getParam("id");
			AsyncWebParameter* ftype = request->getParam("type");
            
			root["url"] = url->value();
			root["id"] = id->value();
			root["type"] = ftype->value();

            int length = root.measureLength()+1;
            char buffer[length];
            root.printTo(buffer,length);
            Application.getMK20Stack()->requestTask(TaskID::SaveProjectWithID,length,(uint8_t*)buffer);

			response->setLength();
			request->send(response);
		} else {
			request->send(500, "text/plain", "Invalid request");
		}
	});

	server.on("/updateconfig", HTTP_POST, [](AsyncWebServerRequest *request) {
		if(request->hasParam("name", true) && request->hasParam("ssid", true)) {
			request->send(200, "text/plain", "config updated...");
			//delay(100);
			doUpdateConfig(request);
		} else {
			request->send(500, "text/plain", "Error: required params missing...");
		}
	});

	server.on("/wifi", HTTP_POST, [](AsyncWebServerRequest *request) {
		AsyncJsonResponse * response = new AsyncJsonResponse();
		response->addHeader("Access-Control-Allow-Origin", "*");
		JsonObject& root = response->getRoot();

		if(request->hasParam("ssid", true)) {
			doUpdateConfig(request);
			root["wifi"] = "configuration updated, restarting...";
			response->setLength();
			request->send(response);
		} else {
			root["error"] = "required parameters missing";
			response->setLength();
			request->send(response);
		}
	});

	server.begin();
}
