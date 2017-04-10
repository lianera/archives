package ai;

import core.Direction;
import core.Element;
import core.Matrix;

/**
 * AI类，用来产生步骤
 * @author duan
 *
 */
public class Solution {
	/**
	 * 最大分数，用于指标的归一化
	 */
	public static int maxScore=0;
	public static int maxStep=(int)((Matrix.Width*Matrix.Height-1)*Matrix.Width*Matrix.Height/2);

	public Solution(){
		if(maxScore==0){
			int gridnum=Matrix.Width*Matrix.Height;
			//maxScore=(int) Math.pow(2, gridnum+1);
			for(int i=0;i<gridnum;i++){
				maxScore+=(i)*Math.pow(2, i+1);
			}
		}
	}
	public Matrix produceNext(){
		
		return null;
		
	}
	
	/**
	 * alpha beta 算法，移动时找最大值，随机放置考虑最小值
	 * @param depth 搜索深度
	 * @param alpha
	 * @param beta
	 * @return
	 */
	private double AlphaBetaSearch(int depth,double alpha,double beta,boolean player,Matrix m,int score,int totalStep){
		if(depth==0)
			return 
	}
	
	/**
	 * 选择最坏放置情况下的极大值
	 * 如果糟糕透了，比现有的坏中最好的还坏，那就可以跳过
	 * @param depth 深度
	 * @param m 矩阵
	 * @param score 分数
	 * @param totalStep 步数
	 * @return 坏中最好的
	 */
	double maxMove(int depth,double alpha,double beta,Matrix m,int score,int totalStep){
		if (depth <= 0) { 
			return fitness(m.toIntMatrix(),score,totalStep); 
		} 
		if(m.dead())	//死都死了，不用继续了
			return Double.NEGATIVE_INFINITY;
		//产生下一步
		Direction[] dir={Direction.LEFT,Direction.UP,Direction.RIGHT,Direction.DOWN};
		for(int i=0;i<4;i++){
			Matrix newMat=new Matrix(m);
			int s=newMat.move(dir[i]);
			if(s<0)
				continue;
			//在最坏的放置中选择最好的
			double val=minPut(depth-1,alpha,beta,newMat,score+s,totalStep+1);
			if(val>alpha)
				alpha=val;
		}
		return alpha;
	}
	/**
	 * 总是随机放置，考虑最坏的情况
	 * 如果有一个比最差的还差，我方肯定不会考虑，放弃
	 * @param depth
	 * @param m
	 * @param score
	 * @param totalStep
	 * @return
	 */
	double minPut(int depth,double alpha,double beta,Matrix m,int score,int totalStep){
		if(depth<0)
			return fitness(m.toIntMatrix(),score,totalStep);
		//遍历
		for(int i=0;i<m.Height;i++){
			for(int j=0;j<m.Width;j++){
				if(m.isEmptyElement(j,i)){
					for(int r=2;r<=4;r+=2){
						Matrix newMat=new Matrix(m);
						m.putElement(new Element(r), j, i);
						maxMove(depth-1,alpha,beta,newMat,score,totalStep);
					}
				}
			}
		}
	}
	
	/**
	 * 评估函数
	 * 策略：
	 * 评估指标：总的分数、总的步数、空格数、最大的离中心的距离、相邻的数相差越小越好
	 * @return 评估值
	 */
	private double fitness(int[][] m,int score,int totalStep){
		//分数
		
		int blankNum=0;
		int max=0;
		int maxX = 0,maxY = 0;
		int diff=0;
		for(int i=0;i<Matrix.Height;i++){
			for(int j=0;j<Matrix.Width;j++){
				int v=m[i][j];
				if(v==0){
					blankNum++;//空格数
				}
				//找到最大的数
				if(v>max){
					max=v;
					maxX=j;
					maxY=i;
				}
				int times = 0;//得到每一个与右边，下边的倍数值
				if(i+1<Matrix.Height){
					if(v>m[i+1][j])
						times=v/m[i+1][j];
					else
						times=m[i+1][j]/v;
				}
				if(j+1<Matrix.Width){
					if(v>m[i][j+1])
						times+=v/m[i][j+1];
					else 
						times+=m[i][j+1]/v;
				}
				diff+=times;//相邻数倍数差
			}
		}
		//大数离中心距离的平方
		int maxNumDistance=(maxX-Matrix.Width/2)*(maxX-Matrix.Width/2)+
				(maxY-Matrix.Height/2)*(maxY-Matrix.Height/2);
		//归一化处理
		double evScore=(double)score/maxScore;
		double evStep=(double)totalStep/maxStep;
		double evBlank=(double)blankNum/(Matrix.Width*Matrix.Height-1);
		double evDistance=maxNumDistance/(Matrix.Width*Matrix.Width/4+Matrix.Height*Matrix.Height/4);
		double evDiff=diff/(Matrix.Width*Matrix.Height*Matrix.Width*Matrix.Height*2);
		double f=evScore-evStep+evBlank+evDistance-evDiff;
		return f;
	}
}
