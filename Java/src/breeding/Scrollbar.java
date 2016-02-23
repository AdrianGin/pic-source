package breeding;


import processing.core.*;



public class Scrollbar {
	
	private static final int cArrowOffset = 10;
	private static final int cArrowSize = 10;
	
	int xPos;
	int yPos;
	
	int length;
	int width;
	
	float sliderPos;
	
	int sliderLength;
	int sliderWidth;
	
	boolean over;
	boolean locked;
	boolean selected;
	
	float max;
	float min;
	
	//For gradients
	int colour1;
	int colour2;
	
	private PApplet p;
	private RectGradient box;
	private ArrowHead arrow;

	
	Scrollbar(PApplet _p, int x, int y, int l, int w, float initPos, int sLength, int sWidth, float minVal, float maxVal, int c1, int c2)
	{
		p = _p;
		
		xPos = x;
		yPos = y;
		length = l;
		width = w;
		
		over = false;
		locked = true;
		selected = false;
		
		min = minVal;
		max = maxVal;
		
		sliderPos = initPos + xPos;
		sliderLength = sLength;
		sliderWidth = sWidth;
		
		colour1 = c1;
		colour2 = c2;
		
		box = new RectGradient(_p);
		arrow = new ArrowHead(_p, (int)sliderPos, yPos-cArrowOffset, cArrowSize, cArrowOffset);
		
		
		
	}
	
	boolean overEvent() 
	{
	    if (arrow.overEvent())
	    {
	    	return true;
	    }
		
		
	    if (p.mouseX > xPos && p.mouseX < xPos+length &&
	       p.mouseY > yPos && p.mouseY < yPos+width) 
	    {
	      return true;
	    } else {
	      return false;
	    }
	  }
	
	
	void onMouseClick(int mb)
	{
		if( overEvent() && mb == PConstants.LEFT)
		{
			selected = true;			
		}
	}
	
	void onMouseRelease(int mb)
	{
		if(selected)
		{
			selected = false;
		}
	}

	
	void update() {
	    if (selected) {
	      locked = false;
	    }
	    else {
	      locked = true;
	    }
	    
	    if(!locked)
	    {
	    	sliderPos = p.constrain(p.mouseX, xPos, xPos + length);
	    	
	    }
	    
	  }
	
	float getValue()
	{
		return sliderPos;
	}
	
	
	void draw()
	{
		update();
		p.noStroke();
		//p.fill(colour1);
		box.draw(xPos, yPos, length, width, colour1, colour2);
		arrow.draw((int)sliderPos, yPos-cArrowOffset, Colours.WHITE);
	}


}
