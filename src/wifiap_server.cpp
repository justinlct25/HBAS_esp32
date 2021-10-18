#include "wifiap_server.h"
#include <AsyncElegantOTA.h>

unsigned long quit_time_wifiap = 300000UL; // 5 minutes

const char* APip = "0.0.0.0";

bool wifi_inited = false;
char wifi_ap_ssid[200] = "Handbrake-Alert-System";
// char wifi_ap_ssid[100];
// char* wifi_ap_ssid = "Handbrake-Alert-System";
const char* wifi_ap_password = "muselabs";
const char* PARAM_DEVICEID = "inputDeviceId";
const char* PARAM_DISTANCE = "inputDistance";
const char* PARAM_ANGLE = "inputAngle";
const char* PARAM_WIFISSID = "inputWifiSsid";
const char* PARAM_WIFIPW = "inputWifiPw";
const char* PARAM_UPDATESERVERURL = "inputServerUrl";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
  <html>
  <head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
  textarea{
      width:300px;
      height:250px;
  }
  #deviceIdContainer{
      display: flex;
  }
  </style>
  </head>
  <body>
    <h1>Handbrake Alert System Configuration</h1>
    <div id="deviceIdContainer">
        %CURRENTDEVICEID%<button id="editDeviceId">Edit</button> 
    </div>
    %CURRENTSTATUS%
    <h3>Alert Threshold: </h3>
    <div id="configContainer">
        %CURRENTCONFIGURATION%
        <button id="editConfig">Edit</button>
    </div>
    <iframe style="display:none" name="hidden-form"></iframe>
    <h3>Measurement of Distance & Angle: </h3>
    <div id="currentMeasure"></div>
    <div id="currentDistance"></div>
    <div id="currentAngle"></div> 
    <h3>System Status Log</h3>
    <textarea id="systemLogger" style="width:100%;height:50%;"></textarea>
    <h3>Wifi STA-mode OTA Update Setting: </h3>
    <div id="updateConfigContainer">
        %CURRENTSTAOTACONFIG%
        <button id="editUpdateConfig">Edit</button>
    </div>

  </body>
  <script>
    const statusGateway = `ws://${window.location.hostname}/statusSocket`;
    const configGateway = `ws://${window.location.hostname}/configSocket`;
    const measureGateway = `ws://${window.location.hostname}/measureSocket`;
    const loggerGateway = `ws://${window.location.hostname}/loggerSocket`;
    let statussocket;
    let configsocket;
    let measuresocket;
    let loggerSocket;
    window.addEventListener('load', onLoad);
    document.getElementById("editConfig").onclick = editConfigButtonEvent;
    document.getElementById("editDeviceId").onclick = editDeviceId;
    document.getElementById("editUpdateConfig").onclick = editUpdateConfig;
    function submitMessage() {}
    function initStatusWebSocket(){
        console.log('Trying to open a StatusSocket connection...');
        statussocket = new WebSocket(statusGateway);
        statussocket.onopen = onOpen;
        statussocket.onclose = onCloseStatusSocket;
        statussocket.onmessage = onStatusMessage;
    }
    function initConfigWebSocket(){
      console.log('Trying to open a ConfigSocket connection...');
      configsocket = new WebSocket(configGateway);
      configsocket.onopen = onOpen;
      configsocket.onclose = onCloseConfigSocket;
      configsocket.onmessage = onConfigMessage;
    }
    function initMeasureWebSocket(){
      console.log('Trying to open a MeasureSocket connection...');
      measuresocket = new WebSocket(measureGateway);
      measuresocket.onopen = onOpen;
      measuresocket.onclose = onCloseMeasureSocket;
      measuresocket.onmessage = onMeasureMessage;
    }
    function initLoggerWebSocket(){
        console.log('Trying to open a loggerSocket connection...');
        loggerSocket = new WebSocket(loggerGateway);
        loggerSocket.onopen = onOpen;
        loggerSocket.onclose = onCloseLoggerSocket;
        loggerSocket.onmessage = onLoggerMessage;
    }
    function onOpen(event) {
      console.log('Connection opened');
    }
    function onCloseStatusSocket(event) {
      console.log('status connection closed');
        setTimeout(initStatusWebSocket, 2000);
    }
    function onCloseConfigSocket(event) {
      console.log('status connection closed');
        setTimeout(initConfigWebSocket, 2000);
    }
    function onCloseMeasureSocket(event) {
      console.log('status connection closed');
        setTimeout(initMeasureWebSocket, 2000);
        
    }
    function onCloseLoggerSocket(event) {
      console.log('status connection closed');
        setTimeout(initLoggerWebSocket, 2000);
    }
    function onStatusMessage(event) {
        document.getElementById("deviceIdContainer").innerHTML=`<div>Device ID: ${event.data}</div><button id="editDeviceId">Edit</button> `
        document.getElementById("editDeviceId").onclick = editDeviceId;
    }
    function onConfigMessage(event) {
      let config = event.data.split(',');
      let distance = config[0].split(':')[1];
      let angle = config[1].split(':')[1];
      document.getElementById('configContainer').innerHTML = `<div>Distance: ${distance} cm</div><div>Angle: ${angle} deg</div><button id="editConfig">EDIT</button>`;
        document.getElementById("editConfig").onclick = editConfigButtonEvent;
    }
    function onMeasureMessage(event){
        let measure = event.data.split(',');
        let angle = measure[0];
        let distance = measure[1];
        document.getElementById('currentMeasure').innerHTML = `<div>Distance: ${distance} cm</div><div>Angle: ${angle} deg</div>`
    }
    function onDistanceMessage(event){
      document.getElementById('currentDistance').innerHTML = `<div>Measured Distance: ${event.data} cm</div>`
    }
    function onLoggerMessage(event){
        // if(event.data=="end"){
            // document.getElementById("systemLogger").value += "\n";
        // }else{
            document.getElementById("systemLogger").value += event.data+"\n";
        // }
    }
    function initAllWebSocket(){
        initMeasureWebSocket();
        initLoggerWebSocket();
        initStatusWebSocket();
        initConfigWebSocket();
    }
    function onLoad(event) {
        initAllWebSocket();
    }
    function editConfigButtonEvent(){
        document.getElementById("configContainer").innerHTML=`
            <form action="/paramconfig" target="hidden-form">
                Distance: <input type="text" name="inputDistance">(in cm)<br>
                Angle : <input type="number " name="inputAngle">(in deg)<br>
                <input type="submit" value="Submit" onclick="submitMessage()">
            </form>`
    }
    function editDeviceId(){
        document.getElementById("deviceIdContainer").innerHTML=`
            <form action="/deviceconfig" target="hidden-form">
                Device ID: HbAs_<input id="idInput" type="text" name="inputDeviceId">
                <input type="submit" value="Submit" >
            </form>`
    }
    function editUpdateConfig(){
        document.getElementById("updateConfigContainer").innerHTML=`
            <form = action="/updateconfig" target="hidden-form">
                Wifi ssid: <input type="text" name="inputWifiSsid"><br>
                Wifi pw: <input type="text" name="inputWifiPw"><br>
                Server URL:<br>
                <input type="text" name="inputServerUrl"><br>
                <input type="submit" value="Submit">
            </form>`
    }
  </script>
