
void awalan(){
  lcd.begin(16,2);
  lcd.init();
  lcd.noBacklight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ketinggian :");
  lcd.setCursor(0, 1);      
  lcd.print("------------");

  portalnya(mulai);
}

void melangggar(){
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Ketinggian :");
  lcd.setCursor(0, 1);      
  lcd.print("melanggar");

  portalnya(tutup);
}

void lolos(){
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Ketinggian :");
  lcd.setCursor(0, 1);      
  lcd.print("lolos");

  portalnya(buka);
}

void ulang(){
  lcd.noBacklight();
  lcd.setCursor(0, 0);
  lcd.print("Ketinggian :");
  lcd.setCursor(0, 1);      
  lcd.print("------------");

  portalnya(buka);
}
