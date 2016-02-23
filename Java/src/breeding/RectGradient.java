package breeding;

import processing.core.*;

public class RectGradient {

	private PApplet p;

	public RectGradient(PApplet _p) {
		p = _p;
	}

	void draw(int x, int y, int l, int w, int c1, int c2) {

		int colour;
		for( int i = 0; i < l; i++)
		{
			float amt = (1.0f / l) * i;
			colour = p.lerpColor(c1, c2, amt);
			p.stroke(colour);
			p.line(x+i, y, x+i, y+w);
		}
		

	}

}
