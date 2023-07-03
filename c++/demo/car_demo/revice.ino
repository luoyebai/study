void setup() {
Serial.begin(9600);
}

bool getSerialPortData(unsigned char* data,const size_t size);
void printSerialPortData(const unsigned char *data,const size_t size);

void loop() {
  unsigned char data[4];
  if(getSerialPortData(data,sizeof(data)))
    printSerialPortData(data,sizeof(data));
  delay(5);
}

bool getSerialPortData(unsigned char* data,const size_t size)
{
    if(!Serial.available())
      return false;
    data[0] = Serial.read();
    if(data[0]!=165)
      return false;
    for(size_t i=1;i<size;++i)
      data[i] = Serial.read();
    if(data[size-1]==254)
      return true;
}

void printSerialPortData(const unsigned char *data,const size_t size)
{
  Serial.print("data:");
  for(size_t i=0;i<size;++i) 
  {
  Serial.print((int)data[i]);
  Serial.print(" ");
  }
  Serial.print("\n");
}
