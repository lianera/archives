package com.lianera.magicplain.resource;

import java.awt.Graphics;
import java.awt.Image;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import com.lianera.magicplain.ctrl.Game;
import com.lianera.magicplain.element.Animation;
import com.lianera.magicplain.element.Background;
import com.lianera.magicplain.element.Block;
import com.lianera.magicplain.element.Bomb;
import com.lianera.magicplain.element.Character;
import com.lianera.magicplain.element.Decorate;
import com.lianera.magicplain.element.Drawable;
import com.lianera.magicplain.element.Effect;
import com.lianera.magicplain.element.Element;
import com.lianera.magicplain.element.Explosion;
import com.lianera.magicplain.element.MainCharacter;
import com.lianera.magicplain.element.Picture;
import com.lianera.magicplain.element.Prop;
import com.lianera.magicplain.element.Robot;
import com.lianera.magicplain.map.OriginImage;
import com.lianera.magicplain.map.TileType;

public class Resource {

	public static Game gameRef = Game.gameRef;
	

	private static List<Image> images = null;
	private static Map<Integer, Image> elemImageMap = null;
	private static Map<Integer, TileType> idTileMap = null;
	private static List<Drawable> drawable = null;
	private static Element background=null;
	private static List<Block> blocks = null;
	private static List<Decorate> decorates = null;
	private static List<Character> chars = null;
	private static List<Bomb> bombs = null;
	private static List<Bomb> scrBombs = null;
	private static List<Effect> effects = null;
	private static List<Effect> scrEffects = null;
	private static List<Explosion> explosions = null;
	private static List<Explosion> scrExplosions = null;
	private static List<Prop> props = null;
	
	//private static List<Prop> scrProps = null;
	private static Map<ELEMTYPE,List> allelems=null;

	private static ELEMTYPE[][] elemMatrix = null;

	static int count=0;
	public Resource() {
		count=0;
		images = new ArrayList<Image>();
		elemImageMap = new HashMap<Integer, Image>();
		idTileMap = new TreeMap<Integer, TileType>();
		drawable = new ArrayList<Drawable>();

		blocks = new ArrayList<Block>();
		decorates=new ArrayList<Decorate>();
		chars = new ArrayList<Character>();
		bombs = new ArrayList<Bomb>();
		scrBombs = new ArrayList<Bomb>();
		effects=new ArrayList<Effect>();
		scrEffects=new ArrayList<Effect>();
		explosions = new ArrayList<Explosion>();
		scrExplosions = new ArrayList<Explosion>();
		props = new ArrayList<Prop>();
		//scrProps = new ArrayList<Prop>();
		elemMatrix = new ELEMTYPE[Game.mapReader.getMapHeight()][Game.mapReader.getMapWidth()];
		allelems=new TreeMap<ELEMTYPE,List>();
		//BLANK, BLOCK, PROP, BOMB, EXPLOSION, CHARACTER
		allelems.put(ELEMTYPE.BLOCK, blocks);
		allelems.put(ELEMTYPE.DECORATE, decorates);
		allelems.put(ELEMTYPE.CHARACTER, chars);
		allelems.put(ELEMTYPE.EXPLOSION, scrExplosions);
		allelems.put(ELEMTYPE.BOMB, scrBombs);
		allelems.put(ELEMTYPE.EFFECT, scrEffects);
		allelems.put(ELEMTYPE.PROP, props);
		LoadBackground();
		LoadImages();
		ReadMap();
		GameSetting();
	}
	public static Prop getCloseProp(int x,int y){
		int d=Integer.MAX_VALUE;
		Prop min=null;
		for(Prop p:props){
			int r=Math.abs(x-p.getGridX())+Math.abs(y-p.getGridY());
			if(r<d){
				min=p;
				d=r;
			}
		}
		return min;
	}
	public static Explosion getExplosion(int id){
		return explosions.get(id);
	}
	public static int getCharNum(){
		return chars.size();
	}
	public void GameSetting(){
		for(Character ch:chars)
			ch.SetBomb(bombs.get(0));
		Effect e=effects.get(2).clone();
		Character ch=getMainChar();
		e.setLocWithApper(ch.getGridX(), ch.getGridY());
		e.Enable(true);
		getMainChar().setFlag(e.getAni());
	
	}
	public static boolean IsBlock(int x,int y){
		return elemMatrix[y][x].equals(ELEMTYPE.BLOCK);
	}
	private static void LoadBackground() {
		// 加载背景
		background=new Background(new Picture(gameRef.LoadImage(Game.mapReader.getBackgroundImage()), 0,
				0,
				Game.mapReader.getMapWidth() * Game.mapReader.getTileWidth(),
				Game.mapReader.getMapHeight() * Game.mapReader.getTileHeight()));
	}

