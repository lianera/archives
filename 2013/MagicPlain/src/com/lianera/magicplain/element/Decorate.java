package com.lianera.magicplain.element;

public class Decorate extends Element{
	public Decorate(Drawable drable,int gridX,int gridY){
		super(drable);
		super.setLocWithApper(gridX, gridY);
	}
}