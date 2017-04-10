package com.lianera.magicplain.element;

import java.awt.Graphics;
import java.awt.Image;

public class Picture extends Drawable{
	private Image img;
	private int sx,sy,sw,sh;
	public Picture(Image image,int imgx,int imgy,int imgw,int imgh){
		img=image;
		sx=imgx;sy=imgy;sw=imgw;sh=imgh;
	}
	public void Draw(Graphics g) {
		g.drawImage(img, dx, dy, dx+dw, dy+dh, 
				sx, sy,sx+sw,sy+sh, null);
	}
	int getSourceW(){
		return sw;
	}
	int getSourceH(){
		return sh;
	}
}