	private static void LoadImages() {
		// 加载图片
		int elemTop = 1, curTile = 0;
		List<TileType> tileTypes = Game.mapReader.getTileTypes();
		List<OriginImage> originImg = Game.mapReader.getOriginImages();
		for (OriginImage img : originImg) {
			elemTop += img.getTileNum();
			Image theImg = gameRef.LoadImage(img.getSource());
			images.add(theImg);
			TileType ct = null;
			while (curTile < tileTypes.size()
					&& (ct = tileTypes.get(curTile)).getId() < elemTop) {
				elemImageMap.put(ct.getId(), theImg);
				idTileMap.put(ct.getId(), ct);
				curTile++;
			}
		}

	}

	private static Animation BuildAnimation(int id) {
		TileType tile = idTileMap.get(id);
		if (tile == null)
			return null;
		Animation ani = new Animation(tile.getPropInt("timegap"));
		int next = 1;
		String aniprop = null;
		for (TileType tilenode = tile; tilenode != null; tilenode = idTileMap
				.get(next)) {
			aniprop = tilenode.getPropString("animation");
			Image img = elemImageMap.get(tilenode.getId());
			int[] pos = Game.mapReader.getPosById(tilenode.getId());
			if (img == null)
				break;
			((Animation) ani).Add(img, pos[0], pos[1], pos[2], pos[3]);
			int diffnext = tilenode.getPropInt("next");
			if (diffnext == 0)
				break;
			next = diffnext + tilenode.getId();
		}
		return ani;
	}

	private static Picture BuildPicture(int id) {
		Image img = elemImageMap.get(id);
		int[] pos = Game.mapReader.getPosById(id);
		return new Picture(img, pos[0], pos[1], pos[2], pos[3]);
	}

	public static void UpdateElems() {
		for (int i = 0; i < elemMatrix.length; i++) {
			Arrays.fill(elemMatrix[i], ELEMTYPE.BLANK);
		}
		for(ELEMTYPE et:allelems.keySet()){
			List<Element> ls=allelems.get(et);
			for(Iterator<Element> it=ls.iterator();it.hasNext();){
				Element e=(Element) it.next();
				if(e.Valid()){
					if(!(elemMatrix[e.getGridY()][e.getGridX()].equals(ELEMTYPE.BOMB)&&
							et.equals(ELEMTYPE.CHARACTER)) &&
							!elemMatrix[e.getGridY()][e.getGridX()].equals(ELEMTYPE.BLOCK)){
						elemMatrix[e.getGridY()][e.getGridX()]=et;
					}
				}else{
					if(et.equals(ELEMTYPE.BOMB)){
						gameRef.PlaySound("res/explosion.wav");
						Explosion exp = ((Bomb)e).GetExp();
						exp.Enable(true);
						scrExplosions.add(exp);
					}else if(et.equals(ELEMTYPE.CHARACTER)){
						Effect eff=effects.get(3).clone();
						eff.setLocWithApper(e.getGridX(), e.getGridY());
						eff.Enable(true);
						scrEffects.add(eff);
						if(getMainChar()==e){
							count++;
							if(count>=60)
								Game.gameRef.setGameFailed();
							return;
						}
					}
					it.remove();
				}
			}
		}
		Game.intellig.setMap(elemMatrix);
		
	}

