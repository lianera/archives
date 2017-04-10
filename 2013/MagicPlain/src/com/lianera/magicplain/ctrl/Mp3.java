package com.lianera.magicplain.ctrl;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;

import javazoom.jl.player.Player;

public class Mp3 extends Thread{
	private String file;
	private Player player;
	boolean closed=true;
	
    public Mp3(String filename){
        file= filename;
        this.start();
            
     }
    public void Play() {
    	synchronized(this){
    		closed=false;
    		this.notify();
    	}
    }
    public void run(){
    	while(true){
    		synchronized(this){
	    	try {
	    		if(closed)this.wait();
	            BufferedInputStream buffer =new BufferedInputStream(new FileInputStream(file));
	            player = new Player(buffer);
				player.play();
				buffer.close();
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
    		}
    	}
    }
    public void Stop(){
     	closed=true;
     	player.close();
    }
}
