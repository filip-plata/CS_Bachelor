package Hotel.DeskClerk;

import static org.junit.Assert.*;
import org.junit.Test;

import Hotel.Questionnaire;
import Hotel.Room;
import Hotel.RoomAtributes;
import Hotel.Questionnaire.QuestionnaireBuilder;
import Hotel.Room.RoomBuilder;
import Hotel.RoomAtributes.Color;
import Hotel.RoomAtributes.InteriorDesign;
import Hotel.RoomAtributes.WindowAlingment;





public class DeskClerkTest {

	@Test
	public void testAproximatingClerk() {
		Room[] rooms = new Room[3];
		DeskClerk aproximating = new AproximatingDeskClerk("Jan", "Emilian");
		assertEquals("Przedstawianie się źle!", "Jan Emilian, aproksymacyjna.", aproximating.toString());
		
		Questionnaire.QuestionnaireBuilder x = new Questionnaire.QuestionnaireBuilder();
		x.setInternetAccess(true);
		x.setWindowAlingment(RoomAtributes.WindowAlingment.Zachód);
		x.setColor(RoomAtributes.Color.MORSKI);
		x.setInteriorDesign(RoomAtributes.InteriorDesign.ORIENTALNY);
		x.setPeopleCapacity(3);
		x.setRoomPrice(100);
		x.setStayPeriod("3-3-1998", 2);
		Questionnaire poll = x.build();
		
		Room.RoomBuilder builder = new Room.RoomBuilder();
		builder.setInteriorDesign(RoomAtributes.InteriorDesign.ORIENTALNY);
		builder.setColor(RoomAtributes.Color.SZARY);
		builder.setInternetAccess(false);
		builder.setPrice(200);
		builder.setPeopleCapacity(3);
		builder.setWindowAlingment(RoomAtributes.WindowAlingment.Zachód);
		builder.setID(4);
		
		rooms[1] = builder.build();
		
		builder.setInteriorDesign(RoomAtributes.InteriorDesign.ORIENTALNY);
		builder.setColor(RoomAtributes.Color.MORSKI);
		builder.setInternetAccess(false);
		builder.setPrice(200);
		builder.setPeopleCapacity(3);
		builder.setWindowAlingment(RoomAtributes.WindowAlingment.Wschód);
		builder.setID(1);
		

		rooms[0] = builder.build();
		
		builder.setInteriorDesign(RoomAtributes.InteriorDesign.ORIENTALNY);
		builder.setColor(RoomAtributes.Color.MORSKI);
		builder.setInternetAccess(true);
		builder.setPrice(99);
		builder.setPeopleCapacity(3);
		builder.setWindowAlingment(RoomAtributes.WindowAlingment.Zachód);
		builder.setID(2);
		

		rooms[2] = builder.build();
		
		rooms[2].updateReservations("2-3-1998", 10);
		
		
		try{
			assertEquals(aproximating.findRoom(poll, rooms), rooms[0]);
		}
		catch(Exception e){
			
		}
		
		
		//fail("Not yet implemented");
	}
	
	@Test
	public void testSnarkyClerk() {
		Room[] rooms = new Room[4];
		DeskClerk aproximating = new SnarkyDeskClerk("Jan", "Emilian");
		assertEquals("Przedstawianie się źle!", "Jan Emilian, złośliwa.", aproximating.toString());
		
		Questionnaire.QuestionnaireBuilder x = new Questionnaire.QuestionnaireBuilder();
		x.setInternetAccess(true);
		x.setWindowAlingment(RoomAtributes.WindowAlingment.Zachód);
		x.setColor(RoomAtributes.Color.MORSKI);
		x.setInteriorDesign(RoomAtributes.InteriorDesign.ORIENTALNY);
		x.setPeopleCapacity(3);
		x.setRoomPrice(100);
		x.setStayPeriod("28-2-1996", 2);
		Questionnaire poll = x.build();
		
		Room.RoomBuilder builder = new Room.RoomBuilder();
		builder.setInteriorDesign(RoomAtributes.InteriorDesign.ORIENTALNY);
		builder.setColor(RoomAtributes.Color.SZARY);
		builder.setInternetAccess(false);
		builder.setPrice(200);
		builder.setWindowAlingment(RoomAtributes.WindowAlingment.Zachód);
		builder.setID(4);
		builder.setPeopleCapacity(3);
		
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
		builder.setPrice(99);
		builder.setWindowAlingment(RoomAtributes.WindowAlingment.Zachód);
		builder.setID(2);
		builder.setPeopleCapacity(3);
		

		rooms[2] = builder.build();
		
		builder.setInteriorDesign(RoomAtributes.InteriorDesign.NOWOCZESNY);
		builder.setColor(RoomAtributes.Color.JASNOZIELONY);
		builder.setInternetAccess(false);
		builder.setPrice(200);
		builder.setWindowAlingment(RoomAtributes.WindowAlingment.Wschód);
		builder.setID(10);
		builder.setPeopleCapacity(3);
		
		rooms[3] = builder.build();
		
		rooms[2].updateReservations("2-3-1998", 10);
		rooms[3].updateReservations("1-3-1996", 140);
		
		try{
			assertEquals(aproximating.findRoom(poll, rooms), rooms[3]);
		}
		catch (Exception e){
			
		}
		
		
		//fail("Not yet implemented");
	}
	
	@Test
	public void testPerfectClerk() {
		Room[] rooms = new Room[5];
		DeskClerk perfect = new PerfectDeskClerk("Jan", "Emilian");
		assertEquals("Przedstawianie się źle!", "Jan Emilian, perfekcjonistyczna.", perfect.toString());
		
		Questionnaire.QuestionnaireBuilder x = new Questionnaire.QuestionnaireBuilder();
		x.setInternetAccess(true);
		x.setWindowAlingment(RoomAtributes.WindowAlingment.Zachód);
		x.setColor(RoomAtributes.Color.MORSKI);
		x.setInteriorDesign(RoomAtributes.InteriorDesign.ORIENTALNY);
		x.setPeopleCapacity(3);
		x.setRoomPrice(100);
		x.setStayPeriod("28-2-1996", 2);
		Questionnaire poll = x.build();
		
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
		builder.setPrice(200);
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
		
		//System.out.println(perfect.findRoom(poll, rooms));
		
		try {
			assertEquals(rooms[4], perfect.findRoom(poll, rooms));
		}
		catch (Exception e){
			
		}
		
		rooms[4].updateReservations("28-2-1996", 1);
		
		try {
			assertEquals(null, perfect.findRoom(poll, rooms));
		}
		catch (Exception e){
			
		}
		
		
		//fail("Not yet implemented");
	}

}
