/*

  Para criar o servidor no PC, abrir o prompt: netsh wlan set hostednetwork mode=allow ssid=Servidor key=12345678
  Para ativar o servidor no PC, abrir o prompt: netsh wlan start hostednetwork
  Para desativar o servidor no PC, abrir o prompt: netsh wlan stop hostednetwork
  Para listar as redes virtuais, abrir o prompt: netsh wlan show hostednetwork

  IP: 192.168.167.1
  ID: Servidor   SENHA: 12345678

*/

/*
 * 
 * ENVIAR "null" na materia onde a TAG é MASTER
 * e wd = -1 no dia da semana
 * 
 * 
 */

/* ===============  INCLUIR BIBLIOTECAS  ============== */
#include <Arduino.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ArduinoJson.h>
#include <SimpleTimer.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <ArduinoWebsockets.h>
#include <FS.h>
#include <SPIFFS.h>
#include <LiquidCrystal.h>
#include <SPI.h>
#include <MFRC522.h>
#include <time.h>


/* ===============  DEFINE PINOS RFID  ============== */
#define SDA_PIN 5
#define RST_PIN 15


/* ===============  DEFINE PINOS LCD  ============== */
#define RS_PIN 0
#define EN_PIN 4
#define D4_PIN 26
#define D5_PIN 25
#define D6_PIN 33
#define D7_PIN 32


/* ===============  DEFINE PINO TRAVA  ============== */
#define TRAVA 2


/* ===============      DEFINES      ============== */
#define USE_SERIAL Serial


/* ===============    INSTANCIAS     ============== */
MFRC522 rfid(SDA_PIN, RST_PIN);                                     // Instancia do leitor RFID
LiquidCrystal lcd(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN);  // Instancia do Display LCD 16x2
using namespace websockets;                                         // Instancia Websocket


/* ===============  LCD - CARACTERES ESPECIAIS  ============== */
byte s0_wifi[8] = {0b00000, // WiFi 0
                   0b00000,
                   0b00011,
                   0b00101,
                   0b01001,
                   0b10001,
                   0b11111,
                   0b00000
                  };

byte s1_wifi[8] = {0b00000, //  WiFi 1
                   0b00000,
                   0b00011,
                   0b00101,
                   0b01001,
                   0b11001,
                   0b11111,
                   0b00000
                  };

byte s2_wifi[8] = {0b00000, //  WiFi 2
                   0b00000,
                   0b00011,
                   0b00101,
                   0b01101,
                   0b11101,
                   0b11111,
                   0b00000
                  };

byte s3_wifi[8] = {0b00000, //  WiFi 3
                   0b00000,
                   0b00011,
                   0b00111,
                   0b01111,
                   0b11111,
                   0b11111,
                   0b00000
                  };

byte websockt[8] = {0b01010, // WEBSOCKET
                    0b01010,
                    0b11111,
                    0b10001,
                    0b10001,
                    0b01110,
                    0b00100,
                    0b00100
                   };

byte locked1[8] = {0b00000,  // CADEADO 1
                   0b00000,
                   0b00111,
                   0b01111,
                   0b11100,
                   0b11000,
                   0b11000,
                   0b11000
                  };

byte locked2[8] = {0b00000,  // CADEADO 2
                   0b00000,
                   0b11100,
                   0b11110,
                   0b00111,
                   0b00011,
                   0b00011,
                   0b00011
                  };

byte locked3[8] = {0b11111,  // CADEADO 3
                   0b11110,
                   0b11110,
                   0b11110,
                   0b11110,
                   0b11111,
                   0b11111,
                   0b00000
                  };

byte locked4[8] = {0b11111,  // CADEADO 4
                   0b01111,
                   0b01111,
                   0b01111,
                   0b01111,
                   0b11111,
                   0b11111,
                   0b00000
                  };



/* ===============      FUNCOES      ============== */
void TaskWiFi( void *parameter );                           // Tarefa WiFi
void TaskRFID( void *parameter );                           // Tarefa RFID
void TaskWebSocket( void *parameter );                      // Tarefa Websocket
void TaskWebSocketPing( void *parameter );                  // Tarefa Ping do Websocket
void onMessageCallback(WebsocketsMessage message);          // Messagem do Websocket
void onEventsCallback(WebsocketsEvent event, String data);  // Evento do Websocket
void Init_SCAF();                                           // Inicializa SCAF
void Unlock(String Line1, String Line2);                    // Função destrava 
void unknown();                                             // Função não destrava
String getID_String();                                      // Pega o ID - MAC do esp
bool loadConfig();                                          // Carrega o arquivo de configurações
bool saveUpdate(String lines);                              // Salva o Banco de Dados
bool loadUpdate();                                          // Carrega o Banco de Dados
String ClassSchedule();                                     // Mostrar no display o nome da aula atual
void hourUpdate(uint32_t TimeStamp);                        // Atualiza hora no RTC
void ClearFlagUpdateTime();                                 // Limpa Flag da variavel UpdateTime por TimeOut
String StatusClass();                                       // Retorna o status da sala [OCUPADA, RESERVADA, LIVRE, EM AULA, ...]
bool AuthorizedSchedule(byte idTag[4]);                     // Verifica a TAG no Banco de Dados local
void AccessOffline(byte idTag[4]);                          // Obtem acesso sem conexão com o Server (Salva as TAGs que acessaram)
void ValidateAccessOff();                                   // Envia para Server os acessos offline



