package Hotel;

import static org.junit.Assert.assertEquals;

import java.util.Arrays;
import java.util.LinkedList;
import java.util.GregorianCalendar;

import Hotel.Client.Client;
import Hotel.Client.ClientBudget;
import Hotel.Client.ClientDovish;
import Hotel.Client.ClientHalf;
import Hotel.Client.ClientPerfectionist;
import Hotel.Client.ClientViewer;
import Hotel.DeskClerk.DeskClerk;
import Hotel.DeskClerk.NoFreeRooms;
import Hotel.DeskClerk.PerfectDeskClerk;
import Hotel.DeskClerk.SnarkyDeskClerk;

public class Hotel {

	private Room[] rooms;
	private DeskClerk[] deskClerks;

	Hotel(Room[] rooms, DeskClerk[] deskClerks) {
		if (rooms != null){
			this.rooms = rooms.clone();
		}
		else{
			this.rooms = null;
		}
		if (deskClerks != null){
			this.deskClerks = deskClerks.clone();
		}
		else{
			this.deskClerks = null;
		}
	}
	
	private void printOrderLog(DeskClerk receptionist, Order order, Room room, boolean success){
		System.out.println("Recepcjonista: " + receptionist.toString());
		System.out.println("Zamówienie: " + order.toString());
		if (room != null){
			System.out.println("Proponowany pokój: " + room.toString());
		}
		else {
			System.out.println("Proponowany pokój:");
		}
		System.out.println("Klient: " + order.getClient().toString());
		System.out.println((success) ? "tak" : "nie");
		System.out.println();
	}
	
	private boolean checkDataIntegrity(Room[] rooms, DeskClerk[] clerks) {
		
		/*
		 * Alternatywnie można posortować te dwi tablice
		 * i w ten sposób sprawdzać zawieranie się argumentów
		 * funkcji w atrybutach obiektu
		 */
		for (Room room : rooms) {
			boolean ret = false;
			for (Room room1 : this.rooms){
				if (room.equals(room1)){
					ret = true;
				}
			}
			if (!ret){
				return false;
			}
		}
		for (DeskClerk clerk : clerks){
			boolean ret = false;
			for (DeskClerk clerk1 : this.deskClerks) {
				if (clerk.equals(clerk1)) {
					ret = true;
				}
			}
			if (!ret) {
				return false;
			}
		}
		return true;
	}

	public void accept(Order[] orders, Room[] rooms, DeskClerk[] deskClerks) {
		LinkedList<Order> ordersList = new LinkedList<Order>(Arrays.asList(orders));
		LinkedList<DeskClerk> deskClerkList = new LinkedList<DeskClerk>(Arrays.asList(deskClerks));
		
		if (this.checkDataIntegrity(rooms, deskClerks)) {
			while (!ordersList.isEmpty()){
				Order orderToResolve = ordersList.removeFirst();
				orderToResolve.reheardAgain();
				DeskClerk receptionist = deskClerkList.removeFirst();
				Questionnaire poll = orderToResolve.getQuestionnaire();
				boolean wasAccepted = false;
				Room room = null;
				boolean noFreeRooms = false;
				
				if (!orderToResolve.reheardTooManyTimes()){
					try{
						room = receptionist.findRoom(poll, rooms);
						if (room != null){
							if (orderToResolve.getClient().clientAccepts(room, poll)){
								wasAccepted = true;
								room.updateReservations(poll.getArrivalDate(), poll.getStayPeriod());
							}
						}
					}
					catch (NoFreeRooms e) {
						noFreeRooms = true;
					}
					if (!wasAccepted && !noFreeRooms){
						ordersList.addLast(orderToResolve);
					}
				}
				printOrderLog(receptionist, orderToResolve, room, wasAccepted );
				deskClerkList.addLast(receptionist);
			}
		}
	}

	public static Order acceptOrder(Client client, Questionnaire questionnaire) {
		return new Order(client, questionnaire);
	}
	
