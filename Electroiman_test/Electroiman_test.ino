
const int iman=7;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  pinMode(iman,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(iman, HIGH);   // poner el Pin en HIGH para activar el electroimán
  Serial.print("HIGH\n");
  delay(10000);               // esperar un segundo
  digitalWrite(iman, LOW);    // poner el Pin en LOW para desactivar el electroimán
  Serial.print("LOW\n");
  delay(10000);               // esperar un segundo

}
