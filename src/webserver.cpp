#include "webserver.h"
#include "ESPAsyncWebServer.h"
#include <ESPmDNS.h>
#include "defs.h"
#include "debug_utils.h"

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Air Quality</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .my-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>Air Quality Server</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="my-labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="my-labels">Humidity</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">&percnt;</sup>
  </p>
  <p>
    <i class="fas fa-thermometer-half" style="color: #500f9a ;"></i> 
    <span class="my-labels">Pressure</span>
    <span id="pressure">%PRESSURE%</span>
    <sup class="units">hPa</sup>
  </p>
  <p>
    <i class="fas fa-thermometer-half" style="color: #78cee8 ;"></i> 
    <span class="my-labels">Gas</span>
    <span id="gas">%GAS%</span>
    <sup class="units">KOhms</sup>
  </p>
  <p>
    <i class="fas fa-thermometer-half" style="color:#072028 ;"></i> 
    <span class="my-labels">Altitude</span>
    <span id="altitude">%ALTITUDE%</span>
    <sup class="units">m</sup>
  </p>
</body>
<script>

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 30000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 30000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("pressure").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/pressure", true);
  xhttp.send();
}, 30000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("gas").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/gas", true);
  xhttp.send();
}, 30000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("altitude").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/altitude", true);
  xhttp.send();
}, 30000 ) ;

</script>
</html>)rawliteral";

String retTemp(void){
  return String(app.aq.temp);
}

String retHum(void){
  return String(app.aq.hum);
}

String retPress(void){
  return String(app.aq.press);
}

String retGas(void){
  return String(app.aq.gas_res);
}

String retAlt(void){
  return String(app.aq.alt);
}

// Replaces placeholder with DHT values
String processor(const String& var){
  if(var == "TEMPERATURE"){
    return retTemp();
  }
  else if(var == "HUMIDITY"){
    return retHum();
  }
  else if(var == "PRESSURE"){
    return retPress();
  }
  else if(var == "GAS"){
    return retGas();
  }
  else if(var == "ALTITUDE"){
    return retAlt();
  }
  return String();
}


/**
 * @brief Initiate the web server
 */
void server_init(void){
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", retTemp().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", retHum().c_str());
  });
  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", retPress().c_str());
  });
  server.on("/gas", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", retGas().c_str());
  });
  server.on("/altitude", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", retAlt().c_str());
  });

  // Give it a local mDNS name
  const char *serverName = "m5Homeserver";
  if(!MDNS.begin(serverName)) {
     MPRINT("Error starting mDNS");
  }
  else{
    MPRINT("New server starting at: "+String(serverName)+".local")
  }
  // Start server
  server.begin();
}