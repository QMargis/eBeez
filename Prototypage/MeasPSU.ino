
//    FILE: INA226_set_average.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo
//    DATE: 2021-05-18
//     URL: https://github.com/RobTillaart/INA226
//
//  run this sketch in the IDE plotter
//  change the setAverage(7)  (line 33)   0..7
//  change the bus voltage
//  0 reads fast  ...  7 stair casing, very slow



void setupINA226()
{
  //Serial.println(__FILE__);
  if (!INA.begin() )
  {
    Serial.println("could not connect. Fix and Reboot");
  }
  INA.setMaxCurrentShunt(1, 0.1);
  INA.setAverage(2);   // <<<<<<<<<<<<<<<
}


void GetPower()
{
  delay (1);
  Serial.print("\t");
  Serial.print(INA.getAverage());
  Serial.print("\t");
  Serial.print(INA.getBusVoltage());
  Serial.print("\t");
  Serial.print(INA.getShuntVoltage_mV());
  Serial.print("\t");
  Serial.print(INA.getPower());
  Serial.println();
}


float MesurePuissance()  // à terminer
{
  float fMesurePuissance = 0 ;   
        fMesurePuissance = INA.getCurrent_mA();
        fMesurePuissance = fMesurePuissance * INA.getBusVoltage();
        delay (1);
        return (fMesurePuissance);    
}

float MesureCourant()    // à terminer
{
  float fMesureCourant = 0 ;   
        fMesureCourant = INA.getCurrent_mA();
         delay (1);
        return (fMesureCourant);    
}

float MesureTension()
{
  float fMesureTension = 0 ;   
        fMesureTension = INA.getBusVoltage();
        delay (1);
        return (fMesureTension); 
}

// -- END OF FILE --
