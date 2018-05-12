package test;

import ai.AiMatrix;
import ai.AiMoveMatrix;
import ai.Solution;
import core.Direction;
import core.Game;

public class AiTest {
	public static void main(String[] args){
    	Game game=new Game(true);
    	Solution sl=new Solution();
		/**
		 *   总的分数、总的步数、空格数、最大数离中心的距离、相邻的数相差越小越好
		 */
    //	AiMatrix.setWeights(0.1, 0.0, 10.0, 1.0, -1.0);
    	System.out.println(game);   
    	while(!game.beenOver()){		
    		Direction d=sl.produceNext(1, new AiMoveMatrix(game.getMatrix(),game.getScore(),game.getStep()));
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
        	System.out.println("解决方案:"+d.toString());   
        	System.out.println(game);    
        	try {
				Thread.sleep(200);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
    	}

	}
}