	public static boolean Bumped(int gx, int gy) {
		if (gx < 0 || gx >= Game.mapReader.getMapWidth() || gy < 0
				|| gy >= Game.mapReader.getMapHeight())
			return true;
		if (elemMatrix[gy][gx].equals(ELEMTYPE.BLOCK)
				|| elemMatrix[gy][gx].equals(ELEMTYPE.BOMB))
			return true;
		return false;
	}
	public static boolean PicProp(Character ch){
		for(Prop p:props)
			if(p.getGridX()==ch.getGridX()&&p.getGridY()==ch.getGridY()){
				p.OnBumped(ch);
				Effect e=effects.get(0).clone();
				e.setLocWithApper(ch.getGridX(), ch.getGridY());
				e.Enable(true);
				scrEffects.add(e);
				return true;
			}
		return false;
	}

	private static void LoadElement(int id, int row, int col, int gw, int gh) {
		final TileType tile = idTileMap.get(id);
		if (tile == null)
			return;
		int rx = gw * col, ry = gh * row;
		if ("block".equals(tile.getPropString("type"))) {
			int life = tile.getPropInt("life");
			Block b = new Block(BuildPicture(id), col, row);
			b.setLife(life);
			blocks.add(b);
		}else if ("decorate".equals(tile.getPropString("type"))) {
				Decorate d = new Decorate(BuildPicture(id), col, row);
				decorates.add(d);
		} else if ("character".equals(tile.getPropString("type"))) {
			int walkup = tile.getPropInt("walkup");
			int walkdown = tile.getPropInt("walkdown");
			int walkleft = tile.getPropInt("walkleft");
			int walkright = tile.getPropInt("walkright");
			Animation[] ani = new Animation[] { BuildAnimation(id + walkup),
					BuildAnimation(id + walkdown),
					BuildAnimation(id + walkleft),
					BuildAnimation(id + walkright) };
			Character ch=chars.isEmpty()?new MainCharacter(ani, col, row):new Robot(ani, col, row);
			int life = tile.getPropInt("life");
			ch.setLife(life);
			int step=tile.getPropInt("step");
			ch.setStep(step);
			int strength=tile.getPropInt("step");
			ch.setStrength(strength);
			chars.add(ch);
		} else if ("prop".equals(tile.getPropString("type"))) {
			Prop pr=null;
			if ("addlife".equals(tile.getPropString("proptype"))){
					pr= new Prop(BuildAnimation(id),col, row){
						public void OnBumped(Character ch){
							int val=tile.getPropInt("value");
							ch.addLife(val);
							super.OnBumped(ch);
						}
					};
			}else if("addstep".equals(tile.getPropString("proptype"))){
				pr= new Prop(BuildAnimation(id),col, row){
					public void OnBumped(Character ch){
						int val=tile.getPropInt("value");
						ch.addStep(val);
						super.OnBumped(ch);
					}
				};
			}else if("addstrength".equals(tile.getPropString("proptype"))){
				pr= new Prop(BuildAnimation(id),col, row){
					public void OnBumped(Character ch){
						int val=tile.getPropInt("value");
						ch.addStrength(val);
						super.OnBumped(ch);
					}
				};
			}else if("setexplosion".equals(tile.getPropString("proptype"))){
				pr= new Prop(BuildAnimation(id),col, row){
					public void OnBumped(Character ch){
						int val=tile.getPropInt("value");
						ch.setExplosion(val);//.addStrength(val);
						super.OnBumped(ch);
					}
				};
			}

			props.add(pr);
		}

	}

