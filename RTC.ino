

 void modifyData()
  {
  data->counter = setTemp;
  Serial.println(String("Set value is: ") + data->counter);

  // Persist data in RTC and in flash memory
  // It preserves data even on power loss
  rtcMemory.persist();
  }
