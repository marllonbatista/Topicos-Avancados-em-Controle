#include <Arduino.h>
// ********** PWM **********
const uint16_t freq =1000; 
const uint8_t resolucao = 8;

class Motor{
  private:
    uint8_t pin_motor_frente;
    uint8_t pin_motor_tras;
    uint8_t canal_motor_frente;
    uint8_t canal_motor_tras;
  public:
    
    void configuracao(uint8_t pin_frente, uint8_t pin_tras, uint8_t canal_frente, uint8_t canal_tras){
        pin_motor_frente= pin_frente;
        pin_motor_tras= pin_tras;
        canal_motor_frente= canal_frente;
        canal_motor_tras= canal_tras;
  
        //** configuracao_PWM ** 
        ledcAttachPin(pin_motor_frente, canal_motor_frente);
        ledcSetup(canal_motor_frente, freq, resolucao);
  
        ledcAttachPin(pin_motor_tras, canal_motor_tras);
        ledcSetup(canal_motor_tras, freq, resolucao);
    }
    void correr( int16_t vel){
        
        if (vel > 0){
           
            ledcWrite(canal_motor_tras,LOW);
            ledcWrite(canal_motor_frente,vel);  
        }
        else{
            vel=vel*(-1);
            ledcWrite(canal_motor_frente,LOW);
            ledcWrite(canal_motor_tras,vel);
        }
    }
    void desligar(){
        ledcWrite(canal_motor_tras,LOW);
        ledcWrite(canal_motor_frente,LOW); 
    }
};
