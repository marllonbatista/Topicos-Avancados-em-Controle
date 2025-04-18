//********** bibliotecas **********
#include <Arduino.h>
#include <Motor.h>
#include <Variaveis.h>
#include <cmath>

// ********** Declaração de funções **********
void funcao_hybridcontrol();

// ********** Motor  **********
Motor motor_d; //objeto motor direito
Motor motor_e; //objeto motor esquerdo

//***** funcao_setup *****
void setup(){
    Serial.begin(115200);
    analogReadResolution(10);
    //***** configuracao dos motores *****
    motor_d.configuracao(21,19,2,3);
    motor_e.configuracao(18,4,0,1);
    //***** configuracao do modulo sensor de 8 canais *****

    //********** helices  *****/
    pinMode(helices,OUTPUT);
}
//***** funcao_loop *****
void loop(){
   
}
// ***** funcao_hybridcontrol *****
void funcao_hybridcontrol(){
  float u_pd; // u_pd[n] saida do controlador pd
  float u_fuzzy; // u_fuzzy[n] saida do controlador fuzzy
  float u;//u[n] saida final
  float erro;
  float abs_erro;
  const uint16_t setpoint=3500;
  const uint16_t velocidade_maxima= 255;// 2^8 - 1 = 255 representa a resolucao do sinal pwm 
  const int16_t velocidade_minima=-180;
  // intervalos das regras
  const int16_t small_error_max = 750;
  const int16_t medium_error_min = 500;
  const int16_t medium_error_max = 1750;
  const int16_t large_error_min = 1500;
  const int16_t large_error_max = 3500;

  erro = (setpoint - posicao);
/******** parte do controlador pd *******/
  proporcional = erro;
  derivativo= erro-erro_anterior;
  u_pd=((proporcional*kp) + (derivativo*kd));
  erro_anterior=erro;
/******** parte do controlador fuzzy *******/
  abs_erro=fabs(erro);
// mf erro pequeno (rampa decrescente)
  float small_error_membership = fmax(0, (small_error_max - abs_erro) / small_error_max);
  small_error_membership = fmin(small_error_membership, 1);

  // mf erro medio (triangular)
  float medium_error_membership = 0;
  float medium_error_mid = (medium_error_max + medium_error_min) / 2;

  if (abs_erro > medium_error_min && abs_erro < medium_error_mid) {
    medium_error_membership = (abs_erro - medium_error_min) / (medium_error_mid - medium_error_min);  // crescente ate o meio
  } else if (abs_erro >= medium_error_mid && abs_erro < medium_error_max) {
    medium_error_membership = (medium_error_max - abs_erro) / (medium_error_max - medium_error_mid);  // decrescente depois do meio
  }
  medium_error_membership = fmin(fmax(medium_error_membership, 0), 1);

  // mf erro grande (rampa crescente)
  float large_error_membership = fmax(0, (abs_erro - large_error_min) / (large_error_max - large_error_min));
  large_error_membership = fmin(large_error_membership, 1);

  u_fuzzy= (small_error_membership * ks) + (medium_error_membership * km) + (large_error_membership * kl);
  // inverte o sinal do fuzzy caso o erro seja negativo (pois estamos usando o erro em modulo)
  if (erro < 0) {
    u_fuzzy = -u_fuzzy;
  }
  //uniao fuzzy e pd
  u=(u_fuzzy+u_pd)/2;
//***** motor esquerdo *****
  int16_t velocidade_motor_esquerdo=velBase+u;
  if (velocidade_motor_esquerdo>velocidade_maxima){     
      velocidade_motor_esquerdo=velocidade_maxima;
  }
  else if(velocidade_motor_esquerdo<velocidade_minima){
      velocidade_motor_esquerdo=velocidade_minima;
  }
//***** motor direito *****
  int16_t velocidade_motor_direito=velBase-u;
  if (velocidade_motor_direito>velocidade_maxima){
      velocidade_motor_direito=velocidade_maxima;
  }
  else if(velocidade_motor_direito<velocidade_minima){
      velocidade_motor_direito=velocidade_minima;
  }
  motor_d.correr(velocidade_motor_direito);
  motor_e.correr(velocidade_motor_esquerdo);
}