</html>
)rawliteral";

AsyncWebServer server(80);
AsyncWebSocket statusSocket("/statusSocket");
AsyncWebSocket configSocket("/configSocket");
AsyncWebSocket measureSocket("/measureSocket");
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
    if(var == "CURRENTDEVICEID"){
        String device = "";
        device += "<div>Device ID: HbAs_";
        device += String(device_id);
        device += "</div>";
        return device;
    }
    if(var == "CURRENTSTATUS"){
        String status = "";
        status += "<div>Version: ";
        status += fw_version;
        status += " Battery: ";
        status += String(bat);
        status += "</div>";
        return status;
    }
    if(var == "CURRENTSTAOTACONFIG"){
        String update = "";
        update += "<div>Wifi ssid: ";
        update += wifi_ssid;
        update += "</div><div>Wifi password: ";
        update += wifi_password;
        update += "</div><div>Update Server URL: ";
        update += update_server_url;
        update += "</div>";
        return update;
    }
  return String();
}

void wifiWebSocket_init(){
    statusSocket.onEvent(onEvent);
    server.addHandler(&statusSocket);
    configSocket.onEvent(onEvent);
    server.addHandler(&configSocket);
    measureSocket.onEvent(onEvent);
    server.addHandler(&measureSocket);
    loggerSocket.onEvent(onEvent);
    server.addHandler(&loggerSocket);
}


