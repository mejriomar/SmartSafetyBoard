#include <Arduino.h>
//-----------------------------------------------------web server-----------------------------------------
#include "EEPROM.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
String pointacc = "ON";  // Control the access point (ON or OFF)
//-------------------------------------------------connection_WIFI--------------------------------------------------------------------
#include <SPIFFS.h>
#include <WiFiSettings.h>
const char* ssidAP = "TABLEAU";
String adressip = "";
// IPAddress ip(192, 168, 1, 199); // Set static IP address
// IPAddress gateway(192, 168, 1, 1); // Set gateway IP address (router)
// IPAddress subnet(255, 255, 255, 0); // Set subnet mask
// IPAddress primaryDNS(8,8,8,8);
// IPAddress secondaryDNS(8,8,4,4);


//-----------------------------------------------------affichage---------------------------------------------------------------
#include <DMD32.h>  // LED matrix display library
#include "fonts/SystemFont5x7.h"
#include "fonts/Arial_black_16.h"
#include "fonts/Arial_Black_16_ISO_8859_1.h"

//Fire up the DMD library as dmd
#define DISPLAYS_ACROSS 3
#define DISPLAYS_DOWN 1
DMD dmd(DISPLAYS_ACROSS, DISPLAYS_DOWN);

// Timer setup
// Create a hardware timer for ESP32
hw_timer_t* timer = NULL;

/*--------------------------------------------------------------------------------------
  Interrupt handler for Timer1 (TimerOne) driven DMD refresh scanning, this gets
  called at the period set in Timer1.initialize();
--------------------------------------------------------------------------------------*/
void IRAM_ATTR triggerScan() {
  dmd.scanDisplayBySPI();
}


//--------------------------------------------temperature-------------------------------------------------------------
#include <HTTPClient.h>
#include <Arduino_JSON.h>

char tabt[5];
String stringt = "";
//-----------------------------------------------time----------------------------------------------------------------------
#include "time.h"

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

String date2, heure1, semaine1, description;
char heure[6], date[11], semaine[3], seconde[3];
String date1, jour1, mois1, annee1, seconde1;
unsigned long lastTime1 = 0, lastTime2 = 0;
int semaine_int;

//--------------------------------------------temperature-------------------------------------------------------------


// Your Domain name with URL path or IP address with path
String openWeatherMapApiKey = "my_secret_key"; // OpenWeatherMap API key (replace with your own key)


// Replace with your country code and city
String city = "ben arous";
String countryCode = "TN";

unsigned long lastTime = 0;
unsigned long timerDelay = 1000;

String jsonBuffer;
int t;
// Variables for accident-free days counter and safety dashboard
AsyncWebServer server(80);
const char* PARAM_INPUT_1 = "input1";
const char* PARAM_INPUT_2 = "input2";
const char* PARAM_INPUT_3 = "input3";
const char* PARAM_INPUT_4 = "input4";
const char* PARAM_INPUT_5 = "input5";
const char* PARAM_INPUT_6 = "input6";
const char* PARAM_INPUT_7 = "input7";
const char* PARAM_INPUT_8 = "input8";
const char* PARAM_INPUT_9 = "input9";
const char* PARAM_INPUT_10 = "input10";
const char* PARAM_API_KEY = "apikey";
const char* adr = "adr";
const char* mot = "mot";
String adr1 = "";
String mot1 = "";

const char affich[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">

      <style>
      #display {
      width: 350px; /* Modifiez la valeur ici */
      border: 1px solid black;
      margin: 0 auto;
    }
    
    .row {
      display: flex;
      justify-content: space-between;
      border-bottom: 1px solid black;
    }
    
    .cell {
      width: 50%;
      padding: 10px;
      box-sizing: border-box;
      text-align: center;
      font-size: 24px; /* Modifiez la valeur ici */
      font-weight: bold;
      color: #333;
    }
    
    .cell.header {
      background-color: #666;
      color: #fff;
      border-bottom: 2px solid #444;
    }
    
    .row:nth-child(odd) .cell {
background-color: rgb(162, 255, 243); /* Changez la valeur ici */
border-right: 1px solid #ccc;
}
.row:nth-child(even) .cell {
background-color: #fff;
border-right: 1px solid #eee;
}
</style>
    <title>affichage</title>
