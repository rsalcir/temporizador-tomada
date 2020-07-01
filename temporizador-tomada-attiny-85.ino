#include <TinyWireM.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int botaoEBuzzer = 1;
int rele = 4;

boolean exibeMensagemDeBoasVindas = true;   
int horas = 4;
int minutos = 0;
int segundos = 0;

void setup(){
  TinyWireM.begin();
  pinMode(rele, OUTPUT);
  lcd.init();
  lcd.backlight();
}

void exibirMensagemDeBoasVindas(){
  lcd.setCursor(2,0);
  lcd.print("Temporizador");
  lcd.setCursor(3,1);
  lcd.print("da Beatriz");
  delay(2500);
  lcd.clear();
}

void exibirMensagemDoMenu(){
  lcd.setCursor(0,0);
  lcd.print("Aperte o botao");
  lcd.setCursor(1,1);
  lcd.print("para iniciar...");
}

void exibirTempoNoLcd(int horasRestantes, int minutosRestantes, int segundosRestantes){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Tempo  restante"); 
  lcd.setCursor(4,1);
  
  if (horasRestantes < 10){
    lcd.print("0");
  }
  lcd.print(horasRestantes);
  lcd.print(":"); 
  if (minutosRestantes < 10){
    lcd.print("0");
  }
  lcd.print(minutosRestantes);
  lcd.print(":");
  if (segundosRestantes < 10){
    lcd.print("0");
  }
  lcd.print(segundosRestantes);    
}

void exibirMensagemDeTempoFinalizado(){
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("Tempo");
  lcd.setCursor(3,1);
  lcd.print("Finalizado!");
}

void alarmeDeTempoFinalizado(){
  pinMode(botaoEBuzzer, OUTPUT);
  for(int indice = 0; indice < 4; indice ++){
    tone(botaoEBuzzer, 2000, 500);
    exibirMensagemDeTempoFinalizado();
    delay(1000);
  }
}

void loop(){

  pinMode(botaoEBuzzer, INPUT);
  int tempoTotalEmSegundos = 0;
  
  if(exibeMensagemDeBoasVindas) {
    exibirMensagemDeBoasVindas();
    exibeMensagemDeBoasVindas = false;
  }
  
  boolean iniciarTemporizador = false;
  exibirMensagemDoMenu();
  
  if(digitalRead(botaoEBuzzer) == HIGH){
    iniciarTemporizador = true;
  }

  if(iniciarTemporizador){
    int horasRestantes = horas;
    int minutosRestantes = minutos;
    int segundosRestantes = segundos;
    
    tempoTotalEmSegundos = segundosRestantes + (minutosRestantes * 60) + (horasRestantes * 60 * 60); 

    while (tempoTotalEmSegundos > 0){
      digitalWrite(rele, HIGH);
      delay (1000);
      tempoTotalEmSegundos--;

      horasRestantes = ((tempoTotalEmSegundos / 60)/ 60);
      minutosRestantes = (tempoTotalEmSegundos / 60) % 60;
      segundosRestantes = tempoTotalEmSegundos % 60;

      exibirTempoNoLcd(horasRestantes, minutosRestantes, segundosRestantes);

      if (tempoTotalEmSegundos == 0){
         digitalWrite(rele, LOW); 
         tempoTotalEmSegundos = 0;
         iniciarTemporizador = false;
         alarmeDeTempoFinalizado();
      }
    }
  }    
}
