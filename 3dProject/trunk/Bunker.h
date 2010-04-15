#pragma once

class Bunker :
	public StaticObject {
private:
		float w; //Width
		float h; //Height
		int* textures; // index: 0 => front, 1 => right, 2 => back, 3 => right,
public:
		Bunker();
		~Bunker();
		Bunker(float wIn, float hIn, float xIn, float yIn, float zIn, int* textures);
		
		void changeTexture(int texture, int index);
		float getWidth();		
		float getHeight();
		virtual void draw();
};