package com.lianera.magicplain.element;

import java.util.Random;
import java.util.Stack;

import com.lianera.magicplain.ctrl.Game;
import com.lianera.magicplain.resource.Resource;

public class Robot extends Character{
	//int pathpos=1;
	Stack<Integer> pathx=null;
	Stack<Integer> pathy=null;
	public Robot(Animation[] animations,int gridX,int gridY){
		super(animations,gridX,gridY);
		pathx=new Stack<Integer>();
		pathy=new Stack<Integer>();
	}
	boolean findedpath=true;
	boolean randseek=false;

	protected void SeekAim(){
		int aimx=0, aimy=0;
		pathx.clear();pathy.clear();
		
		Prop p=Resource.getCloseProp(gx,gy);
		if(p!=null&&Math.abs(gx-p.getGridX())+Math.abs(gy-p.getGridY())<5&&
				(int)(Math.random()*2)==0){
			aimx=p.gx;
			aimy=p.gy;
		}else if(randseek){
 			Random rnd=new Random();
 			aimx=rnd.nextInt(Game.mapReader.getMapWidth());
 			aimy=rnd.nextInt(Game.mapReader.getMapHeight());
 			randseek=false;
		}else{
			aimx=Resource.getMainChar().getGridX();
			aimy=Resource.getMainChar().getGridY();
		}
		findedpath=Game.intellig.getMinPath(pathx,pathy,aimx, aimy, gx, gy);
		//System.out.println("Seeked:"+pathx.get(0)+","+pathy.get(0));
		
		if(pathx.size()<2){
			//走到主角身边或障碍物尽头
			Resource.PutBomb(this);
			randseek=true;
		}
	}
	public void Move(){
		if(moving)return;
		Prop p=Resource.getCloseProp(gx,gy);
		
		if((p!=null && Math.abs(gx-p.getGridX())+Math.abs(gy-p.getGridY())<2)){
			if(Resource.IsBlock(p.getGridX(), p.getGridY()))Resource.PutBomb(this);
			else Game.intellig.getMinPath(pathx,pathy,p.getGridX(), p.getGridY(), gx, gy);
		}
		if(Math.abs(gx-Resource.getMainChar().getGridX())+
				Math.abs(gy-Resource.getMainChar().getGridY())<2){
			Resource.PutBomb(this);
		}
		if(pathx.empty()||(int)(Math.random()*Resource.getCharNum())==0){
			SeekAim();
		}
		//Resource.PutBomb(this);
	/*	if(pathx.empty()){//没找到或距离为一
 			if(finded){//距离为1
				Resource.PutBomb(this);
	 			Random rnd=new Random();
	 			int rx=rnd.nextInt(Game.mapReader.getMapWidth());
	 			int ry=rnd.nextInt(Game.mapReader.getMapHeight());
	 			pathx.clear();pathy.clear();
	 			Game.intellig.getMinPath(pathx,pathy,rx, ry, gx, gy);
			}else{//没找到
				
			}
		}*/
		if(!pathx.empty()){
			int px=pathx.pop();
			int py=pathy.pop();
			int direction=0;
			if(py<gy)direction=0;
			if(py>gy)direction=1;
			if(px<gx)direction=2;
			if(px>gx)direction=3;
			Move(direction);
		}
	}
	
	public void Move(int direction){
		if(moving)return;
		moving=true;
		int x=gx,y=gy;
		switch(direction){
		case 0:y--;break;
		case 1:y++;break;
		case 2:x--;break;
		case 3:x++;break;
		default:return;
		}
		if(!Resource.Bumped(x, y)){
			gx=x;gy=y;
		}else{
		//	pathx.clear();
			//pathy.clear();
		//	Resource.PutBomb(this);
		}
		Resource.PicProp(this);
		direct=direction;
	}
}