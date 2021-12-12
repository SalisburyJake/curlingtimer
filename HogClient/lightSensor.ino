/* 
 *  Function Name: readSensor
 * Description:    Read the sensor data
 * Inputs:         N/A
 * Outputs:        N/A
 */
int readSensor()
{
  int l_lightRead = 0;
  l_lightRead = analogRead(LIGHT_SENSOR);    //Read analog data from the sensor

  if(l_lightRead <= MIN_SENS_CUTOFF)
    digitalWrite(LED_BUILTIN, LOW);
  else
    digitalWrite(LED_BUILTIN, HIGH);
  
  delay(50);
  return l_lightRead;
}
