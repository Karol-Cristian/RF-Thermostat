
void sendMessage() {
  
    const char *msg = "1";
    rf_driver.send((uint8_t *)msg, strlen(msg));
    rf_driver.waitPacketSent();
    delay(0);
}
