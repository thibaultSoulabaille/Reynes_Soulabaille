import processing.serial.*;

Serial myPort;

int x;
float gX, gY;
LedArray leds;
int rotation;
int temps;int temps2;

void setup(){
  printArray(Serial.list());
  //myPort = new Serial(this, Serial.list()[0], 9600);
  size(1500,1000);
  x=0;
  gX = 0F;
  gY = 0F;
  rotation = 0;
  temps =0;temps2=0;
  leds = new LedArray(10, 700F/7F);
  leds.update1(gX, gY);
}

void draw(){
  background(0);
  fill(255);
  rect(leds.coords[0]-4,leds.coords[1]-4,leds.size+8, leds.size+8);
  fill(0);
  rect(leds.coords[0],leds.coords[1],leds.size, leds.size);

  for(Marble marb : leds.marbles.marbles){
     fill(255);
     ellipse(marb.coords[0],marb.coords[1],marb.size,marb.size);
  }
  for(int x=0;x<7;x++){
    for(int y=0;y<7;y++){
      
      int test = (int)(leds.ledArray[x][y]*255);
      fill(test);
      rect(x*100+25+leds.coords[0],y*100+25+leds.coords[1],50,50);
    }
  }
  
  
  leds.update1(gX, gY);
  temps++;
  if(temps > 2) { //<>//
      //float ts = (float)(mouseX/700F);
      //rotation = (int)(ts*360);
      rotation += temps%5 == 0 ? 1 :0;
      println(rotation%360);
      gX = (float)Math.sin(radians(rotation));
      gY = (float)Math.cos(radians(rotation));
  }
  if(temps%100 > 0 && temps%150<25){
    leds.speed[0] = 20F;
  }
  if(temps%150>25 && temps%150<50){
    leds.speed[0] = 0F;
    leds.speed[1] = 10F;
  }
  if(temps%150>50 && temps%150<75){
    leds.speed[1] = 0F;
    leds.speed[0] = -20F;
  }
  if(temps%150>75 && temps%150<100){
    leds.speed[0] = 0F;
    leds.speed[1] = -10F;
  }
  if(temps%150>100){
    leds.speed[0] = 0F;
    leds.speed[1] = 0F;
  }
  //if(temps%150<10) leds.speed[0] = 50F;
  //if(temps%150>10 && temps%150 <12) leds.speed[0]=0F;
  //if(temps%150>=140) leds.speed[0] = -55F;
}
