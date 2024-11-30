#include <WiFi.h>
#include <HTTPClient.h>

#include <UrlEncode.h>

// Defina os dados da rede Wi-Fi
const char* ssid = "<SSID-WIFI>";
const char* password = "<SENHA-WIFI>";

const String API_URL = "https://api.callmebot.com/whatsapp.php";
const String API_KEY = "<API-KEY>";
const String TELEFONE = "<TELEFONE-CLIENTE>";

const int BOTAO_PIN = 21;
const int LED_PIN = 23;

const int PIEZO_PIN = 22;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  Serial.print("Conectando-se ao Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Conectado ao Wi-Fi");

  pinMode(BOTAO_PIN, INPUT_PULLUP);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(PIEZO_PIN, OUTPUT);

}

bool enviar_mensagem(const char *mensagem) {
  HTTPClient http;

  String requisicaoUrl = API_URL + "?phone=" + TELEFONE + "&text=" + urlEncode(mensagem) + "&apikey=" + API_KEY;
  Serial.println("requisicao para:");
  Serial.println(requisicaoUrl);
  
  http.begin(requisicaoUrl);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.addHeader("Accept", "text/html");
  
  int respostaHttp = http.POST(requisicaoUrl);
  
  Serial.print("INFO: código de resposta do servidor: ");
  Serial.println(respostaHttp);

  bool sucesso = respostaHttp == 200;
  http.end();

  return sucesso;
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Erro na conexão Wi-Fi");
  }

  if (digitalRead(BOTAO_PIN) == LOW) {
    Serial.println("INFO: botão apertado");

    tone(PIEZO_PIN, 440);
    digitalWrite(LED_PIN, HIGH);
    
    if (!enviar_mensagem("Socorro!")) {
      Serial.println("ERRO: falha ao enviar mensagem");
    }

    noTone(PIEZO_PIN);
    digitalWrite(LED_PIN, LOW);
  }
}
