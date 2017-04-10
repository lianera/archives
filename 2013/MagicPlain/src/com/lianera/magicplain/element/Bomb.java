package com.lianera.magicplain.element;

import java.awt.Graphics;

public class Bomb extends Element{// implements Cloneable{
	protected Animation anis;
	Explosion exp=null;
	private int expTime;
	boolean exploded=false;
	int power=0;

	public Bomb(Animation animations,int time,int w,int h) {
		super(animations);
		work=false;
		anis=animations;
		expTime=time;
		apper.Move(0, 0, w, h);
	}
	public void setPower(int p){
		power=p;
	}
	void addPower(int p){
		power+=p;
	}
	int getPower(){
		return power;
	}
	void SetExplosion(Explosion e){
		exp=e;
	}
	boolean Exploded(){
		boolean exp=exploded;
		exploded=false;
		return exp;
	}
	public Explosion GetExp(){
		if(exp==null)return null;
		Explosion e=exp.clone();
		if(e==null)return null;
		e.setLocWithApper(gx, gy);
		e.setPower(power);
		return e;
	}


	public void Draw(Graphics g){
		if(!work)return;
		expTime--;
		if(expTime<=0){
			work=false;
			exploded=true;
		}
		super.Draw(g);
	}
	/*public Object clone() {
		try {
			return super.clone();
		} catch (Exception e) {
			return null;
		}
	}*/

	public Bomb clone() { 
		Bomb b = new Bomb((Animation)anis.clone(), expTime, apper.dw, apper.dh); 
		b.setPower(power);
		return b;

	}
}
