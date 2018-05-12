package com.lianera.magicplain.element;

import com.lianera.magicplain.ctrl.Game;

public class Prop extends Element{
	public Prop(Drawable drable,int gridX,int gridY){
		super(drable);
		super.setLocWithApper(gridX, gridY);
		life=1;
	}
	public void OnBumped(Character ch){
		visible=false;
		work=false;
		Game.gameRef.PlaySound("/getprop.wav");
	}
}