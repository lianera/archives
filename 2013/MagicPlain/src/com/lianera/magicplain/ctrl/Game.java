package com.lianera.magicplain.ctrl;

import java.applet.Applet;
import java.awt.Graphics;
import java.awt.GridLayout;
import java.awt.Image;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.FileInputStream;
import java.io.IOException;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.xml.parsers.ParserConfigurationException;

import org.xml.sax.SAXException;

import com.lianera.magicplain.ai.Intelligence;
import com.lianera.magicplain.element.Character;
import com.lianera.magicplain.map.MapReader;
import com.lianera.magicplain.resource.Resource;

import sun.audio.AudioPlayer;
import sun.audio.AudioStream;


public class Game extends Applet implements Runnable{
		
	static GAME_STATE state=GAME_STATE.SHOWLOGO; 
	public static Game gameRef=null;
	private final long timeInterval=35;
	private long timeCount=0;
	private Image swapImage;
	private Graphics swapGraph;
	public static final int windowWidth=960;
	public static final int windowHeight=640;
	
	public static MapReader mapReader=null;
	public static Resource gameRes=null;
	public static Intelligence intellig=null;
	
	Character mainChar=null;
	enum KEYSTATE{DOWN,UP};
	private KEYSTATE upKey=KEYSTATE.UP;
	private KEYSTATE downKey=KEYSTATE.UP;
	private KEYSTATE leftKey=KEYSTATE.UP;
	private KEYSTATE rightKey=KEYSTATE.UP;
	private KEYSTATE spaceKey=KEYSTATE.UP;
	
	private Image imgLogo;
	private Image imgVictory;
	private Image imgFail;
	private Image imgMenuBack;
	private Image[] imgScene;
	private Mp3[] mp3Map;
	private Mp3 mp3Menu=new Mp3("res/menu.mp3");
	
	int gameLevel=0;
	
	JButton butStart=new JButton(new ImageIcon("res/butStart.png"));
	JButton butExit=new JButton(new ImageIcon("res/butExit.png"));
	ImagePanel panMenu=new ImagePanel();
	ImagePanel panClip=new ImagePanel();
	class ImagePanel extends JPanel{
		private static final long serialVersionUID = 1L;
		Image img;
		void setImage(Image image){img=image;}
		protected void paintComponent(Graphics g) {   
            g.drawImage(img, 0, 0, Game.windowWidth, Game.windowHeight, null);  
        } 		
	}
	
	public static void main(String[] args){
		gameRef=new Game();
	    JFrame frame = new JFrame("Magic Plain");

	    frame.addWindowListener(new WindowAdapter()
	    {
	        public void windowClosing(WindowEvent e)
	        {
	        System.exit(0);
	        }
	    });

	    frame.getContentPane().add(gameRef);
	   // frame.getContentPane().setLayout(new BorderLayout());
	   // frame.setLayout(new BorderLayout());
	  //  frame.setSize(600,500);
	  //  JOptionPane.showMessageDialog(null,ClassLoader.getSystemResource(""));
	    frame.setVisible(true);
	    gameRef.init();
	    gameRef.start();
	    frame.setSize(gameRef.getSize().width+frame.getInsets().left+frame.getInsets().right,
	    		gameRef.getSize().height+frame.getInsets().top+frame.getInsets().bottom);
	    
	}
	
	private void LoadData(){
		imgLogo=LoadImage("res/logo.gif");
		imgVictory=LoadImage("res/victory.png");
		imgFail=LoadImage("res/failed.png");
		imgMenuBack=LoadImage("res/menuback.png");
		imgScene=new Image[]{LoadImage("res/scene1.png"),
				LoadImage("res/scene2.png"),
				LoadImage("res/scene3.png"),
				LoadImage("res/scene4.png"),
				};
		mp3Map=new Mp3[]{new Mp3("res/map1.mp3"),new Mp3("res/map2.mp3"),
				new Mp3("res/map3.mp3"),new Mp3("res/map4.mp3")};
	}
	public Image LoadImage(String path){
		return getToolkit().getImage(path);
	}
	
