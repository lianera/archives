package core;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

public class Matrix {
	private int[][] elems=null;
	private int[][] conflict=null;
	private int elemNum=0;
	Matrix(){
		elems=new int[9][9];
		conflict=new int[9][9];
		for(int i=0;i<9;i++){
			for(int j=0;j<9;j++){
				elems[i][j]=0;
				conflict[i][j]=0;
			}
		}
		elemNum=0;
	}
	/**
	 * 从流中（文件或键盘）读取数据
	 * @param is
	 * @throws IOException
	 */
	Matrix(InputStream is) throws IOException{
		elems=loadMatrix(is);
		elemNum=getElemsNum();
		//System.out.println(isComplete());
	}
	
	public int[][] getMatrix(){
		return elems;
	}
	public int[][] getConflicted(){
		return conflict;
	}
	/**
	 * 从文件流中加载
	 * @author duan
	 * @version 1.0
	 * @throws IOException 
	 */
	private int[][] loadMatrix(InputStream is) throws IOException{
		BufferedReader rd=new BufferedReader(new InputStreamReader(is));
		String line;
		
		conflict=new int[9][9];
		int[][] matrix=new int[9][9];
		for(int i=0;i<9;i++)
			for(int j=0;j<9;j++){
				matrix[i][j]=0;
				conflict[i][j]=0;
			}
		while((line=rd.readLine())!=null){
			line.trim();
			if(line.charAt(0)=='#')
				continue;
			String []nums=line.split(" ");
			if(nums.length!=3)
				throw new IOException("File format error on line \""+line+"\"");
			int y=Integer.parseInt(nums[0]);
			int x=Integer.parseInt(nums[1]);
			int n=Integer.parseInt(nums[2]);
			if(matrix[y][x]!=0)
				throw new IOException("File error because of duplication");
			matrix[y][x]=n;
		}
		return matrix;
	}
	public int getNum(){
		return elemNum;
	}
	public int[][] getElems(){
		return elems;
	}
	private int getElemsNum(){
		int n=0;
		for(int i=0;i<9;i++)
			for(int j=0;j<9;j++)
				if(elems[i][j]!=0)
					n++;
		return n;
	}

	public void setElem(int x,int y,int n){
		if(elems[y][x]==0 && n!=0)
			elemNum++;
		if(elems[y][x]!=0 && n==0)
			elemNum--;
		procConflict(x,y,n);		
		elems[y][x]=n;
	}
	
	public int getElem(int x,int y){
		return elems[y][x];
	}
	public boolean willConflict(int newx,int newy,int newval){
			int[] flag=new int[9];
		for(int f=0;f<9;f++)	//计数标记数组
			flag[f]=0;
		
		for(int j=0;j<9;j++){//判断行
			int v=elems[newy][j];
			if(flag[v]!=0)
				return true;
			flag[v]++;
		}	
		for(int i=0;i<9;i++){//判断列
			int v=elems[i][newx];
			if(flag[v]!=1)
				return true;
			flag[v]++;
		}			
		for(int m=newy;m<newy+3;m++){
			for(int n=newx;n<newx+3;n++){
				int v=elems[m][n];
				if(flag[v]!=2)
					return true;
				flag[v]++;
			}
		}		
		return false;		
	}
	public boolean hasConflicted(){
		for(int i=0;i<9;i++)
			for(int j=0;j<9;j++)
				if(conflict[i][j]!=0)
					return true;
		return false;
	}
	/**
	 * 判断是否胜利,用于人机，速度比较慢
	 * @return
	 */
	public boolean isComplete(){
		if(elemNum<81)	//是否达到个数
			return false;		
		

		
		for(int i=0;i<9;i++){
			for(int j=0;j<9;j++){
				if(conflict[i][j]!=0)
					return false;
			}
		}
		return true;
	}
	/**
	 * 处理冲突
	 * @param newx新加入的节点x
	 * @param newy新加入的节点y
	 * @param 新的值
	 * 
	 */
	public void procConflict(int newx,int newy,int newval){
		int oldval=elems[newy][newx];
		if(oldval==newval)
			return;
		
		for(int i=0;i<9;i++){//判断列
			if(i==newy)
				continue;
			if(elems[i][newx]==oldval)	{
				if(oldval!=0)
					conflict[newy][newx]--;
				if(elems[i][newx]!=0)
					conflict[i][newx]--;				
			}else if(elems[i][newx]==newval){
				if(newval!=0)
					conflict[newy][newx]++;
				if(elems[i][newx]!=0)
					conflict[i][newx]++;	
			}
		}
		for(int j=0;j<9;j++){//判断行
			if(j==newx)
				continue;
			if(elems[newy][j]==oldval){
				if(oldval!=0)
					conflict[newy][newx]--;
				if(elems[newy][j]!=0)
					conflict[newy][j]--;
			}else if(elems[newy][j]==newval){
				if(newval!=0)
					conflict[newy][newx]++;
				if(elems[newy][j]!=0)
					conflict[newy][j]++;
			}
		}
		
		//判断宫
		int nonx=newx/3*3;
		int nony=newy/3*3;
		for(int m=nonx;m<nonx+3;m++){
			for(int n=nony;n<nony+3;n++){
				if(m==newx && n==newy)
					continue;
				if(elems[n][m]==oldval){
					if(oldval!=0)
						conflict[newy][newx]--;
					if(elems[n][m]!=0)
						conflict[n][m]--;					
				}else if(elems[n][m]==newval){
					if(newval!=0)
						conflict[newy][newx]++;
					if(elems[n][m]!=0)
						conflict[n][m]++;		
				}
			}
		}
	}
	public String toString(){
		return Matrix.toString(elems);
	}
	public static String toString(int[][] mat){
		String s=new String();
		for(int i=0;i<9;i++){
			for(int j=0;j<9;j++){
				s+=Integer.toString(mat[i][j]);
				s+=',';
			}
			s+='\n';
		}
		return s;
	}
}
