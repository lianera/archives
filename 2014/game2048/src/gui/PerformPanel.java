package gui;

import java.awt.Graphics;
import java.awt.Image;
/**
 * 矩阵面板专用类
 * @author duan
 * @version 1.1
 */
public class PerformPanel extends ImagePanel{

	private static final long serialVersionUID = -3683914630705948513L;
	private Image[] tileImages;
	private int[][] matrix;
	/**
	 * 
	 * @param backImage 背景图像
	 * @param tiles	小格图像
	 * @throws Exception 
	 */
	public PerformPanel(Image backImg,Image[] tiles) {
		super(backImg);
		tileImages=tiles;
		matrix=null;
	}
	/**
	 * 以图片文件名的方式初始化
	 * @param backFile 背景图片
	 * @param tileFiles 小格图片
	 * @throws Exception 
	 */
	public PerformPanel(String backFile,String[] tileFiles){
		super(backFile);
		tileImages=new Image[tileFiles.length];
		for(int i=0;i<tileFiles.length;i++){
			tileImages[i]=Resource.loadImage(tileFiles[i]);
		}
		matrix=null;
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
		if(f==0)
			return null;
		int id=(int)(Math.log(f)/Math.log(2));
		return tileImages[id-1];
	}
	/**
	 * 重画背景和小格
	 */
	protected void paintComponent(Graphics g) {
		int w=getWidth();
		int h=getHeight();
		int tw=w/Window.matWidth;
		int th=h/Window.matHeight;
		super.paintComponent(g);
		if(matrix!=null){
			for(int i=0;i<Window.matHeight;i++){
				for(int j=0;j<Window.matWidth;j++){
					Image tileimg=getTileImg(j,i);
					if(tileimg!=null)
						g.drawImage(tileimg, j*tw, i*th, tw, th,null);
				}
			}
		}
	}
}
