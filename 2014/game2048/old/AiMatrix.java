package ai;

import core.Element;
import core.Matrix;

/**
 * 用于AI研究的矩阵
 * @author duan
 * @version 1.0
 */
abstract public class AiMatrix extends Matrix{
	/**
	 * 获得最大值是为了归一化
	 */
	public static int maxScore=1966080;
	public static int maxStep=(int)((Matrix.Width*Matrix.Height-1)*Matrix.Width*Matrix.Height/2);
	int score;
	int totalStep;
	/**
	 * 各个指标的权值
	 */
	/**
	 * 分数
	 */
	private static double wtScore=0.1;
	/**
	 * 步数
	 */
	private static double wtStep=-0.1;
	/**
	 * 空格数
	 */
	private static double wtBlank=0.1;
	/**
	 * 最大数离中心的距离
	 */
	private static double wtDistance=0.1;
	/**
	 * 相邻数倍数差之和(空格除外)
	 */
	private static double wtDiff=-0.1;
	public AiMatrix(Matrix m,int mScore,int mStep){
		super(m);
		score=mScore;
		totalStep=mStep;
	}
	public AiMatrix(AiMatrix am){
		super(am);
		score=am.score;
		totalStep=am.totalStep;
	}
	public boolean isEmptyElement(int x,int y){
		return elems[y][x].empty();
	}
	
	public void putElement(Element e,int x,int y){
		elems[y][x]=e;
	}	
	/**
	 * 设置权值
	 * @return 
	 */
	public static void setWeights(double weightScore,double weightStep,double weightBlank,double weightDistance,double weightDiff){
		wtScore=weightScore;
		wtStep=weightStep;
		wtBlank=weightBlank;
		wtDistance=weightDistance;
		wtDiff=weightDiff;
	}
	/**
	 * 评估函数
	 * 策略：
	 * 评估指标：总的分数、总的步数、空格数、最大的离中心的距离、相邻的数相差越小越好
	 * @return 评估值
	 */
	public double fitness(){
		if(dead())
			return -Solution.INFINITY;
		//分数
		int[][] m=this.toIntMatrix();
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
				if(v!=0){
					if(i+1<Matrix.Height && m[i+1][j]!=0){
						if(v>m[i+1][j])
							times=v/m[i+1][j];
						else
							times=m[i+1][j]/v;
					}
					if(j+1<Matrix.Width && m[i][j+1]!=0){
						if(v>m[i][j+1])
							times+=v/m[i][j+1];
						else 
							times+=m[i][j+1]/v;
					}
				}
				diff+=times;//相邻数倍数差
			}
		}
		//大数离中心距离的平方
		double maxNumDistance=(maxX-(Matrix.Width-1)/2.0)*(maxX-(Matrix.Width-1)/2.0)+
				(maxY-(Matrix.Height-1)/2.0)*(maxY-(Matrix.Height-1)/2.0);
		
		/*
		//归一化处理
		double evScore=(double)score/maxScore;
		double evStep=(double)totalStep/maxStep;
		double evBlank=(double)blankNum/(Matrix.Width*Matrix.Height-1);
		double evDistance=(double)maxNumDistance/((Matrix.Width-1)*(Matrix.Width-1)/4.0+(Matrix.Height-1)*(Matrix.Height-1)/4.0);
		double evDiff=(double)diff/(Matrix.Width*Matrix.Height*Matrix.Width*Matrix.Height*2);
		*/
		double evScore=score==0?0:(double)Math.log(score)/Math.log(2);
		double evStep=(double)totalStep;
		double evBlank=(double)blankNum;
		double evDistance=(double)maxNumDistance;
		double evDiff=(double)diff;

		/**
		 * 评估值为各个 指标X权值的平均和
		 */
		double f=(wtScore*evScore+wtStep*evStep+wtBlank*evBlank+wtDistance*evDistance+wtDiff*evDiff)/5;
		return -f;
	}	
	
	abstract void generateChildren();
	abstract AiMatrix getNextChild(); 
	abstract boolean childrenLeft();
}
