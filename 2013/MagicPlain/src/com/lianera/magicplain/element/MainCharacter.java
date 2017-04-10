package com.lianera.magicplain.element;

import java.awt.Graphics;

public class MainCharacter extends Character{
	protected Animation flag;
	public MainCharacter(Animation[] animations,int gridX,int gridY){
		super(animations,gridX,gridY);
	}
	public void setFlag(Animation f){
		flag=f;
	}
	public void Draw(Graphics g){
		super.Draw(g);
		flag.Move(apper.getX(),apper.getY()-20);
		flag.Draw(g);
	}
}