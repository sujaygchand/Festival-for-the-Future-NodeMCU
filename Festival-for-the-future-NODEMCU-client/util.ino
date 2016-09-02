
boolean debounce(boolean previousState)
{
  boolean currentState = digitalRead(SW_PIN);
  if (currentState != previousState) {
    delay(17);
    currentState = digitalRead(SW_PIN);
  }
  return currentState;
}
