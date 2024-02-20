
/* T.LOT le 21/03/23
 * Mise en mémoire non volatile des calibrations des 4 capteurs
 * Ecriture data 32bits repartis sur 4 octets de 8bits.
 * il faut un offset de 4 à chaque valeurs de 32 bits.
 * 
 */
 
void setupEEPROM() 
  {  
      Serial.println(F("Initialize EEPROM"));
      EEPROM.begin  (EEPROM_SIZE); 
      lValCalibLoadCPT1  =  iEepromDataRead (0);     // memorisation des valeurs de calibration
      lValCalibLoadCPT2  =  iEepromDataRead (8);
      lValCalibLoadCPT3  =  iEepromDataRead (16);
      lValCalibLoadCPT4  =  iEepromDataRead (24);  
  }

void resetEEPROM()
   {
    for (int iAddress = 0; iAddress < 32; iAddress++)
    {
      EEPROM.put(iAddress,0); //EEPROM.put(address, param) Attention les données serrons sur 32Bits soit 4 regitres address i+4
      EEPROM.commit();
    }
   }
//--------------------------------------------------------------------
// Ecriture d'une valeur de 32bits en mémoire
// Attention les données serons sur 32Bits soit 4 regitres address i+4 
//--------------------------------------------------------------------

void EepromDataWrite ( uint8_t iAddress,long lDataWrite ) 
  {
    EEPROM.put(iAddress,lDataWrite); //EEPROM.put(address, param) Attention les données serrons sur 32Bits soit 4 regitres address i+4
    EEPROM.commit();
    if (bMenu== true) {Serial.print(" iAddress : ");Serial.print(iAddress);Serial.print("   lDataWrite : ");Serial.println(iEepromDataRead(iAddress));}
  }
//----------------------------------------------------------------------------
// Lecture  d'une valeur de 32bits en mémoire
// Attention les données serons sur 32Bits soit 4 regitres de 8bits address i+4 
//-----------------------------------------------------------------------------

int iEepromDataRead (uint8_t iAddress ) 
  {
    long lDataRead = 0;
    EEPROM.get(iAddress, lDataRead);
    return(lDataRead);
  }

  