/* ===============    VARIAVEIS     ============== */
TaskHandle_t TaskWiFi_t, TaskRFID_t, TaskWebSocket_t, TaskWebSocketPing_t;

MFRC522::MIFARE_Key key;
DynamicJsonDocument jsonBuffer(1024);
WebsocketsClient WebSocket;
SimpleTimer timer;

SemaphoreHandle_t SemaphoreDisplay = NULL;

String ID = "";
String firmware_v = "v1.0";
char ssid[20];
char password[10];
char websocketIP[16];
unsigned int websocketPORT = 0;
signed int RSSI = 0;
byte nuidPICC[4];     // Init array that will store new NUID
bool New_tag = false;
bool WiFi_connected = false;
bool WS_connected = false;
bool WS_ping = false;
bool Authenticated = false;
bool UpdateTime = false;
bool flagUpdateTime = false;
bool UpdateDB = false;
String ClassNow; 
bool InClass = false;
bool ReservedClass = false;
bool BusyClass = false;
unsigned char scroll = 0;
unsigned char delay_scroll = 0;
struct tm data; //Cria a estrutura que contem as informacoes da data.


/* ===============      SETUP       ============== */
void setup() {

  SemaphoreDisplay = xSemaphoreCreateMutex();           // Inicializa semaforo do Display

  pinMode(TRAVA, OUTPUT);                               // define o pino de acionamento da trava
  digitalWrite(TRAVA, HIGH);                            // aciona a trava

  USE_SERIAL.begin(115200);                             // Inicializa Serial
  USE_SERIAL.println("Iniciando Sistema...");
  USE_SERIAL.println("SCAF - " + firmware_v);

  lcd.begin(16, 2);                                     // Inicializa LCD
  lcd.clear();                                          // Limpa LCD
  lcd.setCursor(0, 0);                                  // Seta cursor
  lcd.createChar(1, s0_wifi);                           // envia character p/ o display
  lcd.createChar(2, s1_wifi);                           // envia character p/ o display
  lcd.createChar(3, s2_wifi);                           // envia character p/ o display
  lcd.createChar(4, s3_wifi);                           // envia character p/ o display
  lcd.createChar(5, websockt);                          // envia character p/ o display
  lcd.createChar(6, locked1);                           // envia character p/ o display
  lcd.createChar(7, locked2);                           // envia character p/ o display
  lcd.createChar(8, locked3);                           // envia character p/ o display
  lcd.createChar(9, locked4);                           // envia character p/ o display

  SPI.begin();                                          // Inicializa SPI bus - Para comunicação com leitor de TAG

  rfid.PCD_Init();                                      // Inicializa MFRC522 Leitor de TAG
  rfid.PCD_SetAntennaGain(rfid.RxGain_min);             // Configura leitor para Potência minima

  SPIFFS.begin();                                       // Inicializa sistema de arquivos

  if (!loadConfig())                                    // carrega as configurações do arquivo
    USE_SERIAL.println("\n<erro> Falha ao ler arquivo |config.json|");

  WebSocket.onMessage(onMessageCallback);               // Instanciando as mensagens de WebSocket
  WebSocket.onEvent(onEventsCallback);                  // Instanciando os eventos de WebSocket

  Init_SCAF();                                          // Inicializa SCAF
  USE_SERIAL.println("\nID: " + ID);

  USE_SERIAL.println("\nREDE: " + String(ssid));
  USE_SERIAL.println("PASSWORD: " + String(password));

  USE_SERIAL.println("\nIP SERVER: " + String(websocketIP));
  USE_SERIAL.println("PORT SERVER: " + String(websocketPORT) + "\n");

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  // Inicializa tarefa WiFi
  xTaskCreatePinnedToCore(TaskWiFi, "TaskWiFi", 16384, NULL, 1, &TaskWiFi_t, 0);  //Task Function, Task name for humans, Stack size, ... , Priority, Task name, Core number

  delay(500);                                           // Delay para tarefa inicializar

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Conectando WiFi ");
  lcd.setCursor(0, 1);
  lcd.print("   Aguarde...   ");

  while (!WiFi_connected) {                             // Aguarda conexão com rede WiFi
    delay(100);
  }

  lcd.setCursor(0, 1);
  lcd.print("   CONECTADO    ");

  // Inicializa tarefa WebSocket
  xTaskCreatePinnedToCore(TaskWebSocket, "TaskWebSocket", 65535, NULL, 2, &TaskWebSocket_t, 1); //Task Function, Task name for humans, Stack size, ... , Priority, Task name, Core number

  delay(500);                                           // Delay para tarefa inicializar

  // Inicializa tarefa Ping WebSocket
  xTaskCreatePinnedToCore(TaskWebSocketPing, "TaskWebSocketPing", 65535, NULL, 0, &TaskWebSocketPing_t, 1); //Task Function, Task name for humans, Stack size, ... , Priority, Task name, Core number

  delay(500);                                           // Delay para tarefa inicializar

  lcd.clear();

  // Inicializa tarefa RFID
  xTaskCreatePinnedToCore(TaskRFID, "TaskRFID", 16384, NULL, 1, &TaskRFID_t, 0); //Task Function, Task name for humans, Stack size, ... , Priority, Task name, Core number

  delay(500);                                           // Delay para tarefa inicializar

}


