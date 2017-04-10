package gui;

import java.awt.Graphics;
import java.awt.Image;

import javax.swing.JPanel;
/**
 * 矩阵面板专用类
 * @author duan
 * @version 1.0
 */
public class PerformPanel extends JPanel{

	private static final long serialVersionUID = -3683914630705948513L;
	private Image backImage;
	private Image[] tileImages;
	private Image buffImage;//缓冲图片，用来实现双缓冲
	private Graphics buffGrap;
	private int width,height;
	private int[][] matrix;
	/**
	 * 
	 * @param backImage 背景图像
	 * @param tiles	小格图像
	 * @throws Exception 
	 */
	public PerformPanel(Image backImg,Image[] tiles,int w,int h) throws Exception {
		backImage=backImg;
		tileImages=tiles;
		width=w;
		height=h;
		createBuffer();
	}
	/**
	 * 以图片文件名的方式初始化
	 * @param backFile 背景图片
	 * @param tileFiles 小格图片
	 * @throws Exception 
	 */
	public PerformPanel(String backFile,String[] tileFiles,int w,int h) throws Exception{
		backImage=getToolkit().getImage(backFile);
		tileImages=new Image[tileFiles.length];
		for(int i=0;i<tileFiles.length;i++){
			tileImages[i]=getToolkit().getImage(tileFiles[i]);
		}
		width=w;
		height=h;
		createBuffer();
	}
	/**
	 * 更新矩阵面板的数据
	 * @param m 注意大小要一致
	 */
	public void setMatrix(int[][] m){
		matrix=m;
	}
	/**
	 * 获得在矩阵指定坐标应呈现的图像
	 * @param x
	 * @param y
	 * @return 
	 * @return 
	 */
	private Image getTileImg(int x,int y){
		int f=matrix[y][x];
		int id=(int)(Math.log(f)/Math.log(2));
		return tileImages[id-1];
	}
	private void createBuffer(){
		this.setVisible(true);
		buffImage=createImage(width,height);
		buffGrap=buffImage.getGraphics();
	}
	/**
	 * 双缓冲技术，重画背景和小格
	 */
	protected void paintComponent(Graphics g) {
		int w=width;
		int h=height;
		int tw=w/Window.matWidth;
		int th=h/Window.matHeight;
		buffGrap.drawImage(backImage, 0, 0, w,h,null);
		for(int i=0;i<Window.matHeight;i++){
			for(int j=0;j<Window.matWidth;j++){
				buffGrap.drawImage(getTileImg(j,i), j*tw, i*th, tw, th,null);
			}
		}
		g.drawImage(buffImage, 0, 0, w, h, null);
		buffGrap.clearRect(0, 0, w, h);
	}
}
