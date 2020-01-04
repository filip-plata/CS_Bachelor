package Hotel;

import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;
import java.util.HashSet;

import Hotel.RoomAtributes.Color;
import Hotel.RoomAtributes.InteriorDesign;
import Hotel.RoomAtributes.WindowAlingment;



public class Room {
	private int ID;
	private int peopleCapacity;
	private int price;
	private RoomAtributes.InteriorDesign interiorDesign;
	private RoomAtributes.Color color;
	private RoomAtributes.WindowAlingment windowAlingment;
	private boolean internetAccess;
	private HashSet<Calendar> datesWhenReserved;
	
	public int getID(){
		return ID;
	}
	
	public int getPeopleCapacity(){
		return peopleCapacity;
	}
	
	public int getPrice(){
		return price;
	}
	
	public boolean getInternetAccess(){
		return internetAccess;
	}
	
	public RoomAtributes.InteriorDesign getInteriorDesign(){
		return interiorDesign;
	}
	
	public RoomAtributes.Color getColor(){
		return color;
	}
	
	public RoomAtributes.WindowAlingment getWindowAlingment(){
		return windowAlingment;
	}
	
	@Override
	public String toString(){
		return peopleCapacity + " " + price +" " + interiorDesign.toString() + " " + color.toString() +
				" " + windowAlingment.toString() + " " + ((internetAccess) ? "tak" : "nie");
	}
	
	public boolean isFree(String arrival, int stayPeriod){
		DateFormat formatter = new SimpleDateFormat("dd-MM-yyyy");
		Date date = null;
		boolean result = true;
		
		try{
			date = formatter.parse(arrival);
		}
		catch(ParseException e){
			System.out.println("Invalid date format!");
		}
		for (int i=0; i<stayPeriod; i++){
			Calendar calendar = Calendar.getInstance();
			calendar.setTime(date);
			calendar.add(Calendar.DAY_OF_MONTH, i);
			if (datesWhenReserved.contains(calendar)){
				result = false;
			}
			//System.out.println(calendar);
		}
		return result;
	}
	
	Room(RoomBuilder builder){
		ID = builder.ID;
		peopleCapacity = builder.peopleCapacity;
		price = builder.price;
		interiorDesign = builder.interiorDesign;
		color = builder.color;
		windowAlingment = builder.windowAlingment;
		internetAccess = builder.internetAccess;
		datesWhenReserved = new HashSet<Calendar>();
	}
	
	public void updateReservations(String arrival, int stayPeriod){
		DateFormat formatter = new SimpleDateFormat("dd-MM-yyyy");
		Date date = null;
		try{
			date = formatter.parse(arrival);
		}
		catch(ParseException e){
			System.out.println("Invalid date format!");
		}
		for (int i=0; i<stayPeriod; i++){
			Calendar calendar = Calendar.getInstance();
			calendar.setTime(date);
			calendar.add(Calendar.DAY_OF_MONTH, i);
			datesWhenReserved.add(calendar);
			//System.out.println(calendar);
		}
	}
	
	public static class RoomBuilder{
		private int ID;
		private int peopleCapacity;
		private int price;
		private RoomAtributes.InteriorDesign interiorDesign;
		private RoomAtributes.Color color;
		private RoomAtributes.WindowAlingment windowAlingment;
		private boolean internetAccess;
		
		public RoomBuilder(){
		}
		
		public RoomBuilder setColor(RoomAtributes.Color color){
			this.color = color;
			return this;
		}
		
		public RoomBuilder setInteriorDesign(RoomAtributes.InteriorDesign interiorDesign){
			this.interiorDesign = interiorDesign;
			return this;
		}
		
		public RoomBuilder setWindowAlingment(RoomAtributes.WindowAlingment windowAlingment){
			this.windowAlingment = windowAlingment;
			return this;
		}
		
		public RoomBuilder setPrice(int roomPrice){
			this.price = roomPrice;
			return this;
		}
		
		public RoomBuilder setPeopleCapacity(int peopleCapacity){
			this.peopleCapacity = peopleCapacity;
			return this;
		}
		
		public RoomBuilder setInternetAccess(boolean roomInternetAccess){
			this.internetAccess = roomInternetAccess;
			return this;
		}
		
		public RoomBuilder setID(int ID){
			this.ID = ID;
			return this;
		}
		
		public Room build() {
			return new Room(this);
		}
	}
	
	public Room(Room room, int ID){
		this.ID = ID;
		this.peopleCapacity = room.peopleCapacity;
		this.price = room.price;
		this.interiorDesign = room.interiorDesign;
		this.color = room.color;
		this.windowAlingment = room.windowAlingment;
		this.internetAccess = room.internetAccess;
		this.datesWhenReserved = room.datesWhenReserved;
	}
	
	@Override
	public boolean equals(Object x){
		if (x instanceof Room) {
			return (((Room) x).ID == this.ID);
		}
		else {
			return false;
		}
	}

}
