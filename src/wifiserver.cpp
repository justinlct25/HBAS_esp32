#include "wifiserver.h"


const char* wifi_ap_ssid = "Handbrake-Alert-System";
const char* wifi_ap_password = "handbrake";
const char* PARAM_DISTANCE = "inputDistance";
const char* PARAM_ANGLE = "inputAngle";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
  <html>
  <head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <script>
    const configGateway = `ws://${window.location.hostname}/configSocket`;
    const angleGateway = `ws://${window.location.hostname}/angleSocket`;
    const distanceGateway = `ws://${window.location.hostname}/distanceSocket`;
    const loggerGateway = `ws://${window.location.hostname}/loggerSocket`;
    let configsocket;
    let anglesocket;
    let distancesocket;
    let loggerSocket;
    window.addEventListener('load', onLoad);
    function submitMessage() {
    }
    function initConfigWebSocket(){
      console.log('Trying to open a WebSocket connection...');
      configsocket = new WebSocket(configGateway);
      configsocket.onopen = onOpen;
      configsocket.onclose = onClose;
      configsocket.onmessage = onConfigMessage;
    }
    function initAngleWebSocket(){
      console.log('Trying to open a AngleSocket connection...');
      anglesocket = new WebSocket(angleGateway);
      anglesocket.onopen = onOpen;
      anglesocket.onclose = onClose;
      anglesocket.onmessage = onAngleMessage;
    }
    function initDistanceWebSocket(){
        console.log('Trying to open a DistanceSocket connection...');
        distancesocket = new WebSocket(distanceGateway);
        distancesocket.onopen = onOpen;
        distancesocket.onclose = onClose;
        distancesocket.onmessage = onDistanceMessage;
    }
    function initLoggerWebSocket(){
        console.log('Trying to open a loggerSocket connection...');
        loggerSocket = new WebSocket(loggerGateway);
        loggerSocket.onopen = onOpen;
        loggerSocket.onclose = onClose;
        loggerSocket.onmessage = onLoggerMessage;
    }
    function onOpen(event) {
      console.log('Connection opened');
    }
    function onClose(event) {
      console.log('Connection closed');
      setTimeout(initWebSocket, 2000);
    }
    function onConfigMessage(event) {
      let config = event.data.split(',');
      let distance = config[0].split(':')[1];
      let angle = config[1].split(':')[1];
      document.getElementById('currentConfig').innerHTML = `<div>Distance: ${distance} cm</div><div>Angle: ${angle} deg</div>`;
    }
    function onAngleMessage(event){
      console.log(event.data+" deg");
      document.getElementById('currentAngle').innerHTML = `<div>Measured Angle: ${event.data} deg</div>`
    }
    function onDistanceMessage(event){
      console.log(event.data+" cm");
      document.getElementById('currentDistance').innerHTML = `<div>Measured Distance: ${event.data} cm</div>`
    }
    function onLoggerMessage(event){
        console.log(event.data)
        if(event.data=="end"){
            document.getElementById("systemLogger").value += "\n";
        }else{
            document.getElementById("systemLogger").value += event.data;
        }
    }
    function onLoad(event) {
      initConfigWebSocket();
      initAngleWebSocket();
      initDistanceWebSocket();
      initLoggerWebSocket();
    }
  </script>
  <style>
  textarea{
      width:250px;
      height:250px;
  }
  </style>
  </head>
  <body>
  <h1>Truck Alert System Configuration</h1>
  %CURRENTSTATUS%
  <h3>Current Alert Threshold: </h3>
  <div id="currentConfig">
  %CURRENTCONFIGURATION%
  </div>
  <h3>Adjust Alert Threshold: </h3>
  <form action="/config" target="hidden-form">
    Distance: <input type="text" name="inputDistance">(in cm)<br>
    Angle : <input type="number " name="inputAngle">(in deg)<br>
    <input type="submit" value="Submit" onclick="submitMessage()">
  </form><br>
  <iframe style="display:none" name="hidden-form"></iframe>
  </body>
  <h3>Measurement of Distance & Angle: </h3>
  <div id="currentDistance"></div>
  <div id="currentAngle"></div> 
  <h3>System Status Log</h3>
  <textarea id="systemLogger" style="width:100%;height:50%;"></textarea>
</html>
)rawliteral";

