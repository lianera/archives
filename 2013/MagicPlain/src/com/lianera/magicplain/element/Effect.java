package com.lianera.magicplain.element;

import java.awt.Graphics;

public class Effect extends Element{
	protected Animation anis;
	boolean moving=false;
	Effect(Animation animations,int x,int y,int w,int h){
		super(animations);
		anis=animations;
		animations.Move(x, y, w, h);
		work=false;
	}
	public Effect(Animation animations){
		super(animations);
		anis=animations;
		super.setLocWithApper(0, 0);
		work=false;
	}
	public Effect clone() { 
		Effect e = new Effect((Animation)anis.clone()); 
		return e;

	}
	public Animation getAni(){
		return anis;
	}
	public void Draw(Graphics g){
		if(anis.getReped()>=1)
			this.Enable(false);
		super.Draw(g);
	}
}