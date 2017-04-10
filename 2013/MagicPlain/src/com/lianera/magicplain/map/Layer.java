package com.lianera.magicplain.map;

import com.lianera.magicplain.ctrl.Game;

public class Layer {
	private int height, width;
	private String name;
	private int[] data;

	Layer(String layerName,int layerWidth,int layerHeight) {
		height = layerHeight;
		width = layerWidth;
		name = layerName;
	}

	void setData(byte[] byteArr) {
		data = new int[byteArr.length / 4];
		for (int i = 0; i < data.length; i++) {
			//data[i] = (byteArr[i * 4]) | (byteArr[i * 4 + 1] << 8)
			//		| (byteArr[i * 4 + 2] << 16) | (byteArr[i * 4 + 3] << 24);
			data[i]=byteArr[i*4]&0xFF;
			data[i]|=((byteArr[i*4+1]<<8)&0xFF00);
			data[i]|=((byteArr[i*4+2]<<16)&0xFF0000);
			data[i]|=((byteArr[i*4+3]<<24)&0xFF000000);
		}
	}

	int getValue(int gridX, int gridY) {
		return gridY * Game.mapReader.getMapWidth() + gridX;
	}
	public int[] getData(){
		return data;
	}
}