	private static void ReadMap() {
		int mh = Game.mapReader.getMapHeight();
		int mw = Game.mapReader.getMapWidth();
		int gw = Game.mapReader.getTileWidth();
		int gh = Game.mapReader.getTileHeight();

		for (int n = 0; n < Game.mapReader.getLayerNum(); n++) {
			int[] data = Game.mapReader.getData(n);
			int val;
			for (int i = 0; i < mh; i++) {
				for (int j = 0; j < mw; j++) {
					int id = data[mw * i + j];
					if (id > 0) {
						LoadElement(id, i, j, gw, gh);
					}
				}
			}
		}
		for (int it : idTileMap.keySet()) {
			TileType ct = idTileMap.get(it);
			if ("bomb".equals(ct.getPropString("type"))) {
				int time = ct.getPropInt("exptime");
				int power = ct.getPropInt("power");
				Bomb b = new Bomb(BuildAnimation(ct.getId()), time,
						Game.mapReader.getTileWidth(),
						Game.mapReader.getTileHeight());
				b.setPower(power);
				bombs.add(b);
			} else if ("explosion".equals(ct.getPropString("type"))) {
				int time = ct.getPropInt("lasttime");
				String offx = ct.getPropString("offsetx");
				String[] xs = offx.split(",");
				int[] xi = new int[xs.length];
				for (int i = 0; i < xs.length; i++)
					xi[i] = Integer.parseInt(xs[i]);
				String offy = ct.getPropString("offsety");
				String[] ys = offy.split(",");
				int[] yi = new int[ys.length];
				for (int i = 0; i < ys.length; i++)
					yi[i] = Integer.parseInt(ys[i]);
				explosions.add(new Explosion(BuildAnimation(ct.getId()), xi,
						yi, time, Game.mapReader.getTileWidth(), Game.mapReader
								.getTileHeight()));
			}else if("effect".equals(ct.getPropString("type"))){
				Effect e=new Effect(BuildAnimation(ct.getId()));
				effects.add(e);
			}
		}
		// JOptionPane.showMessageDialog(null,
		// "弹出的是消息提示框!", "系统信息", JOptionPane.INFORMATION_MESSAGE);
	}

	public static void DrawAll(Graphics g) {
		background.Draw(g);
		for(ELEMTYPE et:allelems.keySet()){
			List<Element> ls=allelems.get(et);
			for(Element e:ls){
				e.Draw(g);
			}
		}
	}

	public static void PutBomb(Character ch) {
		int gx = ch.getGridX();
		int gy = ch.getGridY();
		for (Bomb bb : scrBombs) {
			if (bb.Valid() && bb.getGridX() == gx && bb.getGridY() == gy)
				return;
		}
		Bomb b = ch.PutBomb();
		scrBombs.add(b);
		//
		MoveRobot();
	}
	
	public static void MoveRobot(){
		Iterator<Character> it=chars.iterator();
		Character mch=it.next();
		while(it.hasNext()){
			Robot rbt=(Robot) it.next();
			rbt.Move();
			//Game.intellig.getMinPath(mch.getGridX(),mch.getGridY(),rbt.getGridX(),rbt.getGridY());
		}
	}

	public static void CalcLife() {
		int maprow = Game.mapReader.getMapHeight();
		int mapcol = Game.mapReader.getMapWidth();
		int[][] fireMatrix = new int[maprow][mapcol];
		for (int i = 0; i < maprow; i++) {
			Arrays.fill(fireMatrix[i], 0);
		}
		for (Explosion e : scrExplosions) {
				e.FillPowerMatrix(fireMatrix);
		}
		for(ELEMTYPE et:allelems.keySet()){
			if(!et.equals(ELEMTYPE.BLOCK) && !et.equals(ELEMTYPE.CHARACTER))
					//&& !et.equals(ELEMTYPE.PROP))
				continue;
			List<Element> ls=allelems.get(et);
			for(Element e:ls){
				int fire=fireMatrix[e.getGridY()][e.getGridX()];
				if(fire>0){
					e.getFired(fire);
					Effect eff=effects.get(1).clone();
					eff.setLocWithApper(e.getGridX(), e.getGridY());
					eff.Enable(true);
					scrEffects.add(eff);
				}
			}
		}
		if(chars.size()==1){
			count++;
			if(count>=60)
				gameRef.setGameVictory();
		}
	}

	public static MainCharacter getMainChar() {
		return (MainCharacter) chars.get(0);
	}

	public static int getGridWidth() {
		return Game.mapReader.getTileWidth();
	}

	public static int getGridHeight() {
		return Game.mapReader.getTileHeight();
	}

	public static Image getImageById(int id) {
		return elemImageMap.get(id);
	}
}
