package ai;

import java.io.IOException;
import java.util.Vector;

import core.Matrix;

public class Producer {
	Vector<int[][]> matSet;
	private int[][][] available=new int[9][9][];//记录可填的数字,0表示可填
	public int[][] getMatrix(int index){
		return matSet.get(index);
	}
	/**
	 * 求一个题目的所有解，并保存起来
	 * @param mat 题目矩阵
	 * @return 解的个数
	 * @throws IOException
	 */
	public int Produce(Matrix mat) throws IOException{
		matSet=new Vector<int[][]>();
		int[][] tmpMatrix=new int[9][9];
		for(int i=0;i<9;i++)
			for(int j=0;j<9;j++)
				tmpMatrix[i][j]=mat.getElem(j, i);
		int direction=1;
		//深度优先遍历
		for(int n=0;n<=81 && n>=0;n+=direction){
			int x=n%9;
			int y=n/9;
			if(n>=81){
				//System.out.println(this);
				int[][] m=new int[9][9];
				for(int im=0;im<9;im++)
					for(int in=0;in<9;in++)
						m[im][in]=tmpMatrix[im][in];
				matSet.add(m);
				direction=-1;
				continue;
			}
			if(mat.getElems()[y][x]==0){//是否需要填
				//boolean hasAva=false;//有可填的
				//对于某个格子
				if(direction==1)
					available[y][x]=getAvailable(tmpMatrix,x,y);
				direction=-1;
				for(int k=tmpMatrix[y][x]+1;k<=9;k++){
					if(available[y][x][k]==0){//如果可填
						tmpMatrix[y][x]=k;
						//hasAva=true;
						direction=1;
						break;
					}
				}
				if(direction==-1)
					tmpMatrix[y][x]=mat.getElem(x, y);
			}

		}
		//System.out.println(Matrix.toString(tmpMatrix));
		return matSet.size();	
	}
	/**
	 * 获取可填的数字
	 * @param matrix
	 * @param x
	 * @param y
	 * @return 可填的数字数组
	 */
	private int[] getAvailable(int[][] matrix,int x,int y){
		int[] n=new int[10];
		for(int i=0;i<=9;i++)
			n[i]=0;
		//列
		for(int i=0;i<9;i++)
			n[matrix[i][x]]++;
		//行
		for(int j=0;j<9;j++)
			n[matrix[y][j]]++;
		int nonx=(x/3)*3;
		int nony=(y/3)*3;
		//宫
		for(int i=nony;i<nony+3;i++)
			for(int j=nonx;j<nonx+3;j++)
				n[matrix[i][j]]++;
		return n;
	}
	public String toString(){
		java.util.Iterator<int[][]> iter=matSet.iterator();
		String s=new String();
		while(iter.hasNext())
			s+=(Matrix.toString((int[][])iter.next())+"\n");
		return s;
	}
}
