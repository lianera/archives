package com.lianera.magicplain.element;

public class Block extends Element{
	Block(Drawable drable,int x,int y,int w,int h){
		super(drable);
		drable.Move(x, y, w, h);
	}
	public Block(Drawable drable,int gridX,int gridY){
		super(drable);
		super.setLocWithApper(gridX, gridY);
	}
}