</head>
<body>
    <div id="display">
      <div class="row">
        <div class="cell">Etat</div>
        <div class="cell">{Etat}</div>
      </div>
        <div class="row">
          <div class="cell">Heure</div>
          <div class="cell">{Heure}</div>
        </div>
        <div class="row">
          <div class="cell">Date</div>
          <div class="cell">{Date}</div>
        </div>
        <div class="row">
            <div class="cell">semaine</div>
            <div class="cell">{semaine}</div>
          </div>
        <div class="row">
            <div class="cell">Heur d'incri</div>
            <div class="cell">{Heur d'incri}</div>
          </div>
          <div class="row">
            <div class="cell">Température</div>
            <div class="cell">{Température}</div>
          </div>
          <div class="row">
            <div class="cell">Ville</div>
            <div class="cell">{Ville}</div>
          </div>
          <div class="row">
            <div class="cell">j sans acci</div>
            <div class="cell">{jour sans acci}</div>
          </div>
          <div class="row">
            <div class="cell">Record</div>
            <div class="cell">{Record}</div>
          </div>

            <div class="row">
            <div class="cell">Adresse IP</div>
            <div class="cell">{Adresse IP}</div>
            </div>
             <div class="row">
            <div class="cell">p d'accés</div>
            <div class="cell">{point d'accés}</div>
            </div>
       
      </div>
</body>
</html>
)rawliteral";

const char login[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>LOGIN</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.1/css/all.min.css">
  <style>
    body {
      font-family: Arial, sans-serif;
      background-color: #eee;
    }
    form {
      width: 300px;
      margin: 50px auto;
      text-align: center;
      box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
      background-color: #fff;
      padding: 20px;
    }
    h3 {
      margin-bottom: 20px;
      color: rgb(2, 1, 10);
    }
    .input {
      width: 100%;
      height: 40px;
      font-size: 16px;
      border: 1px solid #ccc;
      border-radius: 4px;
      padding: 0 10px;
      box-sizing: border-box;
      transition: 0.3s ease all;
    }
    .input:focus {
      border: 1px solid #999;
      box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
    }
    
    input[type="submit"] {
      width: 100%;
      height: 40px;
      font-size: 16px;
      background-color: lightblue;
      border: none;
      border-radius: 4px;
      cursor: pointer;
      transition: 0.3s ease all;
    }
    input[type="submit"]:hover {
      background-color: skyblue;
    }
    .password-container {
      position: relative;
    }

    .password-eye {
      position: absolute;
      right: 10px;
      top: 50%;
transform: translateY(-50%);
cursor: pointer;
}
</style>

</head>
<body>
  <form action="/">
    <h3>Adresse</h3>
    <input type="text" class="input" name="adr">
    <h3>Mot de passe</h3>
    <div class="password-container">
      <input type="password" class="input" name="mot" id="password-input" pattern="[a-zA-Z0-9]*" style="margin-bottom: 30px;" >
      <i class="password-eye fas fa-eye" onclick="
        var input = document.getElementById('password-input');
        if (input.type === 'password') {
          input.type = 'text';
          this.classList.remove('fa-eye');
          this.classList.add('fa-eye-slash');
        } else {
          input.type = 'password';
          this.classList.remove('fa-eye-slash');
          this.classList.add('fa-eye');
        }
      "></i>
    </div>
    <br>
    <input type="submit" value="Envoyer">
  </form>
</body>
</html>)rawliteral";



const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>tbleau de sécurité</title>
  <meta charset="UTF-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">

    <style>
        body {
          font-family: 'Open Sans', sans-serif;
          background-color: #f5f5f5;
        }
        form {
          max-width: 400px;
          margin: 0px auto;
          text-align: left;
         
          background-color: #fff;
          padding: 30px;
        }
        h3 {
          margin-bottom: 20px;
          color: #666;
        }
        input[type="text"] {
          width: 100%;
          height: 50px;
          font-size: 18px;
          border: 1px solid #ccc;
          border-radius: 4px;
          padding: 0 15px;
          box-sizing: border-box;
          transition: 0.3s ease all;
        }
        input[type="text"]:focus {
          border: 1px solid #999;
          box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
        }
        input[type="submit"] {
          width: 100%;
          height: 50px;
          font-size: 18px;
          background-color: #4caf50;
          border: none;
          border-radius: 4px;
          cursor: pointer;
          transition: 0.3s ease all;
        }
        input[type="submit"]:hover {
          background-color: #3d8b40;
        }
        input[type="submit"][name="input5"] {
          background-color: #f44336;
        }
        input[type="submit"][name="input5"]:hover {
          background-color: #c62828;
        }
        input[type="submit"][name="input6"] {
          background-color: #4caf50;
        }
        input[type="submit"][name="input6"]:hover {
          background-color: #3d8b40;
        }
        input[type="submit"] {
          background-color: #2196f3;
        }
        input[type="submit"]:hover {
          background-color: #0d8ecf;
        }
        
      select {
  width: 100%;
  height: 32px;
  font-size: 16px;
  padding: 4px;
  border: 1px solid #ccc;
  border-radius: 4px;
}
      </style>
  </head><body>
  <form action="/get1">
   <h3>Nombre de jours sans accident</h3>
    <input type="text" name="input1">

  <h3>Record de jours sans accident</h3>
    <input type="text" name="input2">

  <h3>Heure d'incrementation</h3>
     <input type="text" name="input3">

  <h3>OpenWeatherMap API Key</h3>
    <input type="text" name="apikey">

         <h3>Choisissez une région :</h3>
        <select id="ma_liste_deroulante" name="input7">
          <option value="">Choisir</option>
          <option value="ariana">Ariana</option>
          <option value="beja">Béja</option>
          <option value="ben arous">Ben Arous</option>
          <option value="bizerte">Bizerte</option>
          <option value="gabes">Gabès</option>
          <option value="gafsa">Gafsa</option>
          <option value="jendouba">Jendouba</option>
          <option value="kairouan">Kairouan</option>
          <option value="kasserine">Kasserine</option>
          <option value="kebili">Kébili</option>
          <option value="kef">Le Kef</option>
          <option value="mahdia">Mahdia</option>
          <option value="manouba">Manouba</option>
          <option value="medenine">Médenine</option>
          <option value="monastir">Monastir</option>
          <option value="nabeul">Nabeul</option>
          <option value="sfax">Sfax</option>
          <option value="sidi bouzid">Sidi Bouzid</option>
          <option value="siliana">Siliana</option>
          <option value="sousse">Sousse</option>
          <option value="tataouine">Tataouine</option>
          <option value="tozeur">Tozeur</option>
          <option value="tunis">Tunis</option>
          <option value="zaghouan">Zaghouan</option>
        </select>
        <input type="submit" value="envoyer">
      </form><br>
  
  <form action="/get2">
    
    <input type="submit" value="Redemarrer" name="input4">
  </form><br>

   <form action="/get2">
 
    <input type="submit" value="    OFF    " name="input5">
  </form><br>

   <form action="/get2">
  
    <input type="submit" value="    ON     " name="input6">
  </form>
    <form action="/get2">
    <input type="submit" value="Deconnecter le WIFI" name="input9">
  </form>
      <form action="/get2">
    <input type="submit" value="Fermer la point d'accés" name="input10">
  </form>
  <form action="/get2">
    <input type="submit" value="Affichage" name="input8">
  </form>
</body></html>)rawliteral";



