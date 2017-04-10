package test;

import java.io.FileNotFoundException;
import java.io.IOException;

import ai.Producer;
import core.Game;

public class AiTest {
	public static void main(String[] args) throws FileNotFoundException, IOException{
		Game.init();;
		if(Game.loadFromFile()){
			Producer p=new Producer();
			System.out.println("查找中...");
			long startTime=System.currentTimeMillis();   //获取开始时间
			p.Produce(Game.matrix);
			long endTime=System.currentTimeMillis(); //获取结束时间
			System.out.println("查找结束!");
			System.out.println(p);
			System.out.println("所用时间： "+(endTime-startTime)+"ms");
		}
	}	
}