	public void init(){
		//gameRef=this;
		LoadData();
		ListenerProc();
		
		panMenu.setLayout(null);
		butStart.setLocation(windowWidth/2, windowHeight*3/5);
		butStart.setSize(windowWidth/3,windowHeight/6);
		butExit.setLocation(windowWidth/2, windowHeight*4/5);
		butExit.setSize(windowWidth/3, windowHeight/6);
			panMenu.add(butStart);
		panMenu.add(butExit);
	
		this.setLayout(new GridLayout(1,1));
		panMenu.setVisible(false);
		panClip.setVisible(false);
		panMenu.setImage(imgMenuBack);
		
		swapImage=createImage(windowWidth,windowHeight);
		swapGraph=swapImage.getGraphics();
		
		Thread mainThread=new Thread(this);
		mainThread.start();	
	
	}
	class MouseEnter implements MouseListener{
		public void mouseClicked(MouseEvent arg0) {}
		public void mouseEntered(MouseEvent arg0) {
			Game.gameRef.PlaySound("res/butenter.wav");	
		}
		public void mouseExited(MouseEvent arg0) {}
		public void mousePressed(MouseEvent arg0) {
			Game.gameRef.PlaySound("res/butpressed.wav");
		}
		public void mouseReleased(MouseEvent arg0) {}
	}
	private void ListenerProc(){
		
		butStart.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent arg0) {
				Game.state=GAME_STATE.SHOWSCENE;
			}});
		butStart.addMouseListener(new MouseEnter());
		butExit.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent arg0) {
				Game.state=GAME_STATE.EXIT;
			}});
		butExit.addMouseListener(new MouseEnter());

		this.setFocusable(true);
		addKeyListener(new KeyListener(){
			public void keyPressed(KeyEvent e) {
				switch(e.getKeyCode()){
				case KeyEvent.VK_UP:
					upKey=KEYSTATE.DOWN;
					break;
				case KeyEvent.VK_DOWN:
					downKey=KEYSTATE.DOWN;
					break;
				case KeyEvent.VK_LEFT:
					leftKey=KEYSTATE.DOWN;
					break;
				case KeyEvent.VK_RIGHT:
					rightKey=KEYSTATE.DOWN;
					break;
				case KeyEvent.VK_SPACE:
					spaceKey=KEYSTATE.DOWN;
					break;
				}
			}
			public void keyReleased(KeyEvent e) {
				switch(e.getKeyCode()){
				case KeyEvent.VK_UP:
					upKey=KEYSTATE.UP;
					break;
				case KeyEvent.VK_DOWN:
					downKey=KEYSTATE.UP;
					break;
				case KeyEvent.VK_LEFT:
					leftKey=KEYSTATE.UP;
					break;
				case KeyEvent.VK_RIGHT:
					rightKey=KEYSTATE.UP;
					break;
				}
			}
			public void keyTyped(KeyEvent e) {
			}});
	}
	public void PlaySound(String snd){
		
		try {
			
		AudioStream as=new AudioStream(new FileInputStream(snd));
		AudioPlayer.player.start(as);
		}
		catch (Exception e) {}
	}
	public void start () {
		setSize(windowWidth,windowHeight);
	}
	private void GameInit(){
		try {
			mp3Map[gameLevel/4].Play();
			mapReader=new MapReader("dat/map"+(int)(gameLevel+1)+".txt");
			gameRes=new Resource();
			mainChar=gameRes.getMainChar();
			intellig=new Intelligence();
			
		} catch (SAXException | IOException | ParserConfigurationException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	private void GameRun(){
		
		gameRes.UpdateElems();
		if(upKey==KEYSTATE.DOWN)
			mainChar.Move(0);
		if(downKey==KEYSTATE.DOWN)
			mainChar.Move(1);
		if(leftKey==KEYSTATE.DOWN)
			mainChar.Move(2);
		if(rightKey==KEYSTATE.DOWN)
			mainChar.Move(3);
		if(spaceKey==KEYSTATE.DOWN){
			spaceKey=KEYSTATE.UP;
			gameRes.PutBomb(mainChar);
		}
		gameRes.MoveRobot();
		gameRes.CalcLife();
	}
	public void ShowLogo(){
		if(!panClip.isVisible()){
			panClip.setImage(imgLogo);
			this.add(panClip);
			panClip.setVisible(true);
			PlaySound("res/logo.wav");
		}
		if(timeCount>3000){
			state=GAME_STATE.SHOWMENU;
			panClip.setVisible(false);
			this.remove(panClip);
		}else if(timeCount>900){
			panClip.setImage(imgLogo);
		}
	}
	public void ShowScene(){
		if(!panClip.isVisible()){
			mp3Menu.Stop();
			if(panMenu.isVisible()){
				panMenu.setVisible(false);
				this.remove(panMenu);			
			}
			int sid=gameLevel/4;
			panClip.setImage(imgScene[sid]);
			this.add(panClip);
			panClip.setVisible(true);
			PlaySound("res/scene.wav");
			timeCount=0;
		}
		if(timeCount>1600){
			state=GAME_STATE.INIT;
			panClip.setVisible(false);
			this.remove(panClip);
		}
	}
	public void GameVictory(){
		//DrawImage(imgVictory,0,0,windowWidth,windowHeight);
		
		if(!panClip.isVisible()){
			panClip.setImage(imgVictory);
			this.add(panClip);
			panClip.setVisible(true);
			mp3Map[gameLevel/4].Stop();
			gameLevel=(gameLevel+1)%16;
			PlaySound("res/victory.wav");
			timeCount=0;
		}
		if(timeCount>3000){
			state=GAME_STATE.DESTROY;
			panClip.setVisible(false);
			this.remove(panClip);
		}
	}
	
	public void GameFail(){
		if(!panClip.isVisible()){
			panClip.setImage(imgFail);
			this.add(panClip);
			panClip.setVisible(true);
			mp3Map[gameLevel/4].Stop();
			PlaySound("res/fail.wav");
			timeCount=0;
		}
		if(timeCount>3000){
			state=GAME_STATE.DESTROY;
			panClip.setVisible(false);
			this.remove(panClip);
		}
	}
	
	public void GameDestory(){
		state=GAME_STATE.SHOWSCENE;
	}
	public void setGameFailed(){
		state=GAME_STATE.FAIL;
		
	}
	public void setGameVictory(){
		state=GAME_STATE.VICTORY;
	}
	public void run() {
		long starttime;
		while(state!=GAME_STATE.EXIT){
			starttime=System.currentTimeMillis();
			switch(state){
			case SHOWLOGO:
				ShowLogo();
				break;
			case SHOWMENU:
				if(!panMenu.isVisible()){
					mp3Menu.Play();
					this.add(panMenu);
					panMenu.setVisible(true);
				}
				break;
			case INIT:
				GameInit();
				state=GAME_STATE.RUN;
				break;
			case DESTROY:
				GameDestory();
				break;
			case RUN:
				GameRun();
				break;
			case VICTORY:
				GameVictory();
				break;
			case FAIL:
				GameFail();
				break;
			case SHOWSCENE:
				ShowScene();
				break;
			default:
				break;
			}
			repaint();
			try {
				long elapsedtime=System.currentTimeMillis()-starttime;
				if(elapsedtime<timeInterval)
					Thread.sleep(timeInterval-elapsedtime);
				else Thread.sleep(5);
				timeCount+=timeInterval;
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
		System.exit(0);
	}

	public void DrawImage(Image img,int sx,int sy,int sw,int sh){
		swapGraph.drawImage(img,sx, sy, sw, sh,this);
	}
	public void update (Graphics g){
		paint(swapGraph);
		if(state==GAME_STATE.RUN)
			Resource.DrawAll(swapGraph);
		g.drawImage(swapImage, 0, 0, windowWidth,windowHeight,this);
		swapGraph.clearRect(0, 0, windowWidth, windowHeight);
	}

}