package ai;

import core.Direction;

/**
 * AI类，用来产生步骤
 * @author duan
 * @version 1.1
 */
public class Solution {
	public static int INFINITY=9999999;
	Direction theBest;
	private int topLevel;
	public Solution(){
	}
	public Direction produceNext(int depth,AiMatrix m){
		topLevel=depth;
		//AlphaBetaSearch(depth,-INFINITY,INFINITY,m);
		GreedySearch(m);
		return theBest;
	}
	
	/**
	 * alpha beta 递归算法，移动时找最大值，随机放置考虑最小值
	 * @param depth 搜索深度
	 * @param alpha
	 * @param beta
	 * @return
	 */
	private double AlphaBetaSearch(int depth,double alpha,double beta,AiMatrix m){
		if(depth==0){
			/*if(m instanceof AiMoveMatrix)
				return -m.fitness();
			else */
				return m.fitness();
		}
		m.generateChildren();
		while(m.childrenLeft()){
			AiMatrix child=m.getNextChild();
			if(child==null)
				continue;
			double childfit=-AlphaBetaSearch(depth-1,-beta,-alpha,child);
			if(childfit>=beta){
				return beta;
			}
			if(childfit>=alpha){
				alpha=childfit;
				if(depth==topLevel)
					theBest=((AiMoveMatrix) m).getCurrentDirection();
			}
		}
		return alpha;
	}
	private double GreedySearch(AiMatrix m){
		double maxfit=-INFINITY;
		m.generateChildren();
		while(m.childrenLeft()){
			AiMatrix child=m.getNextChild();
			if(child==null)
				continue;
			double childfit=-child.fitness();	
			if(childfit>maxfit){
				maxfit=childfit;
				theBest=((AiMoveMatrix) m).getCurrentDirection();
			}
		}
		return maxfit;		
	}/*
	private double GreedySearch2(AiMatrix m,int depth){
		double maxfit=-INFINITY;
		m.generateChildren();
		AiMatrix maxChild=null;
		while(m.childrenLeft()){
			AiMatrix child=m.getNextChild();
			if(child==null)
				continue;
			GreedySearch2(child,depth-1);
			if(maxChild==null)
				maxChild=child;
			double childfit=-child.fitness();	
			if(childfit>maxfit){
				maxfit=childfit;
				maxChild=((AiMoveMatrix) m).getCurrentDirection();
			}
		}
		GreedySearch2(maxChild,depth-1);
		theBest=((AiMoveMatrix) m).getCurrentDirection();
		return maxfit;		
	}*/
	
}
