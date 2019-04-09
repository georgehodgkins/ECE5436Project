boolean statusRed;

void setupHeartBeat() {
  pinMode(78, OUTPUT);
  statusRed = 0;
}

void loopHeartBeat() {
    pinToggle(78);
    delay(1000);
}
