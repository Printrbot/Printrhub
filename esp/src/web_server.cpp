#include "web_server.h"
#include "Hash.h"
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "config.h"
#include "event_logger.h"
#include "controllers/MK20FirmwareUpdate.h"
#include "controllers/ESPFirmwareUpdate.h"
#include "controllers/ManageWifi.h"
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

void handleRoot(AsyncWebServerRequest *request) {
	char temp[] = "<html>\
	  <head>\
	    <title>printrbot</title>\
			<style>body {font-family:Helvetica, Arial;} input {margin:2px; padding: 2px 4px}</style>\
	  </head>\
	  <body>\
			<form method=\"post\" action=\"/updateconfig\">\
				<div><input name=\"name\" type=\"text\" placeholder=\"name\" value=\"printrbot\"/></div>\
		    <div><input name=\"ssid\" type=\"text\" placeholder=\"wifi ssid\" value=\"\"/></div>\
				<div><input name=\"wifipassword\" type=\"password\" placeholder=\"wifi password\" value=\"\"/></div>\
				<div><input name=\"server\" type=\"text\" placeholder=\"cloud server\" value=\"localhost\"/></div>\
				<div><input name=\"port\" type=\"text\" placeholder=\"cloud port\" value=\"8080\"/></div>\
				<div><label><input type=\"checkbox\" name=\"accesspoint\" value=\"1\"/> Access Point </label></div>\
				<div><label><input type=\"checkbox\" name=\"locked\" value=\"1\"/> Password Protect </label></div>\
				<div><input name=\"password\" type=\"password\" placeholder=\"printer password\" value=\"\"/></div>\
				<div><input type=\"submit\" value=\"Save\"/></div>\
			</form>\
	  </body>\
	</html>";
	//server.send ( 200, "text/html", temp );
	request->send(200, "text/html", temp);
}

void doUpdateConfig(AsyncWebServerRequest *request) {
	AsyncWebParameter* n = request->getParam("name", true);
	strcpy(config.data.name, n->value().c_str());
//	EventLogger::log(Config::data.name);

	AsyncWebParameter* ssid = request->getParam("ssid", true);
	strcpy(config.data.wifiSsid, ssid->value().c_str());
//	EventLogger::log(Config::data.wifiSsid);

	AsyncWebParameter* wp = request->getParam("wifipassword", true);
	strcpy(config.data.wifiPassword, wp->value().c_str());
	//EventLogger::log(Config::data.wifiPassword);

	config.data.accessPoint = request->hasParam("accesspoint", true);
	config.data.locked = request->hasParam("locked", true);

	if (request->hasParam("locked", true)) {
		AsyncWebParameter* pp = request->getParam("wifipassword", true);
		strcpy(config.data.password, pp->value().c_str());
//			EventLogger::log(Config::data.password);
	}
	config.data.blank = false;
//	EventLogger::log("CONFIG UPDATED!");

	// do config save last since it restarts the esp and we want
	// some feedback to user

	config.save();
	EventLogger::log("Done updating config, restarting ESP");

	delay(1000);
	ESP.restart();
}

void doUpdateName(AsyncWebServerRequest *request) {
	AsyncWebParameter* n = request->getParam("name", true);
	strcpy(config.data.name, n->value().c_str());
//	EventLogger::log(Config::data.name);

	config.save();
	EventLogger::log("Done updating config, restarting ESP");

	delay(1000);
	ESP.restart();
}

void WebServer::begin() {

	server.addHandler(&events);
	server.begin();

	events.onConnect([](AsyncEventSourceClient *client) {
		char fv[32];
		sprintf(fv, "Firmware: %s", FIRMWARE_VERSION);
    client->send(fv, "YO!");
  });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
		handleRoot(request);
	});

	server.on("/updatefirmware", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncWebParameter* url = request->getParam("url");
		EventLogger::log(url->value().c_str());
		Mode* espFU = new ESPFirmwareUpdate(url->value().c_str());
		Application.pushMode(espFU);
		request->send(200, "text/plain", "update started, please wait...");
	});

	server.on("/updatemk20", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncWebParameter* url = request->getParam("url");
		Mode* mk20FU = new MK20FirmwareUpdate(url->value().c_str());
		Application.pushMode(mk20FU);
		request->send(200, "text/plain", "MK20 firmware update started, please wait...");
	});

	server.on("/info", HTTP_GET, [](AsyncWebServerRequest *request) {
	//	String info = brain.getInfo();
		AsyncJsonResponse * response = new AsyncJsonResponse();
		response->addHeader("Access-Control-Allow-Origin", "*");

		JsonObject& root = response->getRoot();

		root["name"] = config.data.name;
		root["ap"] = config.data.accessPoint;
		root["locked"] = config.data.locked;
		root["ssid"] = config.data.wifiSsid;
		root["firmware"] = FIRMWARE_VERSION;

		response->setLength();
		request->send(response);
	});

	server.on("/test", HTTP_GET, [](AsyncWebServerRequest *request) {
		AsyncWebServerResponse *response = request->beginResponse(200, "text/json", "{'sup':'yo'}");
		EventLogger::log(config.data.name);
		response->addHeader("Access-Control-Allow-Origin", "*");
		request->send(response);
	});

	server.on("/scanwifi", HTTP_GET, [](AsyncWebServerRequest *request) {
		ManageWifi* mode = new ManageWifi();
		mode->setTask(ScanWifi);
		Application.pushMode(mode);
		request->send(200, "text/plain", "Scan started, please wait...");
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

			// extract host and path to file from url
			// check type, and set the appropriate task id
			// hardcoded to SaveProjectWithID for now
			Mode* df = new DownloadFile(SaveProjectWithID, id->value().c_str(), url->value().c_str());
			Application.pushMode(df);

			root["url"] = url->value();
			root["id"] = id->value();
			root["type"] = ftype->value();

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
}
