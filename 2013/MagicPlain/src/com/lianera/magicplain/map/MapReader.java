package com.lianera.magicplain.map;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

import sun.misc.BASE64Decoder;


public class MapReader extends DefaultHandler {

	private List<TileType> tileTypes = null;
	private int currentTile = 0;
	private List<Layer> layers = null;
	private int curLayer = 0;
	private int width, height, tilewidth, tileheight;
	private List<OriginImage> originImg = null;
	private OriginImage backGroundImg=null;

	private String forName = null;
	private int firstgid=0;

	public String getBackgroundImage(){
		return backGroundImg.getSource();
	}
	public int getMapWidth() {
		return width;
	}
	public int getMapHeight() {
		return height;
	}
	public int getTileWidth(){
		return tilewidth;
	}
	public int getTileHeight(){
		return tileheight;
	}
	public List<TileType> getTileTypes(){
		return tileTypes;
	}
	public int getLayerNum(){
		return layers.size();
	}
	public int[] getData(int layer){
		return layers.get(layer).getData();
	}
	public int[] getPosById(int tileId){
		int sum=1;
		for(OriginImage img:originImg){
			if(sum+img.getTileNum()>tileId){
				int idInImg=tileId-sum;
				int[] loc=new int[]{idInImg%(img.getWidth()/tilewidth)*tilewidth,idInImg/(img.getHeight()/tileheight)*tileheight,
										tilewidth,tileheight};
				return loc;
			}
			sum+=img.getTileNum();
		}
		return null;
	}
	public OriginImage getImageById(int tileId){
		int sum=1;
		for(OriginImage img:originImg){
			sum+=img.getTileNum();
			if(sum>tileId)
				return img;
		}
		return null;
	}
	
	
	public List<OriginImage> getOriginImages(){
		return originImg;
	}
	public MapReader(String path) throws SAXException, IOException,
			ParserConfigurationException {
		InputStream xmlStream =new FileInputStream(path);
		SAXParserFactory factory = SAXParserFactory.newInstance();
		SAXParser parser = factory.newSAXParser();
		parser.parse(xmlStream, this);
		/*
		 * for(int i:curLayer.getIntArray()){ System.out.print(i+","); }
		 */
		xmlStream.close();
	}
	public void startDocument() throws SAXException {
		tileTypes = new ArrayList<TileType>();
		layers = new ArrayList<Layer>();
		originImg=new ArrayList<OriginImage>();
	}

	public void startElement(String uri, String localName, String qName,
			Attributes attributes) throws SAXException {

		if (qName.equals("map")) {
			width = Integer.parseInt(attributes.getValue(2));
			height = Integer.parseInt(attributes.getValue(3));
			tilewidth = Integer.parseInt(attributes.getValue(4));
			tileheight = Integer.parseInt(attributes.getValue(5));
		}else if(qName.equals("tileset")){
			firstgid=Integer.parseInt(attributes.getValue(0));
		}else if (qName.equals("image")) {
			if (forName.equals("tileset")) {
				originImg.add(new OriginImage(attributes.getValue(0),
						Integer.parseInt(attributes.getValue(1)),
											Integer.parseInt(attributes.getValue(2)),
											tilewidth,tileheight));
			} else if (forName.equals("imagelayer")) {
				backGroundImg.setSource(attributes.getValue(0));
			}
		} else if (qName.equals("tile")) {
			currentTile =tileTypes.size();
			tileTypes.add( new TileType(firstgid+Integer.parseInt(attributes.getValue(0))));
		} else if (qName.equals("property")) {
			tileTypes.get(currentTile).AddProperty(attributes.getValue(0),attributes.getValue(1));
			
		} else if (qName.equals("imagelayer")) {
			backGroundImg=new OriginImage();
			backGroundImg.setHW(Integer.parseInt(attributes.getValue(1)),
												Integer.parseInt(attributes.getValue(2)));
		} else if (qName.equals("layer")) {
			curLayer =layers.size();
			layers.add( new Layer(attributes.getValue(0),
					Integer.parseInt(attributes.getValue(1)),
					Integer.parseInt(attributes.getValue(2))));

		}

		forName = qName;
	}

	public void characters(char[] ch, int start, int length)
			throws SAXException {
		if (forName.equals("data")) {
			String content = new String(ch, start, length);
//			if (!content.equals("\n") && !content.equals("\n ")) {
				//content = content.substring(4, length - 3);
				content=content.replaceAll("\n\\s*", "");
				if(!content.isEmpty()){
					BASE64Decoder decoder = new BASE64Decoder();
					try {
						layers.get(curLayer).setData(decoder.decodeBuffer(content));
					} catch (Exception e) {
					
				}
	//			}
			}
		}
	}
}
