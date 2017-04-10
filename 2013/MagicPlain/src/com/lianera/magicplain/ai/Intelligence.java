package com.lianera.magicplain.ai;

import java.util.ArrayList;
import java.util.List;
import java.util.Stack;

import com.lianera.magicplain.resource.ELEMTYPE;

public class Intelligence {
	class MapNode{
		int x,y,f;
		MapNode(int mx,int my,int nf){
			x=mx;y=my;f=nf;
		}
	}
	class MapElem{
		int g;
		int parentx,parenty;
		MapElem(){
			g=0;parentx=-1;parenty=-1;
		}
	}
	
	ELEMTYPE[][] map=null;
	MapElem[][]	mapelem=null;
	public List<MapNode> openset = new ArrayList<MapNode>();  
	public List<MapNode> closeset = new ArrayList<MapNode>();  
	int[] childx={0,0,-1,1};
	int[] childy={-1,1,0,0};
	    
	public void setMap(ELEMTYPE[][] elemMatrix){
		map=elemMatrix;
		mapelem=new MapElem[map.length][map[0].length];
		InitMap();
		openset.clear();
		closeset.clear();
	}
	void InitMap(){
		for(int i=0;i<mapelem.length;i++)
			for(int j=0;j<mapelem[0].length;j++)
				mapelem[i][j]=new MapElem();
	}
	private int properValue(int dx,int dy,int sx,int sy){
		return Math.abs(dx-sx)+Math.abs(dy-sy);
	}
	private boolean CanGo(int x,int y){
		return x>=0&&x<map[0].length&&y>=0&&y<map.length &&
				map[y][x]!=ELEMTYPE.BLOCK && map[y][x]!=ELEMTYPE.BOMB;
	}
	/*private boolean NodeInSet(MapNode node,List<MapNode> set){
		for(MapNode sn:set){
			if(node.x==sn.x && node.y==sn.y)
				return true;
		}
		return false;
	}*/
	private boolean NodeInSet(int x,int y,List<MapNode> set){
		for(MapNode n:set)
			if(n.x==x&&n.y==y)
				return true;
		return false;
	}
	private MapNode MinNode(List<MapNode> set){
		MapNode b=set.get(0);
		for(int i=1;i<set.size();i++)
			if(b.f>set.get(i).f)
				b=set.get(i);
		return b;
	}
	
	public boolean getMinPath(Stack<Integer> pathx,Stack<Integer> pathy,int dx,int dy,int sx,int sy){
		
		InitMap();
		MapNode start=new MapNode(sx,sy,0);
		openset.add(start);
		mapelem[sy][sx].g=0;
		int curx=0,cury=0;
		boolean finded=false;
		while(!openset.isEmpty()&&!finded){
			MapNode minNode=MinNode(openset);
			
			openset.remove(minNode);
			closeset.add(minNode);
			curx=minNode.x;
			cury=minNode.y;
			for(int i=0;i<childx.length;i++){
				int cldx=minNode.x+childx[i];
				int cldy=minNode.y+childy[i];
				if(!CanGo(cldx,cldy)||NodeInSet(cldx,cldy,closeset))
					continue;
				if(cldx==dx&&cldy==dy){
					pathx.push(cldx);
					pathy.push(cldy);
					finded=true;
					break;
				}
				int newg=mapelem[cury][curx].g+1;
				MapNode cldNode=new MapNode(cldx,cldy,
						newg+properValue(dx,dy,cldx,cldy));
				if(!NodeInSet(cldx,cldy,openset)){
					openset.add(cldNode);
					mapelem[cldy][cldx].parentx=curx;
					mapelem[cldy][cldx].parenty=cury;
					mapelem[cldy][cldx].g=newg;
				}else{
					if(mapelem[cury][curx].g<mapelem[cldy][cldx].g){
						mapelem[cldy][cldx].parentx=curx;
						mapelem[cldy][cldx].parenty=cury;
						mapelem[cldy][cldx].g=newg;
					}
				}
			}
		}
		int px=curx,py=cury,ox,oy;
		while(px!=sx||py!=sy){
			pathx.push(px);
			pathy.push(py);
			ox=px;oy=py;
			if(px==-1||py==-1)break;
			px=mapelem[oy][ox].parentx;
			py=mapelem[oy][ox].parenty;
		} 
		return false;
	}
}
