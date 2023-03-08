// A develloper
// faire la calibration des capteur de poid et l'archiver en EEProm
// 

void setupEEPROM() 
{
  Serial.println(F("Initialize System"));
  //Init EEPROM
  EEPROM.begin(EEPROM_SIZE);

  //Write data into eeprom
  int address = 0;
  int boardId = 18;
  EEPROM.write(address, boardId);//EEPROM.put(address, boardId);
  address += sizeof(boardId);    //update address value

  float param = 26.5;
  EEPROM.writeFloat(address, param);//EEPROM.put(address, param);
  EEPROM.commit();

  //Read data from eeprom
  address = 0;
  int readId;
  readId = EEPROM.read(address); //EEPROM.get(address,readId);
  Serial.print("Read Id = ");
  Serial.println(readId);
  address += sizeof(readId); //update address value

  float readParam;
  EEPROM.get(address, readParam); //readParam=EEPROM.readFloat(address);
  Serial.print("Read param = ");
  Serial.println(readParam);
  EEPROM.end();
}
