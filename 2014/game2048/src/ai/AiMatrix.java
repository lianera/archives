package ai;

import core.Direction;
import core.Element;
import core.Matrix;

/**
 * 用于AI研究的矩阵
 * @author duan
 * @version 1.1
 */
abstract public class AiMatrix extends Matrix{
	/**
	 * 获得最大值是为了归一化
	 */
	public static int maxScore=1966080;
	public static int maxStep=(int)((Matrix.Width*Matrix.Height-1)*Matrix.Width*Matrix.Height/2);
	
	public static double farPointX=1;//(Matrix.Width-1)/2.0;
	public static double farPointY=(Matrix.Height-1)/2.0;
	
	int score;
	int totalStep;
	/**
	 * 各个指标的权值
	 */
	/**
	 * 分数
	 */
	//private static double wtScore=0.1;
	/**
	 * 步数
	 */
	//private static double wtStep=-0.1;
	/**
	 * 空格数
	 */
	//private static double wtBlank=0.1;
	/**
	 * 最大数离中心的距离
	 */
	//private static double wtDistance=0.1;
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
/*	public static void setWeights(double weightScore,double weightStep,double weightBlank,double weightDistance,double weightDiff){
		wtScore=weightScore;
		wtStep=weightStep;
		wtBlank=weightBlank;
		wtDistance=weightDistance;
		wtDiff=weightDiff;
	}*/
	/**
	 * 评估函数
	 * 策略：
	 * 评估指标：总的分数、总的步数、空格数、最大的离中心的距离、相邻的数相差越小越好
	 * @return 评估值
	 */
	public double fitness(){
		if(dead())
			return Solution.INFINITY;
		Direction allDir[]={Direction.LEFT,Direction.UP,Direction.RIGHT,Direction.DOWN};
		int[][] m=this.toIntMatrix();
		int max=0;
		int maxX = 0,maxY = 0;		
		int maxValue=0;		//最大值
		int blankNum=0;		//空格数
		double smoothness=0;	//平滑度
		double monotonicity=0;	//单调性
		double[] diffDir=new double[allDir.length];//各个方向上的相邻单元倍数差值
		for(int i=0;i<Matrix.Height;i++){
			for(int j=0;j<Matrix.Width;j++){
				//找到最大的数
				if(m[i][j]>max){
					max=m[i][j];
					maxX=j;
					maxY=i;
				}				
				if(m[i][j]>maxValue){
					maxValue=m[i][j];
				}
				if(m[i][j]==0){
					blankNum++;//空格数
				}else{
					double logv=Math.log(m[i][j])/Math.log(2);
					//平滑度
					for(Direction d:allDir){
						Element e=neigbor(d,j,i);
						if(e!=null){
							smoothness-=Math.abs(logv-Math.log(e.getValue())/Math.log(2));
						}
					}
					//单调性
					for(int k=0;k<allDir.length;k++){
						Element e=neigbor(allDir[k],j,i);
						if(e!=null){
							double lognbv=Math.log(e.getValue())/Math.log(2);
							if(lognbv>logv)
								diffDir[k]+=logv-lognbv;
						}
					}

				}
			}
		}
		double maxNumDistance=(maxX-(Matrix.Width-1)/2.0)*(maxX-(Matrix.Width-1)/2.0)+
				(maxY-(Matrix.Height-1)/2.0)*(maxY-(Matrix.Height-1)/2.0);		
		
		double blanks=Math.log(blankNum)/Math.log(2);
		//全局单调性为四个方向上单调性的最大值
		monotonicity=Math.max(diffDir[0],diffDir[2])+Math.max(diffDir[1],diffDir[3]);
		// 评估值为各个 指标X权值的平均和
		double f=1.0*maxValue+0.1*smoothness+2.5*blanks+1.5*monotonicity+2.0*maxNumDistance;
		//System.out.println(max);
		return -f;
	}	
	
	/**
	 * 找到指定方向最近的非空单元
	 */
	Element neigbor(Direction d,int x,int y){
		switch(d){
		case LEFT:
			do{
				x--;
				if(x<0)
					return null;
			}while(elems[y][x].empty());
			break;
		case UP:
			do{
				y--;
				if(y<0)
					return null;
			}while(elems[y][x].empty());
			break;
		case RIGHT:
			do{
				x++;
				if(x>=Matrix.Width)
					return null;
			}while(elems[y][x].empty());
			break;
		case DOWN:
			do{
				y++;
				if(y>=Matrix.Height)
					return null;
			}while(elems[y][x].empty());
			break;
		}
		return elems[y][x];
	}
	
	abstract void generateChildren();
	abstract AiMatrix getNextChild(); 
	abstract boolean childrenLeft();
}
