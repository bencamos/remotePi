void loadPage() {
        digitalWrite(LED1, LOW);
        String requested = server.uri();
        File f = SPIFFS.open("isencrypted", "r");
        if (f && f.size()) {
                // This was intended to encrypt all data on the esp to prevent reading but was never completed.
                String appfile;
                while (f.available()) {
                        appfile += char(f.read());
                }
                if (appfile == "1") {
                        if (requested.equals("/decrypt")) {
                                String encrypted = "sdWH1hvWDcLbNNGj4/jm4YAHfsUt1p73UtvH4qlpRu4=";
                                server.send(200, texttype, "<meta http-equiv='refresh' content='0.25; URL=/files.html' />\nAttempting to decrypt with pass: " + server.arg("pass") + " Decrypted: ");
                        } else {
                                sendFile("/decrypt.html");
                        }
                }
        }
        f.close();
        if (requested.equals("/") || requested.equals("") ) {
                sendFile("/index.html");
        } else if (requested.equals("/isencrypted")) {
                server.send(404, "text/css", "i wonder what this is for ;)");
        } else if (requested.equals("/index.css")) {
                File f = SPIFFS.open(requested, "r");
                if (f && f.size()) {
                        String appfile;
                        while (f.available()) {
                                appfile += char(f.read());
                        }
                        server.send(200, "text/css", appfile);
                        f.close();
                }
        } else if (requested.equals("/reboot")) {
                if (client.connect("192.168.4.2", 48567)) {
                        redirect("<h3 style='color:#8502f7;'>Reboot request sent succesfully! </h3>");
                        client.stop();
                } else {
                        redirect("<h3 style='color:#8502f7;'>Connection Failed. </h3>");
                }
        } else if (requested.equals("/temp")) {
                if (client.connect("192.168.4.2", 1)) {
                        client.print("request\n");
                        String response = client.readStringUntil('\n');
                        sendText("<h3 style='color:#8502f7;'>" + response + " </h3>");
                        client.stop();
                } else {
                        sendText("<h3 style='color:#8502f7;'>Connection Failed. </h3>");
                }
        } else if (requested.equals("/ping")) {
                // "192.168.4.2" Is most likely the rasp Pi
                IPAddress ip (192, 168, 4, 2);
                bool ret = Ping.ping(ip);
                if (ret) {
                        sendText("<h3 style='color:#8502f7;'>Online </h3>");
                } else {
                        sendText("<h3 style='color:#8502f7;'>Offline </h3>");
                }
        } else if (requested.equals("/fileWrite")) {
                // Write file to disk
                Serial.print(server.arg("file") + server.arg("data"));
                SPIFFS.remove(server.arg("file"));
                File file = SPIFFS.open(server.arg("file"), "w");
                int bytesWritten = file.print(server.arg("data"));
                server.send(200, texttype, "<meta http-equiv='refresh' content='0.25; URL=/files' />\nAttempting to process your request: " + server.arg("file") + server.arg("data"));
        } else if (requested.equals("/fileeditor")) {
                // Interactive file editor
                String requested = server.arg("file");
                if (SPIFFS.exists(requested)) {
                        File f = SPIFFS.open(requested, "r");
                        if (f && f.size()) {
                                String appfile;
                                while (f.available()) {
                                        appfile += char(f.read());
                                }
                                String page = "<form action='/fileWrite' id='file'>";
                                page += "<textarea type='text' id='data' rows='25' cols='25' name='data' form='file'>";
                                page += appfile;
                                page += " </textarea>";
                                page += "File: <input type='text' value='" + requested + "' name='file'>";
                                page += " <input type='submit' value='Save'>";
                                page += "</form>";
                                server.send(200, "text/html", "<h3 style='color:#8502f7;'>" + page + "</h3>");
                                f.close();
                        }
                } else {
                        String fileList;
                        Dir dir = SPIFFS.openDir("");
                        while (dir.next()) {
                                fileList += dir.fileName() + " - " + dir.fileSize() + " bytes.<br>";
                        }
                        server.send(404, texttype, "<h3 style='color:#8502f7;'>" + requested + " Dosen't exist.<br>SPIFFS Reports: " + SPIFFS.exists(requested) + "<br>Files:<br>" + fileList + "</h3>");
                }
        } else if (requested.equals("/fileviewer")) {
                // Interactive file viewer
                String requested = server.arg("file");
                if (SPIFFS.exists(requested)) {
                        File f = SPIFFS.open(requested, "r");
                        if (f && f.size()) {
                                String appfile;
                                while (f.available()) {
                                        appfile += char(f.read());
                                }
                                server.send(200, "text/html", "<a href='/fileeditor?file=" + requested + "'> <button>Edit File</button></a>\n<h3 style='color:#8502f7;'>" + appfile + "</h3>");
                                f.close();
                        }
                } else {
                        String fileList;
                        Dir dir = SPIFFS.openDir("");
                        while (dir.next()) {
                                fileList += dir.fileName() + " - " + dir.fileSize() + " bytes.<br>";
                        }
                        server.send(404, texttype, "<h3 style='color:#8502f7;'>" + requested + " Dosen't exist.<br>SPIFFS Reports: " + SPIFFS.exists(requested) + "<br>Files:<br>" + fileList + "</h3>");
                }
        } else if (requested.equals("/files")) {
                // Listing files
                String fileList;
                float totalFileSize;
                Dir dir = SPIFFS.openDir("");
                while (dir.next()) {
                        fileList += "<a style='color:#8502f7;' href=/fileviewer?file=" + dir.fileName() + ">" + dir.fileName() + "</a> - " + dir.fileSize() + " bytes.<br>";
                        totalFileSize = totalFileSize + dir.fileSize();
                }
                totalFileSize = totalFileSize / 1000;
                sendText("<h3 style='color:#8502f7;'>" + fileList + "\n\n\r\rTotal Used: " + totalFileSize + " / 3000 KB</h3>");
        } else if (requested.equals("/delete")) {
                SPIFFS.remove(server.arg("File"));
                server.send(200, texttype, "<meta http-equiv='refresh' content='0.25; URL=/files.html' />\nAttempting to delete:" + server.arg("File"));
        } else if (requested.equals("/executeCommand")) {
          if (client.connect("192.168.4.2", 48567)) {
            // Connecting to rasp Pi socket and sending the "command"
            client.print(server.arg("command"));
            String response = client.readString();
            sendText("<h3 style='color:#8502f7;'>" + response + " </h3>");
            client.stop();
          } else {
                  sendText("<h3 style='color:#8502f7;'>Connection Failed. </h3>");
          }
        } else if (requested.equals("/setnetwork")) {
                // Setting new network details
                SPIFFS.remove(server.arg("/hostNetwork.config"));
                File file = SPIFFS.open("/hostNetwork.config", "w");
                int bytesWritten = file.print(server.arg("ssid") + ":" + server.arg("pass"));
                server.send(200, texttype, "<meta http-equiv='refresh' content='0.25; URL=/files.html' />\nAttempting to set network credentials:" + server.arg("ssid") + ":" + server.arg("pass"));
        } else {
                sendFile(requested);
        }
        digitalWrite(LED1, HIGH);
}