void notFound(AsyncWebServerRequest* request) {
  request->send(404, "text/plain", "Not found");
}
int nb_jour_sans_acc_int = 9, record_int;
String nb_jour_sans_acc, nb_jour_sans_acc_forme = "", record, record_forme = "", heure_dincri = "00:00", heure_dincri_forme = "", ouvre = "ON", city_forme = "";  // Number of days without accidents, record, and form variables
String nb_api, record_api, api, stringt_api;
String inputMessage, inputParam;
String affichage;



// ========================================== Function Declarations ==========================================

// Fetch weather data from OpenWeatherMap API via HTTP GET request
String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;

  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);

  // Send HTTP GET request
  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

// ========================================== Time and Date Functions ==========================================

// Retrieve and format current local time from NTP server
void printLocalTime() {
  // Structure containing time data (year, month, day, hour, minute, second, etc.)
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {

    Serial.println("Failed to obtain time");

    return;
  }

  // Use strftime to extract date, time, week number, and seconds
  strftime(date, 11, "%F", &timeinfo);  // Format: YYYY-MM-DD
  strftime(heure, 6, "%R", &timeinfo);  // Format: HH:MM
  strftime(semaine, 3, "%V", &timeinfo);  // Week number
  strftime(seconde, 3, "%S", &timeinfo);  // Seconds
  // Convert date and time arrays to string variables
  date1 = date;
  heure1 = heure;
  heure1 = heure1.substring(0, 5);
  seconde1 = seconde;
  seconde1 = seconde1.substring(0, 2);
  semaine1 = semaine;
  semaine_int = semaine1.toInt();  // Convert to integer to display as '1' instead of '01'
  // Use substring to extract day, month, and year from date
  annee1 = date1.substring(0, 4);
  mois1 = date1.substring(5, 7);
  jour1 = date1.substring(8, 10);
  // Reformat date to DD/MM format for display
  date2 = jour1 + "/" + mois1;
}
// ========================================== Utility Functions ==========================================