/* ===============   ROTINA PRINCIPAL   ============== */
void loop() {
  delay(100);
  timer.run();                                          // polling do SimpleTimer

  /*
    struct tm
    {
    tm_sec;         // seconds,  range 0 to 59
    tm_min;         // minutes, range 0 to 59
    tm_hour;        // hours, range 0 to 23
    tm_mday;        // day of the month, range 1 to 31
    tm_mon;         // month, range 0 to 11
    tm_year;        // The number of years since 1900
    tm_wday;        // day of the week, range 0 to 6
    tm_yday;        // day in the year, range 0 to 365
    tm_isdst;       // daylight saving time
    };
  */

  time_t tt = time(NULL);                                                   // Obtem o tempo atual total em segundos (timestamp)
  data = *gmtime(&tt);                                                      // Converte o tempo atual e atribui na estrutura
  // USE_SERIAL.printf("Hora: %02d:%02d:%02d - Data: %02d/%02d/%04d Semana: %01d\n",  data.tm_hour, data.tm_min, data.tm_sec, data.tm_mday, data.tm_mon+1, data.tm_year+1900, data.tm_wday);//Mostra na Serial a data formatada

  if (data.tm_hour == 0 && data.tm_min == 0 && data.tm_sec == 0) {          // Atualiza a hora e Banco de Dados com servidor sempre que hora for 00:00:00
    UpdateTime = false;
    delay(1000);
  }
    

  if (Authenticated && WiFi_connected && WS_connected && !UpdateTime && !flagUpdateTime) {  // Pede a Hora para o servidor sempre que liga e está conectado
    WebSocket.send(("{ \"type\":\"HOUR\", \"id\":\"" + ID + "\", \"status\":\"hour\"}"));   // Solicita hora do servidor
    flagUpdateTime = true;
    timer.setTimeout(5000, ClearFlagUpdateTime);                            // Tempo para repetir o pedido caso não receba resposta - 5000ms
  }


  if (Authenticated && WiFi_connected) {                                    // Se está autenticado e conectado a rede WiFi
    
      xSemaphoreTake(SemaphoreDisplay, portMAX_DELAY);                      // Pega semaforo para manipular o Display

      lcd.setCursor(0, 0);
      RSSI = WiFi.RSSI();
      if (RSSI >= -50)                                                      // Mostra no LCD a intensidade do sinal WiFi
        lcd.write(4);
      else if (RSSI < -50 && RSSI >= -65)
        lcd.write(3);
      else if (RSSI < -65 && RSSI >= -80)
        lcd.write(2);
      else if (RSSI < -80)
        lcd.printf("_");
      //lcd.write(1);

      if (WS_connected)                                                     // Mostra no LCD se está conectado com Server - WebSocket
        lcd.write(5);
      else
        lcd.printf(" ");

      lcd.printf(" ");
      lcd.print(StatusClass());                                             // Mostra no LCD qual o Status da Sala [Livre, Reservada, ...]
      
      if (data.tm_sec % 2)                                                  // Mostra no LCD a hora
        lcd.printf(" %02d:%02d",  data.tm_hour, data.tm_min);
      else
        lcd.printf(" %02d %02d",  data.tm_hour, data.tm_min);

      if (ClassNow != ClassSchedule()) {                                    // Retaura o status da sala cada vez que troca de aula
        ClassNow = ClassSchedule();
        ReservedClass = false;
        InClass = false;
        BusyClass = false;
        scroll = 0;
        delay_scroll = 0;
      }
      
      lcd.setCursor(0, 1);
      String Class = ClassSchedule();                                       // Consultar Banco de Dados para retornar a aula atual
      if (Class.length() <= 16){                                            // Centraliza nome
        lcd.print("                ");
        lcd.setCursor(8-(Class.length()/2),1);
      } else {                                                              // Scroll no nome
        Class += "  |  ";
        if (scroll == Class.length())
          scroll = 0;
        String ClassTemp = "";
        for (int i = scroll; i < Class.length(); i++)
          ClassTemp += Class[i];
        for (int j = 0; j < scroll; j++)
          ClassTemp += Class[j];
        delay_scroll++;
        if (delay_scroll == 7) {
          scroll++;
          delay_scroll = 0;
        }
        Class = ClassTemp;                                
      }
      lcd.print(Class);
      
      xSemaphoreGive(SemaphoreDisplay);                                     // Libera semaforo


      if (New_tag) {                                                        // Se foi passado uma TAG no leitor
        if (!AuthorizedSchedule(nuidPICC)) {                                // Verifica se a TAG é Master ou Professor com aula agendada, no Banco de Dados local
            if (BusyClass || InClass) {                                     // Se a TAG não for Master ou Professor, então se o status for EM AULA ou OCUPADA, verifica a TAG no Banco de Dados do Server
              if (WS_connected) {                                           // Se esta conectado com o servidor 
                WebSocket.send(("{ \"type\":\"ACCESS\", \"id\":\"" + ID + "\", \"status\":\"ONLINE\", \"tag\":\"" + nuidPICC[0] + " " + nuidPICC[1] + " " + nuidPICC[2] + " " + nuidPICC[3] + "\", \"time\":\"" + int32_t(time(NULL)) + "\" }"));
              } else {
                AccessOffline(nuidPICC);                                    // Salva TAG
                Unlock("     ACESSO     ", "   AUTORIZADO   ");             // Abre a trava 
              }
            } else                                                          // Se a TAG não for Master ou Professor, então se o status NÃO for EM AULA ou OCUPADA
              unknown();                                                    // Nega o acesso
        }
      }
  }

  if (Authenticated && !WiFi_connected) {                                   // Se está autenticado e NÃO conectado a rede WiFi
      xSemaphoreTake(SemaphoreDisplay, portMAX_DELAY);                      // Pega semaforo para manipular o Display

      lcd.setCursor(0, 0);                                                  // Mostra no LCD sem sinal WiFi
      lcd.printf(" ");

      if (WS_connected)                                                     // Mostra no LCD se está conectado com Server - WebSocket
        lcd.write(5);
      else
        lcd.printf(" ");

      lcd.printf(" ");
      lcd.print(StatusClass());                                             // Mostra no LCD qual o Status da Sala [Livre, Reservada, ...]

      if (data.tm_sec % 2)                                                  // Mostra no LCD a hora
        lcd.printf(" %02d:%02d",  data.tm_hour, data.tm_min);
      else
        lcd.printf(" %02d %02d",  data.tm_hour, data.tm_min);

      if (ClassNow != ClassSchedule()) {                                    // Restaura o status da sala cada vez que troca de aula
        ClassNow = ClassSchedule();
        ReservedClass = false;
        InClass = false;
        BusyClass = false;
        scroll = 0;
        delay_scroll = 0;
      }

      lcd.setCursor(0, 1);
      String Class = ClassSchedule();                                       // Consultar Banco de Dados para retornar a aula atual
      if (Class.length() <= 16){                                            // Centraliza nome
        lcd.print("                ");
        lcd.setCursor(8-(Class.length()/2),1);
      } else {                                                              // Scroll no nome
        Class += "  |  ";
        if (scroll == Class.length())
          scroll = 0;
        String ClassTemp = "";
        for (int i = scroll; i < Class.length(); i++)
          ClassTemp += Class[i];
        for (int j = 0; j < scroll; j++)
          ClassTemp += Class[j];
        delay_scroll++;
        if (delay_scroll == 7) {
          scroll++;
          delay_scroll = 0;
        }
        Class = ClassTemp;                                
      }
      lcd.print(Class);

      xSemaphoreGive(SemaphoreDisplay);                                     // Libera semaforo

      if (New_tag) {                                                        // Se foi passado uma TAG no leitor
        if (!AuthorizedSchedule(nuidPICC)) {                                // Verifica se a TAG é Master ou Professor com aula agendada, no Banco de Dados local
            if (BusyClass || InClass) {                                     // Se a TAG não for Master ou Professor, então se o status for EM AULA ou OCUPADA, verifica a TAG no Banco de Dados do Server
              AccessOffline(nuidPICC);                                      // Salva TAG
              Unlock("     ACESSO     ", "   AUTORIZADO   ");               // Abre a trava 
            } else                                                          // Se a TAG não for Master ou Professor, então se o status NÃO for EM AULA ou OCUPADA
              unknown();                                                    // Nega o acesso
        }
      }
  }


  if (!Authenticated) {                                                     // Se não estiver Autenticado
    xSemaphoreTake(SemaphoreDisplay, portMAX_DELAY);                        // Pega semaforo para manipular o Display
    lcd.setCursor(0, 0);
    lcd.write(6);
    lcd.write(7);
    lcd.printf("   SCAF NAO   ");
    lcd.setCursor(0, 1);
    lcd.write(8);
    lcd.write(9);
    lcd.printf("  AUTORIZADO  ");
    New_tag = false;
    xSemaphoreGive(SemaphoreDisplay);                                       // Libera semaforo
  }


}


