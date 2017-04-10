package gui;

import java.awt.Image;

import javax.swing.ImageIcon;


public class Resource {
	public static Image imgMainback=loadImage("res/mainback.png");
	public static Image imgPanel=loadImage("res/panel.png");

	public static ImageIcon imgNormal=new ImageIcon("res/button_normal.png");
	public static ImageIcon imgEnter=new ImageIcon("res/button_enter.png");
	public static ImageIcon imgPress=new ImageIcon("res/button_press.png");

	public static ImageIcon imgGridNormal=new ImageIcon("res/e1.png");
	public static ImageIcon imgGridEnter=new ImageIcon("res/e2.png");
	public static ImageIcon imgGridPress=new ImageIcon("res/e3.png");
	public static ImageIcon imgDisabled=new ImageIcon("res/disabled.png");
	
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
