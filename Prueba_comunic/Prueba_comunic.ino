void setup() {
  Serial.begin(9600);  // Configura la velocidad de baudios adecuada
}

void loop() {
  if (Serial.available()) {
    char dato = Serial.read();  // Lee el dato recibido

    // Haz algo con el dato recibido
    // Por ejemplo, mostrarlo en el monitor serie de Arduino
    Serial.print("Dato recibido: ");
    Serial.println(dato);
  }
}
