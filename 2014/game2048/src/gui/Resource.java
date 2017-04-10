package gui;

import java.awt.Image;

import javax.swing.ImageIcon;


public class Resource {
	public static Image loadImage(String file){
		return (new ImageIcon(file)).getImage();
	}
	/*
	void PlaySound(String snd){
		
		try {
			
		AudioStream as=new AudioStream(this.getClass().getResourceAsStream(snd));
		AudioPlayer.player.start(as);
		}
		catch (Exception e) {}
	}*/
}
