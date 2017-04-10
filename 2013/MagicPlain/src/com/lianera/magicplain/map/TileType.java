package com.lianera.magicplain.map;

import java.util.Map;
import java.util.TreeMap;

public class TileType {
	private Map<String, String> propertys = null;
	private int id;
	
	TileType(int index) {
		id = index;
		propertys = new TreeMap<String, String>();
	}
	void AddProperty(String name,String value){
		propertys.put(name, value);
	}
	public int getId(){
		return id;
	}
	public String getPropString(String prop){
		return propertys.get(prop);
	}
	public int getPropInt(String prop){
		int r=0;
		try{
			r=Integer.parseInt(propertys.get(prop));
		}catch(Exception e){
			System.out.println("∂¡»°¥ÌŒÛ:"+prop);
		}
		return r;
	}
}