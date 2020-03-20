#include <Arduino.h>
#include "marbles.h"

/*-----------Marble-----------*/

Marble::Marble(){
    this -> size = 0;
};

Marble::Marble(float x, float y, float s, float* c) {
    this -> coords[0] = x; this -> coords[1] = y; 
    this -> size = s;
    for(int i =0; i<3; i++) this -> colorArray[i] = c[i];
} 

float* Marble::getCoords(){return this -> coords;}
float Marble::getSize(){return this -> size;}
float* Marble::getColorArray(){return this -> colorArray;}

void Marble::update(float gX, float gY){
    // les coordonnes sont changees avant de modifier la vitesse car celle ci peut etre modifiee par les fonctions de collisions
    this -> coords[0] += this -> speed[0];
    this -> coords[1] += this -> speed[1];
    // this -> coords[2] += this -> speed[2];
    this -> speed[0] += gX;
    this -> speed[1] += gY;
    // this -> speed[2] += gZ;
}

void Marble::wallCollider(){
    //fonction de collision contre les murs
    for(int i=0; i<2; i++) {
      // pour chaque axe on defini une valeur haute et une valeur basse
      float coord = this -> coords[i];
      float high = coord + this -> size/2;
      float low = coord - this -> size/2;
      float speed = this -> speed[i];
      // float arrayHigh = ledArray.coords[i] + ledArray.size;
      // float arrayLow = ledArray.coords[i];
      //pour chacune de ses deux valeurs on vient verifier si
      //elle depasse les extremites du cube et que sa vitesse sur cette axe s'y dirige
      
      if((high>700 && speed>0) || (low<0 && speed<0)) {
       // si c'est la cas on modifie la coordonnees en fonction de l'endroit ou elle depasse;
       this -> coords[i] = high>700 ? 700 - this -> size/2 : 0 + this -> size/2;
       // la vitesse va soit prendre l'inverse de sa valeur divisee par 2
       // soit 0 si elle est <4 donc tres faible;
       this -> speed[i] = abs(this -> speed[i]) > 50 ? -this -> speed[i]*0.5 : 0;
      }
      
      // if(high>arrayHigh) {
      //   this -> coords[i] = arrayHigh - this -> size/2;
      //   this -> speed[i] = -this -> speed[i]*0.5;
      //   if(ledArray.speed[i]<0) this -> speed[i] += ledArray.speed[i];
      //   this -> speed[i] = abs(this -> speed[i]) > 4 ? this -> speed[i] : 0;
      // }
      // if(low<arrayLow) {
      //   this -> coords[i] = arrayLow + this -> size/2;
      //   this -> speed[i] = -this -> speed[i]*0.5;
      //   if(ledArray.speed[i]>0) this -> speed[i] += ledArray.speed[i];
      //   this -> speed[i] = abs(this -> speed[i]) > 4 ? this -> speed[i] : 0;
      // }
    }
}

void Marble::marbleCollider(Marble &marble){
    //fonction de collisions entre billes
    //on verifie si la distance entre ces deux billes est inferieure a leur taille
    if(this -> isCollidingWith(marble)){
      float x1 = this -> coords[0];   float y1 = this -> coords[1];   
      float x2 = marble.coords[0];    float y2 = marble.coords[1];   

      bool isWallColliding = false; 
      bool marbleWallColliding = false;
      
      //le vecteur correction va nous servir a separer les billes si elle se sont rentrees dedans
      float origin[2] = {0};
      float correction[2] = {x1-x2,y1-y2};
      float d = distance(correction, origin);
      if(d==0){
        d = this -> size -1;
        for(int i=0; i<2; i++) correction[i] = this -> size/2;
      }

      // les billes ont toutes la meme masse donc (m1/(m1+m2)) = 0.5
      
      for(int i=0; i<2; i++){
        correction[i] *= (this -> size -d)/(2*d);
        float change = this -> coords[i] + correction[i];
        float high = change + this -> size/2;
        float low = change - this -> size/2;
        // float arrayHigh = ledArray.coords[i] + ledArray.size;
        // float arrayLow = ledArray.coords[i];
        if(high > 700 || low < 0){
          isWallColliding = true;
          marble.coords[i] -= correction[i]*2;
        }
        else {
          change = marble.coords[i] - correction[i];
          high = change + marble.size/2;
          low = change - marble.size/2;
          if(high > 700 || low < 700){
            marbleWallColliding = true;
            this -> coords[i] += correction[i]*2;
          }
          else {
            this -> coords[i] += correction[i];
            marble.coords[i] -= correction[i];
          }
        }
        //this -> coords[i] += correction[i];
        //marble.coords[i] -= correction[i];
      }
      
      //nouveau referentiel (n,g) n est l'axe qui part de la deuxieme bille vers la premiere
      float nx = (x2 - x1)/(this -> size);
      float ny = (y2 - y1)/(this -> size);
      // float nz = (z2 - z1)/(this -> size);
      float gx = -ny;
      float gy = nx;
      // float gz = nz;
      // float kx = ny*gz - gz*ny;
      // float ky = nz*gx - nx*gz;
      // float kz = nx*gy - ny*gx;
      
      //  calcul des vitesses sur chacun des axes
      float s1n = nx * this -> speed[0] + ny * this -> speed[1] ;
      float s1g = gx * this -> speed[0] + gy * this -> speed[1] ;
      // float s1k = kx * this -> speed[0] + ky * this -> speed[1] + kz * this -> speed[2];
      float s2n = nx * marble.speed[0] + ny * marble.speed[1] ;
      float s2g = gx * marble.speed[0] + gy * marble.speed[1] ;
      // float s2k = kx * marble.speed[0] + ky * marble.speed[1] + kz * marble.speed[2];

     
      //la vitesse de chaque bille est egale a la vitesse de l'autre sur l'axe n plus la sienne sur l'axe g et k
      if(s1n<0 || isWallColliding) {
        marble.speed[0] = nx * 0.5 * s1n + gx * s2g ;
        marble.speed[1] = ny * 0.5 * s1n + gy * s2g ;
        // marble.speed[2] = nz * 0.5 * s1n + gz * s2g + kz * s2k;
      }
      if(s2n>0 || marbleWallColliding){
        this -> speed[0] = nx * 0.5 * s2n + gx * s1g ;
        this -> speed[1] = ny * 0.5 * s2n + gy * s1g ;
        // this -> speed[2] = nz * 0.5 * s2n + gz * s1g + kz * s1k;
      }
      //this -> speed[0] = nx * 0.5 * s2n + gx * s1g;
      //this -> speed[1] = ny * 0.5 * s2n + gy * s1g;
      //marble.speed[0] = nx * 0.5 * s1n + gx * s2g;
      //marble.speed[1] = ny * 0.5 * s1n + gy * s2g;
    }
}

