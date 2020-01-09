class MarbleGroup{
  //classe de groupe de billes qui va principalement nous servir a gerer les collisions
  public int nbMarbles;
  public Marble[] marbles;
  public LedArray ledArray;
  
  public MarbleGroup(int nb, float s, LedArray ledArr){
    this.ledArray = ledArr;
    this.nbMarbles = nb;
    this.marbles = new Marble[nb];
    for(int i=0; i<nb; i++){
      float x = ledArr.coords[0] + ledArr.size/2 ;
      float y = ledArr.coords[1] + ledArr.size/2;
      this.marbles[i] = new Marble(x,y,s);
    } 
  }
  public void update(float gX, float gY){
    //on update les coordonnees et les vitesse de chaque bille avant de calculer les collisions;
    for(int i=0; i<nbMarbles; i++){
      this.marbles[i].update(gX,gY);
    }
    //puis on lance le calcul de chaque billes avec celles avec qui on ne l'a pas deja calcule
    //la fonction de collision entre bille mdifiant aussi la vitesse de la bille passee en parametre
    for(int i=0; i<nbMarbles; i++){
      this.marbles[i].wallCollider(this.ledArray);
      for(int j=i+1; j<nbMarbles; j++){
        this.marbles[i].marbleCollider(this.marbles[j], this.ledArray);
      }
    }
  }
} 
