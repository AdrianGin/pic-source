package breeding;

import processing.core.PApplet;

public class Human {

	public enum Sex {
	    MALE, FEMALE 
	}
	
	
	int age;
	Sex sex;
	int allele;
	
	int nChildren;
	
	private PApplet p;


	public Human(PApplet _p, int iAge, Sex iSex, int iAllele, int iChildren) {
		p = _p;
		
		age = iAge;
		sex = iSex;
		allele = iAllele;
		nChildren = iChildren;
		
		
	}
	
	

	
}
