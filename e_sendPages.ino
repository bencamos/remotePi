void sendFile(String requested) {
        digitalWrite(LED1, LOW);
        if (SPIFFS.exists(requested)) {
                File f = SPIFFS.open(requested, "r");
                if (f && f.size()) {
                        String appfile;
                        while (f.available()) {
                                appfile += char(f.read());
                        }
                        server.send(200, texttype, appfile);
                        f.close();
                }
        } else {
                String fileList;
                Dir dir = SPIFFS.openDir("");
                while (dir.next()) {
                        fileList += dir.fileName() + " - " + dir.fileSize() + " bytes.<br>";
                }
                server.send(404, texttype, requested + " Dosen't exist.<br>SPIFFS Reports: " + SPIFFS.exists(requested) + "<br>Files:<br>" + fileList);
        }
        digitalWrite(LED1, HIGH);
}
void sendText(String text) {
        server.send(200, texttype, text);
}
void redirect(String text) {
        server.send(200, texttype, "<meta http-equiv='refresh' content='0.25; URL=/' />\n" + text);
}