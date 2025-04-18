#include <stdint.h>
//********** PID **********
uint16_t velBase =0;//rpm
float kp=0; //ganho do proporcional
float kd=0;//ganho do derivativo
float erro_anterior=0;
float proporcional=0;
float derivativo=0;
int posicao=0;
/******** fuzzy **************/
float ks=0; // correcao pequena
float km=0; // correcao media
float kl=0; // correcao grande

//********** helice **********
const uint8_t helices = 22;
bool libera_helices=true;
long int tempo_helices=0;
uint8_t contador =0;

