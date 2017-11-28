void portalnya(mode_portal lapo){
  if (lapo==mulai){
    portal.attach(0);
    delay(200);
    portal.write(0);
    delay(15); 
  }

  else if (lapo==buka){
    portal.write(60);
    delay(15); 
  }

  else if (lapo==tutup){
    portal.write(0);
    delay(15); 
  }
}

