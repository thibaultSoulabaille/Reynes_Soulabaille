#ifndef marbles_h
#define marbles_h
#include <Arduino.h>

class Marble{
	public :
		Marble();
		Marble(float x, float y, float s, float* c);
		float* getCoords();
		float getSize();
		float* getColorArray();
		void update(float gX, float gY);
		void wallCollider();
		void marbleCollider(Marble &marble);
		bool isCollidingWith(Marble &marb);
		static float distance(float* c1, float* c2);
	private :
		float coords[2] = {0};
		float speed[2] = {0};
		float size;
		float colorArray[3];
};

class MarbleGroup{
	public :
		MarbleGroup();
		MarbleGroup(int nb, float s);
		Marble* getMarbles();
		void update(float gX, float gY);
	private :
		int nbMarbles;
		bool thereIsColliding();
		Marble marbles[49] = {Marble()};
};

class LedArray{
	public :
		LedArray(int nbLed, float size);
		void update1(float gX, float gY);
		float getValue(int x, int y, int colorIndex);
		float getMarbleColor(int colorIndex);
	private :
		float coords[2] = {0};
		float speed[2] = {0};
		float size;
		float ledArray[7][7][3] = {{{0}}};
		MarbleGroup marbles;
};

#endif
