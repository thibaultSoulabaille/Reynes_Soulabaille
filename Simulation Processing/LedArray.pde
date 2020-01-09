class LedArray {
  // classe responsable de l'assignation des valeurs d'intensite pour chaque led
  public float[] coords;
  public float[] speed;
  public float size;
  public float[][] ledArray;
  private MarbleGroup marbles;
  
  public LedArray(int nbLed, float size){
    this.coords = new float[]{10F,10F};
    this.speed = new float[2];
    this.size = 700F;
    //on initialise un tableau de led en 7x7
    this.ledArray = new float[7][7];
    // et un groupe de led defini par la variable nbLed
    this.marbles = new MarbleGroup(nbLed, size, this);
  }
  
  //fonction update a chaque frame
  public void update1(float gX, float gY) {
    this.coords[0] += this.speed[0];
    this.coords[1] += this.speed[1];
    //on lance l'update de ses billes 
    this.marbles.update(gX,gY);
    for(int x=0;x<7;x++){
      for(int y=0;y<7;y++){
        //pour chacue des led on initialise une intensite a 0;
        float intensite = 0F;
        // ensuite pour chacune des billes ...
        for(Marble marble : this.marbles.marbles) {
          // ... on calcule la distance entre la bille et la led
          double distance = marble.distance(marble.coords, new float[]{x*100+50+this.coords[0],y*100+50+this.coords[1]});
          distance /= marble.size;
          // si cette distance est inferieure a la taille de la bille (ici 100)
          //on ajoute a l'intensite la valeur minimale entre ce qu'il lui faut pour atteindre 1 et 1 moins cette distance;
          if(distance <1F) intensite += Math.min(1-distance, 1-intensite);
        }
        // on associe cette intensite a la place [x][y] du tableau de led
        this.ledArray[x][y] = intensite;
      }
    }
  }
  
  
}
