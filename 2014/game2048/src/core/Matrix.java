package core;

import java.util.Random;

import util.SeqList;

/**
 * 矩阵类，能够进行移位合并操作以及规则检查。
 * @author duan
 * @version 2.0
 */
public class Matrix {
	public static final int Width=4;
	public static final int Height=4;
	protected Element[][] elems;
	private static Random rand=new Random();
	/**
	 * 默认构造函数初始化为游戏初始状态
	 */
	public Matrix(){
		elems=new Element[Height][Width];
		//初始化
		for(int i=0;i<Height;i++){
			for(int j=0;j<Width;j++){
				elems[i][j]=new Element(0);
			}
		}
		//随机产生两个元素
		putRandElement();
		putRandElement();

	}
	/**
	 * 复制构造函数（不产生元素）
	 * @param m
	 */
	public Matrix(Matrix m){
		elems=new Element[Height][Width];
		//初始化
		for(int i=0;i<Height;i++){
			for(int j=0;j<Width;j++){
				elems[i][j]=new Element(m.elems[i][j]);
			}
		}		
	}
	/**
	 * 在空缺处产生一个2或4的随机数
	 * @return 返回1：成功。返回0：没有空元素。
	 */
	public boolean putRandElement(){	
		SeqList blanks=new SeqList();
		//把空元素加入顺序表中
		for(int i=0;i<Height;i++){
			for(int j=0;j<Width;j++){
				if(elems[i][j].empty()){
					blanks.insert(elems[i][j]);
				}
			}
		}
		if(blanks.empty())	//没有空元素
			return false;
		//从表中选定一个位置
		int pos=rand.nextInt(blanks.getSize());
		//将该元素随机化
		((Element) blanks.get(pos)).randomize();
		return true;
	}

	/**
	 * 移动元素的函数，并根据游戏规则进行元素合并。
	 * @param d
	 * @return 返回-1，则表示为无效移动，否则返回本次得分。
	 */
	public int move(Direction d){	
		Element[] arr=new Element[Height];//用来复制待移动元素
		int r,score=0;
		boolean valid=false;
		switch(d){
		case LEFT:
			//对每行进行移动
			for(int i=0;i<Height;i++){
				r=moveLine(elems[i]);
				if(r!=-1){
					valid=true;
					score+=r;
				}
			}
			break;
		case UP:
			for(int j=0;j<Width;j++){
				for(int i=0;i<Height;i++){
					arr[i]=elems[i][j];
				}
				r=moveLine(arr);
				if(r!=-1){
					valid=true;
					score+=r;
				}
			}
			break;
		case RIGHT:
			for(int i=0;i<Height;i++){
				for(int j=0;j<Width;j++){
					arr[Width-1-j]=elems[i][j];
				}
				r=moveLine(arr);
				if(r!=-1){
					valid=true;
					score+=r;
				}
			}
			break;
		case DOWN:
			for(int j=0;j<Width;j++){
				for(int i=0;i<Height;i++){
					arr[Height-1-i]=elems[i][j];
				}
				r=moveLine(arr);
				if(r!=-1){
					valid=true;
					score+=r;
				}
			}
			break;
		}
		if(valid)
			return score;
		return -1;
	}
	/**
	 * 根据规则移动一行（列）
	 * @param arr
	 * @return 返回-1，则表示为无效移动，否则返回本次得分。
	 */
	private int moveLine(Element[] arr){
		boolean[] flag=new boolean[arr.length];	//标记是否合并过
		for(int n=0;n<arr.length;n++)	//清空标记
			flag[n]=false;
		int s=0;
		boolean valid=false;
		for(int i=1;i<arr.length;i++){
			if(arr[i].empty())
				continue;
			int k=i-1;
			while(k>=0 && arr[k].empty())
				k--;
			/**
			 * 此处可优化
			 */
			if(k<0 || !arr[k].equals(arr[i]) || flag[k]==true){	//最末端 或 遇到不同数字  或 遇到相同的但合并过
				if(k+1!=i){
					arr[k+1].swap(arr[i]);	//移动，防止自身被清空，所以用swap
					valid=true;
				}
			}else{	//遇到相等数字且未合并过
				arr[k].upgrade();//合并
				arr[i].clear();
				s+=arr[k].getValue();//加分
				flag[k]=true;	//设置标记
				valid=true;
			}
		}
		if(valid)
			return s;	//返回分数 
		return -1;
	}
	public int getMax(){
		int max=0;
		for(int i=0;i<Height;i++){
			for(int j=0;j<Width;j++){
				if(elems[i][j].getValue()>max)
					max=elems[i][j].getValue();
			}
		}
		return max;
	}
	/**
	 * 判断是否矩阵死亡。
	 * @return 1：死，0：活；
	 */
	public boolean dead(){
		//判断是否有相邻的相同元素
		for(int i=0;i<Height;i++){
			for(int j=0;j<Width;j++){
				if(elems[i][j].empty())
					return false;
				if(i+1<Height)
					if(elems[i][j].equals(elems[i+1][j]))
						return false;
				if(j+1<Width)
					if(elems[i][j].equals(elems[i][j+1]))
						return false;
			}
		}
		return true;
	}
	

	/**
	 * 返回空格数
	 * @return 空格数
	 */
	/*
	public int blankNum(){
		int n=0;
		for(int i=0;i<Height;i++){
			for(int j=0;j<Width;j++){
				if(elems[i][j].empty()){
					n++;
				}
			}
		}
		return n;
	}
	*/
	/**
	 * 将元素矩阵转化成4x4整数矩阵
	 * @return 4x4二维数组
	 */
	public int[][] toIntMatrix(){
		int[][] m=new int[Height][Width];
		for(int i=0;i<Height;i++){
			for(int j=0;j<Width;j++){
				m[i][j]=elems[i][j].getValue();
			}
		}
		return m;
	}
	
	/**
	 * 将矩阵转化成的字符串
	 */
	public String toString(){
		String line="-------------------------\n";
		String s = new String();
		s+=line;
		for(int i=0;i<Height;i++){
			s+="|";
			for(int j=0;j<Width;j++){
				s+=elems[i][j].toString()+"|";
			}
			s+="\n"+line;
		}
		return s;
	}


}
