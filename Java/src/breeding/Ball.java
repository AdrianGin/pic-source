package breeding;


import processing.core.*;


public class Ball {
	
	float mass;
	int radius;
	
	int colour;
	
	private PApplet p;
	private PVector location;
	private PVector velocity;
	
	Ball(PApplet _p, PVector iLoc, PVector iVel, float m, int r)
	{
		p = _p;
		location = iLoc;
		velocity = iVel;
		
		mass = m;
		radius = r;
		
		colour = Colours.BLACK;
	}
	
	float getEnergy()
	{
		//Ke = 0.5 * m |v| ^ 2
		float Ke = 0.5f * (float) mass * velocity.magSq();
		return Ke;
	}
	
	float getMass()
	{
		return mass;
	}
	
	PVector getLoc()
	{
		return location.copy();
	}
	
	PVector getVel()
	{
		return velocity.copy();
	}
	
	void setLoc(PVector newLoc)
	{
		location = newLoc.copy();
	}
	
	void setVel(PVector newVel)
	{
		velocity = newVel.copy();
	}

	void setColour(int newColour)
	{
		colour = newColour;
	}
	
	boolean overEvent() {
		
		int x_diff2 = (p.mouseX - (int)location.x) * (p.mouseX - (int)location.x);
		int y_diff2 = (p.mouseY - (int)location.y) * (p.mouseY - (int)location.y);
		
	    if ( (x_diff2 + y_diff2) < (radius * radius)) {
	      return true;
	    } else {
	      return false;
	    }
	  }
	
	boolean isSelected() {
		
		int x_diff2 = (p.mouseX - (int)location.x) * (p.mouseX - (int)location.x);
		int y_diff2 = (p.mouseY - (int)location.y) * (p.mouseY - (int)location.y);
		
	    if ( ((x_diff2 + y_diff2) < (radius * radius)) && p.mousePressed) {
	      return true;
	    } else {
	      return false;
	    }
	  }
	
	
	void updateBall()
	{
		location.x = location.x + velocity.x;
		location.y = location.y + velocity.y;
	}
	
	void draw()
	{
		p.fill(colour);
		p.stroke(colour);
		//p.line(oldLoc.x, oldLoc.y, newLoc.x, newLoc.y);
		p.ellipse( (int)location.x, (int)location.y, radius*2, radius*2);
	}
	


}
