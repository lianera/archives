package com.lianera.magicplain.element;

import java.awt.Graphics;

public abstract class Drawable{
	protected int dx=0,dy=0,dw=0,dh=0;
	public abstract void Draw(Graphics g);
	void Move(int x,int y,int w,int h){
		dx=x;dy=y;dw=w;dh=h;
	}
	void DiffX(int x){
		dx+=x;
	}
	void DiffY(int y){
		dy+=y;
	}
	void Move(int x,int y){
		dx=x;dy=y;
	}
	int getX(){
		return dx;
	}
	int getY(){
		return dy;
	}
}