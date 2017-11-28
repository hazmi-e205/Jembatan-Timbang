int rata1,rata2,rata3;
int readings1[3],readings2[3],readings3[3];
int readIndex = 0;
int total1 = 0,total2 = 0,total3 = 0;
int moco_;

void cekSensor(){
  total1 = total1 - readings1[readIndex];
  total2 = total2 - readings2[readIndex];
  total3 = total3 - readings3[readIndex];
  readings1[readIndex] = digitalRead(14);
  readings2[readIndex] = digitalRead(2);
  readings3[readIndex] = digitalRead(0);
  total1 = total1 + readings1[readIndex];
  total2 = total2 + readings2[readIndex];
  total3 = total3 + readings3[readIndex];
  readIndex += 1;

  if (readIndex >= 3) readIndex = 0;
  rata1 = total1 / 3;
  rata2 = total2 / 3;
  rata3 = total3 / 3;
  
  if (moco_!=moco){
    if (moco_==0) t1=0,t2=0,t3=0;
    moco_=moco;
  }
  
  if (moco==1){
    if (rata1==0) t1+=1;
    if (rata2==0) t2+=1;
    if (rata3==0) t3+=1;
  } else {
    if (t1>2) sensor_1=1;
    else sensor_1=0;
    if (t2>2) sensor_2=1;
    else sensor_2=0;
    if (t3>2) sensor_3=1;
    else sensor_3=0;
  }
}