AsyncWebServer server(80);
AsyncWebSocket configSocket("/configSocket");
AsyncWebSocket angleSocket("/angleSocket");
AsyncWebSocket distanceSocket("/distanceSocket");
AsyncWebSocket loggerSocket("/loggerSocket");

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len){
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      // handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}



    

String processor(const String& var){
    //Serial.println(var);
    if(var == "CURRENTCONFIGURATION"){
        String configs = "";
        configs += "<div>Distance: ";
        configs += lim_distance;
        configs += " cm</div><div> Angle: ";
        configs += lim_angle;
        configs += " deg</div>";
        return configs;
    }
    if(var == "CURRENTSTATUS"){
        String status = "";
        status += "<div>Version: ";
        status += fw_version;
        status += "   Battery: ";
        status += String(bat);
        status += "</div>";
        return status;
    }
  return String();
}

void wifiServer_run(){
    if(!MDNS.begin("handbrake")){
    Serial.println("Error starting mDNS");
    return;
  }
    configSocket.onEvent(onEvent);
    server.addHandler(&configSocket);
    angleSocket.onEvent(onEvent);
    server.addHandler(&angleSocket);
    distanceSocket.onEvent(onEvent);
    server.addHandler(&distanceSocket);
    loggerSocket.onEvent(onEvent);
    server.addHandler(&loggerSocket);
    WiFi.softAP(wifi_ap_ssid, wifi_ap_password);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("IP Address: ");
    Serial.println(IP);
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/html", index_html, processor);
      // request->send_P(200, "text/html", index_html);
    });
    server.on("/current", HTTP_GET, [](AsyncWebServerRequest *request){
      Serial.print("current");
      char tempDistance=(char)lim_distance;
      Serial.println(tempDistance);
      request->send_P(200, "text/plain", &tempDistance);
    });
    // Send a GET request to <ESP_IP>/config?inputString=<inputMessage>
    server.on("/config", HTTP_GET, [] (AsyncWebServerRequest *request) {
      int tempDistance;
      int tempAngle;
      bool validDistance = false;
      bool validAngle = false;
      Serial.println("config");
      // GET inputString value on <ESP_IP>/config?inputString=<inputMessage>
      if ((request->hasParam(PARAM_DISTANCE))&&(request->hasParam(PARAM_ANGLE))) {
        tempDistance = atof((request->getParam(PARAM_DISTANCE)->value()).c_str());
        tempAngle = atof((request->getParam(PARAM_ANGLE)->value()).c_str());
        if(tempDistance>0){validDistance=true;}
        if(tempAngle>0&&tempAngle<=360){validAngle=true;}
        if(validDistance && validAngle){
          lim_distance = tempDistance;
          lim_angle = tempAngle;
          NVS.setInt("lim_distance",tempDistance);
          NVS.setInt("lim_angle",tempAngle);
          request->send_P(200, "text/html", index_html, processor);
          request->send(200, "text/text", "config successfully updated");
          SerialPrintLimitDistanceAngle();
          configSocket.textAll("distance:"+String(lim_distance)+",angle:"+String(lim_angle));
        }else if(!validDistance){
          request->send(200, "text/text", "invalid distance");
        }else if(!validAngle){
          request->send(200, "text/text", "invalid angle");
        }
        // writeFile(SPIFFS, "/inputString.txt", inputMessage.c_str());
      } else {
        request->send(200, "text/text", "incomplete configuration");
      }
    });
    server.onNotFound(notFound);
    server.begin();
}

void webSocketMeasureInfo(){
    angleSocket.textAll(String(stickrot));
    distanceSocket.textAll(String(distance));
}


void webSocketLoggerInfo(){
    // std::stringstream stream;
    // stream << " isperson: " << isperson;
    // stream << " isbrake: " << isbrake;
    // stream << " isalert: " << isalert;
    // std::string text = stream.c_str();
    // loggerSocket.textAll(&text);
    // std::string s;
    // char ispersonChar = isperson?1:0;
    // s.append(" isperson: ");
    // s.append(&ispersonChar);
    // s.append(" isbrake: ");
    // s.append(String(isbrake));
    // s.append( "isalert: ");
    // s.append(String(isalert));

    char loggerChar[50];
    // char ispersonChar[10];
    // char isbrakeChar[10];
    // char isalertChar[20];
    int ispersonInt = isperson?1:0;
    int isbrakeInt = isbrake?1:0;
    int isalertInt = isalert?1:0;
    sprintf(loggerChar, "isperson: %x isbrake: %x isalert: %x", ispersonInt, isbrakeInt, isalertInt);
    loggerSocket.textAll(String(loggerChar));
    loggerSocket.textAll("end");
    // sprintf(ispersonChar, "isperson: %x", ispersonInt);
    // loggerSocket.textAll(String(ispersonChar));
    // sprintf(isbrakeChar, " isbrake: %x", isbrakeInt);
    // loggerSocket.textAll(String(isbrakeChar));
    // sprintf(isalertChar, " isalert: %x ", isalertInt);
    // loggerSocket.textAll(String(isalertChar));
    // loggerSocket.textAll("end");
}

void SerialPrintLimitDistanceAngle(){
  Serial.print("lim_distance: ");
  Serial.println(lim_distance);
  Serial.print("lim_angle: ");
  Serial.println(lim_angle);
}

