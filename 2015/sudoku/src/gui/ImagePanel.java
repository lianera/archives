package gui;

import java.awt.Graphics;
import java.awt.Image;
import javax.swing.JPanel;
/**
 * 这只是一个带图片的Panel!
 * @author duan
 *
 */
public class ImagePanel extends JPanel{
	/**
	 * 
	 */
	private static final long serialVersionUID = -7910110413151813590L;
	protected Image img;
	public ImagePanel(Image image){
		img=image;
	}
	/**
	 * 从文件中加载图片
	 * @param file
	 */
	public ImagePanel(String file){
		super();
		img=Resource.loadImage(file);
	}
	/**
	 * 重写绘图函数，在绘图时绘制背景图
	 */
	protected void paintComponent(Graphics g) {
		super.paintComponent(g);
		g.drawImage(img, 0, 0, getWidth(), getHeight(), null);
	}

}
