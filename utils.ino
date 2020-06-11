void blinkLed(int numberOfBlinks, int msBetweenBlinks){
  for (int i = 0; i < numberOfBlinks; i++) {
    digitalWrite(LED_BUILTIN, LOW); // Inverted on Wemos D1 Mini
    delay(msBetweenBlinks);
    digitalWrite(LED_BUILTIN, HIGH); // Inverted on Wemos D1 Mini
    if (i != numberOfBlinks - 1){
      delay(msBetweenBlinks);
    }
  }
}
