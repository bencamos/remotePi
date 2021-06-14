float getRemainingSpace() {
  Dir dir = SPIFFS.openDir("");
  String fileList;
  float totalFileSize;
  while (dir.next()) {
          fileList += dir.fileName() + " - " + dir.fileSize() + " bytes.<br>";
          float totalFileSize = totalFileSize + dir.fileSize();
  }
  totalFileSize = totalFileSize / 1000;
  float left = fileSizeCap - totalFileSize;
  return left;
}

float checkLimitMaxed(float incoming) {
  Dir dir = SPIFFS.openDir("");
  String fileList;
  float totalFileSize;
  while (dir.next()) {
          fileList += dir.fileName() + " - " + dir.fileSize() + " bytes.<br>";
          totalFileSize = totalFileSize + dir.fileSize();
  }
  float totalFileSizeWithout = totalFileSize / 1000;
  float totalAddition = totalFileSizeWithout + incoming;
  if (totalAddition > fileSizeCap) {
    return true;
  } else {
    return false;
  }
}

void handleFileUpload(){
        HTTPUpload& upload = server.upload();
        if(upload.status == UPLOAD_FILE_START) {
                String filename = upload.filename;
                if(!filename.startsWith("/")) filename = "/"+filename;
                Serial.print("handleFileUpload Name: "); Serial.println(filename);
                fsUploadFile = SPIFFS.open(filename, "w");
                filename = String();
        } else if(upload.status == UPLOAD_FILE_WRITE) {
                if(fsUploadFile)
                        fsUploadFile.write(upload.buf, upload.currentSize);
        } else if(upload.status == UPLOAD_FILE_END) {
                if(fsUploadFile) {
                        fsUploadFile.close();
                        Serial.print("handleFileUpload Size: "); Serial.println(upload.totalSize);
                        server.sendHeader("Location","/files.html");
                        server.send(303);
                } else {
                        server.send(500, "text/plain", "500: couldn't create file");
                }
        }
}
void handleFileDeletion(){
        HTTPUpload& upload = server.upload();
        String filename = upload.filename;
        if(!filename.startsWith("/")) filename = "/"+filename;
        Serial.print("Deletion Request: "); Serial.println(filename);
        SPIFFS.remove(filename);
        server.sendHeader("Location","/files.html");
}