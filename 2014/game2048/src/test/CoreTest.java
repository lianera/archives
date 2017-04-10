package test;
import java.util.Scanner;

import core.*;
/**
 * 本类用字符界面来测试core模块，输入L、U、R、D来表示方向。
 * @author duan
 *
 */
public class CoreTest {
	public static void main(String[] args){
    	Game game=new Game(false);
    	System.out.println(game);   
    	while(!game.beenOver()){		
    		@SuppressWarnings("resource")
			Scanner s = new Scanner(System.in);
    		String str=s.next();
			switch(str.charAt(0)){
				case 'l':case 'L':
					game.moveLeft();
				break;
				case 'u':case 'U':
					game.moveUp();
				break;
				case 'r':case 'R':
					game.moveRight();
				break;
				case 'd':case 'D':
					game.moveDown();
				break;
			}
    		System.out.println(game);   
    	}

	}
}
