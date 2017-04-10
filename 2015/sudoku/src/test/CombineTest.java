package test;

import gui.Window;

import java.io.FileNotFoundException;
import java.io.IOException;

import core.Game;

public class CombineTest {
	public static void main(String[] args) throws FileNotFoundException, IOException{
		Game.init();
		Window wnd;
		try {
			wnd = new Window();
			wnd.pnPerform.updateMatrix(Game.matrix.getMatrix());
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}		
	}
}
