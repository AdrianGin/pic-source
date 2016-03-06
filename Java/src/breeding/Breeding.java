package breeding;

import java.util.Random;


import processing.core.*;





public class Breeding extends PApplet {

	InteractiveBall[] balls;
	Scrollbar  scroll;
	Human h1;
	
	public final int frameRate = 10;
	public final int tickRate = 120;
	
	
	public final int ballMass = 10;
	public final int ballRadius = 20;
	public final int ballCount = 5;
	

	
	public final int dispWidth = 128;
	public final int dispHeight = 128;
	
	public final int borderOffset = Math.min(dispWidth, dispHeight) / 20;
	
	public final float velocityScaler = 80.0f * frameRate / tickRate;
	public final float velocityOffset = 0.5f;
	
	public void settings() {
		size(dispWidth, dispHeight);
		
	}

	public void setup() {
		
		frameRate(tickRate);
		

		Random rn = new Random();
		balls = new InteractiveBall[ballCount];
		
		for( int i=0; i<ballCount; i++ )
		{
			PVector v1 = new PVector( Math.max(0, (rn.nextInt(dispWidth- (2*borderOffset)) + borderOffset )), Math.max(0, rn.nextInt(dispHeight-(2*borderOffset)) + borderOffset));
			//PVector v1 = new PVector( 100 , 100);
			PVector v2 = new PVector(rn.nextFloat()*velocityScaler - velocityOffset , rn.nextFloat()*velocityScaler - velocityOffset);
			
			int radius = rn.nextInt(20) + 8;
			float mass =  radius*radius * 0.5f;
			
			balls[i] = new InteractiveBall(this,v1, v2, mass, radius, true);
		}
		
		
		scroll = new Scrollbar(this, 50, 200, 100, 20, 0, 2, 2, 60.0f, 600.0f, Colours.GREEN, Colours.BLUE);
		//h1 = new Human(this, 0, Human.Sex.MALE, 0, 0);
		
		
		textSize(20);
		
	}
	
	public void mousePressed()
	{
		for( int i=0; i<ballCount; i++ )
		{
			balls[i].onMouseClick(mouseButton);
		}
		
		scroll.onMouseClick(mouseButton);
		
	}
	
		
	public void mouseReleased()
	{
		for( int i=0; i<ballCount; i++ )
		{
			balls[i].onMouseRelease(mouseButton);
		}
		scroll.onMouseRelease(mouseButton);
	}

	public void draw() {
		
		String s1 = "X:" + mouseX;
		String s2 = "Y:" + mouseY;
		float netKe = 0.0f;
		background(99);
		
		
		
		frameRate( (int)scroll.getValue());
		scroll.draw();
		
		
		
		for( int i=0; i<ballCount; i++ )
		{
			balls[i].clearCollisions();
			balls[i].preCollisionUpdate();
		}
		
		
		for( int i=0; i<ballCount; i++ )
		{			
			for( int j=i; j<ballCount; j++ )
			{
				if( j != i )
				{
					if(balls[i].updateCollision(balls[j]) && balls[i].isNewCollision(balls[j]))
					{						
						balls[i].doCollision(balls[j]);
						//balls[j].doCollision(balls[i]);
						
					}
				}
			}
			
			if( balls[i].isCollision() )
			{
				//balls[i].setColour(Colours.BLACK);
			}
			else
			{
				balls[i].setColour(Colours.BLUE);
			}
			
			if( balls[i].isInteracting() )
			{
				balls[i].setColour(Colours.WHITE);
				cursor(MOVE);
			}
			else if( balls[i].canInteract() )
			{
				balls[i].setColour(Colours.WHITE);
				cursor(HAND);
			}
			else
			{
				cursor(ARROW);
			}
			
			
			balls[i].postCollisionUpdate();
			balls[i].draw();
			
			netKe += balls[i].getEnergy();
		}
		
		
		


		String StringNetKe = "NetKe = " + netKe; 
		text(StringNetKe, 10, 80);
		
		text(s1, 10, 30);
		text(s2, 10, 50);

		//ellipse(mouseX, mouseY, 80, 80);
		
		// test.location.x = test.velocity.y;
	}

	public static void main(String args[]) {
		PApplet.main(new String[] { "breeding.Breeding" });
	}
}