/* ===============   FUNCAO PEGAR ID   ============== */
String getID_String() {

  uint64_t chipid;
  String ID = "";

  chipid = ESP.getEfuseMac();                                               // A identificação do esp está associado ao endereço MAC (length: 6 bytes).

  if ((uint8_t)chipid < 10) ID = "0";
  ID += String(((uint8_t)chipid), HEX);
  if ((uint8_t)(chipid >> 8) < 16) ID += "0";
  ID += String(((uint8_t)(chipid >> 8)), HEX);
  if ((uint8_t)(chipid >> 16) < 16) ID += "0";
  ID += String(((uint8_t)(chipid >> 16)), HEX);
  if ((uint8_t)(chipid >> 24) < 16) ID += "0";
  ID += String(((uint8_t)(chipid >> 24)), HEX);
  if ((uint8_t)(chipid >> 32) < 16) ID += "0";
  ID += String(((uint8_t)(chipid >> 32)), HEX);
  if ((uint8_t)(chipid >> 40) < 16) ID += "0";
  ID += String(((uint8_t)(chipid >> 40)), HEX);
  ID.toUpperCase();

  return ID;
}


/* ===============   FUNCAO INICIAR SCAF   ============== */
void Init_SCAF() {
  lcd.clear();
  lcd.setCursor(0, 0);

  lcd.print("  SCAF  - UFSC  ");
  lcd.setCursor(0, 1);
  lcd.print("      " + firmware_v + "      ");

  delay(3000);

  ID = getID_String();

  lcd.setCursor(0, 1);
  lcd.print("ID: " + ID);

  delay(7000);
}


