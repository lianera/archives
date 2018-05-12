package test;

import ai.AiMatrix;
import ai.AiMoveMatrix;
import ai.Solution;
import core.Direction;
import core.Game;

/**
 * 对数据进行统计
 * @author duan
 *
 */
public class AiStatistic {
	public static final int Times=100;
	public static void main(String[] args){
		/**
		 * 总的分数、总的步数、空格数、最大数离中心的距离、相邻的数相差越小越好
		 */
		//AiMatrix.setWeights(2.0, 0.0, 2.0, 1.0, -2.0);
    	int maxScore=0;
    	int minScore=Integer.MAX_VALUE;
    	int avgScore=0;
    	int num2048=0;
    	int num4096=0;
    	int num8192=0;
    	int num16384=0;
    	int stepNum=0;
    	System.out.println("开始运行……");
    	long startMili=System.currentTimeMillis();
    	for(int i=0;i<Times;i++){
    		/**
    		 * 一下是一次完整的AI游戏过程，之记录最终分数
    		 */
        	Game game=new Game(false);
        	Solution sl=new Solution();
	    	while(!game.beenOver()){		
	    		Direction d=sl.produceNext(3, new AiMoveMatrix(game.getMatrix(),game.getScore(),game.getStep()));
	    		stepNum++;
	    		switch(d){
	    			case LEFT:
	    				game.moveLeft();
	    				break;
	    			case UP:
	    				game.moveUp();
	    				break;
	    			case RIGHT:
	    				game.moveRight();
	    				break;
	    			case DOWN:
	    				game.moveDown();
	    				break;
	    		}
	    	}
	    	System.out.println("已运行"+(i+1)+"局");
	    	//一次游戏结束
	    	int s=game.getScore();
	    	avgScore+=s;
	    	if(s>maxScore)
	    		maxScore=s;
	    	if(s<minScore)
	    		minScore=s;
	    	if(game.getMatrix().getMax()==2048)
	    		num2048++;
	    	if(game.getMatrix().getMax()==4096)
	    		num4096++;
	    	if(game.getMatrix().getMax()==8192)
	    		num8192++;
	    	if(game.getMatrix().getMax()==16384)
	    		num16384++;	    	
    	}
    	//全部结束
    	long endMili=System.currentTimeMillis();
    	long avgMili=(endMili-startMili)/stepNum;
    	avgScore/=Times;
    	System.out.println("最高分:"+maxScore);
    	System.out.println("最低分:"+minScore);
    	System.out.println("平均分:"+avgScore);
    	System.out.println("出现2048次数:"+num2048);
    	System.out.println("出现4096次数:"+num4096);
    	System.out.println("出现8192次数:"+num8192);
    	System.out.println("出现16384次数:"+num16384);
    	System.out.println("平均每步决策时间:"+avgMili+"(毫秒)");
	}
}
