String listDir(fs::FS &fs, const char * dirname, uint8_t levels)
{
  filesDropdownOptions = "";
  String listenFiles = "<table><tr><th id=\"first_td_th\">List the library: </th><th>";
  listenFiles += dirname;
  listenFiles += "</th></tr>";

  File root = fs.open(dirname);
  String fail = "";
  if(!root)
  {
    fail = " the library cannot be opened";
    return fail;
  }
  if(!root.isDirectory())
  {
    fail = " this is not a library";
    return fail;
  }

  File file = root.openNextFile();
  while(file)
  {
    if(file.isDirectory())
    {
      listenFiles += "<tr><td id=\"first_td_th\">Library: ";
      listenFiles += file.name();

      filesDropdownOptions += "<option value=\"";
      filesDropdownOptions += file.name();
      filesDropdownOptions += "\">";
      filesDropdownOptions += file.name();
      filesDropdownOptions += "</option>";

      listenFiles += "</td><td> - </td></tr>";

      if(levels)
      {
        listDir(fs, file.name(), levels -1);
      }
    }
    else
    {
      listenFiles += "<tr><td id=\"first_td_th\">File: ";
      listenFiles += file.name();

      filesDropdownOptions += "<option value=\"";
      filesDropdownOptions += file.name();
      filesDropdownOptions += "\">";
      filesDropdownOptions += file.name();
      filesDropdownOptions += "</option>";

      listenFiles += " </td><td>\tSize: ";
      listenFiles += convertFileSize(file.size());
      listenFiles += "</td></tr>";
    }
    file = root.openNextFile();
  }
  listenFiles += "</table>";
  return listenFiles;  
}

String readFile(fs::FS &fs, const char * path)
{
  String fileContent = "";
  File file = fs.open(path, "r");
  if(!file || file.isDirectory()) { return fileContent; }
  while(file.available()) { fileContent+=String((char)file.read()); }
  file.close();
  return fileContent;
}

void writeFile(fs::FS &fs, const char * path, const char * message)
{
  File file = fs.open(path, "w");
  if(!file) { return; }
  file.print(message);
  file.close();
}

void uploadFile(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) 
{
  if(!index) { request->_tempFile = SPIFFS.open("/" + filename, "w"); }
  if(len) { request->_tempFile.write(data, len); }
  if(final)
  {
    request->_tempFile.close();
    request->redirect("/manager");
  }
}

String convertFileSize(const size_t bytes)
{
  if(bytes < 1024)  { return String(bytes) + " B"; }
  else if (bytes < 1048576) { return String(bytes / 1024.0) + " kB"; }
  else if (bytes < 1073741824) { return String(bytes / 1048576.0) + " MB"; } 
}