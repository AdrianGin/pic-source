package breeding;

import processing.core.*;

//Arrow head is centred on the X,Y
public class ArrowHead {
	private PApplet p;

	int xLoc;
	int yLoc;
	
	int length;
	int height;
	
	public ArrowHead(PApplet _p, int x, int y, int l, int h) {
		p = _p;
		
		xLoc = x;
		yLoc = y;
		length = l;
		height = h;
	
	}

	void draw(int x, int y, int c1) {

		
		xLoc = x;
		yLoc = y;
		
		int colour;
		
		p.fill(c1);
		p.stroke(Colours.BLACK);
		
		p.beginShape();
		p.vertex(xLoc - length/2, yLoc);
		p.vertex(xLoc + length - length/2 , yLoc);
		
		p.vertex(xLoc + length - length/2 , yLoc+height);
		p.vertex(xLoc, yLoc+ height + height/2);
		
		p.vertex(xLoc - length/2, yLoc+height);
		
		p.vertex(xLoc - length/2, yLoc);
		p.endShape();
		
	}
	
	boolean overEvent() {

		PVector p1 = new PVector(xLoc, yLoc);
		PVector p2 = new PVector(xLoc, yLoc);
		
		p1.x -= length / 2;
		p1.y = yLoc;
		
		p2 = p1.copy();
		p2.x += length;
		p2.y += height + height/2;
		
		
		if (p.mouseX >= p1.x && p.mouseX <= p2.x &&
			p.mouseY >= p1.y && p.mouseY <= p2.y ) {
			return true;
		} else {
			return false;
		}
	}
}
