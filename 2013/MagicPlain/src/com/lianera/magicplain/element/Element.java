package com.lianera.magicplain.element;

import java.awt.Graphics;

import com.lianera.magicplain.ctrl.Game;

public abstract class Element {//implements Cloneable{

	int life=Integer.MAX_VALUE;
	boolean work=false;
	protected int gx,gy;
	boolean visible=true;
	protected Drawable apper=null;
	
	Element(Drawable appearance){
		apper=appearance;
		work=true;
	}
	public int getGridX(){
		return gx;
	}
	public int getGridY(){
		return gy;
	}
	public void setLocation(int gridX,int gridY){
		gx=gridX;gy=gridY;
	}
	public void setLocWithApper(int gridX,int gridY){
		gx=gridX;gy=gridY;
		apper.Move(gx*Game.mapReader.getTileWidth(), gy*Game.mapReader.getTileHeight(),
				Game.mapReader.getTileWidth(),Game.mapReader.getTileHeight());
	}
	public void setLocWithApper(int x,int y,int w,int h){
		gx=x;gy=y;
		apper.Move(x, y,w,h);
	}
	public void Enable(boolean e){
		work=e;
		visible=e;
	}
	public void setLife(int a){
		life=a;
	}
	public void addLife(int a){
		life+=a;
	}
	public boolean Valid(){
		return work;
	}
	public void setVisible(boolean v){
		visible=v;
	}
	public void Draw(Graphics g){
		if(visible && work)apper.Draw(g);
	}
	public void getFired(int p){
		life-=p;
		if(life<=0){
			life=0;
			work=false;
		}
	}
	public boolean isAt(int x,int y){
		return x==gx&&y==gy;
	}
	//protected abstract void Move(int gridX,int gridY);
/*	public Object clone() {
		try {
			super.clone();
			return super.clone();
		} catch (Exception e) {
			return null;
		}
	}*/
}
