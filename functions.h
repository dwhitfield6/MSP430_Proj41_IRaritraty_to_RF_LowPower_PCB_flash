
unsigned char ArrayCompareFlash(int*,int*, int, char);
void ArrayCopy(unsigned char*, int*, unsigned char);
void resetBuffer(unsigned char*, unsigned char);
void send_rf(unsigned char, const unsigned char*);
void zero(unsigned char);
void one(unsigned char);
void f(unsigned char);
unsigned char readSwitchP1(unsigned char);
void BlinkLED(unsigned char);
void Blink2LED(unsigned char, unsigned char);
void flash_erase(int*);
void flash_write(int* , int);