// Format a number to always be 4 digits (e.g., 1 => '0001', 123456 => '1234')
String nombre4(String n) {
  if (n.length() == 1) {
    n = "000" + n;
  }
  if (n.length() == 2) {
    n = "00" + n;
  }
  if (n.length() == 3) {
    n = "0" + n;
  }
  if (n.length() > 4) {
    n = n.substring(0, 4);
  }
  return n;
}

// Write content to a file in SPIFFS (SPI Flash File System)
bool spurt(const String& fn, const String& content) {
  File f = SPIFFS.open(fn, "w");
  if (!f) return false;
  auto w = f.print(content);
  f.close();
  return w == content.length();
}


// ========================================== WiFi and Web Server Setup ==========================================

void setup() {

  Serial.begin(115200);
  SPIFFS.begin(true);  // Format on first run if mounting fails
  WiFiSettings.hostname = "TABLEAU";
  WiFiSettings.connect();
  // spurt("/wifi-password", "omar");
  // WiFi.config(ip, gateway, subnet, primaryDNS, secondaryDNS);  // Apply static IP configuration
  Serial.println(WiFi.localIP());
  WiFi.mode(WIFI_AP_STA);  // Use both AP (Access Point) and STA (Station) modes
  // WiFi.softAP(ssid, password);  // Create access point with password
  WiFi.softAP(ssidAP);  // Create access point without password
                        // delay(1000);
  IPAddress localIP = WiFi.localIP();
  adressip = localIP.toString();

  // Initialize EEPROM and retrieve stored values (accident counter, record, and city)
  if (!EEPROM.begin(1000)) {
    Serial.println("Failed to initialise EEPROM");
    Serial.println("Restarting...");
    delay(1000);
    ESP.restart();
  }
  nb_jour_sans_acc = EEPROM.readString(500);  // Load accident-free days counter
  record = EEPROM.readString(300);  // Load record value
  city = EEPROM.readString(0);  // Load city for weather API
  String stored_apikey = EEPROM.readString(600);  // Load API key
  if (stored_apikey.length() > 0) {
    openWeatherMapApiKey = stored_apikey;  // Use stored API key if available
  }

  // Setup login interface
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    adr1 = request->arg(adr);
    mot1 = request->arg(mot);

    if (adr1 == "omar" and mot1 == "1234") {
      request->send_P(200, "text/html", index_html);
      adr1 = "";
      mot1 = "";
    } else {
      request->send_P(200, "text/html", login);
    }
  });

  server.on("/api", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/plain", api);
  });

  server.on("/ip", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "text/plain", adressip);
  });



  server.on("/get1", HTTP_GET, [](AsyncWebServerRequest* request) {
    timerAlarmDisable(timer);
    nb_jour_sans_acc_forme = request->getParam(PARAM_INPUT_1)->value();
    record_forme = request->getParam(PARAM_INPUT_2)->value();
    heure_dincri_forme = request->getParam(PARAM_INPUT_3)->value();
    city_forme = request->getParam(PARAM_INPUT_7)->value();
    String apikey_forme = request->getParam(PARAM_API_KEY)->value();

    if (nb_jour_sans_acc_forme.length() > 0) {
      nb_jour_sans_acc = nb_jour_sans_acc_forme;

      EEPROM.writeString(500, nb_jour_sans_acc);
    }
    if (record_forme.length() > 0) {
      record = record_forme;
      EEPROM.writeString(300, record);
    }
    if (heure_dincri_forme.length() > 4) {
      heure_dincri = heure_dincri_forme;
    }
    if (city_forme.length() > 0) {
      city = city_forme;
      EEPROM.writeString(0, city);
    }
    if (apikey_forme.length() > 0) {
      openWeatherMapApiKey = apikey_forme;
      EEPROM.writeString(600, openWeatherMapApiKey);
    }
    //request->send(200, "text/plain", "check");
    request->send_P(200, "text/html", index_html);
    EEPROM.commit();
    timerAlarmEnable(timer);
  });


  server.on("/get2", HTTP_GET, [](AsyncWebServerRequest* request) {
    if (request->hasParam(PARAM_INPUT_4)) {
      request->send_P(200, "text/html", index_html);
      delay(2000);
      ESP.restart();
    } else if (request->hasParam(PARAM_INPUT_5)) {

      ouvre = "OFF";
    } else if (request->hasParam(PARAM_INPUT_6)) {
      ouvre = "ON";
    }

    else if (request->hasParam(PARAM_INPUT_8)) {
      String html = String(affich);
      html.replace("{Etat}", ouvre);
      html.replace("{Heure}", heure1);
      html.replace("{Date}", date2);
      html.replace("{semaine}", semaine1);
      html.replace("{Heur d'incri}", heure_dincri);
      html.replace("{Température}", stringt);
      html.replace("{Ville}", city);
      html.replace("{jour sans acci}", nb_jour_sans_acc);
      html.replace("{Record}", record);
      html.replace("{Adresse IP}", adressip);
      html.replace("{point d'accés}", pointacc);
      request->send_P(200, "text/html", html.c_str());
    }

    else if (request->hasParam(PARAM_INPUT_9)) {
      timerAlarmDisable(timer);
      spurt("/wifi-password", "deconnecte");
      delay(1000);
      request->send_P(200, "text/html", index_html);
      delay(1000);
      ESP.restart();
      timerAlarmEnable(timer);
    } else if (request->hasParam(PARAM_INPUT_10)) {
      pointacc = "OFF";

      if (pointacc == "OFF") {
        timerAlarmDisable(timer);
        WiFi.mode(WIFI_STA);
        timerAlarmEnable(timer);
        delay(1000);
      }

    } else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    request->send_P(200, "text/html", index_html);
  });


  server.on("/get", HTTP_GET, [](AsyncWebServerRequest* request) {
    timerAlarmDisable(timer);
    if (request->hasParam(PARAM_INPUT_1)) {
      nb_jour_sans_acc = request->getParam(PARAM_INPUT_1)->value();
      EEPROM.writeString(500, nb_jour_sans_acc);
    } else if (request->hasParam(PARAM_INPUT_2)) {
      record = request->getParam(PARAM_INPUT_2)->value();
      EEPROM.writeString(300, record);
    } else if (request->hasParam(PARAM_INPUT_4)) {
      request->send(200, "text/plain", "check");
      delay(2000);
      ESP.restart();
    } else if (request->hasParam(PARAM_INPUT_5)) {
      ouvre = "OFF";
    } else if (request->hasParam(PARAM_INPUT_6)) {
      ouvre = "ON";
    } else if (request->hasParam(PARAM_INPUT_7)) {
      city = request->getParam(PARAM_INPUT_7)->value();
      EEPROM.writeString(0, city);
    } else if (request->hasParam(PARAM_INPUT_9)) {
      spurt("/wifi-password", "deconnecte");
      delay(1000);
      request->send(200, "text/plain", "check");
      delay(1000);
      ESP.restart();
    } else if (request->hasParam(PARAM_INPUT_10)) {
      pointacc = "OFF";
      if (pointacc == "OFF") {
        WiFi.mode(WIFI_STA);
        delay(1000);
      }
    } else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    EEPROM.commit();
    timerAlarmEnable(timer);
    request->send(200, "text/plain", "check");
  });

  server.onNotFound(notFound);
  server.begin();
  
  // Initialize time with NTP server
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  // ========================================== Display (LED Matrix) Setup ==========================================
  
  // Get the CPU clock speed in MHz
  uint8_t cpuClock = ESP.getCpuFreqMHz();

  // Configure first timer (0 out of 4 available timers)
  // Divide CPU clock by speed value in MHz to achieve 1 microsecond per timer tick
  timer = timerBegin(0, cpuClock, true);
  // Attach interrupt routine to timer
  timerAttachInterrupt(timer, &triggerScan, true);
  // Set timer alarm interval (300 microseconds)
  timerAlarmWrite(timer, 300, true);  // Third parameter: true = repeat automatically
  // Enable the timer alarm
  timerAlarmEnable(timer);
  // Clear and initialize all LED matrix pixels in RAM
  dmd.clearScreen(true);  // true = normal mode (all pixels off), false = inverted (all pixels on)
}


