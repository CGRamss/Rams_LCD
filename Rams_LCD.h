#ifndef Rams_LCD_h
#define Rams_LCD_h
#include <Arduino.h>
#include <string.h>

class Rams_LCD{
  private:
  //BIT MODE untuk LCD
    boolean MODE;

  //PIN Kaki Data untuk LCD
    uint8_t ArrayPinLCD[10]; //{RS, E, D7, D6, D5, D4, D3, D2, D1, D0}

  //Panjang dan Tinggi dari LCD
      uint8_t panjangLCD;
      uint8_t tinggiLCD; 
    
  public:

    Rams_LCD(uint8_t RS, uint8_t E, uint8_t D7, uint8_t D6, uint8_t D5, uint8_t D4, uint8_t D3 = -1, uint8_t D2 = -1, uint8_t D1 = -1, uint8_t D0 = -1); // SET PIN
    void initialize(uint8_t Column, uint8_t Row, boolean ModeMPU = 0); //Menginisialisasi Panjang dan Tinggi dari LCD
    void sendData(uint8_t RSvalue, uint8_t Data); //Mengirim Data secara (4 BIT x 2) atau full 8 BIT sekaligus
    void clearDisplay();  //Sesuai Nama
    void home();  //Setcursor ke koordinat 0,0
    void print(const char* Text); //Print teks ke layar
    void println(const char* Text, uint8_t Space = 1); //Print Teks ke layar di tambah spasi setelah teks selasi
    void printMode(const char* Text, uint8_t MODE = 0, uint8_t Row = 0);  //Teks dapat muncul di paling kiri, tengah, atau paling kanan
    void write(uint8_t Data); //Menulis Data HEX Berbasis ASCII
    void setCursor(uint8_t Column, uint8_t Row); //MengSet Cursor Penulisan Teks
    void shiftRight();  //Menggeser semua teks di layar ke kanan
    void shiftLeft();   ////Menggeser semua teks di layar ke kiri
    void createChar(uint8_t Location, uint8_t Charmap[]); //Membuat Custom Char ||Dibantu oleh ChatGPT
    void displayCustomChar(uint8_t Location); //Memanggil Custom Char //Dibantu oleh ChatGPT
    void loopShiftRight(const char* Text, boolean Row = 0, uint32_t Delay = 50, uint8_t Loop = 1, uint8_t StartPosition = 0); //Dibantu oleh ChatGPT
    void loopShiftLeft(const char* Text, boolean Row = 0, uint32_t Delay = 50, uint8_t Loop = 1, uint8_t StartPosition = 0);  //Dibantu oleh ChatGPT
};

#endif