/* ===============   FUNCAO DESTRAVAR   ============== */
void Unlock(String Line1, String Line2) {
  
    xSemaphoreTake(SemaphoreDisplay, portMAX_DELAY);                        // Pega semaforo
    lcd.clear();
    lcd.setCursor(0, 0);

    if (Line1.length() <= 14)                                               // Centraliza nome
      lcd.setCursor(8-(Line1.length()/2),0);
      
    lcd.print(Line1);
    
    lcd.setCursor(0, 1);
    lcd.print(Line2);
    
    digitalWrite(TRAVA, LOW);
    delay(3000);
    digitalWrite(TRAVA, HIGH);
    New_tag = false;
    xSemaphoreGive(SemaphoreDisplay);                                       // Libera semaforo

}


/* ===============   FUNCAO DESCONHECIDO   ============== */
void unknown() {

    xSemaphoreTake(SemaphoreDisplay, portMAX_DELAY);                       // Pega semaforo
    lcd.clear();
    lcd.setCursor(0, 0);

    lcd.print("   ACESSO NAO   ");
    lcd.setCursor(0, 1);
    lcd.print("   AUTORIZADO   ");

    delay(3000);
    New_tag = false;
    xSemaphoreGive(SemaphoreDisplay);                                      // Libera semaforo

}


/* ===============     FUNCOES WEBSOCKET     ============== */
void onMessageCallback(WebsocketsMessage message) {

  deserializeJson(jsonBuffer, message.data());
  JsonObject MSG = jsonBuffer.as<JsonObject>();

  USE_SERIAL.print("<WebSocketEvent> Got Message: ");
  USE_SERIAL.println(message.data());

  if (MSG["type"] == "CONNECT") {                                          // Se recebeu mensagem do tipo CONNECT
    
    if (MSG["status"] == "OK") {                                           // Status OK, foi Autenticado 
      USE_SERIAL.println("<WebSocket> Accepted server connection");
      Authenticated = true;
      ValidateAccessOff();                                                 // Envia os acessos OFFLINE
    }

    if (MSG["status"] == "ERRO"){                                          // Status ERRO, não Autenticado
      USE_SERIAL.println("<WebSocket> Refused server connection");
      Authenticated = false;
    }

  }


  if (MSG["type"] == "HOUR") {                                            // Se recebeu mensagem do tipo HOUR
    
    hourUpdate(MSG["status"].as<uint32_t>()-1061);                        // Atualiza RTC

    USE_SERIAL.println("<WebSocket> Hour update");

    if (!UpdateTime) {                                                    // Solicita o Update do Banco de Dados apenas umas vez após atualizar a hora 
      SPIFFS.remove("/scheduleBAK.json");                                 // Remove o arquivo Backup
      WebSocket.send(("{ \"type\":\"UPDATE\", \"id\":\"" + ID + "\", \"status\":\"REFRESH\"}"));
    }

    UpdateTime = true;
  }


  if (MSG["type"] == "UPDATE") {                                          // Se recebeu mensagem do tipo UPDATE
    
    saveUpdate(MSG["status"]);                                            // Salva o conteudo no Banco de Dados local
    WebSocket.send(("{ \"type\":\"UPDATE\", \"id\":\"" + ID + "\", \"status\":\"OK\"}")); // Envia confirmação de recebimento
    if (MSG["new_line"] == 0)                                             // Se não tiver mais dados para receber
      loadUpdate();                                                       // Atualiza o Banco de Dados local
  }


  if (MSG["type"] == "ACCESS") {                                          // Se recebeu mensagem do tipo ACCESS
    
    if (MSG["status"] == "null")                                          // Se status é null, não há registro
      unknown();                                                          // Nega acesso
    else if (MSG["status"] == "OK") {                                     // Se status é OK, acesso foi registrado
      ValidateAccessOff();                                                // Envia próximo acesso OFFLINE, se existir
    }
    else                                                                  // Se não, a TAG é registrada no Server
      Unlock(MSG["status"], "   AUTORIZADO   ");                          // Destrava e mostra o nome no Display
  }
  
}