void loop() {
  // ========================================== Main Loop: WiFi & Access Point Management ==========================================
  
  // Close access point after 15 minutes (900000 milliseconds) from restart
  if (pointacc == "ON") {
    if ((millis() - lastTime2) > 900000) {
      timerAlarmDisable(timer);
      WiFi.mode(WIFI_STA);
      timerAlarmEnable(timer);
      pointacc = "OFF";
    }
  }
  
  // ========================================== Weather Data Retrieval ==========================================
  
  // Fetch weather data from API at regular intervals
  if ((millis() - lastTime) > timerDelay) {
    // Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;

      jsonBuffer = httpGETRequest(serverPath.c_str());
      //Serial.println(jsonBuffer);
      JSONVar myObject = JSON.parse(jsonBuffer);

      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof(myObject) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }


      Serial.print("Temperature: ");
      //Serial.println(myObject["main"]["temp"]);
      String descri = myObject["weather"][0]["main"];  // Weather description (e.g., 'Clear', 'Rainy')
      description = descri;
      description = "\"" + description + "\"";
      t = int(myObject["main"]["temp"]);  // Temperature in Kelvin from API
      t -= 273;  // Convert to Celsius
      // Cannot directly convert int to char array, so convert to String first
      stringt = String(t);  // Convert temperature to string
      if (stringt == "-273") {
        stringt = "?";  // Handle invalid temperature
      }
      stringt_api = stringt;  // Store for API response (numeric only)
      stringt = stringt + "c";  // Format temperature as '_15c' for display
      // stringt.toCharArray(tabt, 5);  // Convert string to char array if needed
      Serial.println(t);
      Serial.println(stringt);
    }

    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
    timerDelay = 600000;  // After initial fetch, request every 10 minutes (600000 ms) due to OpenWeatherMap API rate limit
  }

  // ========================================== Time and Safety Monitoring ==========================================
  
  // Update and monitor time every second
  if ((millis() - lastTime1) > 1000) {
    printLocalTime();

    Serial.print("Heure   : ");
    Serial.println(heure1);
    Serial.print("seconde : ");
    Serial.println(seconde1);
    Serial.print("date    : ");
    Serial.println(date2);
    Serial.print("semaine : ");
    Serial.println(semaine_int);
    Serial.println("");
    Serial.print("nb jour sans acci : ");
    Serial.println(nb_jour_sans_acc);
    Serial.print("record            : ");
    Serial.println(record);
    Serial.print("heure d'inci      : ");
    Serial.println(heure_dincri);
    Serial.print("ouvre             : ");
    Serial.println(ouvre);
    Serial.println("");
    lastTime1 = millis();
  }

  // ========================================== Accident-Free Days Counter ==========================================
  
  nb_jour_sans_acc_int = nb_jour_sans_acc.toInt();
  record_int = record.toInt();  // Ensure record is stored as integer
  // Increment accident-free days counter at specified time
  if (heure1 == heure_dincri) {
    if (seconde1 == "00") {
      nb_jour_sans_acc_int++;
      Serial.print("nombre de joure incrimenté : ");
      Serial.println(nb_jour_sans_acc_int);
      nb_jour_sans_acc = String(nb_jour_sans_acc_int);
      timerAlarmDisable(timer);
      EEPROM.writeString(500, nb_jour_sans_acc);
      EEPROM.commit();
      timerAlarmEnable(timer);
      delay(1000);
    }
  }
  // If accident-free days count >= current record, update record
  if (nb_jour_sans_acc_int > record_int) {
    record_int = nb_jour_sans_acc_int;
    record = String(record_int);
    timerAlarmDisable(timer);
    EEPROM.writeString(300, record);
    EEPROM.commit();
    timerAlarmEnable(timer);
  }
  nb_jour_sans_acc = String(nb_jour_sans_acc_int);
  record = String(record_int);
  nb_api = nb_jour_sans_acc;  // Days count for API (without leading zeros)
  record_api = record;  // Record for API (normal format: 12, 3, etc.)
  String ouvre_api = "\"" + ouvre + "\"";
  String semaine_api = String(semaine_int);
  String stringt_api1 = "\"" + stringt_api + "\"";
  String city_api = "\"" + city + "\"";
  api = "{\"temp\":" + stringt_api1 + ",\"nbacc\":" + nb_api + ",\"nbst\":" + semaine_api + ",\"record\":" + record + ",\"etat\":" + ouvre_api + ",\"descri\":" + description + ",\"ville\":" + city_api + "}";

  nb_jour_sans_acc = nombre4(nb_jour_sans_acc);
  record = nombre4(record);
  
  // ========================================== LED Matrix Display Rendering ==========================================
  
  String a = "", b = "";  // Strings for top and bottom lines
  char ta[0], tb[0];
  int gdh, gdb;  // X offset for top (gdh) and bottom (gdb) lines
  dmd.clearScreen(true);  // Clear display
  dmd.selectFont(System5x7);  // Select font for rendering

  // Loop through each LED matrix display (3 displays horizontally)
  for (byte x = 0; x < DISPLAYS_ACROSS; x++) {
    for (byte y = 0; y < DISPLAYS_DOWN; y++) {
      // Display 1: Temperature and time
      if (x == 0 and ouvre == "ON") {
        a = stringt;  // Top line: temperature
        b = heure1;  // Bottom line: current time
        gdh = 7;  // X offset for temperature
        gdb = 2;  // X offset for time
      }
      // Display 2: Week number and date
      if (x == 1 and ouvre == "ON") {
        a = semaine1;  // Top line: week number
        b = date2;  // Bottom line: date (DD/MM)
        gdh = 10;  // X offset for week
        gdb = 0;  // X offset for date
      }
      // Display 3: Accident-free days and record
      if (x == 2 and ouvre == "ON") {
        a = nb_jour_sans_acc;  // Top line: days without accident
        b = record;  // Bottom line: record
        gdh = 7;  // X offset for days counter
        gdb = 7;  // X offset for record
      }

      // Draw strings on the corresponding LED matrix
      // Each matrix is 32 pixels wide and 16 pixels tall
      // X coordinate: gdh + (32 * x) - horizontal position
      // Y coordinate: 1 + (16 * y) for top line, 9 + (16 * y) for bottom line
      dmd.drawString(gdh + (32 * x), 1 + (16 * y), a.c_str(), 5, GRAPHICS_NORMAL);  // Draw top line
      dmd.drawString(gdb + (32 * x), 9 + (16 * y), b.c_str(), 5, GRAPHICS_NORMAL);  // Draw bottom line
    }
  }

  delay(100);  // Small delay to prevent overwhelming the display refresh
}
