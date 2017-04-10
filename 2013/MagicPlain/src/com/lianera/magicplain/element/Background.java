package com.lianera.magicplain.element;

public class Background extends Element{
	public Background(Picture pic){
		super(pic);
		pic.Move(0, 0,pic.getSourceW(),pic.getSourceH());
	}
}