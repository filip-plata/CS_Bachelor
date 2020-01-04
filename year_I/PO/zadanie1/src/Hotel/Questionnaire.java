package Hotel;


public class Questionnaire {
	String arrivalDate;
	private int stayPeriod;
	private int forHowManyPeopleRoomIs;
	private int roomPrice;
	private RoomAtributes.Color roomColor;
	private RoomAtributes.InteriorDesign roomInteriorDesign;
	private RoomAtributes.WindowAlingment roomWindowAlingment;
	private boolean roomInternetAccess;
	
	public String getArrivalDate(){
		return arrivalDate;
	}
	
	public int getStayPeriod(){
		return stayPeriod;
	}
	
	public int numberOfRequierments(){
		return 6;
	}
	
	public int howManyRequierments(Room room){
		int result = 0;
		
		/*System.out.println("Test");
		System.out.println(room.getPeopleCapacity());
		System.out.println(forHowManyPeopleRoomIs);*/
		if (room.getPeopleCapacity() >= forHowManyPeopleRoomIs){
			//System.out.println("1");
			result++;
		}
		if (room.getPrice() <= roomPrice){
			//System.out.println("2");
			result++;
		}
		if (room.getColor() == roomColor){
			//System.out.println("3");
			result++;
		}
		if (room.getInteriorDesign() == roomInteriorDesign){
			//System.out.println("4");
			result++;
		}
		if (room.getWindowAlingment() == roomWindowAlingment){
			//System.out.println("5");
			result++;
		}
		if (room.getInternetAccess() == roomInternetAccess){
			//System.out.println("6");
			result++;
		}
		//System.out.println("");
		return result;
	}
	
	public RoomAtributes.WindowAlingment getWindowAlingment(){
		return roomWindowAlingment;
	}
	
	public int getPrice(){
		return roomPrice;
	}
	
	public static class QuestionnaireBuilder{
		String arrivalDate;
		private int stayPeriod;
		private int forHowManyPeopleRoomIs;
		private int roomPrice;
		private RoomAtributes.Color roomColor;
		private RoomAtributes.InteriorDesign roomInteriorDesign;
		private RoomAtributes.WindowAlingment roomWindowAlingment;
		private boolean roomInternetAccess;
		
		public QuestionnaireBuilder(){
		}
		
		public QuestionnaireBuilder setStayPeriod(String arrivalDate, int stayPeriod){
			this.arrivalDate = arrivalDate;
			this.stayPeriod = stayPeriod;
			return this;
		}
		
		public QuestionnaireBuilder setColor(RoomAtributes.Color color){
			this.roomColor = color;
			return this;
		}
		
		public QuestionnaireBuilder setInteriorDesign(RoomAtributes.InteriorDesign interiorDesign){
			this.roomInteriorDesign = interiorDesign;
			return this;
		}
		
		public QuestionnaireBuilder setWindowAlingment(RoomAtributes.WindowAlingment windowAlingment){
			this.roomWindowAlingment = windowAlingment;
			return this;
		}
		
		public QuestionnaireBuilder setPeopleCapacity(int forHowManyPeopleRoomIs){
			this.forHowManyPeopleRoomIs = forHowManyPeopleRoomIs;
			return this;
		}
		
		public QuestionnaireBuilder setRoomPrice(int price){
			this.roomPrice = price;
			return this;
		}
		
		public QuestionnaireBuilder setInternetAccess(boolean roomInternetAccess){
			this.roomInternetAccess = roomInternetAccess;
			return this;
		}
		
		public Questionnaire build() {
			return new Questionnaire(this);
		}
	}
	
	Questionnaire(QuestionnaireBuilder builder){
		this.arrivalDate = builder.arrivalDate;
		this.stayPeriod = builder.stayPeriod;
		this.forHowManyPeopleRoomIs = builder.forHowManyPeopleRoomIs;
		this.roomColor = builder.roomColor;
		this.roomInteriorDesign = builder.roomInteriorDesign;
		this.roomInternetAccess = builder.roomInternetAccess;
		this.roomWindowAlingment = builder.roomWindowAlingment;
		this.roomPrice = builder.roomPrice;
	}
	
	@Override
	public String toString(){
		return this.forHowManyPeopleRoomIs + " " + this.roomPrice +" " + this.roomInteriorDesign.toString() + 
				" " + this.roomColor.toString() + " " + this.roomWindowAlingment.toString() + 
				" " + ((this.roomInternetAccess) ? "tak" : "nie");
	}

}
