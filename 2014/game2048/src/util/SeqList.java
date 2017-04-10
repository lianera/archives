package util;
/**
 * 顺序表类
 * @author duan
 * @version 1.0
 */
public class SeqList {
	private static final int defaultCapacity=8;
	private Object[] objs;
	private int size;
	public SeqList(){
		objs=new Object[defaultCapacity];
		size=0;
	}
	/**
	 * 往表中插入元素
	 * @param pos 位置，从0开始
	 * @param o 对象
	 * @warnings 未检查pos的值
	 */
	public void insert(int pos,Object o){
		//数组已满则扩容
		if(size==objs.length){
			expandCapacity();
		}
		//移动插入位置之后的元素
		for(int i=size;i>pos;i--)
			objs[i]=objs[i-1];
		objs[pos]=o;
		size++;
	}
	/**
	 * 插入到最后
	 * @param o
	 */
	public void insert(Object o){
		this.insert(size, o);
	}
	/**
	 * 返回指定位置的元素
	 * @param pos
	 * @return
	 */
	public Object get(int pos){
		return objs[pos];
	}
	/**
	 * 判断是否为空
	 * @return
	 */
	public boolean empty(){
		return size==0;
	}
	/**
	 * 取得大小
	 * @return
	 */
	public int getSize(){
		return size;
	}
	/**
	 * 扩容
	 * @warnings 未进行内存分配检查，有必要？
	 */
	private void expandCapacity(){
		Object[] newObjs=new Object[objs.length*2];//每次加倍，每次插入复杂度为log2(n)?j
		for(int i=0;i<objs.length;i++)	//复制
			newObjs[i]=objs[i];
		objs=newObjs;
	}
	
}
