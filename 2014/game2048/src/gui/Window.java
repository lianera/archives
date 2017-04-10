package gui;
import java.awt.BorderLayout;
import java.awt.Font;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;

import core.Game;
import core.Matrix;

/**
 * 窗口类，实现游戏运行主窗口
 * @author duan
 * @version 2.0
 */
public class Window extends JFrame implements KeyListener,MouseListener,MouseMotionListener{
	/**
	 * 
	 */
	private static final long serialVersionUID = -8076255828372462197L;
		//一些常量
		public static final int wndWidth=640;
		public static final int wndHeight=480;

		public static final int matWidth=Matrix.Width;
		public static final int matHeight=Matrix.Height;
		public static final String[] tileFiles={"res/2.png","res/4.png","res/8.png",
												"res/16.png","res/32.png","res/64.png",
												"res/128.png","res/256.png","res/512.png",
												"res/1024.png","res/2048.png","res/4096.png",
												"res/8192.png","res/16384.png",};
		//控件
		private JPanel pnMain;
		private JPanel pnPerform;
		private JLabel lbScore;
		private JButton btRestart;
		private JButton btAi;
		/**
		 * 窗口和控件的构造
		 * @throws Exception
		 */
		public Window() throws Exception{
			pnMain=new ImagePanel("res/mainback.png");
			pnPerform=new PerformPanel("res/back.jpg",tileFiles);
			lbScore=new JLabel("分数:");
			btRestart=new ImageButton("res/Restart-1.png","res/Restart-1.png","res/Restart-2.png");
			btAi=new ImageButton("res/Auto Run-1.png","res/Auto Run-1.png","res/Auto Run-2.png");
			//采用绝对布局
			getContentPane().setLayout(new BorderLayout(0, 0));
			getContentPane().add(pnMain);
			pnMain.setLayout(null);
			
			pnMain.add(pnPerform);
			pnMain.add(lbScore);
			pnMain.add(btRestart);
			pnMain.add(btAi);
			
			this.addKeyListener(this);
			this.addMouseListener(this);
			this.addMouseMotionListener(this);
			btRestart.addActionListener(new ActionListener(){
				@Override
				public void actionPerformed(ActionEvent e) {
					Game.theGame.restart();
				}
				
			});
			btAi.addActionListener(new ActionListener(){
				@Override
				public void actionPerformed(ActionEvent e) {
					Game.theGame.startAi();;
				}
				
			});			adjust();
			lbScore.setFocusable(false);
			btRestart.setFocusable(false);
			btAi.setFocusable(false);
			setFocusable(true);
		}
		/**
		 * 窗口、控件调整
		 */
		public void adjust(){
			this.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);	//关闭窗口则退出
			this.setResizable(false);	//防止调整大小
			this.setUndecorated(true);	//去掉标题栏
			//以下是一些大小位置调整
			this.setSize(wndWidth, wndHeight);
			final double consoleX=0.75;
			final double buttonW=0.2;
			final double buttonH=0.15;
			final double labelW=buttonW;
			final double labelH=buttonH;
			pnPerform.setLocation((int)(0.1*wndWidth), (int)(0.1*wndHeight));
			pnPerform.setSize	((int)(0.6*wndWidth), (int)(0.8*wndHeight));
			
			lbScore.setLocation((int)(consoleX*wndWidth), (int)(0.1*wndHeight));
			lbScore.setSize	((int)(labelW*wndWidth), (int)(labelH*wndHeight));
			
			lbScore.setFont(new Font("黑体",0,24));
			
			btRestart.setLocation((int)(consoleX*wndWidth), (int)(0.5*wndHeight));
			btRestart.setSize	((int)(buttonW*wndWidth), (int)(buttonH*wndHeight));
			
			btAi.setLocation((int)(consoleX*wndWidth), (int)(0.7*wndHeight));
			btAi.setSize	((int)(buttonW*wndWidth), (int)(buttonH*wndHeight));
			
			this.setVisible(true);
			
		}
		/**
		 * 根据数据更新窗口
		 * @param matrix 小格数字矩阵
		 * @param score 分数
		 */
		public void refresh(int[][] matrix,int score){
			((PerformPanel) pnPerform).setMatrix(matrix);
			lbScore.setText("分数:"+Integer.toString(score));
			pnPerform.repaint();
		}

		@Override
		public void keyPressed(KeyEvent e) {
			switch(e.getKeyCode()){
			case KeyEvent.VK_LEFT:
				Game.theGame.moveLeft();
				break;
			case KeyEvent.VK_UP:
				Game.theGame.moveUp();
				break;
			case KeyEvent.VK_RIGHT:
				Game.theGame.moveRight();
				break;
			case KeyEvent.VK_DOWN:
				Game.theGame.moveDown();
				break;
			}				
		}

		@Override
		public void keyReleased(KeyEvent arg0) {
		}

		@Override
		public void keyTyped(KeyEvent arg0) {
		}

		@Override
		public void mouseClicked(MouseEvent e) {
			
		}
		@Override
		public void mouseEntered(MouseEvent e) {
		}
		@Override
		public void mouseExited(MouseEvent e) {
		}
		/**
		 * 用于鼠标拖拽，请忽略
		 */
		private Point downPoint;
		private boolean drag=false; 	//窗口拖拽
		private boolean action=false;//鼠标移动动作
		/**
		 * 当鼠标在矩阵框时，实现鼠标动作移动，否则实现拖拽
		 */
		@Override
		public void mousePressed(MouseEvent e) {
			downPoint=e.getPoint();
			if(e.getX()>=pnPerform.getX()
				&& e.getX()<pnPerform.getX()+pnPerform.getWidth()
				&& e.getY()>=pnPerform.getY()
				&& e.getY()<pnPerform.getY()+pnPerform.getHeight()){
				action=true;
			}
			else {
				drag=true;
			}
		}
		@Override
		public void mouseReleased(MouseEvent e) {
			drag=false;
		}
		@Override
		public void mouseDragged(MouseEvent e) {
			/**
			 * 实现鼠标拖拽
			 */
			Point p = this.getLocation();
			if(drag){
				this.setLocation(p.x + e.getX() - downPoint.x, p.y + e.getY() - downPoint.y);
					
			}else if(action && e.getPoint().distance(downPoint.x,downPoint.y)>30){
				/**
				 * 实现动作
				 */
				int dx=e.getX()-downPoint.x;
				int dy=e.getY()-downPoint.y;
				if(Math.abs(dx)>Math.abs(dy)){//比较水平和垂直移动幅度
					if(dx<0)
						Game.theGame.moveLeft();
					else 
						Game.theGame.moveRight();
				}else{
					if(dy<0)
						Game.theGame.moveUp();
					else 
						Game.theGame.moveDown();
				}
				action=false;
			}
		}
		@Override
		public void mouseMoved(MouseEvent e) {
		}
}
