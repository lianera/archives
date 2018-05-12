package gui;

import java.awt.Color;
import java.awt.Font;
import java.awt.Frame;
import java.awt.GridLayout;
import java.awt.Image;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import core.Game;

/**
 * 矩阵面板专用类
 * @author duan
 * @version 1.1
 */
public class PerformPanel extends ImagePanel implements ActionListener{

	private static final long serialVersionUID = -3683914630705948513L;
	private GridButton[][] butMatrix;
	/**
	 * 以图片文件名的方式初始化
	 * @param backFile 背景图片
	 * @param tileFiles 小格图片
	 * @throws Exception 
	 */
	public PerformPanel(String backFile){
		super(backFile);
		init();
	}
	public PerformPanel(Image imgPanel) {
		super(imgPanel);
		init();
	}
	private void init(){
		this.setLayout(new GridLayout(9,9,1,1));
		butMatrix=new GridButton[9][9];	
		
		for(int i=0;i<9;i++){
			for(int j=0;j<9;j++){
				butMatrix[i][j]=new GridButton();
				butMatrix[i][j].setChangable(true);
				butMatrix[i][j].setFont(new Font("Arial black", Font.PLAIN, 26));
				
				butMatrix[i][j].setHorizontalTextPosition(JButton.CENTER);
				this.add(butMatrix[i][j]);
				butMatrix[i][j].x=j;
				butMatrix[i][j].y=i;
				
				butMatrix[i][j].addActionListener(this);
			}
		}
		updateMatrix(null);		
	}
	/**
	 * 更新矩阵面板的数据
	 * @param m 注意大小要一致
	 */
	public void updateMatrix(int[][] m){
		if(m==null)
			return;
		int[][] cnf=Game.matrix.getConflicted();
		for(int i=0;i<9;i++){
			for(int j=0;j<9;j++){
				butMatrix[i][j].setText((m[i][j]==0)?"":Integer.toString(m[i][j]));
				butMatrix[i][j].setForeground(cnf[i][j]==0?Color.BLACK:Color.RED);
			}
		}
	}
	/**
	 * 处理选择按钮事件
	 */
	@Override
	public void actionPerformed(ActionEvent e) {
		GridButton theButton=(GridButton) e.getSource();
		if(!theButton.getChangable())
			return;
		Point p=theButton.getLocation();
		Frame main= (Frame) this.getRootPane().getParent();
		Point fp=main.getLocation();
		Point sp=new Point(fp.x+p.x,fp.y+p.y);
		NumBoard nb=new NumBoard(main, sp.x,sp.y);
		int n=nb.getNumber();
		int bx=theButton.x;
		int by=theButton.y;
		theButton.setText(Integer.toString(n));
		
		Game.matrix.setElem(bx, by, n);
		int[][] cnf=Game.matrix.getConflicted();
		for(int i=0;i<9;i++){
			for(int j=0;j<9;j++){
				butMatrix[i][j].setForeground(cnf[i][j]==0?Color.BLACK:Color.RED);
			}
		}
	}
	/**
	 * 设置题目的初始状态，用于防止被玩家更改
	 * @param ableMatrix
	 */
	public void setOrigin(int[][] ableMatrix){
		for(int i=0;i<9;i++){
			for(int j=0;j<9;j++){
				butMatrix[i][j].setChangable(ableMatrix==null || ableMatrix[i][j]==0);
			}
		}
	}

}
