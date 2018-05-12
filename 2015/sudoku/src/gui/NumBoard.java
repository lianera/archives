package gui;

import java.awt.Font;
import java.awt.Frame;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JDialog;

public class NumBoard extends JDialog implements ActionListener{
	/**
	 * 
	 */
	private static final long serialVersionUID = 2887752651626292137L;
	ImagePanel panel=null;
	private int number=0;
	private JButton[] buttons=null;
	public NumBoard(Frame fr,int x,int y){
		super(fr,"Number board",true);
		panel=new ImagePanel("/panel.png");
		ImageIcon imgGridNormal=Resource.loadImageIcon("/e1.png");
		ImageIcon imgGridEnter=Resource.loadImageIcon("/e2.png");
		ImageIcon imgGridPress=Resource.loadImageIcon("/e3.png");
		Font fnt=new Font("Arial black", Font.PLAIN, 32);
		buttons=new JButton[9];
		panel.setLayout(new GridLayout(3,3,0,0));
		for(int i=0;i<9;i++){
			JButton b=buttons[i];
			b=new JButton();
			b.setIcon(imgGridNormal);
			b.setRolloverIcon(imgGridEnter);
			b.setPressedIcon(imgGridPress);
			b.setFont(fnt);
			b.setHorizontalTextPosition(JButton.CENTER);
			
			b.setText(Integer.toString(i+1));
			b.addActionListener(this);
			panel.add(b);	
		}
		this.add(panel);
		this.setSize(180,180);
		this.setLocation(x,y);
		this.setResizable(false);	//防止调整大小
		this.setUndecorated(true);	//去掉标题栏		
		this.setVisible(true);
	}
	/**
	 * 获得用户选择的数字
	 * @return 选择的数字
	 */
	public int getNumber(){
		return number;
	}
	@Override
	public void actionPerformed(ActionEvent e) {
		number=Integer.parseInt(((JButton)e.getSource()).getText());
		this.dispose();
	}
}
