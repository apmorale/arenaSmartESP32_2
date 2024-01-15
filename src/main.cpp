//Incluir liberia de arduino para comunicacion serial
#include <Arduino.h>

//Librerias conectar SP32---------------
#include <FB_Const.h>
#include <FB_Error.h>
#include <FB_Network.h>
#include <FB_Utils.h>
#include <Firebase.h>
#include <FirebaseESP32.h>
#include <FirebaseFS.h>
//#include <MB_File.h>
/**
 * Created by K. Suwatchai (Mobizt)
 *
 * Email: k_suwatchai@hotmail.com
 *
 * Github: https://github.com/mobizt/Firebase-ESP32
 *
 * Copyright (c) 2022 mobizt
 *
 */

//definición modulo wifi SP32--------------------------------
#if defined(ESP32)
#include <WiFi.h>
//#include <FirebaseESP32.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#endif

// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "Redmi Note 12S"
#define WIFI_PASSWORD "20232023"

// For the following credentials, see examples/Authentications/SignInAsUser/EmailPassword/EmailPassword.ino

/* 2. Define the API Key */
#define API_KEY "AIzaSyBju7sVbB3N-qTY-FmR355xH5vGKLwJcw0"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://control-iot-ec-f4ff1-default-rtdb.firebaseio.com/" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "andgafon@gmail.com"
#define USER_PASSWORD "123asd456zxc"




// Define Firebase Data object---------------------------------------------------------------------
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

unsigned long count = 0;


//Variables volatiles

volatile char activacionLimpieza_char;
volatile int val;

//funcion void setup()


void setup()
{
  Serial.begin(115200);//para inicialmente para wifi, luego para comunicacion serial TX
  Serial2.begin(115200);// comunicacion serial de tipo RX

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

  // Or use legacy authenticate method
  // config.database_url = DATABASE_URL;
  // config.signer.tokens.legacy_token = "<database secret>";

  // To connect without auth in Test Mode, see Authentications/TestMode/TestMode.ino

  //////////////////////////////////////////////////////////////////////////////////////////////
  // Please make sure the device free Heap is not lower than 80 k for ESP32 and 10 k for ESP8266,
  // otherwise the SSL connection will fail.
  //////////////////////////////////////////////////////////////////////////////////////////////

  Firebase.begin(&config, &auth);

  // Comment or pass false value when WiFi reconnection will control by your code or third party library
  Firebase.reconnectWiFi(true);

  Firebase.setDoubleDigits(5);

}

void writeToSerial2Buffer( char str) {
  Serial.print(str);
}


//loop
void loop() {
    //***CODIGO POR MODIFICAR
  bool firebaselimpieza = Firebase.getString(fbdo,"/test/activacionLimpieza");
  String activacionLimpieza;
  activacionLimpieza = fbdo.stringData();
  if(activacionLimpieza=="True"){
    activacionLimpieza_char='a';
  }else{
    activacionLimpieza_char='n';
  }
  
  writeToSerial2Buffer(activacionLimpieza_char);

  if (Serial.available() > 0){	//Se verifica que se haya ingresado un texto por serial0 o cable usb
    Serial2.println(Serial.readStringUntil('\n'));	//se lee el texto ingresado y se lo envia por el serial2

    if(activacionLimpieza_char=='a'){
    Firebase.setString(fbdo,"test/activaciónLimpieza","False");
    // handle error
      
    }




  }
  
  delay(50);
  // set value
  //escribirValorLongEnEEPROM(direccion_contador_limpieza, contador_limpieza);
  
  
  //Serial.println(firebaselimpieza);
  
  //Serial.println(activacionLimpieza);


 }