bool Marble::isCollidingWith(Marble &marb){
    // float distX = this -> coords[0] - marb.coords[0];
    // float distY = this -> coords[1] - marb.coords[1];
    // float distZ = this -> coords[2] - marb.coords[2];
    // float dist = sqrt(distX*distX + distY*distY + distZ*distZ);

    float dist = distance(this -> coords, marb.getCoords());
    bool collide = (dist<this -> size-1);
    return collide;
}

float Marble::distance(float* c1, float* c2){
    float distX = c1[0] - c2[0];
    float distY = c1[1] - c2[1];
    // float distZ = c1[2] - c2[2];
    return sqrt(distX*distX + distY*distY);
}


/*-----------MarbleGroup-----------*/

MarbleGroup::MarbleGroup(){
  this -> nbMarbles = 0;
}

MarbleGroup::MarbleGroup(int nb, float s){
    this -> nbMarbles = nb;
    float colorArray[3] = {0};
    int colorIndex = 0;
    for(int i=0; i<nb; i++){
      for(int c = 0; c<3; c++) colorArray[c] = 0;
      colorArray[colorIndex] = 1;
      this -> marbles[i] = Marble(350,350,s,colorArray);
      colorIndex = (colorIndex+1)%3;
    }
}

Marble* MarbleGroup::getMarbles(){
  return this -> marbles;
}

void MarbleGroup::update(float gX, float gY){
    //on update les coordonnees et les vitesse de chaque bille avant de calculer les collisions;
    for(int i=0; i< this-> nbMarbles; i++){
      this -> marbles[i].update(gX,gY);
    }
    //puis on lance le calcul de chaque billes avec celles avec qui on ne l'a pas deja calcule
    //la fonction de collision entre bille mdifiant aussi la vitesse de la bille passee en parametre
    for(int i=0; i<this-> nbMarbles; i++){
      this -> marbles[i].wallCollider();
    }
    int iter = 0;
    while(thereIsColliding()){
      iter++;
      for(int i=0; i<this-> nbMarbles; i++){
        this -> marbles[i].wallCollider();
        for(int j=i+1; j<this-> nbMarbles; j++){
          this -> marbles[i].marbleCollider(this -> marbles[j]);
        }
      }
      if(iter>1) break;
    }
}

bool MarbleGroup::thereIsColliding(){
    for(int i=0; i<this-> nbMarbles; i++){
      for(int j=i+1; j<this-> nbMarbles; j++){
        if(this -> marbles[i].isCollidingWith(this -> marbles[j])) {
          return true;
        }
      }
    }
    return false;
}

/*-----------LedArray-----------*/

LedArray::LedArray(int nbLed, float size){
    this -> size = 700;
    this -> marbles =  MarbleGroup(nbLed, size);
}

void LedArray::update1(float gX, float gY) {
    // this -> coords[0] += this -> speed[0];
    // this -> coords[1] += this -> speed[1];
    // this -> coords[2] += this -> speed[2];
    //on lance l'update de ses billes 
    this -> marbles.update(gX,gY);

    Marble* marbs = this -> marbles.getMarbles();
  	for(int x=0;x<7;x++){
      for(int y=0;y<7;y++){
        for(int z=0;z<7;z++){
          //pour chacue des led on initialise une intensite a 0;
          float intensite[3] = {0};
          // ensuite pour chacune des billes ...
          for(int i = 0; i<9; i++) {
            // Marble marble = marbs[i];
            // ... on calcule la distance entre la bille et la led
            float ledX = x*100+50; float ledY = y*100+50;
            float ledPos[2] = {ledX,ledY};
            float distance = Marble::distance(marbs[i].getCoords(),ledPos);
            distance /= marbs[i].getSize();
            // si cette distance est inferieure a la taille de la bille
            //on ajoute a l'intensite la valeur minimale entre ce qu'il lui faut pour atteindre 1 et 1 moins cette distance;
            if(distance < 0.85) {
              for(int c=0; c<3; c++) intensite[c] += min(1-distance, 1-intensite[c])*marbs[i].getColorArray()[c]; 
            }
          }
          // on associe cette intensite a la place [x][y] du tableau de led
          for(int c=0; c<3; c++) this -> ledArray[x][y][c] = intensite[c];
        }
      }
    }
}

float LedArray::getValue(int x, int y, int colorIndex) {
  return this -> ledArray[x][y][colorIndex];
}

float LedArray::getMarbleColor(int colorIndex) {return this -> marbles.getMarbles()[0].getColorArray()[colorIndex];}