	public static void main(String[] args){
		Room[] rooms = new Room[5];
		Order[] orders = new Order[8];
		DeskClerk[] clerks = new DeskClerk[2];
		
		DeskClerk perfect = new PerfectDeskClerk("Jan", "Emilian");
		Client client1 = new ClientDovish("Ugodowy", "Król");
		Client client2 = new ClientPerfectionist("Perfekcyjny", "Tadeusz");
		Client client3 = new ClientViewer("Widokowy", "Marek");
		Client client4 = new ClientHalf("Połowiczny", "Krystian");
		Client client5 = new ClientBudget("Budżetowy", "Cezary");
		
		clerks[0] = perfect;
		clerks[1] = new SnarkyDeskClerk("Złośliwy", "Mariusz");
		
		Questionnaire.QuestionnaireBuilder x = new Questionnaire.QuestionnaireBuilder();
		x.setInternetAccess(true);
		x.setWindowAlingment(RoomAtributes.WindowAlingment.Zachód);
		x.setColor(RoomAtributes.Color.MORSKI);
		x.setInteriorDesign(RoomAtributes.InteriorDesign.ORIENTALNY);
		x.setPeopleCapacity(3);
		x.setRoomPrice(100);
		x.setStayPeriod("28-2-1996", 2);
		//Questionnaire poll = x.build();
		
		orders[0] = new Order(client2, x.build());
		x.setColor(RoomAtributes.Color.SELEDYNOWY);
		x.setRoomPrice(1000);
		orders[1] = new Order(client1, x.build());
		x.setInternetAccess(false);
		orders[2] = new Order(client3, x.build());
		x.setPeopleCapacity(4);
		orders[3] = new Order(client4, x.build());
		x.setStayPeriod("3-3-1996", 4);
		orders[4] = new Order(client5, x.build());
		x.setStayPeriod("26-2-1996", 3);
		orders[5] = new Order(client3, x.build());
		x.setStayPeriod("26-2-1996", 3);
		x.setWindowAlingment(RoomAtributes.WindowAlingment.Wschód);
		orders[6] = new Order(client5, x.build());
		x.setStayPeriod("1-3-1996", 5);
		x.setRoomPrice(200);
		orders[7] = new Order(client2, x.build());
		x.setStayPeriod("2-3-1996", 6);
		
		Room.RoomBuilder builder = new Room.RoomBuilder();
		builder.setInteriorDesign(RoomAtributes.InteriorDesign.ORIENTALNY);
		builder.setColor(RoomAtributes.Color.SZARY);
		builder.setInternetAccess(false);
		builder.setPrice(200);
		builder.setWindowAlingment(RoomAtributes.WindowAlingment.Zachód);
		builder.setID(4);
		
		rooms[1] = builder.build();
		
		builder.setInteriorDesign(RoomAtributes.InteriorDesign.ORIENTALNY);
		builder.setColor(RoomAtributes.Color.MORSKI);
		builder.setInternetAccess(false);
		builder.setPrice(200);
		builder.setWindowAlingment(RoomAtributes.WindowAlingment.Wschód);
		builder.setID(1);
		builder.setPeopleCapacity(3);
		

		rooms[0] = builder.build();
		
		builder.setInteriorDesign(RoomAtributes.InteriorDesign.ORIENTALNY);
		builder.setColor(RoomAtributes.Color.MORSKI);
		builder.setInternetAccess(true);
		builder.setPrice(101);
		builder.setWindowAlingment(RoomAtributes.WindowAlingment.Zachód);
		builder.setID(2);
		builder.setPeopleCapacity(3);
		

		rooms[2] = builder.build();
		
		builder.setInteriorDesign(RoomAtributes.InteriorDesign.NOWOCZESNY);
		builder.setColor(RoomAtributes.Color.JASNOZIELONY);
		builder.setInternetAccess(false);
		builder.setPrice(800);
		builder.setWindowAlingment(RoomAtributes.WindowAlingment.Wschód);
		builder.setID(10);
		builder.setPeopleCapacity(3);
		
		rooms[3] = builder.build();
		
		builder.setInteriorDesign(RoomAtributes.InteriorDesign.ORIENTALNY);
		builder.setColor(RoomAtributes.Color.MORSKI);
		builder.setInternetAccess(true);
		builder.setPrice(75);
		builder.setWindowAlingment(RoomAtributes.WindowAlingment.Zachód);
		builder.setID(100);
		builder.setPeopleCapacity(3);
		

		rooms[4] = builder.build();
		
		rooms[2].updateReservations("2-3-1998", 10);
		rooms[3].updateReservations("1-3-1996", 140);
		
		Hotel hotel = new Hotel(rooms, clerks);
		hotel.accept(orders, rooms, clerks);
		
	}

}