void onEventsCallback(WebsocketsEvent event, String data) {
  if (event == WebsocketsEvent::ConnectionOpened) {
    USE_SERIAL.println("<WebSocketEvent> Connnection Opened");
  } else if (event == WebsocketsEvent::ConnectionClosed) {
    USE_SERIAL.println("<WebSocketEvent> Connnection Closed");
  } else if (event == WebsocketsEvent::GotPing) {
    USE_SERIAL.println("<WebSocketEvent> Got a Ping!");
  } else if (event == WebsocketsEvent::GotPong) {
    USE_SERIAL.println("<WebSocketEvent> Got a Pong!");
    WS_ping = false;
  }
}


/* ===============     TAREFA WIFI     ============== */

void TaskWiFi( void *parameter ) {

  (void) parameter;

  bool flagWiFi_connected = WiFi_connected;

  WiFi.mode(WIFI_STA); // STA (conecta-se em alguem), AP (é conectado por alguem)

  for (;;) {
    WiFi.begin(ssid, password);
    for (int i = 0; i < 10; i++) {

      if (WiFi_connected != flagWiFi_connected) {
        flagWiFi_connected = WiFi_connected;
        USE_SERIAL.println("<WiFi> Disconnected");
      }

      if (WiFi.status() != WL_CONNECTED) { // WL_CONNECTED é um define de WiFi.h
        WiFi_connected = false;
        vTaskDelay(500);
      } else {

        WiFi_connected = true;
        while (WiFi.status() == WL_CONNECTED) {
          if (WiFi_connected != flagWiFi_connected) {
            flagWiFi_connected = WiFi_connected;
            USE_SERIAL.println("<WiFi> Connected");
          }
          vTaskDelay(500);
        }
      }
    }
  }
}


/* ===============   TAREFA WEBSOCKET   ============== */
void TaskWebSocket( void *parameter ) {

  (void) parameter;

  bool flagWS_connected = WS_connected;

  //preencha com o IP do servidor
  WS_connected = WebSocket.connect(websocketIP, websocketPORT, "/");

  if (WS_connected) {
    // Send a CONNETC
    WebSocket.send(("{ \"type\":\"CONNECT\", \"id\":\"" + ID + "\", \"status\":\"Connect\"}"));
  }

  vTaskDelay(10);

  for (;;) {

    if (WebSocket.available()) {
      WebSocket.poll();
      WS_connected = true;

    } else {
      WS_connected = false;
      WebSocket.close();
       vTaskDelay(100);

      if (WiFi_connected) {
        WebSocket = WebsocketsClient();
        WebSocket.onMessage(onMessageCallback);               // Instanciando as mensagens de WebSocket
        WebSocket.onEvent(onEventsCallback);                  // Instanciando os eventos de WebSocket
        WS_connected = WebSocket.connect(websocketIP, websocketPORT, "/");
         vTaskDelay(100);
         
        if (WS_connected) {
          // Send a CONNETC
          WebSocket.send(("{ \"type\":\"CONNECT\", \"id\":\"" + ID + "\", \"status\":\"Connect\"}"));
        }
      }
    }

    if (WS_connected != flagWS_connected) {
      flagWS_connected = WS_connected;
      if (WS_connected)
        USE_SERIAL.println("<WebSocket> Connected");
      else
        USE_SERIAL.println("<WebSocket> Disconnected");
    }


    vTaskDelay(10);
  }
}


/* ===============   TAREFA WEBSOCKET PING   ============== */
void TaskWebSocketPing( void *parameter ) {

  (void) parameter;

  for (;;) {

    if (WS_connected && !WS_ping) {
      WS_ping = true;
      WebSocket.ping();
      vTaskDelay(30000);
      if (WS_ping) {
        WS_ping = false;
        WebSocket.close();
        USE_SERIAL.println("<WebSocket> Fail server ping");
      }
    }

    vTaskDelay(100);
  }
}


/* ===============     TAREFA RFID     ============== */
void TaskRFID( void *parameter ) {

  (void) parameter;

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  for (;;) {
    vTaskDelay(100);
    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if ( ! rfid.PICC_IsNewCardPresent())
      continue;

    // Verify if the NUID has been readed
    if ( ! rfid.PICC_ReadCardSerial())
      continue;

    //USE_SERIAL.print(F("<RFID> PICC type: "));
    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
    //USE_SERIAL.println(rfid.PICC_GetTypeName(piccType));

    // Check is the PICC of Classic MIFARE type
    if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
        piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
        piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
      USE_SERIAL.println(F("<RFID> Your tag is not of type MIFARE Classic."));
      continue;
    }

    //if (rfid.uid.uidByte[0] != nuidPICC[0] ||
    //    rfid.uid.uidByte[1] != nuidPICC[1] ||
    //    rfid.uid.uidByte[2] != nuidPICC[2] ||
    //    rfid.uid.uidByte[3] != nuidPICC[3] ) {
    if (!New_tag) {
      USE_SERIAL.print(F("<RFID> A card has been detected: "));

      // Store NUID into nuidPICC array
      for (byte i = 0; i < 4; i++) {
        nuidPICC[i] = rfid.uid.uidByte[i];
        USE_SERIAL.printf(" %02X", nuidPICC[i]);
        //lcd.printf("%02X ",nuidPICC[i]);
      }
	  New_tag = true;
      USE_SERIAL.print("\n");
    }
    //}
    //else USE_SERIAL.println(F("<RFID> Card read previously."));

    // Halt PICC
    rfid.PICC_HaltA();

    // Stop encryption on PCD
    rfid.PCD_StopCrypto1();
  }
}


