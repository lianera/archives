package gui;

import java.awt.Graphics;
import java.awt.Image;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;

import javax.swing.JButton;
/**
 * Æ¯ÁÁµÄ°´Å¥
 * @author duan
 * @version 1.0
 */
public class ImageButton extends JButton{

	/**
	 * 
	 */
	private static final long serialVersionUID = -1575526908844025067L;
	/**
	 * Õý³£Í¼Ïñ
	 */
	protected Image normalImg;
	/**
	 * Êó±êÍ£ÁôÏÔÊ¾Í¼Ïñ
	 */
	protected Image mouseImg;
	/**
	 * Êó±êµã»÷ÏÔÊ¾Í¼Ïñ
	 */
	protected Image downImg;
	private enum ButtonState{
		Normal,Mouse,Down;
	}
	private ButtonState state;
	public ImageButton(Image normal,Image mouse,Image down){
		super();
		normalImg=normal;
		mouseImg=mouse;
		downImg=down;
		init();
		this.setOpaque(false);
	}
	public ImageButton(String normal,String mouse,String down){
		super();
		normalImg=Resource.loadImage(normal);
		mouseImg=Resource.loadImage(mouse);
		downImg=Resource.loadImage(down);
		init();
		this.setOpaque(false);
	}
	
	private void init(){
		state=ButtonState.Normal;
		this.addMouseListener(new MouseListener(){

			@Override
			public void mouseClicked(MouseEvent arg0) {
			}

			@Override
			public void mouseEntered(MouseEvent arg0) {
				state=ButtonState.Mouse;
			}

			@Override
			public void mouseExited(MouseEvent arg0) {
				state=ButtonState.Normal;
			}

			@Override
			public void mousePressed(MouseEvent arg0) {
				state=ButtonState.Down;
			}

			@Override
			public void mouseReleased(MouseEvent arg0) {
				state=ButtonState.Normal;
			}
			
		});
	}
	public void paint(Graphics g){
		Image img=normalImg;
		switch(state){
		case Normal:
			img=normalImg;
			break;
		case Mouse:
			img=mouseImg;
			break;
		case Down:
			img=downImg;
			break;
		}
		g.drawImage(img, 0, 0, getWidth(), getHeight(), null);	
	}
}
