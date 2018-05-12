package core;

import java.util.Random;

/**
 * 矩阵上的元素
 * @author duan
 * @version 2.0
 */
public class Element {
	private int figure;
	private static Random r=new Random();
	/**
	 * 默认初始化为空
	 */
	public Element(){
		figure=0;
	}
	public Element(int n){
		figure=n;
	}
	public Element(Element e){
		this.figure=e.figure;
	}
	public int getValue(){
		return figure;
	}
	/**
	 * 随机化，随机变成2或4
	 * 查文献后发现2和4概率为9:1
	 */
	public void randomize(){
		//figure=(r.nextInt(2)+1)*2;
		figure=r.nextInt(10)==0?4:2;
	}
	/**
	 * 判断是否为空
	 * @return true则为空，否则非空
	 */
	public boolean empty(){
		return figure==0;
	}
	/**
	 * 清空
	 */
	public void clear(){
		figure=0;
	}
	/**
	 * 与目标元素交换值
	 * @param e 目标元素
	 */
	public void swap(Element e){
		int tmp=e.figure;
		e.figure=this.figure;
		this.figure=tmp;
	}
	/**
	 * 元素数值加倍
	 */
	public void upgrade(){
		figure*=2;
	}
	public boolean equals(Object obj){
		if(obj instanceof Integer)	
			return figure==(Integer)obj;
		else if(obj instanceof Element)
			return this.figure==((Element)obj).figure;
		return false;
	}
	public String toString(){
		if(empty())
			return "     ";
		return String.format("%-5d", figure);
	}

}
