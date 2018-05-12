package gui;

import java.awt.Image;
import java.io.IOException;

import javax.imageio.ImageIO;
import javax.swing.ImageIcon;


public class Resource {

	public static Image loadImage(String file){
			return new ImageIcon(Resource.class.getResource(file)).getImage();
		//return null;//(new ImageIcon(file)).getImage();
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