/* ===============   FUNCAO LOAD CONFIG   ============== */
bool loadConfig() {
  File configFile = SPIFFS.open("/config.json", FILE_READ);

  if (!configFile)
    return false;

  StaticJsonDocument<256> doc;

  DeserializationError error = deserializeJson(doc, configFile);
  if (error)
    return false;

  JsonObject json = doc.as<JsonObject>();

  strcpy(ssid, json["servername"]);
  strcpy(password, json["password"]);
  strcpy(websocketIP, json["websocketip"]);
  websocketPORT = json["websocketport"].as<unsigned int>();

  configFile.close();

  return true;
}


/* ===============   FUNCAO SAVE UPDATE   ============== */
bool saveUpdate(String lines) {
  File updateFile = SPIFFS.open("/scheduleBAK.json", FILE_APPEND);

  if (!updateFile) {
    USE_SERIAL.printf("Arquivo nao existe");
    return false;
  }

  updateFile.println(lines);

  updateFile.close();

  return true;
}


/* ===============   FUNCAO LOAD UPDATE   ============== */
bool loadUpdate() {

  if (!SPIFFS.remove("/schedule.json"))
    USE_SERIAL.println("Delete failed");

  if (!SPIFFS.rename("/scheduleBAK.json", "/schedule.json"))
    USE_SERIAL.println("Rename failed");

  File updateFile = SPIFFS.open("/schedule.json", FILE_READ);

  if (!updateFile)
    USE_SERIAL.printf("Arquivo nao existe leitura");

  while (updateFile.available())
    USE_SERIAL.println("<Schedule> " + updateFile.readStringUntil('\n'));

  updateFile.close();

  return true;
}


/* ===============   FUNCAO HOUR UPDATE   ============== */
void hourUpdate(uint32_t TimeStamp) {
  timeval tv;              //Cria a estrutura temporaria para funcao abaixo. (struct timeval {time_t tv_sec; suseconds_t tv_usec;})
  tv.tv_sec = TimeStamp;   //Atribui data atual em TimeStamp
  settimeofday(&tv, NULL); //Configura o RTC para a data atribuida.
}


/* ===============   FUNCAO CLEAR FLAG UPDATE TIME   ============== */
void ClearFlagUpdateTime() {
  flagUpdateTime = false;
}


/* ===============   FUNCAO DEVOLVE A AULA ATUAL   ============== */
String ClassSchedule(){    

  String Class = "  NAO  ALOCADA  ";
  String timer;
  char timerNow[9];

  File schedule = SPIFFS.open("/schedule.json", FILE_READ);

  if (!schedule)
    return Class;

  while (schedule.available()) {

    ReservedClass = false;
    
    StaticJsonDocument<256> doc;

    DeserializationError error = deserializeJson(doc, schedule.readStringUntil('\n'));
    if (error){
      schedule.close();
      return Class;
    }

    JsonObject json = doc.as<JsonObject>();

    if ((json["wd"] == data.tm_wday) && (json["wd"].as<int>() != -1)){   // data.tm_wday = (0 Domingo; 1 Segundo; 2 Terça; 3 Quarta; 4 Quinta; 5 Sexta; 6 Sabado)

      timer = (data.tm_hour > 9 ? String(data.tm_hour) : "0" + String(data.tm_hour)) +":"+
              (data.tm_min > 9 ? String(data.tm_min) : "0" + String(data.tm_min)) +":"+
              (data.tm_sec > 9 ? String(data.tm_sec) : "0" + String(data.tm_sec));

      timer.toCharArray(timerNow, 9); 
      
      if ((strcmp(timerNow, json["inicio"]) >= 0) && (strcmp(timerNow, json["fim"]) < 0)){
        if (json["materia"].as<String>() != "null"){
          Class = json["materia"].as<String>();
          ReservedClass = true; // Se tem alguma aula neste horário, então o status é RESERVADO, até que o prof. altere para EM AULA
          break;
        }
      }
    }
  }
  
  schedule.close();

  return Class;
}


/* ===============   FUNCAO RETORNA O STATUS ATUAL DA SALA   ============== */
String StatusClass(){ 

  String Status = " LIVRE ";

  if (ReservedClass) Status = "RESERV.";
  if (InClass) Status = "EM AULA"; 
  if (BusyClass) Status = "OCUPA. ";

  return Status;
}


