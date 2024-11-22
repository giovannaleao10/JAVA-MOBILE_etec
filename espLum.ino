#include <ESP8266WiFi.h>

 

const char* ssid = "001002003002001";
const char* password = "LatinhaDeCoca0800";

 const int lightPin = A0; // Pino analógico onde o sensor de luminosidade está conectado

 

WiFiServer server(80);

 

const char* htmlContent = R"(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Luminosidade</title>
<style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      padding: 20px;
    }

    h1 {
      font-size: 24px;
    }

    p {
      font-size: 18px;
    }
</style>
</head>
<body>
<h1>Luminosidade</h1>
<p><strong>Luminosidade:</strong> <span id="light">--</span> lux</p>

<script>
    function updateData() {
      fetch("/")
        .then(response => response.text())
        .then(data => {
          const match = data.match(/Luminosidade: ([\d.]+)/);
          if (match) {
            const lightValue = parseFloat(match[1]);
            document.getElementById("light").textContent = lightValue.toFixed(2) + " lux";
          }
        })
        .catch(error => {
          console.error("Erro ao buscar dados:", error);
        });
    }

 

    setInterval(updateData, 5000);
    updateData();
</script>
</body>
</html>
)";

 

void setup(void) {
  Serial.begin(9600);
  delay(10);

 

  // Conectar ao WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");

 

  // Iniciar o servidor
  server.begin();
  Serial.println("Servidor iniciado");

 

  Serial.print("Use este URL para se conectar: http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

 

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

 

  while (!client.available()) {
    delay(1);
  }

 

  String request = client.readStringUntil('\r');
  client.flush();

 

  int lightValue = analogRead(lightPin); // Ler o valor da luminosidade

 

  // Converter valor analógico em lux (ajuste esses valores de acordo com o seu sensor)
  float luxValue = map(lightValue, 0, 1023, 0, 1000); // Exemplo de mapeamento

 

  String response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  response += htmlContent;

 

  response.replace("--", String(lightValue)); // Substituir "--" pelo valor da luminosidade em lux

 

  client.print(response);
  delay(1);
  client.stop();
}
