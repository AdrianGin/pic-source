package breeding;

import processing.core.PApplet;
import processing.core.PConstants;
import processing.core.PVector;
import java.util.*;

public class InteractiveBall extends Ball {

	private PApplet p;

	boolean selected;
	boolean updateFlag;
	boolean isDragged;
	PVector[] selLoc;
	PVector deselLoc;
	PVector selOffset;
	
	
	int frameCount;
	
	boolean isBounce;
	boolean isCollision;
	boolean updateLocation;
	
	public final float Gravity = 0.2f;
	public final float MinBounceSpeed = Gravity*3.5f;
	public final float bounceLoss = 0.7f;
	public final int dragDelay = 5; //number of frames used to determine new velocity;
	
	LinkedList<InteractiveBall> CollisionList = new LinkedList();

	public InteractiveBall(PApplet _p, PVector iLoc, PVector iVel, float m, int r, boolean bounces) {
		super(_p, iLoc, iVel, m, r);
		p = _p;
		selected = false;
		updateFlag = false;
		isDragged = false;
		updateLocation = true;
		
		
		frameCount = 0;
		
		selLoc = new PVector[dragDelay];
		deselLoc = new PVector(0, 0);
		selOffset = new PVector(0, 0);
		

		
		isCollision = false;
		isBounce = bounces;
	}

	boolean overEvent() {

		int x_diff2 = (p.mouseX - (int) getLoc().x) * (p.mouseX - (int) getLoc().x);
		int y_diff2 = (p.mouseY - (int) getLoc().y) * (p.mouseY - (int) getLoc().y);

		if ((x_diff2 + y_diff2) < (radius * radius)) {
			return true;
		} else {
			return false;
		}
	}
	
	void onMouseClick(int mb)
	{
		if( (overEvent() && mb == PConstants.LEFT) )
		{
			selected = true;
			
			PVector mouseVector = new PVector(p.mouseX , p.mouseY);
			
			frameCount = 0;
			selOffset = mouseVector.copy();
			selOffset = selOffset.sub(getLoc());
			
			for( int i = 0; i < selLoc.length; i++)
			{
				selLoc[i] = getLoc();
			}
		}
	}
	
	
	void onMouseRelease(int mb)
	{
		if(selected)
		{
			updateFlag = true;
			isDragged = true;
			selected = false;
			deselLoc = getLoc();
		}
	}

	boolean updateStates() {

		
		if (selected) 
		{			
			if(frameCount >= dragDelay )
			{
				selLoc[frameCount % dragDelay] = getLoc();
			}
			
			frameCount++;
			
			return true;
		}

		return false;
	}
	
	void applyGravity()
	{
		PVector gravVel = new PVector(0.0f, Gravity);
		PVector newVel;
		
		newVel = getVel();
		newVel.add(gravVel);
		setVel( newVel );
		
	}
	
	
	boolean isOutOfBounds()
	{
		boolean doBounce = false;
		PVector newVel = new PVector(0 , 0);
		PVector newLoc = getLoc();
		newVel = getVel().copy();	
		if( (getLoc().x + radius) >= p.width) 
		{
			doBounce = true;
			newLoc.x = p.width - radius;
			newVel.x = -newVel.x;
		}
		
		if( (getLoc().x - radius) < 0 )
		{
			doBounce = true;
			newLoc.x = radius;
			newVel.x = -newVel.x;
		}
		
		
		if( (getLoc().y + radius) >= p.height) 
		{
			doBounce = true;
			newLoc.y = p.height - radius;
			newVel.y = -newVel.y;
		}
		
		if( (getLoc().y - radius) < 0 )
		{
			
			doBounce = true;
			newLoc.y = radius;
			newVel.y = -newVel.y;
		}
		
		if( doBounce )
		{
			setLoc(newLoc);
			newVel.mult(bounceLoss);
			if( newVel.mag() < MinBounceSpeed )
			{
				newVel.mult(0);
			}
			super.setVel( newVel );
		}
		else
		{
			applyGravity();
		}
		
		return doBounce;
	}
	
	void updateVelocity()
	{
		
		PVector newVel = new PVector(0 , 0);
		
		if(isBounce)
		{
			//Check detection with out of bounds.
			isOutOfBounds();

		}
		
		if( updateFlag )
		{
			updateFlag = false;
			isDragged = false;
			newVel = deselLoc.copy();
			newVel.sub(selLoc[frameCount % dragDelay]);
			newVel.div( Math.min(frameCount+1, dragDelay) );
			super.setVel( newVel );
		}
		
		
		
	}
	
	boolean isInteracting()
	{
		return selected;
	}
	
	boolean canInteract()
	{
		return overEvent();
	}
	
	void updateLocation()
	{
		PVector curMouseLoc = new PVector( p.mouseX, p.mouseY);
		if( selected )
		{
			super.setLoc(curMouseLoc.sub(selOffset));
		}
	}
	
	
	
