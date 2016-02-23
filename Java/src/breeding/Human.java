package breeding;

import processing.core.*;

public class Human extends InteractiveBall {

	public enum Sex {
	    MALE, FEMALE 
	}
	
	
	int age;
	Sex sex;
	int allele;
	
	int nChildren;
	
	private PApplet p;


	public Human(PApplet _p, PVector iLoc, PVector iVel, float m, int r, boolean bounces, int iAge, Sex iSex, int iAllele, int iChildren) {
		super(_p, iLoc, iVel, m, r, bounces);
		p = _p;
		
		age = iAge;
		sex = iSex;
		allele = iAllele;
		nChildren = iChildren;
		
		
	}
	
	

	
}
