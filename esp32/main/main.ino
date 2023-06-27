
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "Infinix NOTE 8";
const char* password = "123456789";

const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";

AsyncWebServer server(80);

const char beranda_html[] PROGMEM =  R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>BERANDA</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">

</head>
<body>
  <h2>BERANDA</h2>
  <a href="" target="_blank" rel="">Inputan</a>
  <a href="" target="_blank" rel="">Kontrol Manual</a>
  <progress value="100" max="100"></progress>

  <h3>Status Drone</h3>
  <p>Detail Drone</p>
  <p>MAPS</p>
</body>
</html>
)rawliteral";

const char inputan_html[] PROGMEM =  R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>INPUTAN HTML</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">

</head>
<body>
  <h2>INPUTAN HTML</h2>
  <a href="" target="_blank" rel="">Inputan</a>
  <a href="" target="_blank" rel="">Kontrol Manual</a>
  <progress value="100" max="100"></progress>

  <h3>Status Drone</h3>
  <p>Detail Drone</p>
  <p>MAPS</p>
</body>
</html>
)rawliteral";

const char kontrol_html[] PROGMEM =  R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>KONTROL MANUAL</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">

</head>
<body>
  <h2>KONTROL MANUAL</h2>
  <a href="" target="_blank" rel="">Inputan</a>
  <a href="" target="_blank" rel="">Kontrol Manual</a>
  <progress value="100" max="100"></progress>

  <h3>Status Drone</h3>
  <p>Detail Drone</p>
</script>
</body>
</html>
)rawliteral";

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 6px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 3px}
    input:checked+.slider {background-color: #b30000}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body>
  <h2>WEB Project Drone By Antonius Bun Wijaya</h2>
  %BUTTONPLACEHOLDER%
<script>function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/update?output="+element.id+"&state=1", true); }
  else { xhr.open("GET", "/update?output="+element.id+"&state=0", true); }
  xhr.send();
}
</script>
</body>
</html>
)rawliteral";

String processor(const String& var){
  //Serial.println(var);
  if(var == "BUTTONPLACEHOLDER"){
    String buttons = "";
    buttons += "<h4>Output - GPIO 2</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"2\" " + outputState(2) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>Output - GPIO 4</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"4\" " + outputState(4) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>Output - GPIO 33</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"33\" " + outputState(33) + "><span class=\"slider\"></span></label>";
    return buttons;
  }
  return String();
}

String outputState(int output){
  if(digitalRead(output)){
    return "checked";
  }
  else {
    return "";
  }
}

void setup(){
  Serial.begin(115200);

  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  pinMode(33, OUTPUT);
  digitalWrite(33, LOW);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/beranda", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", beranda_html, processor);
  });

  server.on("/kontrol", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", kontrol_html, processor);
  });

  server.on("/inputan", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", inputan_html, processor);
  });

  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage1;
    String inputMessage2;
 
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
    }
    else {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
    }
    Serial.print("GPIO: ");
    Serial.print(inputMessage1);
    Serial.print(" - Set to: ");
    Serial.println(inputMessage2);
    request->send(200, "text/plain", "OK");
  });

  server.begin();
}

void loop() {

}