/* ===============   FUNCAO VERIFICA SE A TAG ESTA NA AGENDA DE HORARIOS   ============== */
bool AuthorizedSchedule(byte idTag[4]){

  char NumberTag[12];
  String Tag;
  bool result = false;

  sprintf(NumberTag,"%02X %02X %02X %02X", idTag[0],idTag[1],idTag[2],idTag[3]);

  Tag = String(NumberTag);

  File schedule = SPIFFS.open("/schedule.json", FILE_READ);

  if (!schedule)
    return result;

  while (schedule.available()) {
    
    StaticJsonDocument<256> doc;

    DeserializationError error = deserializeJson(doc, schedule.readStringUntil('\n'));
    if (error){
      schedule.close();
      return result;
    }

    JsonObject json = doc.as<JsonObject>();

    if (json["authorized_tag"].as<String>() == Tag){

      if (json["materia"].as<String>() == "null") { // Quando a TAG é master
        
        if (ReservedClass)  // Se a sala tem aula agendada naquele horario
          BusyClass = true;
        
        if (WS_connected)  
          WebSocket.send(("{ \"type\":\"ACCESS\", \"id\":\"" + ID + "\", \"status\":\"OFFLINE\", \"tag\":\"" + idTag[0] + " " + idTag[1] + " " + idTag[2] + " " + idTag[3] + "\", \"time\":\"" + int32_t(time(NULL)) + "\" }"));
        else
          AccessOffline(idTag);
        Unlock(json["authorized_name"], "   AUTORIZADO   ");
        result = true;
        break;

      } else { // Quando a TAG não é master, mas pode ser um professor 
      
        if (json["materia"].as<String>() == ClassSchedule()){ // Se a aula é desse professor
          BusyClass = false;
          InClass = true;
          if (WS_connected)
            WebSocket.send(("{ \"type\":\"ACCESS\", \"id\":\"" + ID + "\", \"status\":\"OFFLINE\", \"tag\":\"" + idTag[0] + " " + idTag[1] + " " + idTag[2] + " " + idTag[3] + "\", \"time\":\"" + int32_t(time(NULL)) + "\" }"));
          else
            AccessOffline(idTag);  
          Unlock(json["authorized_name"], "   AUTORIZADO   ");
          result = true;
          break;
        }
      }
    }
  }

  schedule.close();

  return result;
  
}


/* ===============   FUNCAO SALVA ACESSO OFFLINE   ============== */
void AccessOffline(byte idTag[4]){    

  File accessOff = SPIFFS.open("/accessOff.json", FILE_APPEND);

  if (!accessOff) {
    USE_SERIAL.printf("<AccessOffline> Arquivo nao existe");
    return;
  }

  accessOff.println("{ \"type\":\"ACCESS\", \"id\":\"" + ID + "\", \"status\":\"OFFLINE\", \"tag\":\"" + idTag[0] + " " + idTag[1] + " " + idTag[2] + " " + idTag[3] + "\", \"time\":\"" + int32_t(time(NULL)) + "\" }");

  USE_SERIAL.print("<ValidateAccessOff> File size increment: ");
  USE_SERIAL.print(accessOff.size());
  USE_SERIAL.println("bytes");

  accessOff.close();
}


/* ===============   FUNCAO ENVIA ACESSOS OFFLINE PARA O SERVER   ============== */
void ValidateAccessOff() {

  String line;
  uint16_t c_lines = 0;

  if (!SPIFFS.exists("/accessOff.json")) {
    USE_SERIAL.println("<ValidateAccessOff> File not exist");
    return;
  }

  if (!SPIFFS.rename("/accessOff.json", "/accessOffBAK.json")) {
    USE_SERIAL.println("<ValidateAccessOff> Rename failed");
    return;
  }

  File accessOffBAK = SPIFFS.open("/accessOffBAK.json", FILE_READ);

  if (!accessOffBAK) {
    USE_SERIAL.printf("<ValidateAccessOff> File not exist");
    return;
  }

  while (accessOffBAK.available()) {
    line = accessOffBAK.readStringUntil('\n');
    c_lines++;
  }

  if (c_lines == 0) {
    accessOffBAK.close();
    if (!SPIFFS.remove("/accessOffBAK.json"))
      USE_SERIAL.println("<ValidateAccessOff> Delete failed");
    USE_SERIAL.println("<ValidateAccessOff> File size empty");
    return;
  }

  accessOffBAK.seek(0, SeekSet); // *** rewind 

  File accessOff = SPIFFS.open("/accessOff.json", FILE_APPEND);

  if (!accessOff) {
    USE_SERIAL.printf("<ValidateAccessOff> File not exist");
    accessOffBAK.close();
    return;
  }

  while (--c_lines)
    accessOff.println(accessOffBAK.readStringUntil('\n')); // copia, menos a ultima linha

  accessOff.close();
  
  if (WS_connected){
    USE_SERIAL.print("<ValidateAccessOff> File size decrement: ");
    USE_SERIAL.print(accessOffBAK.size());
    USE_SERIAL.println("bytes");
    WebSocket.send((line));
    accessOffBAK.close();
    if (!SPIFFS.remove("/accessOffBAK.json"))
      USE_SERIAL.println("<ValidateAccessOff> Delete failed");

  } else {

    accessOffBAK.close();
    if (!SPIFFS.remove("/accessOff.json"))
      USE_SERIAL.println("<ValidateAccessOff> Delete failed");

    if (!SPIFFS.rename("/accessOffBAK.json", "/accessOff.json"))
      USE_SERIAL.println("<ValidateAccessOff> Rename failed");
    
  }
}
