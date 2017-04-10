package com.lianera.magicplain.element;

import java.awt.Graphics;

public class Explosion extends Element{
	protected Animation[] anis;
	private int expTime;
	int offsetX[],offsetY[];
	int power=0;

	public Explosion(Animation animation,int offX[],int offY[],int time,int w,int h) {
		super(animation);
		work=false;
		expTime=time;
		offsetX=offX;
		offsetY=offY;
		animation.Move(offX[0]*w, offY[0]*h, w, h);
		anis=new Animation[offX.length];
		for(int i=0;i<offX.length;i++){
			anis[i]=(Animation) animation.clone();
			anis[i].Move(offX[i]*w, offY[i]*h, w, h);
		}
	}
	int getPower(){
		return power;
	}
	void setPower(int p){
		power=p;
	}
	public void FillPowerMatrix(int[][] m){
		for(int i=0;i<offsetX.length;i++){
			int rgx=gx+offsetX[i];
			int rgy=gy+offsetY[i];
			if(rgy>=0&&rgy<m.length&&rgx>=0&&rgx<m[0].length)
				m[rgy][rgx]+=power;
		}
	}
	public boolean Valid(){
		return work;
	}
	public void Draw(Graphics g){
		if(!work)return;
		expTime--;
		if(expTime<=0)
			work=false;
		for(int i=0;i<offsetX.length;i++){
			anis[i].Move((gx+offsetX[i])*anis[i].GetScrW(),(gy+offsetY[i])*anis[i].GetScrH());
			anis[i].Draw(g);
		}
	}
	public Explosion clone() { 
		Animation a=(Animation)((Animation)apper).clone();
		Explosion b = new Explosion(a,offsetX,offsetY, expTime,apper.dw, apper.dh); 
		return b;

	}
	
}