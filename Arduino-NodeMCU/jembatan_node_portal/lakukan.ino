int lakukan_ = 0,lakukan__=0;

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
    lcd.clear();
    lakukan__=lakukan_;
  }
  
  switch (lakukan_){
    case 0:
    ulang();
    break;

    case 1:
    lolos();
    break;

    case 2:
    melangggar();
    break;
  }
}

