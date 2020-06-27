#include <TinyWireM.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int botao = 1;
int buzzer = 3;
int rele = 4;

int horas;
int minutos;
int segundos;
int tempoTotalEmSegundos;
int mensagemDeBoasVindas;  
int frequenciaDoAlarme;  
int atrasoDoAlarme;
int atrasoDeUmSegundo; 

void configurarParametrosDeInicializacao(){
  lcd.init();
  lcd.backlight();
  mensagemDeBoasVindas = 0;
  tempoTotalEmSegundos = 0;
  frequenciaDoAlarme = 2000;
  atrasoDoAlarme = 500;
  atrasoDeUmSegundo = 1000;
  
  horas = 0;
  minutos = 0;
  segundos = 5;
}

void setup(){
  TinyWireM.begin();
  pinMode(rele, OUTPUT);
  pinMode(botao, INPUT);
  pinMode(buzzer, OUTPUT);  
  configurarParametrosDeInicializacao();
}

void exibirMensagemDeBoasVindas(){
  lcd.setCursor(2,0);
  lcd.print("Temporizador");
  lcd.setCursor(3,1);
  lcd.print("da Beatriz");
  delay(2500);
  mensagemDeBoasVindas = 1;
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

void alarmeDeTempoFinalizado(){
  for(int indice = 0; indice < 4; indice ++){
    tone(buzzer, frequenciaDoAlarme, atrasoDoAlarme);
    delay(atrasoDeUmSegundo);
  }
}

void exibirMensagemDeTempoFinalizado(){
lcd.clear();
lcd.setCursor(5,0);
lcd.print("Tempo");
lcd.setCursor(3,1);
lcd.print("Finalizado!");
}

void loop(){

  boolean iniciarTemporizador = false;
  
  if(mensagemDeBoasVindas == 0) {
    exibirMensagemDeBoasVindas();
  }

  do{
    exibirMensagemDoMenu();
    if(digitalRead(botao) == HIGH){
      iniciarTemporizador = true;
    }
  }while(iniciarTemporizador == false);

  if(iniciarTemporizador){
    int horasRestantes = horas;
    int minutosRestantes = minutos;
    int segundosRestantes = segundos;
    
    tempoTotalEmSegundos = segundosRestantes + (minutosRestantes * 60) + (horasRestantes * 60 * 60); 

    while (tempoTotalEmSegundos > 0){
      digitalWrite(rele, HIGH);
      delay (atrasoDeUmSegundo);
      tempoTotalEmSegundos--;

      horasRestantes = ((tempoTotalEmSegundos / 60)/ 60);
      minutosRestantes = (tempoTotalEmSegundos / 60) % 60;
      segundosRestantes = tempoTotalEmSegundos % 60;

      exibirTempoNoLcd(horasRestantes, minutosRestantes, segundosRestantes);

      if (tempoTotalEmSegundos == 0){
         digitalWrite(rele, LOW); 
         exibirMensagemDeTempoFinalizado();
         tempoTotalEmSegundos = 0;
         iniciarTemporizador = false;
         alarmeDeTempoFinalizado();
      }
    }
  }    
}
