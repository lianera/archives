package com.lianera.magicplain.map;

public class OriginImage{
	private int height, width,tileWidth,tileHeight;
	private String source;
	OriginImage(){}
	OriginImage(String imgSource,int imgWidth,int imgHeight,int tilew,int tileh){
		source=imgSource;
		width=imgWidth;
		height=imgHeight;
		tileWidth=tilew;
		tileHeight=tileh;
	}
	void setHW(int imgHeight,int imgWidth){
		height=imgHeight;
		width=imgWidth;
	}	
	public void setSource(String imgSource){
		source=imgSource;
	}
	public String getResource(){
		String resource = source.substring(source.lastIndexOf("/"));
		return resource;
	}
	public int getTileNum(){
		return height/tileWidth*width/tileHeight;
	}
	int getHeight(){
		return height;
	}
	int getWidth(){
		return width;
	}
}