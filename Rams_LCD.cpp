#include "Rams_LCD.h"

Rams_LCD::Rams_LCD(uint8_t RS, uint8_t E, uint8_t D7, uint8_t D6, uint8_t D5, uint8_t D4, uint8_t D3 = -1, uint8_t D2 = -1, uint8_t D1 = -1, uint8_t D0 = -1){
  
  this->ArrayPinLCD[0] = RS;
  this->ArrayPinLCD[1] = E;
  this->ArrayPinLCD[2] = D7;
  this->ArrayPinLCD[3] = D6;
  this->ArrayPinLCD[4] = D5;
  this->ArrayPinLCD[5] = D4;
  this->ArrayPinLCD[6] = D3;
  this->ArrayPinLCD[7] = D2;
  this->ArrayPinLCD[8] = D1;
  this->ArrayPinLCD[9] = D0;

  pinMode(ArrayPinLCD[0], OUTPUT);
  pinMode(ArrayPinLCD[1], OUTPUT);
  pinMode(ArrayPinLCD[2], OUTPUT);
  pinMode(ArrayPinLCD[3], OUTPUT);
  pinMode(ArrayPinLCD[4], OUTPUT);
  pinMode(ArrayPinLCD[5], OUTPUT);
  pinMode(ArrayPinLCD[6], OUTPUT);
  pinMode(ArrayPinLCD[7], OUTPUT);
  pinMode(ArrayPinLCD[8], OUTPUT);
  pinMode(ArrayPinLCD[9], OUTPUT);

}

void Rams_LCD::initialize(uint8_t Column, uint8_t Row, boolean ModeMPU = 0){

  this->MODE = ModeMPU;

  this->panjangLCD = Column;
  this->tinggiLCD = Row;

  delay(100);

  switch(MODE){
    default: case 0:
    delay(100);

      sendData(0, 0x02);//SET ke 4 BIT 
      sendData(0, 0x28); //SET ke 4 BIT Mode 2 line display dan font 5x8 || C berarti font 5x11
    break;

    case 1:
      sendData(0, 0x38); // Mengirim perintah untuk mode bus 8 BIT
    break;
  }

  sendData(0, 0x0C); //Display ON, Cursor OFF

}

void Rams_LCD::sendData(uint8_t RSvalue, uint8_t Data) {
  //RS == 0 data akan masuk ke Data Register || RS == 1 dataakan masuk ke Instruction Register
  //Array urutan pengiriman data sesuai dengan biner dan pin yang sesuai
  uint8_t ArrayDataLCD4BIT[4] = {ArrayPinLCD[5], ArrayPinLCD[4], ArrayPinLCD[3], ArrayPinLCD[2]}; //Array PIN untuk mode 4 Bit
  uint8_t ArrayDataLCD8BIT[8] = {ArrayPinLCD[9], ArrayPinLCD[8], ArrayPinLCD[7], ArrayPinLCD[6], ArrayPinLCD[5], ArrayPinLCD[4], ArrayPinLCD[3], ArrayPinLCD[2]}; //Array PIN untuk mode 8 Bit
  switch(MODE){
    case 0:
      for(uint8_t x = 0; x < 2; x++){ 
        switch(x){
          case 0:
            digitalWrite(ArrayPinLCD[0], RSvalue);  //menyalakan atau mematikan pin RS
            digitalWrite(ArrayPinLCD[1], 1);   //menyalakan pin Enable 

            for(uint8_t y = 7; y > 3; y--){
              if(Data & (1 << y)){
                digitalWrite(ArrayDataLCD4BIT[y-4], 1); // Di offset 4 karena ArrayDataLCD4BIT mulai dari indeks 0 || contoh bit ke 7 maka akan menyalakan pin array ke (7-4 = 3) pin array ke 3 yaitu ArrayPinLCD[5] / kaki 4
              }
              else{
                digitalWrite(ArrayDataLCD4BIT[y-4], 0);}
            }
            //delay(1000);
            digitalWrite(ArrayPinLCD[1], 0);  //mematikan pin Enable
          break;

          case 1:
            digitalWrite(ArrayPinLCD[0], RSvalue);  //menyalakan atau mematikan pin RS
            digitalWrite(ArrayPinLCD[1], 1);   //menyalakan pin Enable

            for(uint8_t y = 4; y > 0; y--){
              if(Data & (1 << (y - 1))){
                digitalWrite(ArrayDataLCD4BIT[y-1], 1); // Di offset 1 karena ArrayDataLCD4BIT mulai dari indeks 0
              }
              else{
                digitalWrite(ArrayDataLCD4BIT[y-1], 0);}
            }
            //delay(1000);
            digitalWrite(ArrayPinLCD[1], 0);  //mematikan pin Enable
          break;
        }
      delayMicroseconds(500);
  }
    break;

    case 1:
      digitalWrite(ArrayPinLCD[0], RSvalue);  //menyalakan atau mematikan pin RS
      digitalWrite(ArrayPinLCD[1], 1);   //menyalakan pin Enable 

      for(uint8_t x = 0; x < 8; x++){
        if(Data & (1 << x)){
          digitalWrite(ArrayDataLCD8BIT[x], 1);
        }

        else{
          digitalWrite(ArrayDataLCD8BIT[x], 0);
        }
      }
      
      digitalWrite(ArrayPinLCD[1], 0);   //mematikan pin Enable 
      delayMicroseconds(750);
      //delay(1);
    break;
  }
}