	void preCollisionUpdate()
	{
		updateLocation();
	}
	
	void postCollisionUpdate()
	{
		updateStates();
		updateVelocity();
		if( updateLocation )
		{
			super.updateBall();
		}
		else
		{
			updateLocation = true;
		}
	}
	
	void resolveCollisionsA()
	{
		Iterator<InteractiveBall> listIterator = CollisionList.iterator();
        while (listIterator.hasNext()) {
            
        	InteractiveBall target = listIterator.next();
        	//if( )
        	while( isCollision(target) )
        	{
        		if( updateLocation )
        		{
        			super.updateBall();
        		}
        		else
        		{
        			updateLocation = true;
        		}
        	}
        	
        }
	}
	
	
		
	boolean isCollision(InteractiveBall target)
	{
		float   fDist;
		PVector iDist = getLoc();
		iDist = iDist.sub(target.getLoc());
		fDist = iDist.mag() + 0.5f;
		
		if(fDist <= (radius + target.radius) )
		{
			CollisionList.add(target);
			target.CollisionList.add(this);
			return true;
		}
		else
		{
		}
		return false;
	}
	
	boolean isNewCollision(InteractiveBall target)
	{
		if( CollisionList.contains(target) )
		{
			return true;
		}
		
		return false;
	}
	
	
	boolean isCollision()
	{
		if ( CollisionList.isEmpty() )
		{
			return false;
		}
		
		return true;
	}
		
	
	void clearCollisions()
	{
		CollisionList.clear();
	}
	
	// V[1] = u[1] (m[2] - m[2]) / ( m[1] + m[2] )
	// V[2] = 2 * m[1] * u[1]   /  ( m[1] + m[2] )
	// assume offset u[2] to create u[2] = 0, and add it back in later.
	void doCollision(InteractiveBall target)
	{
		PVector u1 = getVel();
		PVector u2 = target.getVel();
	
		if( u1.mag() == 0 && u2.mag() == 0)
		{
			return;
		}
		
	
		PVector v1n = new PVector();
		PVector v2n = new PVector();
		
		PVector v1t = new PVector();
		PVector v2t = new PVector();
		
		PVector v1 = new PVector();
		PVector v2 = new PVector();
		
		PVector tempv = new PVector();
		PVector tempv2 = new PVector();
		
		PVector normalVect = new PVector( getLoc().x - target.getLoc().x, getLoc().y - target.getLoc().y );
		PVector tangentVect;
		normalVect = normalVect.normalize();
		
		
		
		tangentVect = normalVect.copy();
		tangentVect = tangentVect.rotate( (float) (Math.PI /2));
		
		float m1 = (float)getMass();
		float m2 = (float)target.getMass();		
		
		//V1
		PVector.mult(normalVect, u1.dot(normalVect)*(m1 - m2) / (m1 + m2), tempv);
		PVector.mult(normalVect, u2.dot(normalVect) * (2*m2) /(m1+m2) , tempv2);
		PVector.add(tempv, tempv2, v1n);
		
		PVector.mult(tangentVect, u1.dot(tangentVect), v1t);
		PVector.add(v1t,  v1n, v1);		
		
		
		//V2
		PVector.mult(normalVect, u2.dot(normalVect)*(m2 - m1) / (m1 + m2), tempv);
		PVector.mult(normalVect, u1.dot(normalVect) * (2*m1) /(m1+m2) , tempv2);
		PVector.add(tempv, tempv2, v2n);
		
		PVector.mult(tangentVect, u2.dot(tangentVect), v2t);
		PVector.add(v2t,  v2n, v2);		
		
		setVel(v1);		
		target.setVel(v2);	
		
		

		PVector newLocation1 = getLoc();
		newLocation1.add(v1);
		PVector newLocation2 = target.getLoc();
		newLocation2.add(v2);
		
		PVector diffLoc = newLocation1.copy();
		diffLoc.sub(newLocation2);
		
		
		float fDist = diffLoc.mag();
		float minDistance = radius + target.radius;
		
		v1.mult(0.05f);
		v2.mult(0.05f);
		
		while(fDist <= (minDistance) && v1.mag() >= 0.05f && v2.mag() >= 0.05f )
		{

			//v1.normalize();
			newLocation1.add(v1);
			
			//v2.normalize();
			newLocation2.add(v2);			
			//updateLocation = false;
			//target.updateLocation = false;
			
			diffLoc = newLocation1.copy();
			diffLoc.sub(newLocation2);
			fDist = diffLoc.mag();
		}
		
		setLoc(newLocation1);
		target.setLoc(newLocation2);
		
	}
	
	
	boolean updateCollision(InteractiveBall target)
	{
		return isCollision(target);
	}

	void draw()
	{
		super.draw();
	}

}