void wifiAPServer_init(){
  Serial.println(wifi_ap_ssid);
    WiFi.softAP(wifi_ap_ssid, wifi_ap_password);
    if(!MDNS.begin("handbrake")){
    Serial.println("Error starting mDNS");
    return;
  }
    wifiWebSocket_init();
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());
    server.on("/", HTTPP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/html", index_html, processor);
      // request->send_P(200, "text/html", index_html);
    });
    server.on("/deviceconfig", HTTPP_GET, [] (AsyncWebServerRequest *request){
        if(request->hasParam(PARAM_DEVICEID)){
            device_id = atof((request->getParam(PARAM_DEVICEID)->value()).c_str());
            // if(device_id!=0){
            //     sprintf(wifi_ap_ssid, "HbAs_%x", device_id);
            // }
            NVS.setInt("device_id",device_id);
            request->send(200, "text/text", "device id set");
            statusSocket.textAll(String(device_id));
        } else{
            request->send(200, "text/text", "incomplete configuration");
        }
    });
    // Send a GET request to <ESP_IP>/config?inputString=<inputMessage>
    server.on("/paramconfig", HTTPP_GET, [] (AsyncWebServerRequest *request) {
      char tempDistance;
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
    server.on("/updateconfig", HTTPP_GET, [] (AsyncWebServerRequest *request) {
        if((request->hasParam(PARAM_WIFISSID))&&(request->hasParam(PARAM_WIFIPW))&&(request->hasParam(PARAM_UPDATESERVERURL))){
            // wifi_ssid = (request->getParam(PARAM_WIFISSID)->value())c_str();
            sprintf(wifi_ssid, "%s", (request->getParam(PARAM_WIFISSID)->value()));
            NVS.setString("wifi_ssid", (request->getParam(PARAM_WIFISSID)->value()));
            // wifi_password = (request->getParam(PARAM_WIFIPW)->value()).c_str();
            sprintf(wifi_password, "%s", (request->getParam(PARAM_WIFIPW)->value()));
            NVS.setString("wifi_password", (request->getParam(PARAM_WIFIPW)->value()));
            String nvs_url = (request->getParam(PARAM_UPDATESERVERURL)->value());
            // Serial.println("take");
            // Serial.println(nvs_url);
            NVS.setString("server_url", nvs_url);
            sprintf(update_server_url, "%s", nvs_url);
            // sprintf(update_server_url, "%s", nvs_url);
            // update_server_url = (char* )nvs_url.c_str();
            sprintf(version_url, "http://%s/version",nvs_url);
            sprintf(bin_url, "http://%s/bin/",nvs_url);
            request->send(200, "text/text", "device id set");
            statusSocket.textAll(String(device_id));
        } else{
            request->send(200, "text/text", "incomplete configuration");
        }
    });
    server.onNotFound(notFound);
    server.begin();
    //AsyncElegantOTA.begin(&server);
}


void webSocketMeasureInfo(){
    char measure[50];
    sprintf(measure, "%d,%d", stickrot, distance);
    measureSocket.textAll(String(measure));
}


void webSocketLoggerInfo(){
    char loggerChar[50];
    int ispersonInt = isperson?1:0;
    int isbrakeInt = isbrake?1:0;
    int isalertInt = isalert?1:0;
    sprintf(loggerChar, "isperson: %x isbrake: %x isalert: %x", ispersonInt, isbrakeInt, isalertInt);
    loggerSocket.textAll(String(loggerChar));
}

void SerialPrintLimitDistanceAngle(){
  Serial.print("lim_distance: ");
  Serial.println(lim_distance);
  Serial.print("lim_angle: ");
  Serial.println(lim_angle);
}

void wifiAPServer_routine(){
  if (millis()<=quit_time_wifiap){
  // if (bat >= highvolt){
    // if(wifi_inited==false && WiFi.softAPgetStationNum()==0){
    if(WiFi.softAPIP().toString().equals(APip)){
      Serial.println("wifiinit");
      wifiAPServer_init();
      wifi_inited = true;
    }
  } else{
    //Serial.println("wifiapstatusclose");
    //Serial.print(WiFi.softAPgetStationNum());
    WiFi.softAPdisconnect(true);
    wifi_inited = false;
  }
  if(bat < highvolt)
  {
    WiFi.softAPdisconnect(true);
    wifi_inited = false;
  }
}

void AsyncElegantOTALoop(){
    AsyncElegantOTA.loop();
}