void Rams_LCD::clearDisplay(){
  sendData(0, 0x01); //clearDisplay
}

void Rams_LCD::home(){
  sendData(0, 0x02); //Return Home
}

void Rams_LCD::print(const char *Text){
  // Serial.println(strlen(Teks));
  while (*Text) {
    sendData(1, *Text); // Kirim karakter satu per satu
    Text++; // Pindah ke karakter berikutnya
  }
}

void Rams_LCD::println(const char* Text, uint8_t Space = 1){
  print(Text);
  for(uint8_t y = 0; y < Space; y++){
    sendData(1, 0x20);
  }
}

void Rams_LCD::printMode(const char* Text, uint8_t MODE, uint8_t Row){

    switch(MODE){
    default: case 0:
      setCursor(0, Row);
    break;

    case 1:
      setCursor((16 - strlen(Text)) / 2, Row);
    break;

    case 2:
      setCursor((16 - strlen(Text)), Row);
    break;
  }

  
  while (*Text) {
    sendData(1, *Text); // Kirim karakter satu per satu
    Text++; // Pindah ke karakter berikutnya
  }
}

void Rams_LCD::write(uint8_t Data){
  sendData(1, Data);
}

void Rams_LCD::setCursor(uint8_t Column, uint8_t Row){
    uint8_t Alamat;
  //Menentukan Alamat DDRAM LCD
  switch(Row){
    case 0: // Baris 0
    Alamat = Column; break;
    case 1: // Baris 1 (Yang Bawah)
    Alamat = 0x40 + Column; break;
    default: Alamat = Column;
  }
  sendData(0, 0x80 | Alamat);
}

void Rams_LCD::shiftRight(){
  sendData(0, 0x1C); //1C Menggeser Ke kanan
}

void Rams_LCD::shiftLeft(){
  sendData(0, 0x18); //18 Menggeser Ke Kiri
}

void Rams_LCD::createChar(uint8_t Location, uint8_t Charmap[]) {
  Location &= 0x07;  // Hanya 8 slot (0-7) untuk karakter kustom
  sendData(0, 0x40 | (Location << 3));  // Perintah untuk memulai penulisan karakter kustom ke CGRAM
  for (uint8_t i = 0; i < 8; i++) {
    sendData(1, Charmap[i]);  // Kirim setiap baris karakter ke CGRAM
  }
}

void Rams_LCD::displayCustomChar(uint8_t Location) {
  sendData(1, Location);  // Tampilkan karakter kustom dengan lokasi yang diberikan
}

void Rams_LCD::loopShiftRight(const char* Text, boolean Row, uint32_t Delay, uint8_t Loop, uint8_t StartPosition){
  char buffer[panjangLCD + 1];
  memset(buffer, 0x20, panjangLCD); // Mengisi buffer dengan spasi
  buffer[panjangLCD] = '\0'; // Penanda akhir string

  uint8_t startOffset = 0;

  // Set startOffset berdasarkan StartPosition
  switch(StartPosition){
    case 1: // TENGAH
      startOffset = (panjangLCD - strlen(Text)) / 2;
      break;
    case 2: // KANAN
      startOffset = panjangLCD - strlen(Text);
      break;
    default: // KIRI atau nilai lain
      startOffset = 0;
      break;
  }

  // Perulangan untuk jumlah loop
  for (uint8_t l = 0; l < Loop; l++) {
    // Perulangan untuk menggeser teks
    for (int y = 0; y < panjangLCD + 1; y++) {
      memset(buffer, 0x20, panjangLCD); // Reset buffer untuk setiap iterasi
      for (uint8_t x = 0; x < strlen(Text); x++) {
        int pos = (x + y + startOffset) % panjangLCD;
        buffer[pos] = Text[x];
      }

      setCursor(0, Row); // Set cursor ke posisi awal baris
      print(buffer); // Cetak buffer ke LCD

      delay(Delay); // Tunggu sesuai delay yang ditentukan
    }
  }
}


void Rams_LCD::loopShiftLeft(const char* Text, boolean Row, uint32_t Delay, uint8_t Loop, uint8_t StartPosition) {
  char buffer[panjangLCD + 1];
  memset(buffer, 0x20, panjangLCD); // Mengisi buffer dengan spasi
  buffer[panjangLCD] = '\0'; // Penanda akhir string

  uint8_t startOffset;
  switch(StartPosition) {
    default: case 0: // KIRI
      startOffset = 0;
      break;

    case 1: // TENGAH
      startOffset = (panjangLCD - strlen(Text)) / 2;
      break;

    case 2: // KANAN
      startOffset = panjangLCD - strlen(Text);
      break;
  }

  for(uint8_t l = 0; l < Loop; l++) {
    for(int y = 0; y < panjangLCD + 1; y++) {
      memset(buffer, 0x20, panjangLCD); // Reset buffer untuk setiap iterasi

      for(uint8_t x = 0; x < strlen(Text); x++) {
        int pos = (x - y + startOffset + panjangLCD) % panjangLCD;
        buffer[pos] = Text[x];
      }

      setCursor(0, Row);
      print(buffer); // PRINT KE LCD
      delay(Delay);
    }
  }
}

