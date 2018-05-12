package core;
import gui.Window;

import java.awt.Dimension;
import java.awt.Toolkit;

import javax.swing.JOptionPane;

import ai.AiMoveMatrix;
import ai.Solution;
/**
 * 游戏类，用于模块的协调和程序的整体控制
 * @author duan
 * @version 1.0
 */
public class Game extends Thread{
	/**
	 * 全局只有一个Game对象
	 */
	public static Game theGame;
	Matrix mat;
	private int score;
	private int step;
	private GameState state;
	private Window wnd;
	/**
	 * 是否以图形界面运行
	 */
	private boolean guiMode;
	enum GameState{
		RUNNING,BEENOVER;
	}
	public Game(boolean gmode){
		//theGame=this;
		guiMode=gmode;
		try {
			if(guiMode){
				wnd=new Window();
				//获取屏幕大小
			    Dimension scrSize =Toolkit.getDefaultToolkit().getScreenSize();
			    //移动到屏幕中央
			    wnd.setLocation((int)(scrSize.getWidth()-wnd.getWidth())/2, 
			    		(int)(scrSize.getHeight()-wnd.getHeight())/2);
			}
			initGame();

		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	/**
	 * 重新开始游戏
	 */
	public void restart(){
		initGame();
	}
	/**
	 * 对游戏参数进行初始化，用于最开始或者重新开始游戏时
	 */
	private void initGame(){
		mat=new Matrix();
		score=0;	
		step=0;
		state=GameState.RUNNING;
		if(guiMode)
			wnd.refresh(mat.toIntMatrix(), score);
	}
	private void move(Direction d){
		if(state!=GameState.RUNNING)
			return;
		int r=mat.move(d);
		if(r>=0){	//移动有效时才进行加分
			score+=r;
			step++;
			if(mat.dead()){
				state=GameState.BEENOVER;
			}else{ 
				mat.putRandElement();
			}
		}
		if(mat.dead()){
			state=GameState.BEENOVER;
		}
		//数据改变了，需要更新窗口
		if(guiMode){
			//System.out.println(guiMode);
			wnd.refresh(mat.toIntMatrix(), score);
			if(state==GameState.BEENOVER)
				JOptionPane.showMessageDialog(null, "游戏结束！", "结束", JOptionPane.ERROR_MESSAGE);
		}
	}
	
	public void moveLeft(){
		move(Direction.LEFT);
	}
	public void moveUp(){
		move(Direction.UP);
	}
	public void moveRight(){
		move(Direction.RIGHT);
	}
	public void moveDown(){
		move(Direction.DOWN);
	}
	
	public boolean beenOver(){
		return state==GameState.BEENOVER;
	}
	public Matrix getMatrix(){
		return mat;
	}
	public int getScore(){
		return score;
	}
	public int getStep(){
		return step;
	}
	public String toString(){
		return mat+"当前得分:"+Integer.toString(score)+"   当前步数："+Integer.toString(step);
	}
	
	public void startAi(){
		Thread trd=new Game(true);
		((Thread) trd).start();
	}
	public void Ai(){
		//Game game=new Game(true);
    	Solution sl=new Solution();
		/**
		 *   总的分数、总的步数、空格数、最大数离中心的距离、相邻的数相差越小越好
		 */
    //	AiMatrix.setWeights(0.1, 0.0, 10.0, 1.0, -1.0);
    	System.out.println(this);   
    	while(!this.beenOver()){		
    		Direction d=sl.produceNext(1, new AiMoveMatrix(this.getMatrix(),this.getScore(),this.getStep()));
    		switch(d){
    			case LEFT:
    				this.moveLeft();
    				break;
    			case UP:
    				this.moveUp();
    				break;
    			case RIGHT:
    				this.moveRight();
    				break;
    			case DOWN:
    				this.moveDown();
    				break;
    		}
        	System.out.println("解决方案:"+d.toString());   
        	System.out.println(this);    
        	try {
				Thread.sleep(5);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}		
    	}
    	System.out.println("游戏结束！");      	
	}
	/**
	 * 正式的程序入口
	 * @param args
	 */
	public static void main(String[] args){
		theGame=new Game(true);
	}
	@Override
	public void run() {
		Ai();
	}
}
