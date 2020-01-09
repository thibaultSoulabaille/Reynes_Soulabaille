class Marble{
  //classe bille definie par des coordonnees une vitesse et une taille
  public float[] coords;
  public float[] speed;
  public float size;
  
  public Marble(float x, float y, float s) {
    this.coords = new float[]{x,y};
    this.speed = new float[2];
    this.size = s;
  } 
  public void update(float gX, float gY){
    // les coordonnes sont changees avant de modifier la vitesse car celle ci peut etre modifiee par les fonctions de collisions
    this.coords[0] += this.speed[0];
    this.coords[1] += this.speed[1];
    this.speed[0] += gX;
    this.speed[1] += gY;
  }
  
  private void wallCollider(LedArray ledArray){
    //fonction de collision contre les murs
    for(int i=0; i<2; i++) {
      // pour chaque axe on defini une valeur haute et une valeur basse
      float coord = this.coords[i];
      float speed = this.speed[i];
      float high = coord + this.size/2;
      float low = coord - this.size/2;
      float arrayHigh = ledArray.coords[i] + ledArray.size;
      float arrayLow = ledArray.coords[i];
      //pour chacune de ses deux valeurs on vient verifier si
      //elle depasse les extremites du cube et que sa vitesse sur cette axe s'y dirige
      
      //if((high>arrayHigh && speed>0) || (low<arrayLow && speed<0)) {
      //  // si c'est la cas on modifie la coordonnees en fonction de l'endroit ou elle depasse;
      //  this.coords[i] = high>arrayHigh ? arrayHigh - this.size/2 : arrayLow + this.size/2;
      //  // la vitesse va soit prendre l'inverse de sa valeur divisee par 2
      //  // soit 0 si elle est <4 donc tres faible;
      //  this.speed[i] = Math.abs(this.speed[i]) > 4F ? -this.speed[i]*0.5F : 0F;
      //}
       //<>//
      if(high>arrayHigh) {
        this.coords[i] = arrayHigh - this.size/2;
        if(speed>0) this.speed[i] = -this.speed[i]*0.5F;
        if(ledArray.speed[i]<0) this.speed[i] += ledArray.speed[i];
        this.speed[i] = Math.abs(this.speed[i]) > 4F ? this.speed[i] : 0F;
      }
      if(low<arrayLow) {
        this.coords[i] = arrayLow + this.size/2;
        if(speed<0) this.speed[i] = -this.speed[i]*0.5F;
        if(ledArray.speed[i]>0) this.speed[i] += ledArray.speed[i];
        this.speed[i] = Math.abs(this.speed[i]) > 4F ? this.speed[i] : 0F;
      }
    }
  }
  
  private void marbleCollider(Marble marble, LedArray ledArray){
    //fonction de collisions entre billes
    //on verifie si la distance entre ces deux billes est inferieure a leur taille
    if(this.isCollidingWith(marble)){
      float x1 = this.coords[0];   float y1 = this.coords[1];
      float x2 = marble.coords[0]; float y2 = marble.coords[1];
      
      boolean isWallColliding = false; boolean marbleWallColliding = false;
      
      //le vecteur correction va nous servir a separer les billes si elle se sont rentrees dedans
      float[] correction = new float[]{x1-x2,y1-y2};
      double d = distance(correction,new float[]{0F,0F});
      if(d==0){
        d = this.size -1;
        correction = new float[]{this.size/2,this.size/2};
      }
      correction = mult(correction,(this.size-d)/d);
      
      // les billes ont toutes la meme masse donc (m1/(m1+m2)) = 0.5
      correction =mult(correction,0.5F);
      
      for(int i=0; i<2; i++){
        float change = this.coords[i] + correction[i];
        float high = change + this.size/2;
        float low = change - this.size/2;
        float arrayHigh = ledArray.coords[i] + ledArray.size;
        float arrayLow = ledArray.coords[i];
        if(high > arrayHigh || low < arrayLow){
          isWallColliding = true;
          marble.coords[i] -= correction[i]*2F;
        }
        else {
          change = marble.coords[i] - correction[i];
          high = change + marble.size/2;
          low = change - marble.size/2;
          if(high > 700F || low < 0F){
            marbleWallColliding = true;
            this.coords[i] += correction[i]*2F;
          }
          else {
            this.coords[i] += correction[i];
            marble.coords[i] -= correction[i];
          }
        }
        //this.coords[i] += correction[i];
        //marble.coords[i] -= correction[i];
      }
      
      //nouveau referentiel (n,g) n est l'axe qui part de la deuxieme bille vers la premiere
      float nx = (x2 - x1)/(this.size);
      float ny = (y2 - y1)/(this.size);
      float gx = -ny;
      float gy = nx;
      
      //  calcul des vitesses sur chacun des axes
      float s1n = nx * this.speed[0] + ny * this.speed[1];
      float s1g = gx * this.speed[0] + gy * this.speed[1];
      float s2n = nx * marble.speed[0] + ny * marble.speed[1];
      float s2g = gx * marble.speed[0] + gy * marble.speed[1];
     
      //la vitesse de chaque bille est egale a la vitesse de l'autre sur l'axe n plus la sienne sur l'axe g
      if(s1n<0 || isWallColliding) {
        marble.speed[0] = nx * 0.5F * s1n + gx * s2g;
        marble.speed[1] = ny * 0.5F * s1n + gy * s2g;
      }
      if(s2n>0 || marbleWallColliding){
        this.speed[0] = nx * 0.5F * s2n + gx * s1g;
        this.speed[1] = ny * 0.5F * s2n + gy * s1g;
      }
      //this.speed[0] = nx * 0.5F * s2n + gx * s1g; //<>//
      //this.speed[1] = ny * 0.5F * s2n + gy * s1g;
      //marble.speed[0] = nx * 0.5F * s1n + gx * s2g;
      //marble.speed[1] = ny * 0.5F * s1n + gy * s2g;
    }
  }
  
  private boolean isCollidingWith(Marble marb){
    double dist = this.distance(this.coords, marb.coords);
    return (dist<=this.size);
  }
  
  private double distance(float[] c1, float[] c2){
    double distX = c1[0] - c2[0];
    double distY = c1[1] - c2[1];
    return Math.sqrt(distX*distX + distY*distY);
  }
  
  private float[] mult(float[] v, double m){
    float[] v2 = v.clone();
    v2[0] *= m;
    v2[1] *= m;
    return v2;
  }
}
