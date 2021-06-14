void setup() {
        pinMode(LED1, OUTPUT);
        pinMode(LED2, OUTPUT);
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, HIGH);
        Serial.begin(9600);
        Serial.print("Welcome to debug!\n");
        if(!SPIFFS.begin()) {
                Serial.println("An Error has occurred while mounting SPIFFS");
                return;
        }
        MDNS.addService("http", "tcp", 80);
        if (SPIFFS.exists("/hostNetwork.config")) {
                // Create a new network using the details inside "hostNetwork.config"
                // The user connects to this network to properly access the server.
                Serial.println("Primary network configuration file detected.\n");
                File f = SPIFFS.open("/hostNetwork.config", "r");
                if (f && f.size()) {
                        String appfile1;
                        while (f.available()) {
                                appfile1 += char(f.read());
                        }
                        int str_len = appfile1.length() + 1;
                        char appfile[str_len];
                        appfile1.toCharArray(appfile, str_len);
                        int ex = 0;
                        String tmpbuf = strtok(appfile, ":");
                        String tempstor1 = tmpbuf;
                        String tempstor2 = strtok(NULL, ":");
                        Serial.println("SSID: " + tempstor1 + " Password: " + tempstor2);
                        WiFi.softAP(tempstor1, tempstor2);
                }
        }
        if (SPIFFS.exists("/clientNetwork.config")) {
                // Used to connect to a second network usually to get internet connection but will work the same in the network as if it was its own.
                Serial.println("Secondary network configuration file detected.\n");
                File f = SPIFFS.open("/clientNetwork.config", "r");
                if (f && f.size()) {
                        String appfile2;
                        while (f.available()) {
                                appfile2 += char(f.read());
                        }
                        Serial.println("Available networks given: " + appfile2 + "\nTrying them now...\n");
                        int str_len = appfile2.length() + 1;
                        char appfile[str_len];
                        appfile2.toCharArray(appfile, str_len);
                        int ex = 0;
                        String tempstor1;
                        String tempstor2;
                        String tmpbuf = strtok(appfile, ":");
                        while (tmpbuf) {
                                if (ex = 2) {
                                        WiFi.begin(tempstor1, tempstor2);
                                        delay(3000);
                                        if (WiFi.status() == WL_CONNECTED) {
                                                Serial.print("Connected, IP address: ");
                                                Serial.println(WiFi.localIP());
                                                WiFi.hostname("hacker.pi");
                                                break;
                                        }
                                        int ex = 0;
                                }
                                String tempstor1 = tmpbuf;
                                strtok(NULL, ":");
                                String tempstor2 = tmpbuf;
                         ex++;
                        }
                }
        }
        if (!MDNS.begin("hackerpi")) {
          Serial.println("Error setting up DNS responder!");
          while (1) {
            delay(1000);
          }
        } else {
          Serial.println("DNS responder started");
        }
        //Little bitch boy dosent let you accept all so i had to hijack the 404.
        server.onNotFound(loadPage);
        //File uploading
        server.on("/upload", HTTP_POST,
                  [](){
                server.send(200);
        },
                  handleFileUpload
                  );
        server.begin();
}
