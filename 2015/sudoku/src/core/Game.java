/**********************************************
 Copyright 2014 lianera (www.lianera.com)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
**********************************************/

package core;

import gui.Window;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

import javax.swing.JFileChooser;
import javax.swing.JOptionPane;

import ai.Producer;

public class Game {
	public static Matrix matrix=null;
	static Window wnd=null;
	static int solutionNum=0;
	static int currentSolution=0;
	private static long totleTime=0;
	static Producer currentProducer=null;
	
	public static void init() throws FileNotFoundException, IOException{
		matrix=new Matrix();
	}
	/**
	 * 从文件中加载题目
	 * @return是否成功
	 */
	public static boolean loadFromFile(){
		try {
			JFileChooser chooser = new JFileChooser();
			chooser.setCurrentDirectory(new File("./res/dataset"));
			if(chooser.showOpenDialog(null)==JFileChooser.APPROVE_OPTION){
				matrix=new Matrix(new FileInputStream(chooser.getSelectedFile()));
				if(wnd!=null){
					wnd.pnPerform.updateMatrix(matrix.getMatrix());
					wnd.pnPerform.setOrigin(matrix.getMatrix());
				}
				return true;				
			}
		} catch (FileNotFoundException e) {
			JOptionPane.showMessageDialog(null,"文件没有找到！");
		} catch (Exception e) {
			//e.printStackTrace();
			JOptionPane.showMessageDialog(null,"文件错误！");
		}
		return false;
	}
	/**
	 * 重置游戏
	 */
	public static void reset(){
		matrix=new Matrix();
		wnd.pnPerform.updateMatrix(matrix.getMatrix());
		wnd.pnPerform.setOrigin(null);
	}
	/**
	 * 开始运行人工智能
	 */
	public static void startAi(){
		Producer p=new Producer();
		try {
			if(matrix.hasConflicted())
				JOptionPane.showMessageDialog(null,"已经有错误了！");
			else{
					
				long startTime=System.currentTimeMillis();   //获取开始时间
				int num=p.Produce(matrix);
				long endTime=System.currentTimeMillis(); //获取结束时间
				long t=endTime-startTime;				
				
				if(num>0){
					totleTime=t;
					currentProducer=p;
					solutionNum=num;
					currentSolution=0;
					applyAiSolution(currentSolution);
				}else
					JOptionPane.showMessageDialog(null,"找不到可行的解！");
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
	/**
	 * 切换到下一个解
	 */
	public static void switchToNextSolution(){
		currentSolution++;
		applyAiSolution(currentSolution);
	}
	
	private static void applyAiSolution(int index){
		matrix=new Matrix();
		//wnd.pnPerform.updateMatrix(p.getMatrix());
		for(int i=0;i<9;i++)
			for(int j=0;j<9;j++)
				matrix.setElem(j, i,currentProducer.getMatrix(currentSolution)[i][j]);
		wnd.pnPerform.updateMatrix(matrix.getMatrix());
		wnd.updateInfo(solutionNum, currentSolution,totleTime);	
	}
	public static void main(String[] args) throws FileNotFoundException, IOException{
		Game.init();
		try {
			wnd = new Window();
			Game.reset();
		//	g.loadFromFile();
			wnd.pnPerform.updateMatrix(Game.matrix.getMatrix());
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}		
	}	
}
