package com.lianera.magicplain.element;

import java.awt.Graphics;
import java.awt.Image;
import java.util.ArrayList;
import java.util.List;

public class Animation extends Drawable{// implements Cloneable{
	private List<Image> imgs=null;
	private List<Integer> sx,sy,sw,sh;
	private int curImg=0,timeGap=0,gapPos=0;
	int reped=0;
	boolean paused=false;
	
	public Animation(int times){
		super();
		timeGap=times;
		imgs=new ArrayList<Image>();
		sx=new ArrayList<Integer>();
		sy=new ArrayList<Integer>();
		sw=new ArrayList<Integer>();
		sh=new ArrayList<Integer>();
	}
	int getReped(){
		return reped;
	}
	int GetScrW(){
		return sw.get(0);
	}
	int GetScrH(){
		return sh.get(0);
	}
	public int getTimeGap(){
		return timeGap;
	}
	public void Add(Image image,int imgx,int imgy,int imgw,int imgh){
		imgs.add(image);
		sx.add(imgx);
		sy.add(imgy);
		sw.add(imgw);
		sh.add(imgh);
	}
	public void SetTimeGap(int times){
		timeGap=times;
	}
	public void Pause(){
		paused=true;
	}
	public void Continue(){
		paused=false;
	}
	public void Draw(Graphics g) {
		g.drawImage(imgs.get(curImg), dx, dy, dx+dw, dy+dh, 
				sx.get(curImg), sy.get(curImg),
				sx.get(curImg)+sw.get(curImg),sy.get(curImg)+sh.get(curImg), null);
		if(!paused){
			gapPos++;
			if(gapPos>=timeGap){
				gapPos=0;
				curImg++;//(curImg+1)%imgs.size();
				if(curImg>=imgs.size()){
					curImg=0;
					reped++;
				}
			}
		}
	}
	public Object clone() {
		try {
			Animation a=new Animation(timeGap);
			for(int i=0;i<imgs.size();i++)
				a.Add(imgs.get(i), sx.get(i), sy.get(i), sw.get(i),sh.get(i));
			return a;
		} catch (Exception e) {
			return null;
		}
	}
}