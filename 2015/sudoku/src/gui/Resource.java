package gui;

import java.awt.Image;

import javax.swing.ImageIcon;


public class Resource {
	public static Image imgMainback=loadImage("/mainback.png");
	public static Image imgPanel=loadImage("/panel.png");

	public static ImageIcon imgNormal=loadImageIcon("/button_normal.png");
	public static ImageIcon imgEnter=loadImageIcon("/button_enter.png");
	public static ImageIcon imgPress=loadImageIcon("/button_press.png");

	public static ImageIcon imgGridNormal=loadImageIcon("/e1.png");
	public static ImageIcon imgGridEnter=loadImageIcon("/e2.png");
	public static ImageIcon imgGridPress=loadImageIcon("/e3.png");
	public static ImageIcon imgDisabled=loadImageIcon("/disabled.png");

	public static ImageIcon loadImageIcon(String file){
		return new ImageIcon(Resource.class.getResource(file));
	}

	public static Image loadImage(String file){
		return new ImageIcon(Resource.class.getResource(file)).getImage();
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
