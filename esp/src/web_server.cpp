#include "web_server.h"
#include "Hash.h"
#include <ESPAsyncTCP.h>
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

        //Request MK20 to show the WiFi settings screen
        Application.getMK20Stack()->showWiFiInfo();
	}

	config.data.blank = false;
	config.save();
	EventLogger::log("Done updating config, restarting ESP");

	delay(1000);
	ESP.restart();
}

bool WebServer::validateAuthentication(AsyncWebServerRequest *request)
{
    EventLogger::log("Authenticating with username: %s, password: %s",config.data.name,config.data.password);
    if (!config.data.locked) return true;
    if (!request->authenticate("printrbot",config.data.password)) {
        AsyncWebServerResponse* response = request->beginResponse(403, "text/json", "{'success':'false','error':'Authentication failed'}");
        EventLogger::log("Authentication failed");
        response->addHeader("Access-Control-Allow-Origin", "*");
        request->send(response);
        return false;
    }

    return true;
}

void WebServer::addOptionsRequest(String path)
{
    server.on(path.c_str(), HTTP_OPTIONS, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse* response = request->beginResponse(200, "text/json");
        response->addHeader("Allow", "GET,POST,OPTIONS");
        response->addHeader("Access-Control-Allow-Origin", "*");
        response->addHeader("Access-Control-Allow-Headers", "Authorization, Content-Type");
        response->addHeader("Access-Control-Allow-Credentials", "true");
        request->send(response);
    });
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

    webserver.addOptionsRequest("/update_esp");
	server.on("/update_esp", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncWebParameter* url = request->getParam("url");
		EventLogger::log(url->value().c_str());
		Mode* espFU = new ESPFirmwareUpdate(url->value().c_str());
		Application.pushMode(espFU);
		request->send(200, "text/plain", "\nupdate started, please wait...\n\n");
	});

    webserver.addOptionsRequest("/update_mk20");
    server.on("/update_mk20", HTTP_GET, [](AsyncWebServerRequest *request) {
        String mk20FirmwareFile("/mk20_100.bin");
        FirmwareUpdateInfo* updateInfo = Application.getFirmwareUpdateInfo();
        if (updateInfo != NULL) {
            DownloadFileToSPIFFs* downloadMK20Firmware = new DownloadFileToSPIFFs(updateInfo->mk20_url,mk20FirmwareFile);
            MK20FirmwareUpdate* mk20UpdateFirmware = new MK20FirmwareUpdate(mk20FirmwareFile);
            downloadMK20Firmware->setNextMode(mk20UpdateFirmware);

            Application.pushMode(downloadMK20Firmware);

            request->send(200, "text/plain", "\nupdate of MK20 started, please wait...\n\n");
        } else {
            request->send(200, "text/plain", "\nupdate of MK20 failed, please wait until firmware data have been loaded and try again...\n\n");
        }
    });

    webserver.addOptionsRequest("/format_spiffs");
	server.on("/format_spiffs", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncWebServerResponse *response = NULL;
		if (SPIFFS.format()) {
			response = request->beginResponse(200, "text/json", "{'success':'yes'}");
			EventLogger::log("Formatting SPIFFS successfull");
		} else {
			response = request->beginResponse(200, "text/json", "{'success':'no'}");
			EventLogger::log("Formatting SPIFFS failed");
		}

		response->addHeader("Access-Control-Allow-Origin", "*");
		request->send(response);
	});

    webserver.addOptionsRequest("/update_ui");
	server.on("/update_ui", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = NULL;

        FirmwareUpdateInfo* updateInfo = Application.getFirmwareUpdateInfo();
        if (updateInfo == NULL) {
            response = request->beginResponse(200, "text/json", "{'success':'false','error':'Firmware update info not loaded yet'}");
        } else {
            response = request->beginResponse(200, "text/json", "{'success':'true'}");

            String uiFilePath("/ui.min");
            DownloadFileToSPIFFs* downloadUI = new DownloadFileToSPIFFs(updateInfo->mk20_ui_url,uiFilePath);
            PushFileToSDCard* pushUIFile = new PushFileToSDCard(uiFilePath,uiFilePath,false,Compression::RLE16);
            downloadUI->setNextMode(pushUIFile);
            Application.pushMode(downloadUI);
        }

        if (response != NULL) {
            EventLogger::log(config.data.name);
            response->addHeader("Access-Control-Allow-Origin", "*");
            request->send(response);
        }
    });

    webserver.addOptionsRequest("/status");
    server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request) {
        //Validate request
        if (!webserver.validateAuthentication(request)) {
            return;
        }

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

    webserver.addOptionsRequest("/info");
	server.on("/info", HTTP_GET, [](AsyncWebServerRequest *request) {
	//	String info = brain.getInfo();
		AsyncJsonResponse * response = new AsyncJsonResponse();
		response->addHeader("Access-Control-Allow-Origin", "*");

		JsonObject& root = response->getRoot();

		root["name"] = config.data.name;
		root["locked"] = config.data.locked;
		root["firmware"] = FIRMWARE_BUILDNR;
		root["mk20"] = Application.isMK20Available();

		response->setLength();
		request->send(response);
	});

    webserver.addOptionsRequest("/test");
	server.on("/test", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncWebServerResponse *response = request->beginResponse(200, "text/json", "{'sup':'yo'}");
		EventLogger::log(config.data.name);
		response->addHeader("Access-Control-Allow-Origin", "*");
		request->send(response);
	});

    webserver.addOptionsRequest("/updatefirmware");
	server.on("/updatefirmware", HTTP_GET, [](AsyncWebServerRequest *request) {
        //Validate request
        if (!webserver.validateAuthentication(request)) {
            return;
        }

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

    webserver.addOptionsRequest("/scanwifi");
	server.on("/scanwifi", HTTP_GET, [](AsyncWebServerRequest *request) {
        //Validate request
        if (!webserver.validateAuthentication(request)) {
            return;
        }

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

    webserver.addOptionsRequest("/networks");
	server.on("/networks", HTTP_GET, [](AsyncWebServerRequest *request) {
        //Validate request
        if (!webserver.validateAuthentication(request)) {
            return;
        }

		AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/net.json");
		response->addHeader("Access-Control-Allow-Origin", "*");
		request->send(response);
	});

    webserver.addOptionsRequest("/fetch");
	server.on("/fetch", HTTP_GET, [](AsyncWebServerRequest *request) {
        //Validate request
        if (!webserver.validateAuthentication(request)) {
            return;
        }

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

    webserver.addOptionsRequest("/updateconfig");
	server.on("/updateconfig", HTTP_POST, [](AsyncWebServerRequest *request) {
        //Validate request
        if (!webserver.validateAuthentication(request)) {
            return;
        }

		if(request->hasParam("name", true) && request->hasParam("ssid", true)) {
			request->send(200, "text/plain", "config updated...");
			//delay(100);
			doUpdateConfig(request);
		} else {
			request->send(500, "text/plain", "Error: required params missing...");
		}
	});

    webserver.addOptionsRequest("/wifi");
	server.on("/wifi", HTTP_POST, [](AsyncWebServerRequest *request) {
        //Validate request
        if (!webserver.validateAuthentication(request)) {
            return;
        }
        
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
