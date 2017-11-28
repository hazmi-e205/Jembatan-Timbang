int lakukan_ = 0,lakukan__=0;
double last;

int a=0;

void lakukan_reset(){
  lakukan_=0;
}

void lakukan_ya(){
  lakukan_=1;
}

void lakukan_tidak(){
  lakukan_=2;
}

void lakukan(){
  if (lakukan_!=lakukan__){
    last=millis();
    a=0;
    lakukan__=lakukan_;
  }
  
  switch (lakukan_){
    case 0:
    digitalWrite(16, 0);
    digitalWrite(5, 0);
    digitalWrite(4, 0);
    break;

    case 1:
    if (a<3){
      if (millis()-last>700){
        digitalWrite(16, 1);
        digitalWrite(5, 1);
        digitalWrite(4, 0);
        a++;
        last=millis();
      }
    } else {
      digitalWrite(16, 0);
      digitalWrite(5, 0);
      digitalWrite(4, 0);
      last=millis();
    }
    break;

    case 2:
    if (millis()-last>300){
      if (a==0){
        digitalWrite(16, 1);
        digitalWrite(5, 0);
        digitalWrite(4, 1);
        a=1;
      } else {
        digitalWrite(16, 0);
        digitalWrite(5, 0);
        digitalWrite(4, 0);
        a=0;
      }
      last=millis();
    }
    break;
  }
}
