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
  delay(50);
  return l_lightRead;
}
