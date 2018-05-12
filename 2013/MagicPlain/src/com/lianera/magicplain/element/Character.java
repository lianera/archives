package com.lianera.magicplain.element;

import java.awt.Graphics;

import com.lianera.magicplain.ctrl.Game;
import com.lianera.magicplain.resource.Resource;

public class Character extends Element{
	protected Animation[] anis;
	Bomb bomb=null;
	int explosion=0;
	//DIRECTION direct=DIRECTION.UP;
	int step;
	boolean moving=false;
	int direct=0;
	int strength=0;

	Character(Animation[] animations,int gridX,int gridY){
		super(animations[2]);
		gx=gridX;gy=gridY;
		anis=animations;
		int gw=Game.mapReader.getTileWidth();
		int gh=Game.mapReader.getTileHeight();
		for(Animation a:animations){
			a.Move(gridX*gw, gridY*gh,gw,gh);
		}
	}
	public void addStrength(int s){
		strength+=s;
	}
	public void setExplosion(int e){
		explosion=e;
	}
	public void setStep(int walkstep){
		step=walkstep;
	}
	public void SetBomb(Bomb b){
		bomb=b;
	}
	public void addStep(int s){
		step+=s;
	}
	public Bomb PutBomb(){
		Game.gameRef.PlaySound("/putbomb.wav");
		Bomb b=(Bomb) bomb.clone();
		b.setLocWithApper(gx, gy);
		b.addPower(strength);
		b.SetExplosion(Resource.getExplosion(explosion));
		b.Enable(true);
		return b;
	}
	private boolean Registration(){
		return Math.abs(scrX()-anis[0].dx)<step && Math.abs(scrY()-anis[0].dy)<step;
	}
	private int scrX(){
		return Game.mapReader.getTileWidth()*gx;
	}
	private int scrY(){
		return Game.mapReader.getTileHeight()*gy;
	}
	private void ApperDiffX(int x){
		for(Animation a:anis){
			a.DiffX(x);
		}
	}
	private void ApperDiffY(int y){
		for(Animation a:anis){
			a.DiffY(y);
		}
	}
	private void Moving(){
		if(moving){
			if(scrX()-anis[0].dx>=step)	
				ApperDiffX(step);
			else if(anis[0].dx-scrX()>=step)	
				ApperDiffX(-step);
			if(scrY()-anis[0].dy>=step)	
				ApperDiffY(step);
			else if(anis[0].dy-scrY()>=step)	
				ApperDiffY(-step);
		}
		if(Registration())
			moving=false;
	}
	public void Draw(Graphics g){
		if(!visible || !work)return;
		Moving();
		if(moving)
			anis[direct].Continue();
		else
			anis[direct].Pause();
		anis[direct].Draw(g);
	}

	public void Move(int direction){
		if(moving)return;
		int x=gx,y=gy;
		switch(direction){
		case 0:y--;break;
		case 1:y++;break;
		case 2:x--;break;
		case 3:x++;break;
		default:return;
		}
		if(!Resource.Bumped(x, y)){
			gx=x;gy=y;
		}
		Resource.PicProp(this);
		direct=direction;
		moving=true;
	}
	public void setStrength(int s) {
		strength=s;
	}
}
